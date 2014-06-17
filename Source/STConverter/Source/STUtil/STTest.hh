// =================================================
//  STTest Class
// 
//  Description:
//    Test AGET & UnitAsAd map with plot
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 30
// =================================================

#ifndef STTEST_H
#define STTEST_H

#include "TObject.h"

#include "TCanvas.h"
#include "TH2D.h"
#include "TList.h"

#include "STMap.hh"

class STTest : public TObject
{
  public:
    STTest();
    ~STTest();

    void ShowAGETMap();
    void ShowUAMap();
    void PrintMap(Int_t padRow, Int_t padLayer);

  private:
    STMap *map;

    TCanvas *agetCvs;
    TH2D *agetHist;
    TList *agetChList;
    TList *agetLineList;

    TCanvas *uaCvs;
    TH2D *uaHist;
    TList *uaList;
    TList *uaMapList;
    TList *uaLineList;

  ClassDef(STTest, 1);
};

#endif
