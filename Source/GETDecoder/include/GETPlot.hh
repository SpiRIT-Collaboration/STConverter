// =================================================
//  GETPlot Class
// 
//  Description:
//    Thinking what to write
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 10. 01
//      Start writing class
// =================================================

#ifndef _GETPLOT_H_
#define _GETPLOT_H_

#include "TObject.h"

class GETDecoder;
class GETFrame;
class TCanvas;
class TGraph;

class GETPlot : public TObject
{
  public:
    GETPlot();
    GETPlot(GETDecoder *decoder);
    ~GETPlot();

    // setters
    void SetDecoder(GETDecoder *decoder);

    void SetAgetRange(Double_t minx, Double_t maxx, Double_t miny, Double_t maxy);

    // getters
    TCanvas *ShowSummarySpectra();
    TCanvas *ShowRawFrame(Int_t frameNo = -1, Int_t numChannels = 0, Int_t *chList = NULL);
    TCanvas *ShowFrame(Int_t frameNo = -1, Int_t startTb = 10, Int_t numTbs = 20, Int_t numChannels = 0, Int_t *chList = NULL);
    TCanvas *ShowAverage(Int_t numChannels, Int_t *chList, Int_t frameNo = -1);

  private:
    void Initialize();

    Int_t minTb;
    Int_t maxTb;

    GETDecoder *fDecoder;
    GETFrame *fFrame;
    TCanvas *PrepareCanvas(Int_t type);
    void ResetGraph(Int_t type, Bool_t first = 0);

    TGraph *fAget[4];

    Double_t fAgetMinX;
    Double_t fAgetMaxX;
    Double_t fAgetMinY;
    Double_t fAgetMaxY;

  ClassDef(GETPlot, 1);
};

#endif
