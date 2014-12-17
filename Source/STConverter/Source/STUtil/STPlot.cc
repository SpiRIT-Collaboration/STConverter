// =================================================
//  STPlot Class
//
//  Description:
//    Plot event data
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 24
// =================================================

#include "STPlot.hh"
#include "STRawEvent.hh"
#include "STPad.hh"

#include "TCanvas.h"
#include "TStyle.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLine.h"
#include "Riostream.h"

ClassImp(STPlot);

STPlot::STPlot()
{
  Clear();
}

void STPlot::Clear()
{
//  if (padplaneHist)
 //   delete padplaneHist;

  isAutodelete = kFALSE;
  event = NULL;

  padplaneCvs = NULL;
  padplaneHist = NULL;

  padCvs = NULL;
  padGraph[0] = NULL;
  padGraph[1] = NULL;
}

void STPlot::SetAutodelete(Bool_t value)
{
  isAutodelete = value;
}

Bool_t STPlot::CheckEvent()
{
  if (event == NULL) {
    cerr << "Event is not set!" << endl;
    return 1;
  }

  return 0;
}

// Setters
void STPlot::SetEvent(STRawEvent *anEvent)
{
  Clear();

  event = anEvent;
}

void STPlot::DrawPadplane()
{
  if (padplaneHist)
    return;

  if (CheckEvent())
    return;

  PreparePadplaneHist();

  Int_t numPads = event -> GetNumPads();
  Double_t max = 0;

  for (Int_t iPad = 0; iPad < numPads; iPad++) {
    STPad *aPad = event -> GetPad(iPad);

    Double_t *adc = aPad -> GetADC();

    Double_t maxADC = 0;
    for (Int_t i = 0; i < 512; i++) {
      if (maxADC < aPad -> GetADC(i))
        maxADC = aPad -> GetADC(i);
    }

    padplaneHist -> SetBinContent(aPad -> GetRow() + 1, aPad -> GetLayer() + 1, maxADC);
    if(maxADC > max) max = maxADC;
//    padplaneHist -> SetBinContent(aPad -> GetRow() + 1, aPad -> GetLayer() + 1, aPad -> GetADC(aPad -> GetMaxADCIdx()));
//    if(aPad -> GetADC(aPad -> GetMaxADCIdx()) > max) max = aPad -> GetADC(aPad -> GetMaxADCIdx());
  }

  padplaneHist -> SetMaximum(max);
}

void STPlot::DrawPad(Int_t row, Int_t layer)
{
  if (!padCvs)
    PreparePadCanvas();

  if (padGraph[0] && padGraph[1] && isAutodelete) {
    delete padGraph[0];
    delete padGraph[1];
  }

  if (CheckEvent())
    return;

  STPad *pad = event -> GetPad(row, layer);
  if (!pad)
    return;

  Int_t *tempRawAdc = pad -> GetRawADC();
  Double_t tb[512] = {0};
  Double_t rawAdc[512] = {0};
  for (Int_t iTb = 0; iTb < 512; iTb++) {
    tb[iTb] = iTb;
    rawAdc[iTb] = tempRawAdc[iTb];
  }

  padGraph[0] = new TGraph(512, tb, rawAdc);
  padGraph[0] -> SetTitle(Form("Raw ADC - (%d, %d)", row, layer));
  padGraph[0] -> SetLineColor(2);
  padGraph[0] -> GetHistogram() -> GetXaxis() -> SetTitle("Time bucket");
  padGraph[0] -> GetHistogram() -> GetXaxis() -> CenterTitle();
  padGraph[0] -> GetHistogram() -> GetYaxis() -> SetTitle("ADC");
  padGraph[0] -> GetHistogram() -> GetYaxis() -> CenterTitle();
  padGraph[0] -> GetHistogram() -> GetYaxis() -> SetLimits(-10, 4106);
  padGraph[0] -> GetHistogram() -> GetYaxis() -> SetRangeUser(-10, 4106);

  padCvs -> cd(1);
  padGraph[0] -> Draw("AL");

  Double_t *adc = pad -> GetADC();
  padGraph[1] = new TGraph(512, tb, adc);
  padGraph[1] -> SetTitle(Form("ADC(FPN pedestal subtracted) - (%d, %d)", row, layer));
  padGraph[1] -> SetLineColor(2);
  padGraph[1] -> GetHistogram() -> GetXaxis() -> SetTitle("Time bucket");
  padGraph[1] -> GetHistogram() -> GetXaxis() -> CenterTitle();
  padGraph[1] -> GetHistogram() -> GetYaxis() -> SetTitle("ADC");
  padGraph[1] -> GetHistogram() -> GetYaxis() -> CenterTitle();
  padGraph[1] -> GetHistogram() -> GetYaxis() -> SetLimits(-10, 4106);
  padGraph[1] -> GetHistogram() -> GetYaxis() -> SetRangeUser(-10, 4106);

  padCvs -> cd(2);
  padGraph[1] -> Draw("AL");
}

void STPlot::DrawLayer(Int_t layerNo)
{
  if (layerHist && isAutodelete)
    delete layerHist;

  if (CheckEvent())
    return;
}
// Getters

// -------------------------
void STPlot::PreparePadplaneHist()
{
//    cvs = new TCanvas("Event Display", "", 1600, 1000); // For large monitor
    padplaneCvs = new TCanvas("Event Display", "", 1200, 750);
    padplaneCvs -> Draw();

    gStyle -> SetOptStat(0000);
    gStyle -> SetPadRightMargin(0.08);
    gStyle -> SetPadLeftMargin(0.06);
    gStyle -> SetPadTopMargin(0.08);
    gStyle -> SetPadBottomMargin(0.08);
    gStyle -> SetTitleOffset(1.0, "X");
    gStyle -> SetTitleOffset(0.85, "Y");

/*
    timePad = new TPad("timePad", "Time Plane Display", 0.5, 0, 1, 1);
    timePad -> Divide(2, 2);
    timePad -> Draw();
*/
    
    padplaneCvs -> cd();
    padplaneHist = new TH2D("padplaneHist", "Pad Plane", 112, 0, 1344, 108, -432, 432);
//        padplane -> GetYaxis() -> SetNdivisions(15, kTRUE);
        padplaneHist -> GetXaxis() -> SetTickLength(0.01);
        padplaneHist -> GetXaxis() -> SetTitle("z (mm)");
        padplaneHist -> GetXaxis() -> CenterTitle();
        padplaneHist -> GetYaxis() -> SetTickLength(0.01);
        padplaneHist -> GetYaxis() -> SetTitle("x (mm)");
        padplaneHist -> GetYaxis() -> CenterTitle();
        padplaneHist -> Draw("colz");

    Double_t padLX = 8; // mm
    Double_t padLZ = 12; // mm

    for (Int_t i = 0; i < 107; i++) {
        horizLine[i] = new TLine(0, -432 + (i + 1)*padLX, 1344, -432 + (i + 1)*padLX);
        if ((i + 1)%9 == 0)
            horizLine[i] -> SetLineStyle(1);
        else
            horizLine[i] -> SetLineStyle(3);
        horizLine[i] -> Draw("same");
    }

    for (Int_t i = 0; i < 111; i++) {
        vertLine[i] = new TLine((i + 1)*padLZ, -432, (i + 1)*padLZ, 432);
        if ((i + 1)%7 == 0)
            vertLine[i] -> SetLineStyle(1);
        else
            vertLine[i] -> SetLineStyle(3);
        vertLine[i] -> Draw("same");
    }
}

void STPlot::PreparePadCanvas()
{
  gStyle -> SetOptStat(0000);
  gStyle -> SetPadRightMargin(0.03);
  gStyle -> SetPadLeftMargin(0.16);
  gStyle -> SetPadTopMargin(0.09);
  gStyle -> SetPadBottomMargin(0.11);
  gStyle -> SetTitleOffset(1.05, "X");
  gStyle -> SetTitleOffset(1.75, "Y");
  gStyle -> SetTitleSize(0.05, "X");
  gStyle -> SetTitleSize(0.05, "Y");
  gStyle -> SetLabelSize(0.05, "X");
  gStyle -> SetLabelSize(0.05, "Y");

  padCvs = new TCanvas("padCvs", "", 1100, 550);
  padCvs -> Divide(2, 1);
  padCvs -> Draw();
}
