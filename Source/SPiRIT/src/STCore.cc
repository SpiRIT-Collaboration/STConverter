// =================================================
//  STCore Class
// 
//  Description:
//    Process CoBoFrame data into STRawEvent data
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 09. 25
// =================================================

#include <iostream>
#include <fstream>

#include "STCore.hh"

#include "STMap.hh"
#include "STPedestal.hh"
#include "STRawEvent.hh"

#include "GETDecoder.hh"
#include "GETFrame.hh"

ClassImp(STCore);

STCore::STCore()
{
  Initialize();
}

STCore::STCore(Char_t *filename)
{
  Initialize();
  SetGraw(filename);
}

STCore::~STCore()
{
  delete fFramePtr;
  delete fDecoderPtr;
  
  delete fPedestalPtr;
  delete fMapPtr;
}

void STCore::Initialize()
{
  fRawEventPtr = NULL;

  fMapPtr = new STMap();
  fPedestalPtr = new STPedestal();

  fDecoderPtr = new GETDecoder();
  fDecoderPtr -> SetDebugMode(1);
  fFramePtr = new GETFrame();

  fIsGraw = 0;
  fIsPedestalData = 0;
  fStoppedFrameNo = 0;
}

void STCore::SetGraw(Char_t *filename)
{
  fIsGraw = fDecoderPtr -> SetGraw(filename);
}

void STCore::SetPedestalData(Char_t *filename)
{
  fIsPedestalData = fPedestalPtr -> SetPedestalData(filename);
}

STRawEvent *STCore::GetRawEvent()
{
  if (!fIsGraw) {
    std::cout << "== Graw file is not set!" << std::endl;

    return NULL;
  }

  if (!fIsPedestalData) {
    std::cout << "== Pedestal data file is not set!" << std::endl;
  }

  if (fRawEventPtr != NULL)
    delete fRawEventPtr;

  fRawEventPtr = new STRawEvent();

  Int_t prevEventNo = -1;
  Int_t currentEventNo = -1;
  Int_t numFrames = fDecoderPtr -> GetNumFrames();
  for (Int_t iFrame = fStoppedFrameNo; iFrame < numFrames; iFrame++) {
    GETFrame *frame = fDecoderPtr -> GetFrame(iFrame);

    if (prevEventNo == -1) {
      prevEventNo = frame -> GetEventID();
      currentEventNo = prevEventNo;
    }

    currentEventNo = frame -> GetEventID();

    if (currentEventNo != prevEventNo) {
      fStoppedFrameNo = iFrame;

      return fRawEventPtr;
    }

    fRawEventPtr -> SetEventID(currentEventNo);

    Int_t coboID = frame -> GetCoboID();
    Int_t asadID = frame -> GetAsadID();

    for (Int_t iAget = 0; iAget < 4; iAget++) {
      for (Int_t iCh = 0; iCh < 68; iCh++) {
        Int_t row, layer;
        fMapPtr -> GetRowNLayer(coboID, asadID, iAget, iCh, row, layer);

        if (row == -2 || layer == -2)
          continue;

        Int_t *adc = frame -> GetADC(iAget, iCh);
        STPad *pad = new STPad(row, layer);
        for (Int_t iTb = 0; iTb < 512; iTb++)
          pad -> SetADC(iTb, adc[iTb]);

        fRawEventPtr -> SetPad(pad);
      }
    }
  }

  return NULL;
}

STRawEvent *STCore::GetRawEvent(Int_t eventID)
{
  return NULL;
}
