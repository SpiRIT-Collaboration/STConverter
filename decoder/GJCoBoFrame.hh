// =================================================
//  GJCoBoFrame Class
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

#ifndef _GJCOBOFRAME_H_
#define _GJCOBOFRAME_H_

#include "TObject.h"

class GJCoBoFrame : public TObject
{
  public:
    GJCoBoFrame();
    ~GJCoBoFrame();

    // setters
    void SetEventID(UInt_t value);
    void SetCoboID(UShort_t value);
    void SetAsadID(UShort_t value);

    void SetADC(UShort_t agetIdx, UShort_t chIdx, UShort_t buckIdx, UShort_t value);

    // getters
    UInt_t GetEventID();
    Int_t GetCoboID();
    Int_t GetAsadID();

    Int_t *GetADC(Int_t agetIdx, Int_t chIdx);
    Int_t GetADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx);

  private:
    UInt_t eventIdx;
    Int_t coboIdx;
    Int_t asadIdx;
    Int_t adc[4*68*512];

  ClassDef(GJCoBoFrame, 1);
};

#endif
