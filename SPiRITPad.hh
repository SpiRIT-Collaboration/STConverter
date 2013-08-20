// =================================================
//  SPiRITPad Class
//
//  Description:
//    Container for a pad data
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 16
// ================================================= 

#ifndef SPIRITPAD_H
#define SPIRITPAD_H

#include "TROOT.h"

class SPiRITPad {
public:
  SPiRITPad()
    { padLayer = -2; padRow = -2; Initialize(); }
  SPiRITPad(Int_t row, Int_t layer)
    { padRow = row; padLayer = layer; Initialize(); }
  ~SPiRITPad() {}
  
  void Initialize();
  void Process();

  // Setters
  void SetPadLayer(Short_t val) { padLayer = val; }
  void SetPadRow(Short_t val) { padRow = val; }
  void SetADC(Double_t *val) { for (Int_t i = 0; i < 512; i++) ADC[i] = val[i]; };
  void SetADC(Short_t idx, Double_t val) { ADC[idx] = val; };

  // Getters
  Short_t GetPadLayer() { return padLayer; }
  Short_t GetPadRow() { return padRow; }
  Double_t *GetADC() { return ADC; }
  Double_t GetADC(Int_t idx) { return ADC[idx]; }
  Short_t GetNumPeaks();

private:
  Short_t padLayer;
  Short_t padRow;  

  Bool_t isPeak[512];
  Short_t maxADCIdx;
  Double_t ADC[512];
  
  ClassDef(SPiRITPad, 1);
};

#endif
