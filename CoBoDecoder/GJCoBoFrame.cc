// =================================================
//  GJCoBoFrame Class
// 
//  Description:
//    Storage of a frame from CoBo
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 09. 23
//      Start writing class
// =================================================

#include "GJCoBoFrame.hh"

ClassImp(GJCoBoFrame);

GJCoBoFrame::GJCoBoFrame()
{
  eventIdx = 0;
  coboIdx = 0;
  asadIdx = 0;

  for (Int_t i = 0; i < 4*68*512; i++)
    adc[i] = 0;
}

GJCoBoFrame::~GJCoBoFrame()
{
}

void GJCoBoFrame::SetEventID(UInt_t value)
{
  eventIdx = value;
}

void GJCoBoFrame::SetCoboID(UShort_t value)
{
  coboIdx = value;
}

void GJCoBoFrame::SetAsadID(UShort_t value)
{
  asadIdx = value;
}

void GJCoBoFrame::SetADC(UShort_t agetIdx, UShort_t chIdx, UShort_t buckIdx, UShort_t value)
{
  Int_t index = agetIdx*68*512 + chIdx*512 + buckIdx;

  adc[index] = value;
}

UInt_t GJCoBoFrame::GetEventID()
{
  return eventIdx;
}

Int_t GJCoBoFrame::GetCoboID()
{
  return coboIdx;
}

Int_t GJCoBoFrame::GetAsadID()
{
  return asadIdx;
}

Int_t *GJCoBoFrame::GetADC(Int_t agetIdx, Int_t chIdx)
{
  Int_t index = agetIdx*68*512 + chIdx*512;

  return adc + index;
}

Int_t GJCoBoFrame::GetADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx)
{
  Int_t index = agetIdx*68*512 + chIdx*512 + buckIdx;

  return adc[index]; 
}
