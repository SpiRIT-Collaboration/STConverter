// =================================================
//  STGraw Class
//
//  Description:
//    Container for graw event data
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 09. 11
// =================================================

#ifndef STGRAW_H
#define STGRAW_H

#include "TObject.h"

class STPedestal;

class STGraw : public TObject
{
  public:
    STGraw();
    ~STGraw() {};

    Int_t GetEventID();
    Int_t GetCoboID();
    Int_t GetAsadID();

    Int_t GetMaxADCIdx(Int_t agetIdx, Int_t chIdx);

    Int_t *GetRawADC(Int_t agetIdx, Int_t chIdx);
    Int_t GetRawADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx);
    Int_t GetMaxRawADC(Int_t agetIdx, Int_t chIdx);

    Double_t *GetADC(Int_t agetIdx, Int_t chIdx);
    Double_t GetADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx);
    Double_t GetMaxADC(Int_t agetIdx, Int_t chIdx);

    void SetEventID(Int_t value);
    void SetCoboID(Int_t value);
    void SetAsadID(Int_t value);
    void SetRawADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx, Int_t value);

    void Process(STPedestal *pedestalPtr);

  private:
    Int_t eventIdx;
    Int_t coboIdx;
    Int_t asadIdx;
    Int_t rawADC[4*68*512];
    Double_t adc[4*68*512];
    Int_t maxADCIdx[4*68];

    Bool_t isProcessed;

  ClassDef(STGraw, 1);
};

#endif
