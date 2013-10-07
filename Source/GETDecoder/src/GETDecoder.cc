// =================================================
//  GETDecoder Class
// 
//  Description:
//    Read the raw file from GET electronics
//    and process it into GETFrame class
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

#include "GETDecoder.hh"
#include "GETFrame.hh"
#include "GETPlot.hh"

ClassImp(GETDecoder);

GETDecoder::GETDecoder()
{
  // Constructor

  Initialize();
}

GETDecoder::GETDecoder(Char_t *filename)
{
  // Constructor with raw data file

  Initialize();
  SetGraw(filename);
}

GETDecoder::~GETDecoder()
{
  // Destructor

  if(fFrame != NULL)
    delete fFrame;
}

void GETDecoder::Initialize()
{
  // Initialize variables used in the class

  fDebugMode = 0;

  fNumFrames = 0;

  fFirstGraw = "";
  fNextGraw = "";

  fFrame = NULL;
  fCurrentFrameID = -1;

  fGETPlot = NULL;
}

void GETDecoder::SetDebugMode(Bool_t value)
{
  // Setting debug mode.
  // If set to 1, more information is printed out on the screen.

  fDebugMode = value;
}

Bool_t GETDecoder::SetGraw(const Char_t *filename)
{
  // Set the data file to the class.
  // After setting, it counts how many frames in the file including continuing files.
  // This method returns 1 if the file is loaded properly.

  Bool_t isSetFile = SetFile(filename);

  if (isSetFile)
    CountFrames();

  return isSetFile;
}

Bool_t GETDecoder::SetFile(const Char_t *filename)
{
  // Set the data file to the class.

  if (fGraw.is_open())
    fGraw.close();

  fGraw.open(filename, std::ios::in|std::ios::binary);

  if (!(fGraw.is_open())) {
    std::cout << "== GRAW file open error! Check it exists!" << std::endl;

    return 0;
  } else {
    std::cout << "== " << filename << " is opened!" << std::endl;
    fGraw.seekg(0);
    fFirstGraw = filename;

    return 1;
  }
}

Bool_t GETDecoder::IsNextFile()
{
  // Searches the next file and set it if exists.
  // Returns 1 if successful.

  TObjArray *pathElements = NULL;
  if (!fNextGraw.EqualTo(""))
   pathElements = fNextGraw.Tokenize("/");
  else
   pathElements = fFirstGraw.Tokenize("/");

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
    fNextGraw = gSystem -> Which(path, tempGrawFile);
    if (!fNextGraw.EqualTo("")) {
      std::cout << "== Found continuing file: " << tempGrawFile.Data() << std::endl;
      isNextFile = 1;
      break;
    }
  }

  delete pathElements;

  if (isNextFile) {
    fGraw.close();
    fGraw.open(fNextGraw.Data(), std::ios::in|std::ios::binary);
    fGraw.seekg(0);
  }

  return isNextFile;
}

GETPlot *GETDecoder::GetGETPlot()
{
  // Returns GETPlot object pointer if there exists.
  // If not, create a new one and return it.

  if (!fGETPlot)
    fGETPlot = new GETPlot(this);

  return fGETPlot;
}

Int_t GETDecoder::GetNumFrames()
{
  // Returns the number of frames counted by CountFrames() method.

  return fNumFrames;
}

Int_t GETDecoder::GetCurrentFrameID()
{
  // Returns the frame number currently read and returned frame.

  return fCurrentFrameID;
}

void GETDecoder::CountFrames()
{
  // Counts the number of frames in the set file and continuing files.

  fNumFrames = 0;
  UInt_t frameSize = 0;

  while (1) {
    fGraw.ignore(1);

    if (fGraw.eof()) {
      Bool_t isNextFile = IsNextFile();

      if (isNextFile)
        continue;
      else 
        break;
    }

    fGraw.read(reinterpret_cast<Char_t *>(&frameSize), 3);

    frameSize = (htonl(frameSize) >> 8)*64;
    fGraw.seekg((Int_t)fGraw.tellg() - 4 + frameSize);
    fNumFrames++;
  }

  SetFile(fFirstGraw.Data());
}

GETFrame *GETDecoder::GetFrame()
{
  // Returns next frame.

  return GetFrame(fCurrentFrameID + 1);
}

GETFrame *GETDecoder::GetFrame(Int_t frameNo)
{
  // Returns specific frame of given frame number.

  if (fCurrentFrameID == frameNo) {
    if (fDebugMode)
      PrintFrameInfo(frameNo, fFrame -> GetEventID(), fFrame -> GetCoboID(), fFrame -> GetAsadID());

    return fFrame;
  }
  else if (frameNo >= fNumFrames) {
    std::cout << "== Last frame reached! (" << fNumFrames - 1 << ")" << std::endl;

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

    while (frameNo > fCurrentFrameID + 1) {
      if (fDebugMode)
        std::cout << "== Skipping Frame No. " << fCurrentFrameID + 1 << std::endl;

      fGraw.ignore(1);
      fGraw.read(reinterpret_cast<Char_t *>(&frameSize), 3);

      frameSize = (htonl(frameSize) >> 8)*64;

      fGraw.seekg((Int_t)fGraw.tellg() - 4 + frameSize);
      fCurrentFrameID++;
    }

    if (frameNo < fCurrentFrameID) {
      SetFile(fFirstGraw.Data());
      fCurrentFrameID = -1;

      return GetFrame(frameNo);
    }

    fGraw.ignore(8);
    fGraw.read(reinterpret_cast<Char_t *>(&headerSize), 2);
    fGraw.ignore(2);
    fGraw.read(reinterpret_cast<Char_t *>(&nItems), 4);
    fGraw.ignore(6);
    fGraw.read(reinterpret_cast<Char_t *>(&eventIdx), 4);
    fGraw.read(reinterpret_cast<Char_t *>(&coboIdx), 1);
    fGraw.read(reinterpret_cast<Char_t *>(&asadIdx), 1);

    if (fGraw.eof()) {
      std::cout << "== End of the file!" << std::endl;

      Bool_t isNextFile = IsNextFile();

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

    if (fDebugMode)
      PrintFrameInfo(frameNo, eventIdx, coboIdx, asadIdx);

    if (fFrame != NULL)
      delete fFrame;

    fFrame = new GETFrame();
    fFrame -> SetEventID(eventIdx);
    fFrame -> SetCoboID(coboIdx);
    fFrame -> SetAsadID(asadIdx);
    fFrame -> SetFrameID(frameNo);

    fGraw.seekg((Int_t) fGraw.tellg() - 28 + headerSize);

    UInt_t data;
    for (Int_t iItem = 0; iItem < nItems; iItem++) {
      fGraw.read(reinterpret_cast<Char_t *>(&data), 4);
      data = htonl(data);

      UShort_t agetIdx = ((data & 0xc0000000) >> 30);
      UShort_t chanIdx = ((data & 0x3f800000) >> 23);
      UShort_t buckIdx = ((data & 0x007fc000) >> 14);
      UShort_t sample = (data & 0x00000fff);         

      if (chanIdx >= 68 || agetIdx >= 4 || buckIdx >= GETNumTbs)
        continue; 
                                                                     
      fFrame -> SetRawADC(agetIdx, chanIdx, buckIdx, sample); 
    }

    fCurrentFrameID = frameNo;

    return fFrame;
  }
}

void GETDecoder::PrintFrameInfo(Int_t frameNo, Int_t eventID, Int_t coboID, Int_t asadID)
{
  // Prints the information of the returned frame.

  std::cout << "== Frame Info -";
  std::cout << " Frame:" << frameNo;
  std::cout << " Event:" << eventID;
  std::cout << " CoBo:" << coboID;
  std::cout << " AsAd:" << asadID << std::endl;
}
