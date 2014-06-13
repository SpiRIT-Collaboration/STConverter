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

    padplaneHist -> SetBinContent(aPad -> GetLayer() + 1, aPad -> GetRow() + 1, aPad -> GetADC(aPad -> GetMaxADCIdx()));
    if(aPad -> GetADC(aPad -> GetMaxADCIdx()) > max) max = aPad -> GetADC(aPad -> GetMaxADCIdx());
  }

  padplaneHist -> SetMaximum(max);
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
