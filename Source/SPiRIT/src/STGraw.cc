// =================================================
//  STGraw Class
//
//  Description:
//    Container for graw event data
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 09. 11
// =================================================

#include "Riostream.h"

#include "STGraw.hh"

#include "STPedestal.hh"

ClassImp(STGraw);

STGraw::STGraw()
{
  isProcessed = 0;

  eventIdx = 0;
  coboIdx = 0;
  asadIdx = 0;

  for (Int_t i = 0; i < 4*68*512; i++) {
    rawADC[i] = 0;
    adc[i] = 0;
  }

  for (Int_t i = 0; i < 4*68; i++)
    maxADCIdx[i] = 0;
}

STGraw::~STGraw()
{
}

Int_t STGraw::GetEventID()
{
  return eventIdx;
}

Int_t STGraw::GetCoboID()
{
  return coboIdx;
}

Int_t STGraw::GetAsadID()
{
  return asadIdx;
}

Int_t STGraw::GetMaxADCIdx(Int_t agetIdx, Int_t chIdx)
{
  Int_t index = agetIdx*68 + chIdx;

  return maxADCIdx[index];
}

Int_t *STGraw::GetRawADC(Int_t agetIdx, Int_t chIdx)
{
  Int_t index = agetIdx*68*512 + chIdx*512;

  return rawADC + index;
}

Int_t STGraw::GetRawADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx)
{
  Int_t index = agetIdx*68*512 + chIdx*512 + buckIdx;

  return rawADC[index]; 
}

Int_t STGraw::GetMaxRawADC(Int_t agetIdx, Int_t chIdx)
{
  Int_t maxIdx = GetMaxADCIdx(agetIdx, chIdx);

  return GetRawADC(agetIdx, chIdx, maxIdx);
}

Double_t *STGraw::GetADC(Int_t agetIdx, Int_t chIdx)
{
  if (!isProcessed)
    return NULL;

  Int_t index = agetIdx*68*512 + chIdx*512;

  return adc + index;
}

Double_t STGraw::GetADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx)
{
  if (!isProcessed)
    return -9999;

  Int_t index = agetIdx*68*512 + chIdx*512 + buckIdx;

  return adc[index];
}

Double_t STGraw::GetMaxADC(Int_t agetIdx, Int_t chIdx)
{
  if (!isProcessed)
    return -9999;

  Int_t maxIdx = GetMaxADCIdx(agetIdx, chIdx);

  return GetADC(agetIdx, chIdx, maxIdx);
}

void STGraw::SetEventID(Int_t value)
{
  eventIdx = value;
}

void STGraw::SetCoboID(Int_t value)
{
  coboIdx = value;
}

void STGraw::SetAsadID(Int_t value)
{
  asadIdx = value;
}

void STGraw::SetRawADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx, Int_t value)
{
  Int_t index = agetIdx*68*512 + chIdx*512 + buckIdx;

  rawADC[index] = value;
}

void STGraw::Process(STPedestal *pedestalPtr)
{
  for (Int_t agetIdx = 0; agetIdx < 4; agetIdx++) {
    for (Int_t chIdx = 0; chIdx < 68; chIdx++) {
      Int_t gChIdx = agetIdx*68 + chIdx;
      Double_t pedestal[2] = {0};

/*
      if (pedestalPtr -> GetUsePedestalData())
        pedestalPtr -> GetPedestal(coboIdx, asadIdx, agetIdx, chIdx, pedestal);
      else
        pedestalPtr -> GetPedestal(rawADC + gChIdx*512, pedestal, 10, 20);
        */

      for (Int_t buckIdx = 5; buckIdx < 507; buckIdx++) {
        Int_t gBuckIdx = agetIdx*68*512 + chIdx*512 + buckIdx;

        adc[gBuckIdx] = pedestal[0] - rawADC[gBuckIdx];

        if (adc[gBuckIdx] > adc[gChIdx*512 + maxADCIdx[gChIdx]] && rawADC[gBuckIdx] < pedestal[0] - 5*pedestal[1] && rawADC[gBuckIdx] != 0)
          maxADCIdx[gChIdx] = buckIdx;
      }

/*
      Int_t gChIdx = agetIdx*68 + chIdx;

//      for (Int_t buckIdx = 0; buckIdx < 512; buckIdx++) {
      for (Int_t buckIdx = 0; buckIdx < 200; buckIdx++) {
        Int_t gBuckIdx = agetIdx*68*512 + chIdx*512 + buckIdx;

        if (rawADC[gBuckIdx] < rawADC[maxADCIdx[gChIdx]] && rawADC[gBuckIdx] != 0)
          maxADCIdx[gChIdx] = buckIdx;
      }
      */
    }
  }

  isProcessed = 1;
}
