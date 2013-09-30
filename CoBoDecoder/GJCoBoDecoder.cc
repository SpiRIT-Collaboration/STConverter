// =================================================
//  GJCoBoDecoder Class
// 
//  Description:
//    Read the raw file from CoBo and process it
//    into GETEvent class
// 
//  Author:
//    Genie Jhang ( geniejhang@majimak.com )
//  
//  Log:
//    - 2013. 09. 23
//      Start writing class
// =================================================

#include <iostream>
#include <fstream>
#include <arpa/inet.h>

#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TString.h"

#include "GJCoBoDecoder.hh"
#include "GJCoBoFrame.hh"

ClassImp(GJCoBoDecoder);

GJCoBoDecoder::GJCoBoDecoder()
{
  Initialize();
}

GJCoBoDecoder::GJCoBoDecoder(Char_t *filename)
{
  Initialize();
  SetGrawFile(filename);
  CountFrames();
}

GJCoBoDecoder::~GJCoBoDecoder()
{
  if(aFrame != NULL)
    delete aFrame;
}

void GJCoBoDecoder::Initialize()
{
  debugMode = 0;

  numFrames = 0;

  firstGrawFile = "";
  nextGrawFile = "";

  aFrame = NULL;
  currentFrameNo = -1;
}

void GJCoBoDecoder::SetDebugMode(Bool_t value)
{
  debugMode = value;
}

void GJCoBoDecoder::SetGrawFile(const Char_t *filename)
{
  if (grawFile.is_open())
    grawFile.close();

  grawFile.open(filename, std::ios::in|std::ios::binary);

  if (!(grawFile.is_open())) {
    std::cout << "== GRAW file open error! Check it exists!" << std::endl;

    return;
  } else {
    grawFile.seekg(0);
    firstGrawFile = filename;
  }
}

Bool_t GJCoBoDecoder::NextFile()
{
  TObjArray *pathElements = NULL;
  if (!nextGrawFile.EqualTo(""))
   pathElements = nextGrawFile.Tokenize("/");
  else
   pathElements = firstGrawFile.Tokenize("/");

  Int_t numElements = pathElements -> GetLast();

  TString path = "";
  if (numElements == 0)
    path = ".";
  else {
    path = "/";
    for (Int_t i = 0; i < numElements; i++) {
      path.Append(((TObjString *) pathElements -> At(i)) -> GetString());
      path.Append("/");
    }
  }

  TString tempGrawFile = ((TObjString *) pathElements -> Last()) -> GetString();
  Int_t fileNumPointer = tempGrawFile.Length() - 9;
  TString fileNumText = tempGrawFile(fileNumPointer, 4);
  Int_t nextFileNum = fileNumText.Atoi() + 1;
  tempGrawFile = tempGrawFile.Replace(fileNumPointer, 4, Form("%04d", nextFileNum));

  Bool_t isNextFile = 0;
  Int_t msPointer = tempGrawFile.Length() - 13;
  for (Int_t ms = 0; ms < 999; ms++) {
    tempGrawFile = tempGrawFile.Replace(msPointer, 3, Form("%03d", ms));
    nextGrawFile = gSystem -> Which(path, tempGrawFile);
    if (!nextGrawFile.EqualTo("")) {
      std::cout << "== Found continuing file: " << tempGrawFile.Data() << std::endl;
      isNextFile = 1;
      break;
    }
  }

  delete pathElements;

  if (isNextFile) {
    grawFile.close();
    grawFile.open(nextGrawFile.Data(), std::ios::in|std::ios::binary);
    grawFile.seekg(0);
  }

  return isNextFile;
}

Int_t GJCoBoDecoder::GetNumFrames()
{
  return numFrames;
}

void GJCoBoDecoder::CountFrames()
{
  UInt_t frameSize = 0;

  while (1) {
    grawFile.ignore(1);

    if (grawFile.eof()) {
      Bool_t isNextFile = NextFile();

      if (isNextFile)
        continue;
      else 
        break;
    }

    grawFile.read(reinterpret_cast<Char_t *>(&frameSize), 3);

    frameSize = (htonl(frameSize) >> 8)*64;
    grawFile.seekg((Int_t)grawFile.tellg() - 4 + frameSize);
    numFrames++;
  }

  SetGrawFile(firstGrawFile.Data());
}

GJCoBoFrame *GJCoBoDecoder::GetFrame()
{
  return GetFrame(currentFrameNo + 1);
}

GJCoBoFrame *GJCoBoDecoder::GetFrame(Int_t frameNo)
{
  if (currentFrameNo == frameNo) {
    if (debugMode)
      PrintFrameInfo(frameNo, aFrame -> GetEventID(), aFrame -> GetCoboID(), aFrame -> GetAsadID());

    return aFrame;
  }
  else if (frameNo >= numFrames) {
    std::cout << "== Last Frame!" << std::endl;

    return NULL;
  } else if (frameNo < 0) {
    std::cout << "== Frame number should be a positive integer!" << std::endl;

    return NULL;
  }

  while (1) {
    UInt_t frameSize;
    UShort_t headerSize;
    UInt_t nItems;
    UInt_t eventIdx;
    UShort_t coboIdx;
    UShort_t asadIdx;

    while (frameNo > currentFrameNo + 1) {
      if (debugMode)
        std::cout << "== Skipping Frame No. " << currentFrameNo + 1 << std::endl;

      grawFile.ignore(1);
      grawFile.read(reinterpret_cast<Char_t *>(&frameSize), 3);

      frameSize = (htonl(frameSize) >> 8)*64;

      grawFile.seekg((Int_t)grawFile.tellg() - 4 + frameSize);
      currentFrameNo++;
    }

    if (frameNo < currentFrameNo) {
      SetGrawFile(firstGrawFile.Data());
      currentFrameNo = -1;

      return GetFrame(frameNo);
    }

    grawFile.ignore(8);
    grawFile.read(reinterpret_cast<Char_t *>(&headerSize), 2);
    grawFile.ignore(2);
    grawFile.read(reinterpret_cast<Char_t *>(&nItems), 4);
    grawFile.ignore(6);
    grawFile.read(reinterpret_cast<Char_t *>(&eventIdx), 4);
    grawFile.read(reinterpret_cast<Char_t *>(&coboIdx), 1);
    grawFile.read(reinterpret_cast<Char_t *>(&asadIdx), 1);

    if (grawFile.eof()) {
      std::cout << "== End of the file!" << std::endl;

      Bool_t isNextFile = NextFile();

      if (isNextFile)
        continue;
      else
        return NULL;
    }

    headerSize = htons(headerSize)*64;
    nItems = htonl(nItems);
    eventIdx = htonl(eventIdx);
    coboIdx = (htons(coboIdx) >> 8);
    asadIdx = (htons(asadIdx) >> 8);

    if (debugMode)
      PrintFrameInfo(frameNo, eventIdx, coboIdx, asadIdx);

    if (aFrame != NULL)
      delete aFrame;

    aFrame = new GJCoBoFrame();
    aFrame -> SetEventID(eventIdx);
    aFrame -> SetCoboID(coboIdx);
    aFrame -> SetAsadID(asadIdx);

    grawFile.seekg((Int_t) grawFile.tellg() - 28 + headerSize);

    UInt_t data;
    for (Int_t iItem = 0; iItem < nItems; iItem++) {
      grawFile.read(reinterpret_cast<Char_t *>(&data), 4);
      data = htonl(data);

      UShort_t agetIdx = ((data & 0xc0000000) >> 30);
      UShort_t chanIdx = ((data & 0x3f800000) >> 23);
      UShort_t buckIdx = ((data & 0x007fc000) >> 14);
      UShort_t sample = (data & 0x00000fff);         

      if (chanIdx >= 68 || agetIdx >= 4 || buckIdx >= 512)
        continue; 
                                                                     
      aFrame -> SetADC(agetIdx, chanIdx, buckIdx, sample); 
    }

    currentFrameNo = frameNo;

    return aFrame;
  }
}

void GJCoBoDecoder::PrintFrameInfo(Int_t frameNo, Int_t eventID, Int_t coboID, Int_t asadID)
{
    std::cout << "== Frame Info -";
    std::cout << " Frame:" << frameNo;
    std::cout << " Event:" << eventID;
    std::cout << " CoBo:" << coboID;
    std::cout << " AsAd:" << asadID << std::endl;
}
