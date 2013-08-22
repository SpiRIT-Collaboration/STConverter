// =================================================
//  SPiRITEvent Class                          
//                                                  
//  Description:                                    
//    Container for an event data
//                                                  
//  Genie Jhang ( geniejhang@majimak.com )          
//  2013. 08. 16                                    
// =================================================

#ifndef SPIRITEVENT_H
#define SPIRITEVENT_H

#include "TROOT.h"
#include "TObject.h"

#include "SPiRITPad.hh"

class SPiRITEvent : public TObject {
  public:
    SPiRITEvent();
    ~SPiRITEvent();

    void PrintPads();
    void GoFirst() { currentPadNo = 0; }

    // Setters
    void SetPad(SPiRITPad *pad);

    // Getters
    SPiRITPad *GetNextPad();
    SPiRITPad *GetPad(Int_t row, Int_t layer); 
    Int_t GetNumPads();

  private:
    Bool_t isPadFired[12096];
    SPiRITPad *pads[12096];
    Int_t firedPads;
    Int_t currentPadNo;

  ClassDef(SPiRITEvent, 1);
};

#endif
