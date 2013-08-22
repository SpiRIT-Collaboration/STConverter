// =================================================
//  SPiRITPedestal Class
// 
//  Description:
//    This class is used for calculating or finding
//    pedestal value and sigma corresponding to
//    user-input agetIdx & chIdx.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 14
// =================================================

#include "SPiRITPedestal.hh"

#include "TMath.h"
#include "TFile.h"
#include "TTree.h"

ClassImp(SPiRITPedestal);

SPiRITPedestal::SPiRITPedestal() {
  Initialize();
}

SPiRITPedestal::SPiRITPedestal(Char_t *pedestalData) {
  Initialize();

  SetPedestalData(pedestalData);
}

void SPiRITPedestal::Initialize() {
  pedestal = 0;
  pedestalSigma = 0;
}

void SPiRITPedestal::SetPedestalData(Char_t *pedestalData) {
  openFile = new TFile(pedestalData);

  pedestalTree = (TTree *) openFile -> Get("pedestal");
    pedestalTree -> SetBranchAddress("pedestal", &pedestal);
    pedestalTree -> SetBranchAddress("pedestalSigma", &pedestalSigma);
}

void SPiRITPedestal::GetPedestal(Int_t *samples, Double_t *pedestalArray) {
  Initialize();
  
  Int_t numPedestalSamples = 20;

  for (Int_t i = 0; i < numPedestalSamples; i++)
    pedestal += samples[i];

  pedestal /= (Double_t) numPedestalSamples;

  for (Int_t i = 0; i < numPedestalSamples; i++)
    pedestalSigma += TMath::Power(pedestal - (Double_t) samples[i], 2);

  pedestalArray[0] = pedestal;
  pedestalArray[1] = TMath::Sqrt(pedestalSigma/(Double_t)numPedestalSamples);

  return;
}

void SPiRITPedestal::GetPedestal(Int_t coboIdx, Int_t asadIdx, Int_t agetIdx, Int_t chIdx, Double_t *pedestalArray) {
  // To one CoBo, connected are 4 AsAds, one of which has 4 AGETs composed of 68 channels
  Int_t pedestalIndex = coboIdx*(68*4*4) + asadIdx*(68*4) + agetIdx*68 + chIdx;

  pedestalTree -> GetEntry(pedestalIndex);
  
  pedestalArray[0] = pedestal;
  pedestalArray[1] = pedestalSigma;

  return;
}
