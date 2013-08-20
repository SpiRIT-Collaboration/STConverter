// =================================================
//  SPiRITMap Class
// 
//  Description:
//    This class is used for finding the local pad
//    coordinates corresponding to user-input
//    agetIdx & chIdx using map by Tadaaki Isobe.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 13
// =================================================

#ifndef STPCMAP_H
#define STPCMAP_H

#include "TROOT.h"

class SPiRITMap {
  public:
    SPiRITMap();
    ~SPiRITMap() {};

    void GetRowNLayer(Int_t coboIdx, Int_t asadIdx, Int_t agetIdx, Int_t chIdx, Short_t &padRow, Short_t &padLayer);

  private:
    Int_t padRowOfCh[68];
    Int_t padLayerOfCh[68];

  ClassDef(SPiRITMap, 1);
};

#endif
