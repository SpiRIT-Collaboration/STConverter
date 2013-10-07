// =================================================
//  GETDecoder Class
// 
//  Description:
//    Read the raw file from GET electronics
//    and process it into GETFrame class
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

#include "GETConfig.hh"

#include "TObject.h"
#include "TROOT.h"
#include "TString.h"

class GETFrame;
class GETPlot;

class GETDecoder : public TObject
{
  public:
    GETDecoder();
    GETDecoder(Char_t *filename);
    ~GETDecoder();

    // setters
    void SetDebugMode(Bool_t value);
    Bool_t SetGraw(const Char_t *filename);

    // getters
    GETPlot *GetGETPlot();
    Int_t GetNumFrames();
    Int_t GetCurrentFrameID();
    GETFrame *GetFrame();
    GETFrame *GetFrame(Int_t frameIdx);

  private:
    void Initialize();

    Bool_t fDebugMode;
    void PrintFrameInfo(Int_t frameID, Int_t eventID, Int_t coboID, Int_t asadID);

    Bool_t SetFile(const Char_t *filename);
    void CountFrames();
    Bool_t IsNextFile();

    Int_t fNumFrames;
  
    std::ifstream fGraw;
    TString fFirstGraw;
    TString fNextGraw;

    GETFrame *fFrame;
    Int_t fCurrentFrameID;

    GETPlot *fGETPlot;

  ClassDef(GETDecoder, 1);
};

#endif
