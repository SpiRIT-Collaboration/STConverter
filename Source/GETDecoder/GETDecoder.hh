// =================================================
//  GETDecoder Class
// 
//  Description:
//    Read the raw file from CoBo and process it
//    into GETEvent class
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 09. 23
//      Start writing class
// =================================================

#ifndef _GETDECODER_H_
#define _GETDECODER_H_

#include <fstream>

#include "TObject.h"
#include "TROOT.h"
#include "TString.h"

class GETFrame;

class GETDecoder : public TObject
{
  public:
    GETDecoder();
    GETDecoder(Char_t *filename);
    ~GETDecoder();

    void Initialize();

    // setters
    void SetDebugMode(Bool_t value);
    Bool_t SetGraw(const Char_t *filename);

    // getters
    Int_t GetNumFrames();
    GETFrame *GetFrame();
    GETFrame *GetFrame(Int_t frameNo);

  private:
    Bool_t fIsDebugMode;
    void PrintFrameInfo(Int_t frameNo, Int_t eventID, Int_t coboID, Int_t asadID);

    void CountFrames();
    Bool_t IsNextFile();

    Int_t fNumFrames;
  
    std::ifstream fGraw;
    TString fFirstGraw;
    TString fNextGraw;

    GETFrame *fFrame;
    Int_t fCurrentFrameNo;

  ClassDef(GETDecoder, 1);
};

#endif
