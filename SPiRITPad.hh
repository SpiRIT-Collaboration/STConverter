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
#include "TObject.h"

class SPiRITPad : public TObject {
public:
  SPiRITPad()
    { padLayer = -2; padRow = -2; Initialize(); }
  SPiRITPad(Int_t row, Int_t layer)
    { padRow = row; padLayer = layer; Initialize(); }
  ~SPiRITPad() {}
  
  void Initialize();
  void Process();

  // Setters
  void SetPadLayer(Int_t val) { padLayer = val; }
  void SetPadRow(Int_t val) { padRow = val; }
  void SetADC(Double_t *val) { for (Int_t i = 0; i < 512; i++) ADC[i] = val[i]; };
  void SetADC(Int_t idx, Double_t val) { ADC[idx] = val; };

  // Getters
  Int_t GetPadLayer() { return padLayer; }
  Int_t GetPadRow() { return padRow; }
  Double_t *GetADC() { return ADC; }
  Double_t GetADC(Int_t idx) { return ADC[idx]; }
  Int_t GetMaxADCIdx() { return maxADCIdx; }
  Int_t GetNumPeaks();

private:
  Int_t padLayer;
  Int_t padRow;  

  Bool_t isPeak[512];
  Int_t maxADCIdx;
  Double_t ADC[512];
  
  ClassDef(SPiRITPad, 1);
};

#endif
