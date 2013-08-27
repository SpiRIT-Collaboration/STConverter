// =================================================
//  STEvent Class                          
//                                                  
//  Description:                                    
//    Container for an event data
//                                                  
//  Genie Jhang ( geniejhang@majimak.com )          
//  2013. 08. 16                                    
// =================================================

#include "Riostream.h"

#include "STEvent.hh"
#include "STPad.hh"

ClassImp(STEvent);

STEvent::STEvent()
{
  firedPads = 0;
  currentPadNo = 0;

  for (Int_t iPad = 0; iPad < 12096; iPad++)
    pads[iPad] = 0;
}

STEvent::~STEvent()
{
  for (Int_t iPad = 0; iPad < firedPads; iPad++)
    delete pads[iPad];
}

void STEvent::PrintPads()
{
  for (Int_t iPad = 0; iPad < firedPads; iPad++)
    cout << "Pad: " << setw(5) << iPad << " (" << setw(3) << pads[iPad] -> GetPadRow() << ", " << setw(3) << pads[iPad] -> GetPadLayer() << ")" << endl;
}

void STEvent::GotoFirst()
{
   currentPadNo = 0;
}

// Setters
void STEvent::SetEventID(Int_t evtid)
{
  eventID = evtid;
}

void STEvent::SetPad(STPad *pad)
{
  pads[firedPads] = pad;
  firedPads++;
}

// Getters
Int_t STEvent::GetNumPads()
{
  return firedPads;
}

STPad *STEvent::GetNextPad()
{
  return (currentPadNo < firedPads ? pads[currentPadNo++] : NULL);
}

STPad *STEvent::GetPad(Int_t padNo)
{
  return (padNo < firedPads ? pads[padNo] : NULL);
}

STPad *STEvent::GetPad(Int_t row, Int_t layer)
{
  for (Int_t iPad = 0; iPad < firedPads; iPad++) {
    Int_t padRow = pads[iPad] -> GetPadRow();
    Int_t padLayer = pads[iPad] -> GetPadLayer();

    if (row == padRow && layer == padLayer)
      return pads[iPad];
  }

  return NULL;
}

Int_t STEvent::GetEventID()
{
  return eventID;
}
