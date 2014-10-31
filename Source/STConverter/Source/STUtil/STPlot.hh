// =================================================
//  STPlot Class
//
//  Description:
//    Plot event data
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 24
// =================================================

#ifndef STPLOT_H
#define STPLOT_H

#include "TROOT.h"

#include "STRawEvent.hh"
#include "STPad.hh"

#include "TCanvas.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TLine.h"

class STPlot : public TObject
{
  public:
    STPlot();
    ~STPlot() {};

    void SetAutodelete(Bool_t value);
    
    void DrawPadplane();
    void DrawADC(Int_t padNo);
    void DrawLayer(Int_t layer);

    // Setters
    void SetEvent(STRawEvent *anEvent);

    // Getters

  private:
    void Clear();
    void PreparePadplaneHist();
    Bool_t CheckEvent();

    Bool_t isAutodelete;
    STRawEvent *event;
    TCanvas *padplaneCvs;
    TH2D *padplaneHist;
    TH1D *padHist;
    TH2D *layerHist;
    TLine *horizLine[11];
    TLine *vertLine[95];

  ClassDef(STPlot, 1);
};

#endif
