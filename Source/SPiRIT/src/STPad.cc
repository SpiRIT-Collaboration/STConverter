// =================================================
//  STEvent Class
//
//  Description:
//    Container for a pad data
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 16
// ================================================= 

#include "STPad.hh"

ClassImp(STPad);

void STPad::Initialize() {
 fMaxADCIdx = 0;

 for (Int_t i = 0; i < 512; i++)
   fADC[i] = 0;
}

// setters
void STPad::SetLayer(Int_t val)
{
  fLayer = val;
}

void STPad::SetRow(Int_t val)
{
  fRow = val;
}

void STPad::SetADC(Double_t *val)
{
  for (Int_t i = 0; i < 512; i++)
    fADC[i] = val[i];
}

void STPad::SetADC(Int_t idx, Double_t val)
{
  ADC[idx] = val;
};

// getters
Int_t STPad::GetPadLayer()
{
  return fLayer;
}

Int_t STPad::GetPadRow()
{
  return fRow;
}

Double_t *STPad::GetADC()
{
  return fADC;
}

Double_t STPad::GetADC(Int_t idx)
{
  return fADC[idx];
}

Int_t STPad::GetMaxADCIdx()
{
  return fMaxADCIdx;
}
