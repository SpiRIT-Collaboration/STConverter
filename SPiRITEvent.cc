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

SPiRITEvent::SPiRITEvent()
{
  firedPads = 0;
  currentPadNo = 0;

  for (Int_t iPad = 0; iPad < 12096; iPad++)
    pads[iPad] = 0;
}

SPiRITEvent::~SPiRITEvent()
{
  for (Int_t iPad = 0; iPad < firedPads; iPad++)
    delete pads[iPad];
}

void SPiRITEvent::PrintPads()
{
  for (Int_t iPad = 0; iPad < firedPads; iPad++)
    cout << "Pad: " << setw(5) << iPad << " (" << setw(3) << pads[iPad] -> GetPadRow() << ", " << setw(3) << pads[iPad] -> GetPadLayer() << ")" << endl;
}

void SPiRITEvent::GotoFirst()
{
   currentPadNo = 0;
}

// Setters
void SPiRITEvent::SetEventID(Int_t evtid)
{
  eventID = evtid;
}

void SPiRITEvent::SetPad(SPiRITPad *pad)
{
  pads[firedPads] = pad;
  firedPads++;
}

// Getters
Int_t SPiRITEvent::GetNumPads()
{
  return firedPads;
}

SPiRITPad *SPiRITEvent::GetNextPad()
{
  return (currentPadNo < firedPads ? pads[currentPadNo++] : NULL);
}

SPiRITPad *SPiRITEvent::GetPad(Int_t padNo)
{
  return (padNo < firedPads ? pads[padNo] : NULL);
}

SPiRITPad *SPiRITEvent::GetPad(Int_t row, Int_t layer)
{
  for (Int_t iPad = 0; iPad < firedPads; iPad++) {
    Int_t padRow = pads[iPad] -> GetPadRow();
    Int_t padLayer = pads[iPad] -> GetPadLayer();

    if (row == padRow && layer == padLayer)
      return pads[iPad];
  }

  return NULL;
}

Int_t SPiRITEvent::GetEventID()
{
  return eventID;
}
