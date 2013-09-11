#ifndef __CINT__
#include "TSystem.h"
#include "TCanvas.h"
#include "TH1D.h"

#include "STReadRaw.hh"
#include "STEvent.hh"
#include "STPad.hh"

#include <iostream>
#endif

void testplot() {
  gSystem -> Load("libSPiRIT.so");

  TCanvas *cvs = new TCanvas("cvs", "", 1400, 400);
    cvs -> Divide(3, 1);

  TH1D *hist[3];
  hist[0] = new TH1D("firedPads", "number of fired pads dist.", 252, -0.5, 251.5);
    hist[0] -> GetXaxis() -> SetTitle("# of fired pads");
    hist[0] -> GetXaxis() -> CenterTitle();
    hist[0] -> GetYaxis() -> SetTitle("counts");
    hist[0] -> GetYaxis() -> CenterTitle();
  hist[1] = new TH1D("maxSmplIdx", "max. sample idx dist.", 512, -0.5, 511.5);
    hist[1] -> GetXaxis() -> SetTitle("bucket index of Max. ADC");
    hist[1] -> GetXaxis() -> CenterTitle();
    hist[1] -> GetYaxis() -> SetTitle("counts");
    hist[1] -> GetYaxis() -> CenterTitle();
  hist[2] = new TH1D("maxADC", "max. ADC dist.", 4096, -0.5, 4095.5);
    hist[2] -> GetXaxis() -> SetTitle("max. ADC");
    hist[2] -> GetXaxis() -> CenterTitle();
    hist[2] -> GetYaxis() -> SetTitle("counts");
    hist[2] -> GetYaxis() -> CenterTitle();

//  STReadRaw *readraw = new STReadRaw("data/CoBo_2013-07-27T11_33_14.112_0000.graw");
//  STReadRaw *readraw = new STReadRaw("data/CoBo_2013-07-25T22_36_38.647_0000.graw");
  STReadRaw *readraw = new STReadRaw("test_20130911/ramp_1pC/CoBo_AsAd3_2013-09-11T11-31-19.157_0000.graw");
//    readraw -> SetPedestalData("data/CoBo_2013-07-25T05_44_12.511_0000_pedestal.root");
  STEvent *event = NULL;
  while (event = readraw -> GetEvent()) {
    Int_t numPads = event -> GetNumPads();
    hist[0] -> Fill(numPads);

    STPad *pad = NULL;
    for (Int_t i = 0; i < numPads; i++) {
      pad = event -> GetNextPad();
      Int_t maxADCIdx = pad -> GetMaxADCIdx();
      Int_t maxADC = pad -> GetADC(maxADCIdx);
      hist[1] -> Fill(maxADCIdx);
      hist[2] -> Fill(maxADC);
    }

//    delete event;
  }

  for (Int_t i = 0; i < 3; i++) {
    cvs -> cd(i + 1);
    hist[i] -> Draw();
  }

  cvs -> SaveAs("test.png");
}

#ifndef __CINT__
Int_t main() {
  testplot();

  return 0;
}
#endif
