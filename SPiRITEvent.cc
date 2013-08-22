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
  currentPadNo = 0;

  for (Int_t iPad = 0; iPad < 12096; iPad++) {
    pads[iPad] = 0;
    isPadFired[iPad] = 0;
  }
}

SPiRITEvent::~SPiRITEvent() {
  for (Int_t iPad = 0; iPad < 12096; iPad++)
    if (isPadFired[iPad]) {
      delete pads[iPad];

      isPadFired[iPad] = 0;
      pads[iPad] = 0;
    }
}

void SPiRITEvent::PrintPads() {
  for (Int_t iPad = 0; iPad < 12096; iPad++)
    if (isPadFired[iPad])
      cout << "Pad Number: " << setw(5) << iPad << " (" << setw(3) << iPad%108 << ", " << setw(3) << iPad/108 << ")" << endl;
}

// Setters
void SPiRITEvent::SetPad(SPiRITPad *pad) {
  Int_t row = pad -> GetPadRow();
  Int_t layer = pad -> GetPadLayer();
  
  Int_t globalPadNo = layer*108 + row;
  isPadFired[globalPadNo] = 1;
  pads[globalPadNo] = pad;
}

// Getters
SPiRITPad *SPiRITEvent::GetNextPad() {
  for (Int_t iPad = currentPadNo; iPad < 12096; iPad++)
    if (isPadFired[iPad]) {
      currentPadNo = iPad + 1;

      return pads[iPad];
    }

  return NULL;
}

SPiRITPad *SPiRITEvent::GetPad(Int_t row, Int_t layer) {
  Int_t globalPadNo = layer*108 + row;
  if (isPadFired[globalPadNo])
    return pads[globalPadNo];
  else
    return NULL;
}

Int_t SPiRITEvent::GetNumPads() {
  firedPads = 0;

  for (Int_t iPad = 0; iPad < 12096; iPad++)
    if (isPadFired[iPad])
      firedPads++;

  return firedPads;
}
