#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

#include <fstream>
#include <iostream>
#include <arpa/inet.h>
#include <cmath>

using namespace std;

void MakePedestal(Char_t *pedestalFile) {
  unsigned short metaType;
  unsigned int frameSize;
  unsigned short dataSource;
  unsigned short frameType;
  unsigned short revision;
  unsigned short headerSize;
  unsigned short itemSize;
  unsigned int nItems;
  unsigned short eventTime[3];
  unsigned int eventIdx;
  unsigned short coboIdx;
  unsigned short asadIdx;
  unsigned short readOffset;
  unsigned short status;
  unsigned int hitpat0[3];
  unsigned int hitpat1[3];
  unsigned int hitpat2[3];
  unsigned int hitpat3[3];
  unsigned short multip0;
  unsigned short multip1;
  unsigned short multip2;
  unsigned short multip3;
  unsigned int windowOut;
  unsigned short lastCell0;
  unsigned short lastCell1;
  unsigned short lastCell2;
  unsigned short lastCell3;

  Int_t treeCoboIdx;
  Int_t treeAsadIdx;
  Int_t treeAgetIdx;
  Int_t treeChIdx;

  UInt_t numSamples[48*4*4*68] = {0};
  Double_t treePedestal = 0;
  Double_t treePedestalSigma = 0;
  Double_t prevPedestal[48*4*4*68] = {0};
  Double_t prevPedestalSigma[48*4*4*68] = {0};
  Double_t pedestal[48*4*4*68] = {0};
  Double_t pedestalSigma[48*4*4*68] = {0};

  TString output = pedestalFile;
  output.ReplaceAll(".graw", "_pedestal.root");
  TFile *writeFile = new TFile(output.Data(), "recreate");
  TTree *writeTree = new TTree("pedestal", "");
    writeTree -> Branch("coboIdx", &treeCoboIdx, "coboIdx/I");
    writeTree -> Branch("asadIdx", &treeAsadIdx, "asadIdx/I");
    writeTree -> Branch("agetIdx", &treeAgetIdx, "agetIdx/I");
    writeTree -> Branch("chIdx", &treeChIdx, "chIdx/I");
    writeTree -> Branch("pedestal", &treePedestal, "pedestal/D");
    writeTree -> Branch("pedestalSigma", &treePedestalSigma, "pedestalSigma/D");

  frameSize = 0;
  ifstream openFile(pedestalFile, ios::in|ios::binary);
  openFile.seekg(0);
  while (!openFile.eof()) {
    openFile.read(reinterpret_cast<char *>(&metaType), 1);
    openFile.read(reinterpret_cast<char *>(&frameSize), 3);
    openFile.read(reinterpret_cast<char *>(&dataSource), 1);
    openFile.read(reinterpret_cast<char *>(&frameType), 2);
    openFile.read(reinterpret_cast<char *>(&revision), 1);
    openFile.read(reinterpret_cast<char *>(&headerSize), 2);
    openFile.read(reinterpret_cast<char *>(&itemSize), 2);
    openFile.read(reinterpret_cast<char *>(&nItems), 4);
    openFile.read(reinterpret_cast<char *>(&eventTime), 6);
    openFile.read(reinterpret_cast<char *>(&eventIdx), 4);
    openFile.read(reinterpret_cast<char *>(&coboIdx), 1);
    openFile.read(reinterpret_cast<char *>(&asadIdx), 1);
    openFile.read(reinterpret_cast<char *>(&readOffset), 2);
    openFile.read(reinterpret_cast<char *>(&status), 1);
    openFile.read(reinterpret_cast<char *>(hitpat0), 9);
    openFile.read(reinterpret_cast<char *>(hitpat1), 9);
    openFile.read(reinterpret_cast<char *>(hitpat2), 9);
    openFile.read(reinterpret_cast<char *>(hitpat3), 9);
    openFile.read(reinterpret_cast<char *>(&multip0), 2);
    openFile.read(reinterpret_cast<char *>(&multip1), 2);
    openFile.read(reinterpret_cast<char *>(&multip2), 2);
    openFile.read(reinterpret_cast<char *>(&multip3), 2);
    openFile.read(reinterpret_cast<char *>(&windowOut), 4);
    openFile.read(reinterpret_cast<char *>(&lastCell0), 2);
    openFile.read(reinterpret_cast<char *>(&lastCell1), 2);
    openFile.read(reinterpret_cast<char *>(&lastCell2), 2);
    openFile.read(reinterpret_cast<char *>(&lastCell3), 2);

    if (openFile.eof()) break;

/*
    cout << "metaType: " << (htons(metaType) >> 8) << endl;
    cout << "frameSize: " << (htonl(frameSize) >> 8) << endl;
    cout << "dataSource: " << (htons(dataSource) >> 8) << endl;
    cout << "frameType: " << htons(frameType) << endl;
    cout << "revision: " << (htons(revision) >> 8) << endl;
    cout << "headerSize: " << htons(headerSize) << endl;
    cout << "itemSize: " << htons(itemSize) << endl;
    cout << "nItems: " << htonl(nItems) << endl;
    cout << "eventTime: " << (htons(eventTime[2]) << 32) + (htons(eventTime[1]) << 16) + htons(eventTime[0]) << endl;
    */
    cout << "eventIdx: " << htonl(eventIdx) << endl;
//    cout << "coboIdx: " << (htons(coboIdx) >> 8) << endl;
//    cout << "asadIdx: " << (htons(asadIdx) >> 8) << endl;

    coboIdx = (htons(coboIdx) >> 8);
    asadIdx = (htons(asadIdx) >> 8);

/*
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

    Int_t rawdata[4*68*512] = {0};

    unsigned int data;
    openFile.ignore(41);
    for (int i = 0; i < htonl(nItems); i++) {
      openFile.read(reinterpret_cast<char *>(&data), 4);
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

      Int_t index = agetIdx*68*512 + chanIdx*512 + buckIdx;
      rawdata[index] = sample;
    }

    for (Int_t agetIdx = 0; agetIdx < 4; agetIdx++) {
      for (Int_t chanIdx = 0; chanIdx < 68; chanIdx++) {
        Int_t takenSamples = 511;

        Int_t index = agetIdx*68*512 + chanIdx*512;
        Int_t indexPed = coboIdx*4*4*68 + asadIdx*4*68 + agetIdx*68 + chanIdx;
        for (Int_t i = 0; i < takenSamples; i++) {
          prevPedestal[indexPed] = pedestal[indexPed];
          pedestal[indexPed] = prevPedestal[indexPed] + (rawdata[index + i] - prevPedestal[indexPed])/(Double_t)(numSamples[indexPed] + 1);

          if (numSamples[indexPed] > 1) {
            prevPedestalSigma[indexPed] = pedestalSigma[indexPed];
            pedestalSigma[indexPed] = numSamples[indexPed]*prevPedestalSigma[indexPed]/(Double_t)(numSamples[indexPed] + 1) + TMath::Power(rawdata[index + i] - pedestal[indexPed], 2)/(Double_t)(numSamples[indexPed]);
          }

          numSamples[indexPed]++;
        }
      }
    }
  }

  cout << "Writing in file!" << endl;
  for (Int_t coboIdx = 0; coboIdx < 48; coboIdx++) {
    for (Int_t asadIdx = 0; asadIdx < 4 ; asadIdx++) {
      for (Int_t agetIdx = 0; agetIdx < 4; agetIdx++) {
        for (Int_t chanIdx = 0; chanIdx < 68; chanIdx++) {
          Int_t index = coboIdx*4*4*68 + asadIdx*4*68 + agetIdx*68 + chanIdx;
          pedestalSigma[index] = TMath::Sqrt(pedestalSigma[index]);

          treeCoboIdx = coboIdx;
          treeAsadIdx = asadIdx;
          treeAgetIdx = agetIdx;
          treeChIdx = chanIdx;
          treePedestal = pedestal[index];
          treePedestalSigma = pedestalSigma[index];

          writeTree -> Fill();
        }
      }
    }
  }

  writeFile -> Write();

  return;
}

Int_t main(Int_t argc, Char_t ** argv) {
  if (argc == 1) {
      cout << "======================================================" << endl;
      cout << " Usage:"<< endl;
      cout << "     " << argv[0] << " <graw file>" << endl;
      cout << "======================================================" << endl;
  } else if (argc == 2)
    MakePedestal(argv[1]);

  return 0;
}
