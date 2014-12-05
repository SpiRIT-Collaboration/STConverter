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

  padplaneCvs = NULL;
  isAutodelete = 0;
  event = NULL;
  padplaneHist = NULL;
  padHist = NULL;
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
  event = anEvent;
}

void STPlot::DrawPadplane()
{
  if (CheckEvent())
    return;

  if (padplaneCvs == NULL)
    PreparePadplaneHist();

  padplaneHist -> Reset();

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

  padplaneCvs -> Modified();
  padplaneCvs -> Update();
}

void STPlot::DrawADC(Int_t padNo)
{
  if (padHist && isAutodelete)
    delete padHist;

  if (CheckEvent())
    return;
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
    padplaneCvs = new TCanvas("Event Display", "", 870, 780);
    padplaneCvs -> Draw();

    gStyle -> SetOptStat(0000);
    gStyle -> SetPadRightMargin(0.08);
    gStyle -> SetPadLeftMargin(0.11);
    gStyle -> SetPadTopMargin(0.08);
    gStyle -> SetPadBottomMargin(0.08);
    gStyle -> SetTitleOffset(1.2, "X");
    gStyle -> SetTitleOffset(1.4, "Y");

/*
    timePad = new TPad("timePad", "Time Plane Display", 0.5, 0, 1, 1);
    timePad -> Divide(2, 2);
    timePad -> Draw();
*/
    
    padplaneCvs -> cd();
    padplaneHist = new TH2D("padplaneHist", "BRAHMS-TPC Pad Plane", 96, -187.2, 187.2, 12, 0, 360);
//        padplane -> GetYaxis() -> SetNdivisions(15, kTRUE);
    padplaneHist -> GetXaxis() -> SetTickLength(0.01);
    padplaneHist -> GetXaxis() -> SetTitle("x (mm)");
    padplaneHist -> GetXaxis() -> CenterTitle();
    padplaneHist -> GetYaxis() -> SetTickLength(0.01);
    padplaneHist -> GetYaxis() -> SetTitle("z (mm)");
    padplaneHist -> GetYaxis() -> CenterTitle();
    padplaneHist -> Draw("colz");

    Double_t padLX = 3.9; // mm
    Double_t padLZ = 30.; // mm

    for (Int_t i = 0; i < 11; i++) {
      horizLine[i] = new TLine(-187.2, (i + 1)*padLZ, 187.2, (i + 1)*padLZ);
      horizLine[i] -> SetLineStyle(3);
      horizLine[i] -> Draw("same");
    }

    for (Int_t i = 0; i < 95; i++) {
      vertLine[i] = new TLine((i + 1)*padLX - 187.2, 0, (i + 1)*padLX - 187.2, 360);
      vertLine[i] -> SetLineStyle(3);
      vertLine[i] -> Draw("same");
    }
}
