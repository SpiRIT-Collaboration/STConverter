// =================================================
//  STReadRaw Class
// 
//  Description:
//    Read the raw file from CoBo and process it
//    into STEvent class.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 16
// =================================================

#include <iostream>
#include <fstream>
#include <arpa/inet.h>

#include "STReadRaw.hh"

#include "STGraw.hh"

#include "STEvent.hh"
#include "STPad.hh"

#include "STMap.hh"
#include "STPedestal.hh"

ClassImp(STReadRaw);

STReadRaw::STReadRaw()
{
  Initialize();
}

STReadRaw::STReadRaw(Char_t *filename)
{
  SetRawfile(filename);
  Initialize();
}

void STReadRaw::SetRawfile(Char_t *filename)
{
  datafile = filename;

  rawfile.open(filename, std::ios::in|std::ios::binary);
  rawfile.seekg(0);

  firstFlag = 1;
}

Bool_t STReadRaw::NextFile()
{
  rawfile.close();

  TString fileNumText = datafile(datafile.Length() - 9, 4);
  Int_t nextFileNum = fileNumText.Atoi() + 1;
  datafile = datafile.Replace(datafile.Length() - 9, 4, Form("%04d", nextFileNum));
  
  rawfile.open(datafile, std::ios::in|std::ios::binary);
  rawfile.seekg(0);

  if (!(rawfile.is_open())) {
    std::cerr << "Next data file doesn't exist!" << std::endl;

    return 0;
  }

  std::cout << "Loaded file: " << datafile.Data() << std::endl;

  return 1;
}

void STReadRaw::SetPedestalData(Char_t *filename)
{
  usePedestalData = 1;

  pedestalPtr -> SetPedestalData(filename);
}

void STReadRaw::Initialize()
{
  usePedestalData = 0;
  prevEventIdx = 0;

  mapperPtr = new STMap();
  pedestalPtr = new STPedestal();

  aGraw = NULL;
  anEvent = NULL;
}

STEvent *STReadRaw::GetEvent() {
  if (!(rawfile.is_open())) {
    std::cerr << "Raw data file is not loaded!" << std::endl;

    return NULL;
  }

  if (anEvent != NULL) {
    delete anEvent;
    anEvent = NULL;
  }

  anEvent = new STEvent();

  while (1) {
    unsigned short metaType = 0;
    unsigned int frameSize = 0;
    unsigned short dataSource = 0;
    unsigned short frameType = 0;
    unsigned short revision = 0;
    unsigned short headerSize = 0;
    unsigned short itemSize = 0;
    unsigned int nItems = 0;
    unsigned short eventTime[3] = {0};
//    unsigned short eventTimeH = 0;
//    unsigned int eventTimeT = 0;
    unsigned int eventIdx = 0;
    unsigned short coboIdx = 0;
    unsigned short asadIdx = 0;
    unsigned short readOffset = 0;
    unsigned short status = 0;
    unsigned int hitpat0[3] = {0};
    unsigned int hitpat1[3] = {0};
    unsigned int hitpat2[3] = {0};
    unsigned int hitpat3[3] = {0};
    unsigned short multip0 = 0;
    unsigned short multip1 = 0;
    unsigned short multip2 = 0;
    unsigned short multip3 = 0;
    unsigned int windowOut = 0;
    unsigned short lastCell0 = 0;
    unsigned short lastCell1 = 0;
    unsigned short lastCell2 = 0;
    unsigned short lastCell3 = 0;

    rawfile.read(reinterpret_cast<char *>(&metaType), 1);
    rawfile.read(reinterpret_cast<char *>(&frameSize), 3);
    rawfile.read(reinterpret_cast<char *>(&dataSource), 1);
    rawfile.read(reinterpret_cast<char *>(&frameType), 2);
    rawfile.read(reinterpret_cast<char *>(&revision), 1);
    rawfile.read(reinterpret_cast<char *>(&headerSize), 2);
    rawfile.read(reinterpret_cast<char *>(&itemSize), 2);
    rawfile.read(reinterpret_cast<char *>(&nItems), 4);
    rawfile.read(reinterpret_cast<char *>(&eventTime), 6);
//    rawfile.read(reinterpret_cast<char *>(&eventTimeH), 2);
//    rawfile.read(reinterpret_cast<char *>(&eventTimeT), 4);
    rawfile.read(reinterpret_cast<char *>(&eventIdx), 4);
    rawfile.read(reinterpret_cast<char *>(&coboIdx), 1);
    rawfile.read(reinterpret_cast<char *>(&asadIdx), 1);
    rawfile.read(reinterpret_cast<char *>(&readOffset), 2);
    rawfile.read(reinterpret_cast<char *>(&status), 1);
    rawfile.read(reinterpret_cast<char *>(hitpat0), 9);
    rawfile.read(reinterpret_cast<char *>(hitpat1), 9);
    rawfile.read(reinterpret_cast<char *>(hitpat2), 9);
    rawfile.read(reinterpret_cast<char *>(hitpat3), 9);
    rawfile.read(reinterpret_cast<char *>(&multip0), 2);
    rawfile.read(reinterpret_cast<char *>(&multip1), 2);
    rawfile.read(reinterpret_cast<char *>(&multip2), 2);
    rawfile.read(reinterpret_cast<char *>(&multip3), 2);
    rawfile.read(reinterpret_cast<char *>(&windowOut), 4);
    rawfile.read(reinterpret_cast<char *>(&lastCell0), 2);
    rawfile.read(reinterpret_cast<char *>(&lastCell1), 2);
    rawfile.read(reinterpret_cast<char *>(&lastCell2), 2);
    rawfile.read(reinterpret_cast<char *>(&lastCell3), 2);

    if (rawfile.eof()) { 
      std::cout << "End of this file!" << std::endl;
      
      Bool_t isNextFile = NextFile();

      if (isNextFile)
        continue;
      else
        break;
    }

    /*
       cout << "metaType: " << (htons(metaType) >> 8) << endl;
       cout << "frameSize: " << (htonl(frameSize) >> 8) << endl;
       cout << "dataSource: " << (htons(dataSource) >> 8) << endl;
       cout << "frameType: " << htons(frameType) << endl;
       cout << "revision: " << (htons(revision) >> 8) << endl;
       cout << "headerSize: " << htons(headerSize) << endl;
       cout << "itemSize: " << htons(itemSize) << endl;
       cout << "nItems: " << htonl(nItems) << endl;
    //     cout << "eventTime: " << (htons(eventTimeH) << 32) + htonl(eventTimeT) << endl;
    cout << "eventTime: " << (htons(eventTime[2]) << 32) + (htons(eventTime[1]) << 16) + htons(eventTime[0]) << endl;
     */
    eventIdx = htonl(eventIdx);
    coboIdx = (htons(coboIdx) >> 8);
    asadIdx = (htons(asadIdx) >> 8);
    /*
       cout << "coboIdx: " << (htons(coboIdx) >> 8) << endl;
       cout << "asadIdx: " << (htons(asadIdx) >> 8) << endl;
       cout << "readOffset: " << htons(readOffset) << endl;
       cout << "status: " << (htons(status) >> 8) << endl;
       cout << "hitpat_0: ";
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat0[2]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat0[1]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat0[0]) & (int)pow(2., i)) >> i);
       cout << endl;
       cout << "hitpat_1: ";
       for (int i = 24; i < 32; i++) cout << ((htonl(hitpat1[2]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat1[1]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 24; i++) cout << ((htonl(hitpat1[0]) & (int)pow(2., i)) >> i);
       cout << endl;
       cout << "hitpat_2: ";
       for (int i = 24; i < 32; i++) cout << ((htonl(hitpat2[2]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat2[1]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 24; i++) cout << ((htonl(hitpat2[0]) & (int)pow(2., i)) >> i);
       cout << endl;
       cout << "hitpat_3: ";
       for (int i = 24; i < 32; i++) cout << ((htonl(hitpat3[2]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat3[1]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 24; i++) cout << ((htonl(hitpat3[0]) & (int)pow(2., i)) >> i);
       cout << endl;
       cout << "multip_0: " << htons(multip0) << endl;
       cout << "multip_1: " << htons(multip1) << endl;
       cout << "multip_2: " << htons(multip2) << endl;
       cout << "multip_3: " << htons(multip3) << endl;
       cout << "windowOut: " << htonl(windowOut) << endl;
       cout << "lastCell_0: " << htons(lastCell0) << endl;
       cout << "lastCell_1: " << htons(lastCell1) << endl;
       cout << "lastCell_2: " << htons(lastCell2) << endl;
       cout << "lastCell_3: " << htons(lastCell3) << endl;
     */

    // If loaded event is the first one of user-set file,
    // set prevEventIdx to the current eventIdx.
    if (firstFlag) {
      prevEventIdx = eventIdx;
      firstFlag = 0;
    }

    // When loaded event is not the same event as the previous one,
    // put the position in input sequence to the head of the frame
    // and return the event class object pointer.
    if (prevEventIdx != eventIdx) {
      rawfile.seekg((Int_t)rawfile.tellg() - 87);
      anEvent -> SetEventID(prevEventIdx);
      std::cout << "eventIdx: " << prevEventIdx << std::endl;

      prevEventIdx = eventIdx;

      return anEvent;
    }

    Int_t rawdata[4][68][512] = {0};

    unsigned int data;
//    rawfile.ignore(41); 
    rawfile.ignore(425); // HeaderSize is changed to 512 with CoBo using external dataRouter
    for (int i = 0; i < htonl(nItems); i++) {
      rawfile.read(reinterpret_cast<char *>(&data), 4);
      data = htonl(data);

      /*
         cout << "sampIdx: " << i << endl;
         cout << "agetIdx: " << ((data & 0x0c000000) >> 30) << endl;
         cout << "chanIdx: " << ((data & 0x3f800000) >> 23) << endl;
         cout << "buckIdx: " << ((data & 0x007fc000) >> 14) << endl;
         cout << " sample: " <<  (data & 0x00000fff) << endl;
       */

      Int_t sampleIdx = i;
      Int_t agetIdx = ((data & 0xc0000000) >> 30);
      Int_t chanIdx = ((data & 0x3f800000) >> 23);
      Int_t buckIdx = ((data & 0x007fc000) >> 14);
      Int_t sample = (data & 0x00000fff);

      rawdata[agetIdx][chanIdx][buckIdx] = sample;
    }

    STPad *pad = NULL;
    for (Int_t agetIdx = 0; agetIdx < 4; agetIdx++) {
      for (Int_t chanIdx = 0; chanIdx < 68; chanIdx++) {
        pad = new STPad();

        Int_t row, layer;
        mapperPtr -> GetRowNLayer((Int_t)coboIdx, (Int_t)asadIdx, agetIdx, chanIdx, row, layer);

        if (row == -2 || layer == -2) {
          delete pad;
          continue;
        }

        pad -> SetPadRow(row);
        pad -> SetPadLayer(layer);

        Double_t ped[2] = {0};
        if (usePedestalData == 0)
          pedestalPtr -> GetPedestal(rawdata[agetIdx][chanIdx], ped);
        else
          pedestalPtr -> GetPedestal(coboIdx, asadIdx, agetIdx, chanIdx, ped);

        for (Int_t buckIdx = 0; buckIdx < 512; buckIdx++) {
          if (rawdata[agetIdx][chanIdx][buckIdx] < ped[0] - 5*ped[1])
            pad -> SetADC(buckIdx, ped[0] - 5*ped[1] - rawdata[agetIdx][chanIdx][buckIdx]);
          else
            pad -> SetADC(buckIdx, 0);
        }

        pad -> Process();

        if (pad -> GetNumPeaks() == 0)
          delete pad;
        else
          anEvent -> SetPad(pad);

        pad = NULL;
      }
    }

    prevEventIdx = eventIdx;
  }
}

STGraw *STReadRaw::GetGraw() {
  if (!(rawfile.is_open())) {
    std::cerr << "Raw data file is not loaded!" << std::endl;

    return NULL;
  }

  if (aGraw != NULL) {
    delete aGraw;
    aGraw = NULL;
  }

  aGraw = new STGraw();

  while (1) {
    unsigned short metaType = 0;
    unsigned int frameSize = 0;
    unsigned short dataSource = 0;
    unsigned short frameType = 0;
    unsigned short revision = 0;
    unsigned short headerSize = 0;
    unsigned short itemSize = 0;
    unsigned int nItems = 0;
    unsigned short eventTime[3] = {0};
//    unsigned short eventTimeH = 0;
//    unsigned int eventTimeT = 0;
    unsigned int eventIdx = 0;
    unsigned short coboIdx = 0;
    unsigned short asadIdx = 0;
    unsigned short readOffset = 0;
    unsigned short status = 0;
    unsigned int hitpat0[3] = {0};
    unsigned int hitpat1[3] = {0};
    unsigned int hitpat2[3] = {0};
    unsigned int hitpat3[3] = {0};
    unsigned short multip0 = 0;
    unsigned short multip1 = 0;
    unsigned short multip2 = 0;
    unsigned short multip3 = 0;
    unsigned int windowOut = 0;
    unsigned short lastCell0 = 0;
    unsigned short lastCell1 = 0;
    unsigned short lastCell2 = 0;
    unsigned short lastCell3 = 0;

    rawfile.read(reinterpret_cast<char *>(&metaType), 1);
    rawfile.read(reinterpret_cast<char *>(&frameSize), 3);
    rawfile.read(reinterpret_cast<char *>(&dataSource), 1);
    rawfile.read(reinterpret_cast<char *>(&frameType), 2);
    rawfile.read(reinterpret_cast<char *>(&revision), 1);
    rawfile.read(reinterpret_cast<char *>(&headerSize), 2);
    rawfile.read(reinterpret_cast<char *>(&itemSize), 2);
    rawfile.read(reinterpret_cast<char *>(&nItems), 4);
    rawfile.read(reinterpret_cast<char *>(&eventTime), 6);
//    rawfile.read(reinterpret_cast<char *>(&eventTimeH), 2);
//    rawfile.read(reinterpret_cast<char *>(&eventTimeT), 4);
    rawfile.read(reinterpret_cast<char *>(&eventIdx), 4);
    rawfile.read(reinterpret_cast<char *>(&coboIdx), 1);
    rawfile.read(reinterpret_cast<char *>(&asadIdx), 1);
    rawfile.read(reinterpret_cast<char *>(&readOffset), 2);
    rawfile.read(reinterpret_cast<char *>(&status), 1);
    rawfile.read(reinterpret_cast<char *>(hitpat0), 9);
    rawfile.read(reinterpret_cast<char *>(hitpat1), 9);
    rawfile.read(reinterpret_cast<char *>(hitpat2), 9);
    rawfile.read(reinterpret_cast<char *>(hitpat3), 9);
    rawfile.read(reinterpret_cast<char *>(&multip0), 2);
    rawfile.read(reinterpret_cast<char *>(&multip1), 2);
    rawfile.read(reinterpret_cast<char *>(&multip2), 2);
    rawfile.read(reinterpret_cast<char *>(&multip3), 2);
    rawfile.read(reinterpret_cast<char *>(&windowOut), 4);
    rawfile.read(reinterpret_cast<char *>(&lastCell0), 2);
    rawfile.read(reinterpret_cast<char *>(&lastCell1), 2);
    rawfile.read(reinterpret_cast<char *>(&lastCell2), 2);
    rawfile.read(reinterpret_cast<char *>(&lastCell3), 2);

    if (rawfile.eof()) { 
      std::cout << "End of this file!" << std::endl;
      
      Bool_t isNextFile = NextFile();

      if (isNextFile)
        continue;
      else {
        if (prevEventIdx != eventIdx) {
          aGraw -> SetEventID(prevEventIdx);
          aGraw -> SetCoboID(coboIdx);
          aGraw -> SetAsadID(asadIdx);
          std::cout << "eventIdx: " << prevEventIdx << std::endl;

          prevEventIdx = eventIdx;

          return aGraw;
        }
      }
    }

    /*
       cout << "metaType: " << (htons(metaType) >> 8) << endl;
       cout << "frameSize: " << (htonl(frameSize) >> 8) << endl;
       cout << "dataSource: " << (htons(dataSource) >> 8) << endl;
       cout << "frameType: " << htons(frameType) << endl;
       cout << "revision: " << (htons(revision) >> 8) << endl;
       cout << "headerSize: " << htons(headerSize) << endl;
       cout << "itemSize: " << htons(itemSize) << endl;
       cout << "nItems: " << htonl(nItems) << endl;
    //     cout << "eventTime: " << (htons(eventTimeH) << 32) + htonl(eventTimeT) << endl;
    cout << "eventTime: " << (htons(eventTime[2]) << 32) + (htons(eventTime[1]) << 16) + htons(eventTime[0]) << endl;
     */
    eventIdx = htonl(eventIdx);
    coboIdx = (htons(coboIdx) >> 8);
    asadIdx = (htons(asadIdx) >> 8);
    /*
       cout << "coboIdx: " << (htons(coboIdx) >> 8) << endl;
       cout << "asadIdx: " << (htons(asadIdx) >> 8) << endl;
       cout << "readOffset: " << htons(readOffset) << endl;
       cout << "status: " << (htons(status) >> 8) << endl;
       cout << "hitpat_0: ";
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat0[2]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat0[1]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat0[0]) & (int)pow(2., i)) >> i);
       cout << endl;
       cout << "hitpat_1: ";
       for (int i = 24; i < 32; i++) cout << ((htonl(hitpat1[2]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat1[1]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 24; i++) cout << ((htonl(hitpat1[0]) & (int)pow(2., i)) >> i);
       cout << endl;
       cout << "hitpat_2: ";
       for (int i = 24; i < 32; i++) cout << ((htonl(hitpat2[2]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat2[1]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 24; i++) cout << ((htonl(hitpat2[0]) & (int)pow(2., i)) >> i);
       cout << endl;
       cout << "hitpat_3: ";
       for (int i = 24; i < 32; i++) cout << ((htonl(hitpat3[2]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 32; i++) cout << ((htonl(hitpat3[1]) & (int)pow(2., i)) >> i);
       for (int i = 0; i < 24; i++) cout << ((htonl(hitpat3[0]) & (int)pow(2., i)) >> i);
       cout << endl;
       cout << "multip_0: " << htons(multip0) << endl;
       cout << "multip_1: " << htons(multip1) << endl;
       cout << "multip_2: " << htons(multip2) << endl;
       cout << "multip_3: " << htons(multip3) << endl;
       cout << "windowOut: " << htonl(windowOut) << endl;
       cout << "lastCell_0: " << htons(lastCell0) << endl;
       cout << "lastCell_1: " << htons(lastCell1) << endl;
       cout << "lastCell_2: " << htons(lastCell2) << endl;
       cout << "lastCell_3: " << htons(lastCell3) << endl;
     */

    // If loaded event is the first one of user-set file,
    // set prevEventIdx to the current eventIdx.
    if (firstFlag) {
      prevEventIdx = eventIdx;
      firstFlag = 0;
    }

    // When loaded event is not the same event as the previous one,
    // put the position in input sequence to the head of the frame
    // and return the event class object pointer.
    if (prevEventIdx != eventIdx) {
      rawfile.seekg((Int_t)rawfile.tellg() - 87);
      aGraw -> SetEventID(prevEventIdx);
      aGraw -> SetCoboID(coboIdx);
      aGraw -> SetAsadID(asadIdx);
      std::cout << "eventIdx: " << prevEventIdx << std::endl;

      prevEventIdx = eventIdx;

      return aGraw;
    }

    unsigned int data;
//    rawfile.ignore(41); 
    rawfile.ignore(425); // HeaderSize is changed to 512 with CoBo using external dataRouter
    for (int i = 0; i < htonl(nItems); i++) {
      rawfile.read(reinterpret_cast<char *>(&data), 4);
      data = htonl(data);

      /*
         cout << "sampIdx: " << i << endl;
         cout << "agetIdx: " << ((data & 0x0c000000) >> 30) << endl;
         cout << "chanIdx: " << ((data & 0x3f800000) >> 23) << endl;
         cout << "buckIdx: " << ((data & 0x007fc000) >> 14) << endl;
         cout << " sample: " <<  (data & 0x00000fff) << endl;
       */

      Int_t sampleIdx = i;
      Int_t agetIdx = ((data & 0xc0000000) >> 30);
      Int_t chanIdx = ((data & 0x3f800000) >> 23);
      Int_t buckIdx = ((data & 0x007fc000) >> 14);
      Int_t sample = (data & 0x00000fff);

      if (chanIdx > 68) continue;

      aGraw -> SetRawADC(agetIdx, chanIdx, buckIdx, sample);
    }

    aGraw -> Process(pedestalPtr);

    prevEventIdx = eventIdx;
  }
}
