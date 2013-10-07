// =================================================
//  STCore Class
// 
//  Description:
//    Process CoBoFrame data into STRawEvent data
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 09. 25
// =================================================

#ifndef _STCORE_H_
#define _STCORE_H_

#include "GETConfig.hh"

#include "TObject.h"

class STRawEvent;
class STMap;
class STPedestal;
class GETDecoder;
class GETFrame;

class STCore : public TObject {
  public:
    STCore();
    STCore(Char_t *filename);
    ~STCore();

    void Initialize();

    // setters
    void SetGraw(Char_t *filename);
    void SetInternalPedestal(Int_t startTb = 10, Int_t numTbs = 20);
    void SetPedestalData(Char_t *filename);

    // getters
    STRawEvent *GetRawEvent(Int_t eventID = -1);

  private:
    STMap *fMapPtr;

    GETDecoder *fDecoderPtr;
    Bool_t fIsGraw;

    STPedestal *fPedestalPtr;
    Bool_t fIsInternalPedestal;
    Bool_t fIsPedestalData;
    Int_t fStartTb;
    Int_t fNumTbs;

    STRawEvent *fRawEventPtr;

    UInt_t fPrevEventNo;
    UInt_t fCurrEventNo;

    Int_t fCurrFrameNo;

  ClassDef(STCore, 1);
};

#endif
