// =================================================
//  STGainCheck Class
// 
//  Description:
//    Test gain using processed pulser data
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2014. 09. 07
// =================================================

#include "STGainCheck.hh"
#include "STRawEvent.hh"
#include "STPad.hh"

#include "TObjArray.h"
#include "TSystem.h"
#include "TSpectrum.h"
#include "TStyle.h"
#include "TPaletteAxis.h"

#include <fstream>
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
using std::ifstream;

STGainCheck::STGainCheck()
{
  fCanvas = NULL;
  fSpectra = NULL;

  fCore = new STCore();

  fIsGainCalibrationData = kFALSE;
  fUAIdx = 0;

  fParReader = NULL;
}

STGainCheck::~STGainCheck()
{
}

void
STGainCheck::SetUAIndex(Int_t uaIdx)
{
  fUAIdx = uaIdx;

  Int_t uaRow = fUAIdx%100;
  Int_t uaLayer = fUAIdx/100;
  
  fMinRow = uaRow*9;
  fMaxRow = (uaRow + 1)*9;
  fMinLayer = uaLayer*28;
  fMaxLayer = (uaLayer + 1)*28;

  for (Int_t iLayer = fMinLayer; iLayer < fMaxLayer; iLayer++) {
    for (Int_t iRow = fMinRow; iRow < fMaxRow; iRow++) {
      Int_t padIdx = GetPadIndex(iRow, iLayer);

      fPadName[padIdx] = Form("pad_%d_%d", iRow, iLayer);
    }
  }
}

void
STGainCheck::SetGainReference(Double_t constant, Double_t linear, Double_t quadratic)
{
  fCore -> SetGainReference(constant, linear, quadratic);
}

void
STGainCheck::SetGainReference(Int_t row, Int_t layer)
{
  fCore -> SetGainReference(row, layer);
}

void
STGainCheck::SetInternalPedestal(Int_t startTb, Int_t numTbs)
{
  fCore -> SetInternalPedestal(startTb, numTbs);
}

void
STGainCheck::SetFPNPedestal(Double_t sigmaThreshold)
{
  fCore -> SetFPNPedestal(sigmaThreshold);
}


Bool_t
STGainCheck::SetPedestalData(TString filename)
{
  return fCore -> SetPedestalData(filename);
}

Bool_t
STGainCheck::AddPulserData(TString filename)
{
  return fCore -> AddData(filename);
}

void
STGainCheck::SetGainCalibrationData(TString filename, TString dataType)
{
  fCore -> SetGainCalibrationData(filename, dataType);
}

TCanvas *
STGainCheck::DrawSpectra()
{
  if (fCanvas)
    delete fCanvas;

  if (fSpectra)
    delete fSpectra;

  gStyle -> SetOptStat(0);
  gStyle -> SetPadLeftMargin(0.07);
  gStyle -> SetPadRightMargin(0.045);
  gStyle -> SetTitleSize(0.05, "x");
  gStyle -> SetTitleSize(0.05, "y");
  gStyle -> SetLabelSize(0.02, "x");
  gStyle -> SetLabelSize(0.05, "y");
  gStyle -> SetTitleOffset(0.65, "x");
  gStyle -> SetTitleOffset(0.72, "y");
  gStyle -> SetTickLength(0.010, "y");
  gStyle -> SetPalette(55);
  gStyle -> SetNumberContours(100);

  fCanvas = new TCanvas("GainCheckCvs", "", 1400, 500);
  fCanvas -> SetLogz();

  fSpectra = new TH2D("GainCheckHist", Form("Gain Check - UAIdx:%03d", fUAIdx), 252, 0, 252, 2048, 0, 4096);
  fSpectra -> SetBit(TH1::kCanRebin);

  for (Int_t iLayer = fMinLayer; iLayer < fMaxLayer; iLayer++) {
    for (Int_t iRow = fMinRow; iRow < fMaxRow; iRow++) {
      Int_t padIdx = GetPadIndex(iRow, iLayer);

      fSpectra -> Fill(fPadName[padIdx].Data(), 0., 0.);
    }
  }

  fSpectra -> Reset();

  fCore -> SetNoAutoReload();

  Int_t numVoltages = fCore -> GetNumData();

  for (Int_t iVoltage = 0; iVoltage < numVoltages; iVoltage++) {
    fCore -> SetData(iVoltage);
    STRawEvent *event = NULL;
    while ((event = fCore -> GetRawEvent())) {
//    for (Int_t iEvent = 0; iEvent < 100; iEvent++) {
//      STRawEvent *event = fCore -> GetRawEvent();

      cout << "Start data: " << iVoltage << " event: " << event -> GetEventID() << endl;

      Int_t numPads = event -> GetNumPads();
      for (Int_t iPad = 0; iPad < numPads; iPad++) {
        STPad *pad = event -> GetPad(iPad);

        Int_t row = pad -> GetRow();
        Int_t layer = pad -> GetLayer();

        if (!(row >= fMinRow && row < fMaxRow && layer >= fMinLayer && layer < fMaxLayer))
          continue;

        Double_t *adc = pad -> GetADC();

        Double_t max = -10;
        for (Int_t iTb = 0; iTb < fNumTbs; iTb++) {
          if (adc[iTb] > max)
            max = adc[iTb];
        }

        Int_t padIdx = GetPadIndex(row, layer);

        fSpectra -> Fill(fPadName[padIdx].Data(), max, 1);
      }

      cout << "Done voltage: " << iVoltage << " event: " << event -> GetEventID() << endl;
    }
  }

  fCanvas -> cd();
  fSpectra -> GetYaxis() -> SetRangeUser(0, 4096);
  fSpectra -> GetYaxis() -> SetTitle("ADC Ch.");
  fSpectra -> GetYaxis() -> CenterTitle();
  fSpectra -> Draw("colz");

  fCanvas -> Update();

  TPaletteAxis *axis = (TPaletteAxis *) fSpectra -> GetListOfFunctions() -> FindObject("palette");
  axis -> SetX2NDC(0.975);
  axis -> GetAxis() -> SetTickSize(0.008);
  axis -> SetLabelSize(0.045);
  axis -> SetLabelOffset(-0.005);
  axis -> Draw("same");

  return fCanvas;
}

Bool_t
STGainCheck::SetParameterDir(TString dir)
{
  TString parameterFile = dir;
  parameterFile.Append("/ST.parameters.par");

  if (fParReader != NULL)
    delete fParReader;

  fParReader = new STParReader(parameterFile);
  if (!(fParReader -> IsGood()))
    return kFALSE;

  fNumTbs = fParReader -> GetIntPar("NumTbs");
  fCore -> SetNumTbs(fNumTbs);

  fRows = fParReader -> GetIntPar("PadRows");
  fLayers = fParReader -> GetIntPar("PadLayers");
  fPadX = fParReader -> GetDoublePar("PadSizeX");
  fPadZ = fParReader -> GetDoublePar("PadSizeZ");

  Int_t uaMapIndex = fParReader -> GetIntPar("UAMapFile");
  TString uaMapFile = fParReader -> GetFilePar(uaMapIndex);
  Bool_t okay = fCore -> SetUAMap(uaMapFile);
  if (okay)
    cout << "== [STGainCheck] Unit AsAd mapping file set: " << uaMapFile << endl;
  else {
    cout << "== [STGainCheck] Cannot find Unit AsAd mapping file!" << endl;

    return kFALSE;
  }

  Int_t agetMapIndex = fParReader -> GetIntPar("AGETMapFile");
  TString agetMapFile = fParReader -> GetFilePar(agetMapIndex);
  okay = fCore -> SetAGETMap(agetMapFile);
  if (okay)
    cout << "== [STGainCheck] AGET mapping file set: " << agetMapFile << endl;
  else {
    cout << "== [STGainCheck] Cannot find AGET mapping file!" << endl;

    return kFALSE;
  }

  return kTRUE;
}

Int_t
STGainCheck::GetPadIndex(Int_t row, Int_t layer)
{
  return (row - fMinRow)*28 + layer - fMinLayer;
}
