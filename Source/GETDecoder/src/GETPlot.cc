// =================================================
//  GETPlot Class
// 
//  Description:
//    Thinking what to write
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 10. 01
//      Start writing class
// =================================================

#include <iostream>

#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TColor.h"
#include "TROOT.h"
#include "TFrame.h"
#include "TH2D.h"
#include "TPaletteAxis.h"
#include "TPad.h"
#include "TText.h"

#include "GETPlot.hh"
#include "GETDecoder.hh"
#include "GETFrame.hh"

ClassImp(GETPlot);

GETPlot::GETPlot()
{
  // Constructor

  Initialize();
}

GETPlot::GETPlot(GETDecoder *decoder)
{
  // Constructor
  Initialize();
  SetDecoder(decoder);
}

GETPlot::~GETPlot()
{
  // Destructor
}

void GETPlot::Initialize()
{
  // Initializes variables

  minTb = 0;
  maxTb = GETNumTbs;

  SetAgetRange(-1, -1, minTb - 1, maxTb + 1, -10, 4106);

  fDecoder = NULL;

  for (Int_t i = 0; i < 4; i++)
    fAget[i] = new TGraph();

  fGraph = new TGraph();
  fAsad = NULL;
}

void GETPlot::SetDecoder(GETDecoder *decoder)
{
  // Sets the decoder pointer

  fDecoder = decoder;

  SetAgetRange(-1, -1, minTb - 1, maxTb + 1, -10, 4106);

  TCanvas *cvs = NULL;
  for (Int_t i = 0; i < 3; i++) {
    cvs = (TCanvas *) gROOT -> FindObject(Form("cvsType%d", i));

    if (cvs)
      delete cvs;
  }
}

void GETPlot::SetAgetRange(Int_t type, Int_t agetIdx, Double_t minx, Double_t maxx, Double_t miny, Double_t maxy)
{
  // Sets the plotting range.
  // 
  // Type:
  //    0 - not assigned
  //    1 - canvas returned by GetRawFrame() method
  //    2 - canvas returned by GetFrame() method
  //    3 - canvas returned by GetAverage() method

  if (type == -1) {
    for (Int_t iType = 0; iType < 4; iType++) {
      for (Int_t iAget = 0; iAget < 4; iAget++) {
        fAgetMinX[iType][iAget] = minx;
        fAgetMaxX[iType][iAget] = maxx;
        fAgetMinY[iType][iAget] = miny;
        fAgetMaxY[iType][iAget] = maxy;
      }
    }
  } else {
    fAgetMinX[type][agetIdx] = minx;
    fAgetMaxX[type][agetIdx] = maxx;
    fAgetMinY[type][agetIdx] = miny;
    fAgetMaxY[type][agetIdx] = maxy;
  }
}

TCanvas *GETPlot::ShowSummarySpectra(Int_t startTb, Int_t numTbs)
{
  // Draws the summary spectra of the file set to the decoder,
  // and returns the canvas.
  // 
  // startTb: time bucket index from which the pedestal calculation starts
  //  numTbs: the number of time buckets to use in the pedestal calculation

  if (fDecoder == NULL) {
    std::cout << "== GETDecoder is not set!" << std::endl;

    return NULL;
  }

  TCanvas *cvs = (TCanvas *) gROOT -> FindObject("cvsType0");
  if (!cvs)
    cvs = PrepareCanvas(0);
  else
    fAsad -> Reset();

  Int_t numFrames = fDecoder -> GetNumFrames();

  fFrame = fDecoder -> GetFrame(0);
  fAsad -> SetTitle(Form("CoBo %d - AsAd %d", fFrame -> GetCoboID(), fFrame -> GetAsadID()));

  for (Int_t iFrame = 0; iFrame < numFrames; iFrame++) {
    fFrame = fDecoder -> GetFrame(iFrame);
    fFrame -> CalcPedestal(startTb, numTbs);

    for (Int_t iAget = 0; iAget < 4; iAget++) {
      for (Int_t iCh = 0; iCh < 68; iCh++) {
        Int_t maxADCIdx = fFrame -> GetMaxADCIdx(iAget, iCh);
        fAsad -> Fill(iAget*68 + iCh, fFrame -> GetADC(iAget, iCh, maxADCIdx));
      }
    }
  }

  cvs -> Update();

  TPaletteAxis *axis = (TPaletteAxis *) fAsad -> GetListOfFunctions() -> FindObject("palette");
  axis -> SetX2NDC(0.975);
  axis -> GetAxis() -> SetTickSize(0.008);
  axis -> SetLabelSize(0.045);
  axis -> SetLabelOffset(-0.005);

  Double_t maskChannel[5] = {11, 22, 45, 56, 67};
  for (Int_t iAget = 0; iAget < 4; iAget++) {
    for (Int_t iMask = 0; iMask < 5; iMask++) {
      Double_t pointX1[2] = {iAget*68 + maskChannel[iMask] - 0.5, iAget*68 + maskChannel[iMask] - 0.5};
      Double_t pointX2[2] = {iAget*68 + maskChannel[iMask] + 0.5, iAget*68 + maskChannel[iMask] + 0.5};
      Double_t pointY[2] = {-0.5, 4095.5};
      fGraph -> SetLineColor(15);
      fGraph -> DrawGraph(2, pointX1, pointY, "L same");
      fGraph -> DrawGraph(2, pointX2, pointY, "L same");
    }
  }

  cvs -> Update();

  return cvs;
}

TCanvas *GETPlot::ShowRawFrame(Int_t frameNo, Int_t numChannels, Int_t *chList)
{
  // Draw raw signals in the frame whose number is frameNo.
  //
  //     frameNo: frame number that will be drawn.
  //              If -1, the method automatically load the next frame.
  // numChannels: the number of channels which will be drawn (positive number)
  //              or will be masked (negative number).
  //      chList: an array having the channel numbers
  //              array size should be bigger than the absolute value numChannels
  //
  // example)
  //   ShowRawFrame(): draws all the channels
  //   ShowRawFrame(2): draw all the channels in the frame number 2
  //
  //   Int_t list[4] = {11, 22, 45, 56};
  //   ShowRawFrame(-1, 4, list): draws 4 channels in the array, list, of the next frame
  //   ShowRawFrame(-1, -4, list): draws all channels except 4 channels in the array, list, of the next frame

  if (fDecoder == NULL) {
    std::cout << "== GETDecoder is not set!" << std::endl;

    return NULL;
  }

  if (frameNo == -1)
    fFrame = fDecoder -> GetFrame();
  else
    fFrame = fDecoder -> GetFrame(frameNo);

  if (!fFrame)
    return NULL;

  std::cout << "== Drawing raw frame " << fDecoder -> GetCurrentFrameID() << std::endl;

  TCanvas *cvs = (TCanvas *) gROOT -> FindObject("cvsType1");
  if (!cvs)
    cvs = PrepareCanvas(1);
  else 
    ResetGraph(1);

  TPad *namePad = (TPad *) cvs -> FindObject("namePadType1");
  PrintInfo(1, namePad, fFrame -> GetCoboID(), fFrame -> GetAsadID(), fFrame -> GetFrameID());

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    for (Int_t iCh = 0; iCh < 68; iCh++) {
      Bool_t isSkip = 0;
      if (numChannels < 0) {
        for (Int_t iMask = 0; iMask < -numChannels; iMask++)
          if (iCh == chList[iMask]) {
            isSkip = 1;
            break;
          }
      } else if (numChannels > 0) {
        isSkip = 1;

        for (Int_t iDraw = 0; iDraw < numChannels; iDraw++)
          if (iCh == chList[iDraw]) {
            isSkip = 0;
            break;
          }
      }

      if (isSkip)
        continue;

      Int_t *rawadc = fFrame -> GetRawADC(iAget, iCh);
      
      Int_t numSamples = 0;
      Int_t tb[GETNumTbs] = {0};
      Int_t charge[GETNumTbs] = {0};

      for (Int_t iTb = 0; iTb < GETNumTbs; iTb++) {
        if (rawadc[iTb] == 0)
          continue;
        
        tb[numSamples] = iTb;
        charge[numSamples] = rawadc[iTb];
        numSamples++;
      }

      Float_t r = 0, g = 0, b = 0;
      Float_t h = iCh/67.*360, s = 0.5, l = 0.5;
      TColor::HLS2RGB(h, l, s, r, g, b);
      Int_t markerColor = TColor::GetColor(r, g, b);

      fGraph -> SetMarkerColor(markerColor);
      fGraph -> SetLineColor(markerColor);
      fGraph -> SetMarkerStyle(2);
      fGraph -> SetMarkerSize(0.3);
      cvs -> cd(iAget + 1);
      fGraph -> DrawGraph(numSamples, tb, charge, "PL same");
    }
  }

  return cvs;
}

TCanvas *GETPlot::ShowFrame(Int_t frameNo, Int_t startTb, Int_t numTbs, Int_t numChannels, Int_t *chList)
{
  // Draw pedestal-subtracted signals in the frame whose number is frameNo.
  //
  //     frameNo: frame number that will be drawn.
  //              If -1, the method automatically load the next frame.
  //     startTb: time bucket index from which the pedestal calculation starts
  //      numTbs: the number of time buckets to use in the pedestal calculation
  // numChannels: the number of channels which will be drawn (positive number)
  //              or will be masked (negative number).
  //      chList: an array having the channel numbers
  //              array size should be bigger than the absolute value numChannels
  //
  // example) See ShowRawFrame() method.

  if (fDecoder == NULL) {
    std::cout << "== GETDecoder is not set!" << std::endl;

    return NULL;
  }

  if (frameNo == -1)
    fFrame = fDecoder -> GetFrame();
  else
    fFrame = fDecoder -> GetFrame(frameNo);

  if (!fFrame)
    return NULL;

  fFrame -> CalcPedestal(startTb, numTbs);

  std::cout << "== Drawing frame " << fDecoder -> GetCurrentFrameID() << std::endl;

  TCanvas *cvs = (TCanvas *) gROOT -> FindObject("cvsType2");
  if (!cvs)
    cvs = PrepareCanvas(2);
  else 
    ResetGraph(2);

  TPad *namePad = (TPad *) cvs -> FindObject("namePadType2");
  PrintInfo(2, namePad, fFrame -> GetCoboID(), fFrame -> GetAsadID(), fFrame -> GetFrameID());

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    for (Int_t iCh = 0; iCh < 68; iCh++) {
      Bool_t isSkip = 0;
      if (numChannels < 0) {
        for (Int_t iMask = 0; iMask < -numChannels; iMask++)
          if (iCh == chList[iMask]) {
            isSkip = 1;
            break;
          }
      } else if (numChannels > 0) {
        isSkip = 1;

        for (Int_t iDraw = 0; iDraw < numChannels; iDraw++)
          if (iCh == chList[iDraw]) {
            isSkip = 0;
            break;
          }
      }

      if (isSkip)
        continue;

      Double_t *adc = NULL;
      adc = fFrame -> GetADC(iAget, iCh);
      
      Int_t numSamples = 0;
      Double_t tb[GETNumTbs] = {0};
      Double_t charge[GETNumTbs] = {0};

      for (Int_t iTb = 0; iTb < GETNumTbs; iTb++) {
//        if (rawadc[iTb] == 0)
//          continue;
        
        tb[numSamples] = iTb;
        charge[numSamples] = adc[iTb];
        numSamples++;
      }

      Float_t r = 0, g = 0, b = 0;
      Float_t h = iCh/67.*360, s = 0.5, l = 0.5;
      TColor::HLS2RGB(h, l, s, r, g, b);
      Int_t markerColor = TColor::GetColor(r, g, b);

      fGraph -> SetMarkerColor(markerColor);
      fGraph -> SetLineColor(markerColor);
      fGraph -> SetMarkerStyle(2);
      fGraph -> SetMarkerSize(0.3);
      cvs -> cd(iAget + 1);
      fGraph -> DrawGraph(numSamples, tb, charge, "PL same");
    }
  }

  return cvs;
}

TCanvas *GETPlot::ShowAverage(Int_t numChannels, Int_t *chList, Int_t frameNo)
{
  // Draw averaged-raw signals in the frame whose number is frameNo.
  //
  // numChannels: the number of channels which will be averaged (positive number)
  //              or will be masked in averaging (negative number).
  //      chList: an array having the channel numbers
  //              array size should be bigger than the absolute value of numChannels
  //     frameNo: frame number that will be drawn.
  //              If -1, the method automatically load the next frame.
  //
  // example)

  if (fDecoder == NULL) {
    std::cout << "== GETDecoder is not set!" << std::endl;

    return NULL;
  }

  if (frameNo == -1)
    fFrame = fDecoder -> GetFrame();
  else
    fFrame = fDecoder -> GetFrame(frameNo);

  if (!fFrame)
    return NULL;

  std::cout << "== Drawing average of frame " << fDecoder -> GetCurrentFrameID() << std::endl;

  TCanvas *cvs = (TCanvas *) gROOT -> FindObject("cvsType3");
  if (!cvs)
    cvs = PrepareCanvas(3);
  else 
    ResetGraph(3);

  TPad *namePad = (TPad *) cvs -> FindObject("namePadType3");
  PrintInfo(3, namePad, fFrame -> GetCoboID(), fFrame -> GetAsadID(), fFrame -> GetFrameID());

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    Int_t divider = 0;
    Double_t tb[GETNumTbs] = {0};
    Double_t charge[GETNumTbs] = {0};

    for (Int_t iCh = 0; iCh < 68; iCh++) {
      Bool_t isSkip = 0;
      if (numChannels < 0) {
        for (Int_t iMask = 0; iMask < -numChannels; iMask++)
          if (iCh == chList[iMask]) {
            isSkip = 1;
            break;
          }
      } else if (numChannels > 0) {
        isSkip = 1;

        for (Int_t iDraw = 0; iDraw < numChannels; iDraw++)
          if (iCh == chList[iDraw]) {
            isSkip = 0;
            break;
          }
      }

      if (isSkip)
        continue;

      Int_t *rawadc = fFrame -> GetRawADC(iAget, iCh);

      for (Int_t iTb = 0; iTb < GETNumTbs; iTb++) {
        tb[iTb] = iTb;
        charge[iTb] += rawadc[iTb];
      }

      divider++;
    }

    for (Int_t iTb = 0; iTb < GETNumTbs; iTb++) 
      charge[iTb] /= (Double_t) divider;

    fGraph -> SetMarkerColor(2);
    fGraph -> SetLineColor(2);
    fGraph -> SetMarkerStyle(2);
    fGraph -> SetMarkerSize(0.3);
    cvs -> cd(iAget + 1);
    fGraph -> DrawGraph(GETNumTbs, tb, charge, "PL same");
  }

  return cvs;
}

TCanvas *GETPlot::PrepareCanvas(Int_t type)
{
  // Internal method to prepare canvases

  TCanvas *cvs = NULL;

  if (type == 0) {
    gStyle -> SetOptStat(0);
    gStyle -> SetPadLeftMargin(0.06);
    gStyle -> SetPadRightMargin(0.045);
    gStyle -> SetTitleSize(0.05, "x");
    gStyle -> SetTitleSize(0.05, "y");
    gStyle -> SetLabelSize(0.05, "x");
    gStyle -> SetLabelSize(0.05, "y");
    gStyle -> SetTitleOffset(0.65, "x");
    gStyle -> SetTitleOffset(0.62, "y");
    gStyle -> SetTickLength(0.010, "y");
    gStyle -> SetPalette(55);
    gStyle -> SetNumberContours(100);

    cvs = new TCanvas("cvsType0", "", 1200, 400);

    fAsad = new TH2D("fAsad", "", 272, -0.5, 271.5, 8192, -0.5, 4095.5);
    fAsad -> GetXaxis() -> SetTitle("Channel");
    fAsad -> GetXaxis() -> CenterTitle();
    fAsad -> GetYaxis() -> SetTitle("Amplitude");
    fAsad -> GetYaxis() -> CenterTitle();
    fAsad -> Fill(-10, -10);
    fAsad -> Draw("colz");

    cvs -> SetLogz();

    fAsad -> Reset();

    return cvs;
  } else if (type > 0) {
    gStyle -> SetOptStat(0);
    gStyle -> SetTitleSize(0.05, "x");
    gStyle -> SetTitleSize(0.05, "y");
    gStyle -> SetLabelSize(0.05, "x");
    gStyle -> SetLabelSize(0.04, "y");
    gStyle -> SetTitleOffset(1.0, "x");
    gStyle -> SetTitleOffset(1.2, "y");
    gStyle -> SetPadLeftMargin(0.12);
    gStyle -> SetPadRightMargin(0.03);

    cvs = new TCanvas(Form("cvsType%d", type), "", 1200, 1000);
    cvs -> Divide(2, 2);

    ResetGraph(type, 1);

    cvs -> cd();
    TPad *namePad = new TPad(Form("namePadType%d", type), "", 0.32, 0.46, 0.68, 0.54);   
    namePad -> SetFillStyle(4000);
    namePad -> Draw();

    return cvs;
  }

  return NULL;
}

void GETPlot::ResetGraph(Int_t type, Bool_t first)
{
  // Internal function to prepare canvases

  TCanvas *cvs = (TCanvas *) gROOT -> FindObject(Form("cvsType%d", type));

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    if (!first) {
      cvs -> cd(iAget + 1);

      TFrame *box = gPad -> GetFrame();
      SetAgetRange(type, iAget, box -> GetX1(), box -> GetX2(), box -> GetY1(), box -> GetY2());

      fAget[iAget] -> Clear();
    }

    fAget[iAget] -> SetTitle(Form("AGET %d", iAget));
    fAget[iAget] -> SetMarkerStyle(2);
    fAget[iAget] -> SetPoint(0, -100, -100);

    cvs -> cd(iAget + 1);
    fAget[iAget] -> Draw("AP");
    fAget[iAget] -> GetXaxis() -> SetTitle("Time Bucket");
    fAget[iAget] -> GetXaxis() -> CenterTitle();
    fAget[iAget] -> GetXaxis() -> SetLimits(-5, 516);
    fAget[iAget] -> GetXaxis() -> SetRangeUser(fAgetMinX[type][iAget], fAgetMaxX[type][iAget]);
    fAget[iAget] -> GetYaxis() -> SetTitle("ADC Channel");
    fAget[iAget] -> GetYaxis() -> CenterTitle();
    fAget[iAget] -> GetYaxis() -> SetLimits(-10, 4106);
    fAget[iAget] -> GetYaxis() -> SetRangeUser(fAgetMinY[type][iAget], fAgetMaxY[type][iAget]);
  }
}

void GETPlot::PrintInfo(Int_t type, TPad *namePad, Int_t coboIdx, Int_t asadIdx, Int_t frameNo)
{
  // Internal function to prepare canvases

  namePad -> cd();

  TText *name = (TText *) namePad -> FindObject("frameInfo");
  if (name)
    delete name;

  name = new TText(0.5, 0.5, Form("CoBo %d - AsAd %d - Frame %d", coboIdx, asadIdx, frameNo));
  name -> SetName("frameInfo");
  name -> SetTextSize(0.40);
  name -> SetTextAlign(22);
  name -> Draw("same");
}
