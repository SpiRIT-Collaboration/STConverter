// =================================================
//  SPiRITEvent Class
//
//  Description:
//    Container for a pad data
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 16
// ================================================= 

#include "SPiRITPad.hh"

ClassImp(SPiRITPad);

void SPiRITPad::Initialize() {
 maxADCIdx = 0;

 for (Int_t i = 0; i < 512; i++) {
   isPeak[i] = 0;
   ADC[i] = 0;
 }
}

void SPiRITPad::Process() {
  Int_t diff[511];
  Int_t peakReached = 0;
  Int_t prevPeakIdx = 0;
  Int_t peakIdx = 0;

  for (Int_t i = 0; i < 511; i++) {
    diff[i] = ADC[i + 1] - ADC[i];

    if (ADC[maxADCIdx] < ADC[i + 1])
      maxADCIdx = i + 1;
  }

  // Peak finding algorithm should be here
  for (Int_t i = 0; i < 511; i++) {
    if (ADC[i + 1] >= ADC[i]) {
      prevPeakIdx = peakIdx;
      peakIdx = i + 1;

      peakReached = 0;
    } else {
      peakReached++;
    }

    if (peakReached == 5) {
      isPeak[peakIdx] = 1;

      peakReached = 0;
      prevPeakIdx = 0;
      peakIdx = i;
    }
  }

/*
  // If the first ch is in the middle of falling, set it to be a peak;
  if (diff[0] < 0) isPeak[0] = 1;

  // If the last ch is in the middle of rising, set it to be a peak;
  if (diff[510] > 0) isPeak[511] = 1;
*/
}

Int_t SPiRITPad::GetNumPeaks() {
  Int_t numPeaks = 0;
  for (Int_t i = 0; i < 512; i++)
    if (isPeak[i])
      numPeaks++;

  return numPeaks;
}
