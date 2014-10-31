// =================================================
//  STMapConverter Class
//
//  Description:
//    Convert mapping data
//    between STConverter and get-config-wizard
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2014. 10. 31
// =================================================

#pragma once

#include "STMap.hh"

#include "TObject.h"
#include "TString.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::setw;
using std::ios;

class STMapConverter : public TObject {
  public:
    STMapConverter();

    Bool_t LoadSTMap(TString uaMap, TString agetMap);
    Bool_t LoadGETMap(TString filename);

    void SaveSTMap(TString uaMap, TString agetMap);
    void SaveGETMap(TString filename);

  private:
    STMap *fMap;

    Bool_t fIsSTMap;
    Bool_t fIsGETMap;

  ClassDef(STMapConverter, 1)
};
