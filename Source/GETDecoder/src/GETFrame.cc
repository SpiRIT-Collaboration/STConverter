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

#include <iostream>
#include <cmath>

#include "GETFrame.hh"

ClassImp(GETFrame);

GETFrame::GETFrame()
{
  // Constructor
  // Initializes the variables used in the class.

  fEventIdx = 0;
  fCoboIdx = 0;
  fAsadIdx = 0;

  fPedestalSubtracted = 0;

  for (Int_t i = 0; i < 4*68*GETNumTbs; i++) {
    fRawAdc[i] = 0;
    fMaxAdcIdx[i%GETNumTbs] = 0;
    fAdc[i] = 0;
  }
}

GETFrame::~GETFrame()
{
  // Destructor
}

void GETFrame::SetEventID(UInt_t value)
{
  // Sets the event number.

  fEventIdx = value;
}

void GETFrame::SetCoboID(UShort_t value)
{
  // Sets the CoBo ID.

  fCoboIdx = value;
}

void GETFrame::SetAsadID(UShort_t value)
{
  // Sets the AsAd ID.

  fAsadIdx = value;
}

void GETFrame::SetFrameID(Int_t value)
{
  // Sets the frame number.

  fFrameIdx = value;
}

void GETFrame::SetRawADC(UShort_t agetIdx, UShort_t chIdx, UShort_t buckIdx, UShort_t value)
{
  // Sets the raw ADC value of the specific bucket of the channel, chIdx, in the AGET, agetIdx.

  Int_t index = GetIndex(agetIdx, chIdx, buckIdx);

  fRawAdc[index] = value;
}

UInt_t GETFrame::GetEventID()
{
  // Returns the event number of this frame.

  return fEventIdx;
}

Int_t GETFrame::GetCoboID()
{
  // Returns the CoBo ID of this frame.

  return fCoboIdx;
}

Int_t GETFrame::GetAsadID()
{
  // Returns the AsAd ID of this frame.

  return fAsadIdx;
}

Int_t GETFrame::GetFrameID()
{
  // Returns the frame number of this frame.

  return fFrameIdx;
}

Int_t *GETFrame::GetRawADC(Int_t agetIdx, Int_t chIdx)
{
  // Returns the raw ADC values array with 512 time buckets of the channel, chIdx, in the AGET, agetIdx.

  Int_t index = GetIndex(agetIdx, chIdx, 0);

  return fRawAdc + index;
}

Int_t GETFrame::GetRawADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx)
{
  // Returns the ADC value of the specific time bucket of the channel, chIdx, in the AGET, agetIdx.

  Int_t index = GetIndex(agetIdx, chIdx, buckIdx);

  return fRawAdc[index]; 
}

void GETFrame::CalcPedestal(Int_t startTb, Int_t numTbs)
{
  // Calculates pedestal and its RMS value using numTbs time buckets starting
  // from startTb and subtract the pedestal value from raw ADC.
  // After this method, user can get the pedestal-subtracted values using
  // GetADC() method.
  //
  // Average: Begin_Latex(fontsize=24)#mu_{n+1} = #mu_{n} + #frac{x_{n} - #mu_{n}}{n+1}End_Latex
  // RMS: Begin_Latex(fontsize=24)#sigma_{n+1}^{2} = #frac{n}{n+1} #sigma_{n}^{2} + #frac{(x_{n+1} - #mu_{n+1})^{2}}{n}End_Latex
  

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    for (Int_t iCh = 0; iCh < 68; iCh++) {
      Int_t index = GetIndex(iAget, iCh, startTb);

      Double_t pedestal[2] = {0};
      for (Int_t iTb = 0; iTb < numTbs; iTb++) {
        pedestal[0] += (fRawAdc[index + iTb] - pedestal[0])/(Double_t)(iTb + 1);

        if (iTb > 0)
          pedestal[1] = iTb*pedestal[1]/(Double_t)(iTb + 1) + pow(fRawAdc[index + iTb] - pedestal[0], 2)/(Double_t)(iTb);
      }

      pedestal[1] = sqrt(pedestal[1]);

      index = GetIndex(iAget, iCh, 0);
      for (Int_t iTb = 0; iTb < GETNumTbs; iTb++) {
//        Double_t adc = pedestal[0] - 5*pedestal[1] - fRawAdc[index + iTb];
        Double_t adc = pedestal[0] - fRawAdc[index + iTb];
        fAdc[index + iTb] = (adc < 0 || fRawAdc[index + iTb] == 0 ? 0 : adc);

        // Discard the first and the last bins
        if (iTb > 0 && iTb < GETNumTbs - 1) {
          if (fAdc[index + iTb] > fAdc[index + fMaxAdcIdx[index/GETNumTbs]])
            fMaxAdcIdx[index/GETNumTbs] = iTb;
        }
      }
    }
  }

  fPedestalSubtracted = 1;
}

Int_t GETFrame::GetMaxADCIdx(Int_t agetIdx, Int_t chIdx)
{
  // Returns the time bucket index of the maximum ADC value.
  // This method is enabled after CalcPedestal() method.

  if (!fPedestalSubtracted) {
    std::cout << "== Run CalcPedstal(Int_t start, Int_t numBins) first!" << std::endl;

    return -1;
  }

  Int_t index = GetIndex(agetIdx, chIdx, 0)/GETNumTbs;

  return fMaxAdcIdx[index];
}

Double_t *GETFrame::GetADC(Int_t agetIdx, Int_t chIdx)
{
  // Returns the pedestal-subtracted ADC values array with 512 time buckets of the channel, chIdx, in the AGET, agetIdx.
  // This method is enabled after CalcPedestal() method.

  if (!fPedestalSubtracted) {
    std::cout << "== Run CalcPedstal(Int_t startTb, Int_t numTbs) first!" << std::endl;

    return NULL;
  }

  Int_t index = GetIndex(agetIdx, chIdx, 0);

  return fAdc + index;
}

Double_t GETFrame::GetADC(Int_t agetIdx, Int_t chIdx, Int_t buckIdx)
{
  // Returns the pedestal-subtracted ADC value of the specific time bucket of the channel, chIdx, in the AGET, agetIdx.
  // This method is enabled after CalcPedestal() method.

  if (!fPedestalSubtracted) {
    std::cout << "== Run CalcPedstal(Int_t start, Int_t numBins) first!" << std::endl;

    return -1;
  }

  Int_t index = GetIndex(agetIdx, chIdx, buckIdx);

  return fAdc[index]; 
}

Int_t GETFrame::GetIndex(Int_t agetIdx, Int_t chIdx, Int_t buckIdx)
{
  // Internally used method to get the index of the array.

  return agetIdx*68*GETNumTbs + chIdx*GETNumTbs + buckIdx;
}
