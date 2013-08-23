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
    void GotoFirst();

    // Setters
    void SetEventID(Int_t evtid);
    void SetPad(SPiRITPad *pad);

    // Getters
    Int_t GetEventID();
    Int_t GetNumPads();
    SPiRITPad *GetNextPad();
    SPiRITPad *GetPad(Int_t padNo);
    SPiRITPad *GetPad(Int_t row, Int_t layer); 

  private:
    Int_t eventID;
    Int_t firedPads;
    Int_t currentPadNo;
    SPiRITPad *pads[12096];

  ClassDef(SPiRITEvent, 1);
};

#endif
