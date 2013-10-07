// =================================================
//  GETFrame Class
// 
//  Description:
//    Storage of a frame from CoBo
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 09. 23
//      Start writing class
// =================================================

#ifndef _GETFRAME_H_
#define _GETFRAME_H_

#include "GETConfig.hh"
#ifdef __CINT__
extern const Int_t GETNumTbs;
#endif

#include "TObject.h"

class GETFrame : public TObject
{
  public:
    GETFrame();
    ~GETFrame();

    // setters
    void SetEventID(UInt_t value);
    void SetCoboID(UShort_t value);
    void SetAsadID(UShort_t value);
    void SetFrameID(Int_t value);

    void SetRawADC(UShort_t agetIdx, UShort_t chIdx, UShort_t buckIdx, UShort_t value);

    // getters
    UInt_t GetEventID();
    Int_t GetCoboID();
    Int_t GetAsadID();
    Int_t GetFrameID();

    Int_t *GetRawADC(Int_t agetIdx, Int_t chIdx);
    Int_t GetRawADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx);

    void CalcPedestal(Int_t startTb = 10, Int_t numTbs = 20);
    Int_t GetMaxADCIdx(Int_t agetIdx, Int_t chIdx);
    Double_t *GetADC(Int_t agetIdx, Int_t chIdx);
    Double_t GetADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx);

  private:
    Int_t GetIndex(Int_t agetIdx, Int_t chIdx, Int_t buckIdx);

    UInt_t fEventIdx;
    Int_t fCoboIdx;
    Int_t fAsadIdx;
    Int_t fFrameIdx;
    Int_t fRawAdc[4*68*GETNumTbs];

    Bool_t fPedestalSubtracted;
    Int_t fMaxAdcIdx[4*68];
    Double_t fAdc[4*68*GETNumTbs];

  ClassDef(GETFrame, 1);
};

#endif
