// =================================================
//  GJCoBoDecoder Class
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

#ifndef _GJCOBODECODER_H_
#define _GJCOBODECODER_H_

#include <fstream>

#include "TObject.h"
#include "TROOT.h"
#include "TString.h"

class GJCoBoFrame;

class GJCoBoDecoder : public TObject
{
  public:
    GJCoBoDecoder();
    GJCoBoDecoder(Char_t *filename);
    ~GJCoBoDecoder();

    void Initialize();

    // setters
    void SetDebugMode(Bool_t value);
    void SetGrawFile(const Char_t *filename);

    // getters
    Int_t GetNumFrames();
    GJCoBoFrame *GetFrame();
    GJCoBoFrame *GetFrame(Int_t frameNo);

  private:
    Bool_t debugMode;
    void PrintFrameInfo(Int_t frameNo, Int_t eventID, Int_t coboID, Int_t asadID);

    void CountFrames();
    Bool_t NextFile();

    Int_t numFrames;
  
    std::ifstream grawFile;
    TString firstGrawFile;
    TString nextGrawFile;

    GJCoBoFrame *aFrame;
    Int_t currentFrameNo;

  ClassDef(GJCoBoDecoder, 1);
};

#endif
