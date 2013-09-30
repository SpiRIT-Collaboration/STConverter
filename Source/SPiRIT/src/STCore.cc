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

#include "GETDecoder/GETDecoder.hh"
#include "GETDecoder/GETFrame.hh"

ClassImp(STCore);

STCore::STCore()
{
  Initialize();
}

STCore::STCore(Char_t *filename)
{
  Initialize();
  SetGrawFile(filename);
}

STCore::~STCore()
{
}

void STCore::Initialize()
{
  fRawEvent = NULL;

  fMapPtr = new STMap();
  fPedestalPtr = new STPedestal();

  fDecoder = new GETDecoder();
  fFrame = new GETFrame();

  fIsGraw = 0;
  fIsPedestal = 0;
}

void STCore::SetGraw(Char_t *filename)
{
  fIsGraw = fDecoder -> SetGraw(filename);
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

  return fRawEvent;
}
