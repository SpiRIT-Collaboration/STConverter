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

class TCanvas;
class TH2D;
class STMap;
class TList;

class STTest : public TObject
{
  public:
    STTest();
    ~STTest();

    void ShowAGETMap();
    void ShowUAMap();

  private:
    STMap *map;

    TCanvas *agetCvs;
    TH2D *agetHist;
    TList *agetChList;
    TList *agetLineList;

    TCanvas *uaCvs;
    TH2D *uaHist;
    TList *uaList;
    TList *uaLineList;

  ClassDef(STTest, 1);
};

#endif
