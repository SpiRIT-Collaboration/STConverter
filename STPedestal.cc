// =================================================
//  STPedestal Class
// 
//  Description:
//    This class is used for calculating or finding
//    pedestal value and sigma corresponding to
//    user-input coboIdx, asadIdx, agetIdx & chIdx.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 14
// =================================================

#include "STPedestal.hh"

#include "Riostream.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"

ClassImp(STPedestal);

STPedestal::STPedestal() {
  Initialize();
}

STPedestal::STPedestal(Char_t *pedestalData) {
  Initialize();

  SetPedestalData(pedestalData);
}

void STPedestal::Initialize() {
  usePedestalData = 0;

  openFile = NULL;
  pedestalTree = NULL;

  pedestal = 0;
  pedestalSigma = 0;
}

void STPedestal::SetPedestalData(Char_t *pedestalData) {
  if (openFile != NULL)
    delete openFile;

  if ((openFile = new TFile(pedestalData))) {
    pedestalTree = (TTree *) openFile -> Get("pedestal");
    pedestalTree -> SetBranchAddress("pedestal", &pedestal);
    pedestalTree -> SetBranchAddress("pedestalSigma", &pedestalSigma);

    SetUsePedestalData(1);
  }
}

Bool_t STPedestal::GetUsePedestalData()
{
  return usePedestalData;
}

void STPedestal::SetUsePedestalData(Bool_t value)
{
  usePedestalData = value;
}

void STPedestal::GetPedestal(Int_t *samples, Double_t *pedestalArray, Int_t startIdx, Int_t numPedestalSamples) {
  Initialize();
  
  for (Int_t i = startIdx; i < startIdx + numPedestalSamples; i++)
    pedestal += samples[i];

  pedestal /= (Double_t) numPedestalSamples;

  for (Int_t i = startIdx; i < startIdx + numPedestalSamples; i++)
    pedestalSigma += TMath::Power(pedestal - (Double_t) samples[i], 2);

  pedestalArray[0] = pedestal;
  pedestalArray[1] = TMath::Sqrt(pedestalSigma/(Double_t)numPedestalSamples);

  return;
}

void STPedestal::GetPedestal(Int_t coboIdx, Int_t asadIdx, Int_t agetIdx, Int_t chIdx, Double_t *pedestalArray) {
  if (openFile == NULL) {
    cerr << "Pedestal data file is not set!" << endl;

    return;
  }
  // To one CoBo, connected are 4 AsAds, one of which has 4 AGETs composed of 68 channels
  Int_t pedestalIndex = coboIdx*(68*4*4) + asadIdx*(68*4) + agetIdx*68 + chIdx;

  pedestalTree -> GetEntry(pedestalIndex);
  
  pedestalArray[0] = pedestal;
  pedestalArray[1] = pedestalSigma;

  return;
}
