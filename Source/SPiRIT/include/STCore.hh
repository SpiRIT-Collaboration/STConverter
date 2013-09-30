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
    void SetPedestalData(Char_t *filename);

    // getters
    STRawEvent *GetRawEvent();
    STRawEvent *GetRawEvent(Int_t eventID);

  private:
    Int_t fStoppedFrameNo;

    STRawEvent *fRawEventPtr;
    STMap *fMapPtr;
    STPedestal *fPedestalPtr;

    GETDecoder *fDecoderPtr;
    GETFrame *fFramePtr;

    Bool_t fIsGraw;
    Bool_t fIsPedestalData;

  ClassDef(STCore, 1);
};

#endif
