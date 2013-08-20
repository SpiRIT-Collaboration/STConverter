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

#include "SPiRITPad.hh"

class SPiRITEvent {
  public:
    SPiRITEvent();
    ~SPiRITEvent();

    void PrintPads();

    // Setters
    void SetPad(SPiRITPad *pad);

    // Getters
    SPiRITPad *GetNextPad();
    SPiRITPad *GetPad(Short_t row, Short_t layer); 
    Int_t GetNumPads();

  private:
    Bool_t isPadFired[108][112];
    SPiRITPad *pads[108][112];
    Int_t firedPads;
    Short_t currentPadNo;
    Short_t currentRow;
    Short_t currentLayer;

  ClassDef(SPiRITEvent, 1);
};

#endif
