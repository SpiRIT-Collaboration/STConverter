// =================================================
//  STPad Class
//
//  Description:
//    Container for a pad data
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 16
// ================================================= 

#ifndef STPAD_H
#define STPAD_H

#include "TROOT.h"
#include "TObject.h"

class STPad : public TObject
{
  public:
    STPad();
    STPad(Int_t row, Int_t layer);
    ~STPad();

    void Initialize();
    void Process();

    // setters
    void SetLayer(Int_t val);
    void SetRow(Int_t val);
    void SetADC(Double_t *val);
    void SetADC(Int_t idx, Double_t val);

    // getters
    Int_t GetLayer();
    Int_t GetRow();
    Double_t *GetADC();
    Double_t GetADC(Int_t idx);
    Int_t GetMaxADCIdx();

  private:
    Int_t fLayer;
    Int_t fRow;  

    Int_t fMaxADCIdx;
    Double_t fADC[512];

  ClassDef(STPad, 1);
};

#endif
