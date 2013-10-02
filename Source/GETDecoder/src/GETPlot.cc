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
  maxTb = 276;

  SetAgetRange(-1, minTb - 1, maxTb + 1, -10, 4106);

  fDecoder = NULL;
  for (Int_t i = 0; i < 4; i++)
    fAget[i] = new TGraph();
}

void GETPlot::SetDecoder(GETDecoder *decoder)
{
  // Sets the decoder pointer

  fDecoder = decoder;

  SetAgetRange(-1, minTb - 1, maxTb + 1, -10, 4106);

  TCanvas *cvs = NULL;
  for (Int_t i = 0; i < 3; i++) {
    cvs = (TCanvas *) gROOT -> FindObject(Form("cvsType%d", i));

    if (cvs)
      delete cvs;
  }
      
}

void GETPlot::SetAgetRange(Int_t type, Double_t minx, Double_t maxx, Double_t miny, Double_t maxy)
{
  if (type == -1) {
    for (Int_t iType = 0; iType < 3; iType++) {
      fAgetMinX[iType] = minx;
      fAgetMaxX[iType] = maxx;
      fAgetMinY[iType] = miny;
      fAgetMaxY[iType] = maxy;
    }
  } else {
    fAgetMinX[type] = minx;
    fAgetMaxX[type] = maxx;
    fAgetMinY[type] = miny;
    fAgetMaxY[type] = maxy;
  }
}

TCanvas *GETPlot::ShowSummarySpectra()
{
  if (fDecoder == NULL) {
    std::cout << "== GETDecoder is not set!" << std::endl;

    return NULL;
  }

  return NULL;
}

TCanvas *GETPlot::ShowRawFrame(Int_t frameNo, Int_t numChannels, Int_t *chList)
{
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

  std::cout << "== Drawing raw frame " << fDecoder -> GetCurrentFrameNo() << std::endl;

  TCanvas *cvs = (TCanvas *) gROOT -> FindObject("cvsType1");
  if (!cvs)
    cvs = PrepareCanvas(1);
  else 
    ResetGraph(1);

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
      Int_t tb[512] = {0};
      Int_t charge[512] = {0};

      for (Int_t iTb = 0; iTb < 512; iTb++) {
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

      fAget[iAget] -> SetMarkerColor(markerColor);
      fAget[iAget] -> SetLineColor(markerColor);
      fAget[iAget] -> SetMarkerStyle(2);
      fAget[iAget] -> SetMarkerSize(0.3);
      cvs -> cd(iAget + 1);
      fAget[iAget] -> DrawGraph(numSamples, tb, charge, "PL same");
    }
  }

  return cvs;
}

TCanvas *GETPlot::ShowFrame(Int_t frameNo, Int_t startTb, Int_t numTbs, Int_t numChannels, Int_t *chList)
{
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

  std::cout << "== Drawing frame " << fDecoder -> GetCurrentFrameNo() << std::endl;

  TCanvas *cvs = (TCanvas *) gROOT -> FindObject("cvsType2");
  if (!cvs)
    cvs = PrepareCanvas(2);
  else 
    ResetGraph(2);

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
      Double_t tb[512] = {0};
      Double_t charge[512] = {0};

      for (Int_t iTb = 0; iTb < 512; iTb++) {
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

      fAget[iAget] -> SetMarkerColor(markerColor);
      fAget[iAget] -> SetLineColor(markerColor);
      fAget[iAget] -> SetMarkerStyle(2);
      fAget[iAget] -> SetMarkerSize(0.3);
      cvs -> cd(iAget + 1);
      fAget[iAget] -> DrawGraph(numSamples, tb, charge, "PL same");
    }
  }

  return cvs;
}

TCanvas *GETPlot::ShowAverage(Int_t numChannels, Int_t *chList, Int_t frameNo)
{
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

  std::cout << "== Drawing average of frame " << fDecoder -> GetCurrentFrameNo() << std::endl;

  TCanvas *cvs = (TCanvas *) gROOT -> FindObject("cvsType3");
  if (!cvs)
    cvs = PrepareCanvas(3);
  else 
    ResetGraph(3);

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    Int_t divider = 0;
    Double_t tb[512] = {0};
    Double_t charge[512] = {0};

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

      for (Int_t iTb = 0; iTb < 512; iTb++) {
        tb[iTb] = iTb;
        charge[iTb] += rawadc[iTb];
      }

      divider++;
    }

    for (Int_t iTb = 0; iTb < 512; iTb++) 
      charge[iTb] /= (Double_t) divider;

    fAget[iAget] -> SetMarkerColor(2);
    fAget[iAget] -> SetLineColor(2);
    fAget[iAget] -> SetMarkerStyle(2);
    fAget[iAget] -> SetMarkerSize(0.3);
    cvs -> cd(iAget + 1);
    fAget[iAget] -> DrawGraph(512, tb, charge, "PL same");
  }

  return cvs;
}

TCanvas *GETPlot::PrepareCanvas(Int_t type)
{
  TCanvas *cvs = NULL;

  if (type == 0) {
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

    return cvs;
  }

  return NULL;
}

void GETPlot::ResetGraph(Int_t type, Bool_t first)
{
  if (type == 0) {
  } else if (type > 0) {
    TCanvas *cvs = (TCanvas *) gROOT -> FindObject(Form("cvsType%d", type));

    for (Int_t iAget = 0; iAget < 4; iAget++) {
      if (!first) {
        cvs -> cd(iAget + 1);

        TFrame *box = gPad -> GetFrame();
        fAgetMinX[type] = box -> GetX1();
        fAgetMaxX[type] = box -> GetX2();
        fAgetMinY[type] = box -> GetY1();
        fAgetMaxY[type] = box -> GetY2();

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
      fAget[iAget] -> GetXaxis() -> SetRangeUser(fAgetMinX[type], fAgetMaxX[type]);
      fAget[iAget] -> GetYaxis() -> SetTitle("ADC Channel");
      fAget[iAget] -> GetYaxis() -> CenterTitle();
      fAget[iAget] -> GetYaxis() -> SetLimits(-10, 4106);
      fAget[iAget] -> GetYaxis() -> SetRangeUser(fAgetMinY[type], fAgetMaxY[type]);
    }
  }
}
