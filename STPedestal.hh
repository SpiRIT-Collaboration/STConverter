// =================================================
//  STPedestal Class
// 
//  Description:
//    This class is used for calculating or finding
//    pedestal value corresponding to user-input
//    agetIdx & chIdx.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 14
// =================================================

#ifndef STPEDESTAL_H
#define STPEDESTAL_H

#include "TROOT.h"
#include "TObject.h"

class TFile;
class TTree;

#include <fstream>

class STPedestal : public TObject {
  public:
    STPedestal();
    STPedestal(Char_t *pedestalData);
    ~STPedestal() {};

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

  ClassDef(STPedestal, 1);
};

#endif
