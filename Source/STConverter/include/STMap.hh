// =================================================
//  STMap Class
// 
//  Description:
//    This class is used for finding the local pad
//    coordinates corresponding to user-input
//    agetIdx & chIdx using map by Tadaaki Isobe.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 13
// =================================================

#ifndef STMAP_H
#define STMAP_H

#include "TROOT.h"
#include "TObject.h"
#include "TString.h"

#include <map>
#include <fstream>

using std::map;

class STMap : public TObject {
  public:
    STMap();
    ~STMap() {}

    // Setter
    Bool_t SetUAMap(TString filename);
    Bool_t SetAGETMap(TString filename);

    // Getter
    Bool_t IsSetUAMap();
    Bool_t IsSetAGETMap();

    Bool_t GetRowNLayer(Int_t coboIdx, Int_t asadIdx, Int_t agetIdx, Int_t chIdx, Int_t &padRow, Int_t &padLayer);
    Bool_t GetMapData(Int_t padRow, Int_t padLayer, Int_t &UAIdx, Int_t &coboIdx, Int_t &asadIdx, Int_t &agetIdx, Int_t &chIdx);

    Int_t GetUAIdx(Int_t coboIdx, Int_t asadIdx);
    Int_t GetCoboIdx(Int_t uaIdx);
    Int_t GetAsadIdx(Int_t uaIdx);

    void SetUAMap(Int_t uaIdx, Int_t coboIdx, Int_t asadIdx);
    void SetAGETMap(Int_t chIdx, Int_t padRow, Int_t padLayer);
    void GetAGETMap(Int_t chIdx, Int_t &padRow, Int_t &padLayer);

  private:
    Bool_t fIsSetUAMap;
    Bool_t fIsSetAGETMap;

    Int_t fPadRowOfCh[68];
    Int_t fPadLayerOfCh[68];

    Int_t fUAMap[12][4];

    std::fstream fStream;

  ClassDef(STMap, 1);
};

#endif
