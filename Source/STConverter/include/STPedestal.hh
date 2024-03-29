// =================================================
//  STPedestal Class
// 
//  Description:
//    This class is used for calculating or finding
//    pedestal value and sigma corresponding to
//    user-input padRow and padLayer.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 14
// =================================================

#ifndef STPEDESTAL_H
#define STPEDESTAL_H

#include "TROOT.h"
#include "TObject.h"

#include "TFile.h"
#include "TTree.h"

#include <fstream>

class STPedestal : public TObject {
  public:
    STPedestal();
    STPedestal(TString pedestalData);
    ~STPedestal() {};

    void Initialize();
    Bool_t SetPedestalData(TString filename);

    Bool_t IsSetPedestalData();
    void GetPedestal(Int_t padRow, Int_t padLayer, Double_t *pedestal, Double_t *pedestalSigma);

  private:
    TFile *fOpenFile;
    TTree *fPedestalTree;

    Bool_t fIsSetPedestalData;

    Double_t fPedestal[108][112][512];
    Double_t fPedestalSigma[108][112][512];

  ClassDef(STPedestal, 1);
};

#endif
