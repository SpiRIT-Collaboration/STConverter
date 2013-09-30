// =================================================
//  GETFrame Class
// 
//  Description:
//    Storage of a frame from GET electronics
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 09. 23
//      Start writing class
// =================================================

#include "GETFrame.hh"

ClassImp(GETFrame);

GETFrame::GETFrame()
{
  fEventIdx = 0;
  fCoboIdx = 0;
  fAsadIdx = 0;

  for (Int_t i = 0; i < 4*68*512; i++)
    fAdc[i] = 0;
}

GETFrame::~GETFrame()
{
}

void GETFrame::SetEventID(UInt_t value)
{
  fEventIdx = value;
}

void GETFrame::SetCoboID(UShort_t value)
{
  fCoboIdx = value;
}

void GETFrame::SetAsadID(UShort_t value)
{
  fAsadIdx = value;
}

void GETFrame::SetADC(UShort_t agetIdx, UShort_t chIdx, UShort_t buckIdx, UShort_t value)
{
  Int_t index = agetIdx*68*512 + chIdx*512 + buckIdx;

  fAdc[index] = value;
}

UInt_t GETFrame::GetEventID()
{
  return fEventIdx;
}

Int_t GETFrame::GetCoboID()
{
  return fCoboIdx;
}

Int_t GETFrame::GetAsadID()
{
  return fAsadIdx;
}

Int_t *GETFrame::GetADC(Int_t agetIdx, Int_t chIdx)
{
  Int_t index = agetIdx*68*512 + chIdx*512;

  return fAdc + index;
}

Int_t GETFrame::GetADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx)
{
  Int_t index = agetIdx*68*512 + chIdx*512 + buckIdx;

  return fAdc[index]; 
}
