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

#include "SPiRITMap.hh"

ClassImp(SPiRITMap);

SPiRITMap::SPiRITMap() {
  Int_t definePadRowOfCh[68] = {-2,1,2,2,2,1,3,1,3,0,3,-2,0,4,0,4,2,4,1,3,0,3,-2,0,3,0,3,0,2,1,2,1,2,1,7,6,6,6,7,5,7,5,8,5,8,-2,4,8,5,6,4,7,4,8,4,8,-2,5,8,5,8,5,7,6,7,6,7,6};
  Int_t definePadLayerOfCh[68] = {-2,0,1,0,2,1,0,2,1,0,2,-2,1,0,2,1,3,2,3,3,3,4,-2,4,5,5,6,6,6,6,4,4,5,5,2,0,2,1,0,0,1,1,0,2,1,-2,3,2,3,3,4,3,5,3,6,4,-2,5,5,4,6,6,5,6,4,4,6,5};

  memcpy(padRowOfCh, definePadRowOfCh, sizeof(definePadRowOfCh));
  memcpy(padLayerOfCh, definePadLayerOfCh, sizeof(definePadLayerOfCh));
}

void SPiRITMap::GetRowNLayer(Int_t coboIdx, Int_t asadIdx, Int_t agetIdx, Int_t chIdx, Short_t &padRow, Short_t &padLayer) {
  if (padLayerOfCh[chIdx] == -2) {
    padLayer = -2;
    padRow = -2;

    return;
  }

  padRow = coboIdx*9 + padRowOfCh[chIdx];
  padLayer = asadIdx*28 + agetIdx*7 + padLayerOfCh[chIdx];

  return;
}
