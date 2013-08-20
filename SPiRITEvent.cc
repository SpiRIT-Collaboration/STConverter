// =================================================
//  SPiRITEvent Class                          
//                                                  
//  Description:                                    
//    Container for an event data
//                                                  
//  Genie Jhang ( geniejhang@majimak.com )          
//  2013. 08. 16                                    
// =================================================

#include "Riostream.h"

#include "SPiRITEvent.hh"
#include "SPiRITPad.hh"

ClassImp(SPiRITEvent);

SPiRITEvent::SPiRITEvent() {
  currentRow = 0;
  currentLayer = 0;
}

SPiRITEvent::~SPiRITEvent() {
  for (Short_t iRow = 0; iRow < 108; iRow++)
    for (Short_t iLayer = 0; iLayer < 112; iLayer++)
      if (isPadFired[iRow][iLayer])
        delete pads[iRow][iLayer];
}

void SPiRITEvent::PrintPads() {
  Int_t padNo = 0;
  for (Short_t iRow = 0; iRow < 108; iRow++)
    for (Short_t iLayer = 0; iLayer < 112; iLayer++)
      if (isPadFired[iRow][iLayer]) {
        cout << "Pad Number: " << setw(5) << padNo << " (" << setw(3) << iRow << ", " << setw(3) << iLayer << ")" << endl;
        padNo++;
      }
}

// Setters
void SPiRITEvent::SetPad(SPiRITPad *pad) {
  Short_t row = pad -> GetPadRow();
  Short_t layer = pad -> GetPadLayer();
  
  isPadFired[row][layer] = 1;
  pads[row][layer] = pad;
}

// Getters
SPiRITPad *SPiRITEvent::GetNextPad() {
}

SPiRITPad *SPiRITEvent::GetPad(Short_t row, Short_t layer) {
  if (isPadFired[row][layer])
    return pads[row][layer];
  else
    return NULL;
}

Int_t SPiRITEvent::GetNumPads() {
  firedPads = 0;
  for (Short_t iRow = 0; iRow < 108; iRow++)
    for (Short_t iLayer = 0; iLayer < 112; iLayer++)
      if (isPadFired[iRow][iLayer])
        firedPads++;

  return firedPads;
}
