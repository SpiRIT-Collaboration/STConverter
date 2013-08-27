// =================================================
//  STEvent Class                          
//                                                  
//  Description:                                    
//    Container for an event data
//                                                  
//  Genie Jhang ( geniejhang@majimak.com )          
//  2013. 08. 16                                    
// =================================================

#ifndef STEVENT_H
#define STEVENT_H

#include "TROOT.h"
#include "TObject.h"

#include "STPad.hh"

class STEvent : public TObject {
  public:
    STEvent();
    ~STEvent();

    void PrintPads();
    void GotoFirst();

    // Setters
    void SetEventID(Int_t evtid);
    void SetPad(STPad *pad);

    // Getters
    Int_t GetEventID();
    Int_t GetNumPads();
    STPad *GetNextPad();
    STPad *GetPad(Int_t padNo);
    STPad *GetPad(Int_t row, Int_t layer); 

  private:
    Int_t eventID;
    Int_t firedPads;
    Int_t currentPadNo;
    STPad *pads[12096];

  ClassDef(STEvent, 1);
};

#endif
