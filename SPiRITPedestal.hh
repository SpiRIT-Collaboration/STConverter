// =================================================
//  SPiRITPedestal Class
// 
//  Description:
//    This class is used for calculating or finding
//    pedestal value corresponding to user-input
//    agetIdx & chIdx.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 14
// =================================================

#ifndef STPCPEDESTAL_H
#define STPCPEDESTAL_H

#include "TROOT.h"

class TFile;
class TTree;

#include <fstream>

class SPiRITPedestal {
  public:
    SPiRITPedestal();
    SPiRITPedestal(Char_t *pedestalData);
    ~SPiRITPedestal() {};

    void Initialize();
    void SetPedestalData(Char_t *pedestalData);

    // Calculate pedestal value(0) and sigma(1) array from first 20 buckets
    void GetPedestal(Int_t coboIdx, Int_t asadIdx, Int_t agetIdx, Int_t chIdx, Double_t *pedestal);

    // Look up pedestal value(0) and sigma(1) array from pedestal run data
    void GetPedestal(Int_t *samples, Double_t *pedestal);

  private:
    TFile *openFile;
    TTree *pedestalTree;

    Double_t pedestal;
    Double_t pedestalSigma;

  ClassDef(SPiRITPedestal, 1);
};

#endif
