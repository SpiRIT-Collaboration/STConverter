#include "TCanvas.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TStyle.h"
#include "Riostream.h"
#include "TClonesArray.h"

#ifndef __CINT__
#include "SPiRITReadRaw.hh"
#include "SPiRITEvent.hh"
#include "SPiRITPad.hh"
#else
class SPiRITReadRaw;
class SPiRITEvent;
class SPiRITPad;
#endif

gSystem -> Load("libSPiRIT.so");

void evtdisplay(Char_t *rawdata, Char_t *pedestaldata, Char_t *outdir);
void evtdisplay(Int_t evt = 0);
void next();
void evt(Int_t evt);
void save(Char_t *ext = "png");
void PrepareCanvas();
void ClearCanvas();

TCanvas *cvs;
TPad *padPad;
TCanvas *timeCvs;
TH2D *padplane;
TLine *horizLine[107];
TLine *vertLine[111];
TH2D *timeplane[7*4];
/*
  TLine *layer0Line[5];
  TLine *layer1Line[5];
  TLine *layer2Line[5];
  TLine *layer3Line[5];
*/

SPiRITReadRaw *readraw;
SPiRITEvent *anEvent;
SPiRITPad *aPad;
TPaletteAxis *axis;

Char_t *outDir;

Char_t *saveName;
Int_t evtNum;
Int_t endEvent;
Double_t max;

Double_t padLX = 8; // mm
Double_t padLZ = 12; // mm

void evtdisplay() {
  gSystem -> Load("libSPiRIT");
}

void evtdisplay(Int_t evt) {
gSystem -> Load("libSPiRIT.so");

  cout << "Event Number: " << evt << endl;

  ResetCanvas();

  delete anEvent;
  anEvent = readraw -> GetEvent();

  Int_t numPads = anEvent -> GetNumPads();

  for (Int_t iPad = 0; iPad < numPads; iPad++) {
    aPad = anEvent -> GetNextPad();

    Double_t *adc = aPad -> GetADC();

    padplane -> SetBinContent(aPad -> GetPadLayer() + 1, aPad -> GetPadRow() + 1, aPad -> GetADC(aPad -> GetMaxADCIdx()));
    if(aPad -> GetADC(aPad -> GetMaxADCIdx()) > max) max = aPad -> GetADC(aPad -> GetMaxADCIdx());

    for (Int_t iTime = 0; iTime < 511; iTime++)
      timeplane[aPad -> GetPadLayer()] -> SetBinContent(aPad -> GetPadRow() + 1, iTime + 1, adc[iTime]);
  }

  padplane->SetMaximum(max);
  for (Int_t i = 0; i < 4*7; i++) {
    timeplane[i]->SetMaximum(max);
    timeplane[i]->SetMinimum(0);
  }

  cvs -> Modified();
  cvs -> Update();

  for (Int_t i = 0; i < 28; i++) {
    timeCvs -> cd(i+1);
    gPad -> Modified();
    gPad -> Update();

//    timeCvs -> Modified();
//    timeCvs -> Update();
  }

//  axis = (TPaletteAxis *) padplane -> GetListOfFunctions() -> FindObject("palette");
//  axis -> SetX2NDC(0.97);
//  axis -> GetAxis() -> SetTickSize(0.01);
  /*
  for (Int_t i = 0; i < 4*7; i++) {
    axis = (TPaletteAxis *) timeplane[i] -> GetListOfFunctions() -> FindObject("palette");
    axis -> SetX2NDC(0.925);
  }
  */

}

void save(Char_t *ext) {
  cvs -> SaveAs(Form("%s/%s_evt%d.%s", outDir, saveName, evtNum, ext));
}

void evtdisplay(Char_t *rawdata, Char_t *pedestaldata, Char_t *outdir) {
  gSystem -> Load("libSPiRIT.so");

  cout << endl;
  cout << "=========================================" << endl;
  cout << " - next()        : display next event" << endl;
  cout << " - next(#)       : display event number #" << endl;
  cout << " - save()        : save display in png (default: in 'saved' folder)" << endl;
  cout << " - save(\"pdf\") : save display in PDF (default: in 'saved' folder)" << endl;
  cout << " - save(\"eps\") : save display in eps (default: in 'saved' folder)" << endl;
  cout << "=========================================" << endl;
  cout << endl;

  max = 0;

  outDir = outdir;

  readraw = new SPiRITReadRaw(rawdata);
  if (pedestaldata != NULL)
    readraw -> SetPedestalData(pedestaldata);

  PrepareCanvas();

/*
  cout << endl;
  cout << "=========================================" << endl;
  cout << " - Total events in this file: " << endEvent << endl;
  cout << "=========================================" << endl;
  cout << endl;
*/

  evtNum = 0;

  evtdisplay(evtNum);
}

void next() {
  if (endEvent == evtNum + 1) {
    cout << "This is the end of this file!" << endl;
    return;
  }

  evtdisplay(++evtNum);
}

void next(Int_t evt) {
  if (endEvent <= evt) {
    cout << "The event number is out of bound!" << endl;
    return;
  }

  evtNum = evt;
  evtdisplay(evtNum);
}

void ResetCanvas() {
  for (Int_t i = 0; i < 28; i++)
    timeplane[i] -> Reset();

  padplane -> Reset();
}

void PrepareCanvas() {
  gStyle -> SetOptStat(0000);
  gStyle -> SetPadRightMargin(0.05);
  gStyle -> SetPadLeftMargin(0.04);
  gStyle -> SetPadTopMargin(0.08);
  gStyle -> SetPadBottomMargin(0.16);
  gStyle -> SetTitleSize(0.1, "X");
  gStyle -> SetTitleSize(0.1, "Y");
  gStyle -> SetTitleOffset(0.55, "X");
  gStyle -> SetTitleOffset(0.18, "Y");

  cvs = new TCanvas("Event Display", "", 1200, 280);
  cvs -> Draw();

  timeCvs = new TCanvas("timeCvs", "Time Plane Display", 1600, 900);
  timeCvs -> Divide(7, 4);
  timeCvs -> Draw();

  cvs -> cd();
  padplane = new TH2D("padplane", "", 28, 0, 336, 9, 0, 72);
  padplane -> GetXaxis() -> SetTickLength(0.01);
  padplane -> GetXaxis() -> SetTitle("z (mm)");
  padplane -> GetXaxis() -> CenterTitle();
  padplane -> GetYaxis() -> SetTickLength(0.01);
  padplane -> GetYaxis() -> SetTitle("x (mm)");
  padplane -> GetYaxis() -> CenterTitle();
  padplane -> Draw("colz");

  for (Int_t i = 0; i < 8; i++) {
    horizLine[i] = new TLine(0, (i + 1)*padLX, 336,  (i + 1)*padLX);
    if ((i + 1)%9 == 0)
      horizLine[i] -> SetLineStyle(1);
    else
      horizLine[i] -> SetLineStyle(3);
    horizLine[i] -> Draw("same");
  }

  for (Int_t i = 0; i < 27; i++) {
    vertLine[i] = new TLine((i + 1)*padLZ, 0, (i + 1)*padLZ, 72);
    if ((i + 1)%7 == 0)
      vertLine[i] -> SetLineStyle(1);
    else
      vertLine[i] -> SetLineStyle(3);
    vertLine[i] -> Draw("same");
  }

  char dummy[64];
  for (Int_t i = 0; i < 7*4; i++) {
    timeCvs -> cd(i+1);
    sprintf(dummy,"timeplane_layer%d",i);
    timeplane[i] = new TH2D(dummy,dummy, 9, -0.5, 8.5, 512, 0, 512);
    timeplane[i] -> Draw("colz");
  }
}

void ClearCanvas() {
  if (timeCvs)  delete timeCvs;
  if (timeplane[0]) for (Int_t i = 0; i < 7*4; i++)  delete timeplane[i];
  if (horizLine[0]) for (Int_t i = 0; i < 107; i++)  delete horizLine[i];
  if (vertLine[0])  for (Int_t i = 0; i < 111; i++)  delete vertLine[i];

  if (padplane) delete padplane;
  if (padPad)   delete padPad;
  if (cvs)      delete cvs;
}

#ifndef __CINT__
Int_t main() {
  gSystem -> Load("libSPiRIT");
  return 0;
}
