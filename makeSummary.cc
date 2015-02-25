#ifndef __CINT__
#include "GETDecoder.hh"
#include "GETFrame.hh"

#include "TString.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TH1D.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using std::ofstream;
using std::cout;
using std::endl;
using std::setw;

void makeSummary(Int_t numData, TString **grawFile, Int_t numTbs, Bool_t positivePolarity);
void Print(ofstream &outFile,
              TTree *outTree,
              Int_t  coboIdx,
              Int_t  asadIdx,
              Int_t  agetIdx,
              Int_t  chIdx,
           Double_t  mean,
           Double_t  sigma,
           Double_t  sigmaWOFPN, /// sigma with the FPN-subtracted
           Double_t  maxADCMean,
           Double_t  maxADCSigma);

Int_t wCoboIdx, wAsadIdx, wAgetIdx, wChIdx;
Double_t wMean, wSigma, wSigmaWOFPN, wMaxADCMean, wMaxADCSigma;

void makeSummary(Int_t numData, TString **grawFile, Int_t numTbs, Bool_t positivePolarity) {
#ifdef __CINT__
  gSystem -> Load("libSTConverter.so");
#endif

  // Create txt and root output file
  TString outName = grawFile[0] -> Data();
  TObjArray *elements = outName.Tokenize("/");

  Int_t lastIndex = elements -> GetLast();
  outName = ((TObjString *) elements -> At(lastIndex)) -> GetString();

  outName.ReplaceAll(".graw", ".summary.txt");
  cout << "== [" << __FUNCTION__ << "] Output ASCII file name: " << outName << endl;

  ofstream outFile(outName.Data());
  outFile << "# CoBo  AsAd  AGET  Ch" << setw(20) << "mean" << setw(20) << "sigma";
  outFile << setw(20) << "sigma w/o FPN" << setw(20) << "max ADC mean" << setw(20) << "max ADC sigma" << endl;

  outName.ReplaceAll(".txt", ".root");
  TFile *outRootFile = new TFile(outName.Data(), "recreate");
  cout << "== [" << __FUNCTION__ << "] Output ROOT file name: " << outName << endl;

  TTree *outTree = new TTree("summary", "");
  outTree -> Branch("cobo", &wCoboIdx, "wCoboIdx/I");
  outTree -> Branch("asad", &wAsadIdx, "wAsadIdx/I");
  outTree -> Branch("aget", &wAgetIdx, "wAgetIdx/I");
  outTree -> Branch("ch", &wChIdx, "wChIdx/I");
  outTree -> Branch("mean", &wMean, "wMean/D");
  outTree -> Branch("sigma", &wSigma, "wSigma/D");
  outTree -> Branch("sigmaWOFPN", &wSigmaWOFPN, "wSigmaWOFPN/D");
  outTree -> Branch("maxADCMean", &wMaxADCMean, "wMaxADCMean/D");
  outTree -> Branch("maxADCSigma", &wMaxADCSigma, "wMaxADCSigma/D");

  // Initialize histograms
  TH1D *hMean[12][3][4][68];
  TH1D *hSigma[12][3][4][68];
  TH1D *hSigmaWOFPN[12][3][4][68];
  TH1D *hMaxADCMean[12][3][4][68];

  for (Int_t iCobo = 0; iCobo < 12; iCobo++) {
    for (Int_t iAsad = 0; iAsad < 4; iAsad++) {
      for (Int_t iAget = 0; iAget < 4; iAget++) {
        for (Int_t iCh = 0; iCh < 68; iCh++) {
          hMean[iCobo][iAsad][iAget][iCh] = new TH1D(Form("hMean_%d_%d_%d_%d", iCobo, iAsad, iAget, iCh), "", 4096, 0, 4096);
          hSigma[iCobo][iAsad][iAget][iCh] = new TH1D(Form("hSigma_%d_%d_%d_%d", iCobo, iAsad, iAget, iCh), "", 4224, -128, 4096);
          hSigmaWOFPN[iCobo][iAsad][iAget][iCh] = new TH1D(Form("hSigmaWOFPN_%d_%d_%d_%d", iCobo, iAsad, iAget, iCh), "", 4224, -128, 4096);
          hMaxADCMean[iCobo][iAsad][iAget][iCh] = new TH1D(Form("hMaxADCMean_%d_%d_%d_%d", iCobo, iAsad, iAget, iCh), "", 4096, 0, 4096);
        }
      }
    }
  }

  TH1D *raHist = new TH1D("rawADCHist", "", 4096, 0, 4096); // reused histogram
  TH1D *aHist = new TH1D("adcHist", "", 4096, 0, 4096); // reused histogram

  // Load GRAW file and initialize mapping
  GETDecoder *fDecoder = new GETDecoder();
  for (Int_t iData = 0; iData < numData; iData++)
    fDecoder -> AddData(grawFile[iData] -> Data());

  fDecoder -> SetData(0);

  fDecoder -> SetPositivePolarity(positivePolarity);
  fDecoder -> SetNumTbs(numTbs);

  // Start processing
  GETFrame *fFrame = NULL;
  while ((fFrame = fDecoder -> GetFrame())) {
    Int_t eventIdx = fFrame -> GetEventID();

    // For M-CoBo
    Int_t coboIdx = fFrame -> GetCoboID();
    Int_t asadIdx = fFrame -> GetAsadID();

    // For R-CoBo
//    Int_t asadIdx = fFrame -> GetCoboID();
//    Int_t coboIdx = fFrame -> GetAsadID();

    if (eventIdx != 0 && eventIdx%100 == 0 && asadIdx == 0)
      cout << "== [" << __FUNCTION__ << "] Event index: " << eventIdx << endl;

    fFrame -> SetFPNPedestal(200);

    for (Int_t iAget = 0; iAget < 4; iAget++) {
      for (Int_t iCh = 0; iCh < 68; iCh++) {
        raHist -> Reset();
        aHist -> Reset();

        fFrame -> SubtractPedestal(iAget, iCh);
        Int_t *rawAdc = fFrame -> GetRawADC(iAget, iCh);
        Double_t *adc = fFrame -> GetADC(iAget, iCh);

        for (Int_t iAdc = 1; iAdc < numTbs - 1; iAdc++) {
          raHist -> Fill(rawAdc[iAdc]);
          hSigma[coboIdx][asadIdx][iAget][iCh] -> Fill(rawAdc[iAdc]); // <-- Sigma

          aHist -> Fill(adc[iAdc]);
          hSigmaWOFPN[coboIdx][asadIdx][iAget][iCh] -> Fill(adc[iAdc]); // <-- FPN-subtracted Sigma
        }

/*
        raHist -> Fit("gaus", "0Q"); // Fit to find mean

        Double_t mean = ((TF1 *) raHist -> GetFunction("gaus")) -> GetParameter(1);
*/
        Double_t mean = raHist -> GetMean();
        hMean[coboIdx][asadIdx][iAget][iCh] -> Fill(mean); // <-- Mean

        Double_t maxADC = 0;
        for (Int_t iTb = 1; iTb < numTbs - 1; iTb++) {
          Double_t temp = adc[iTb];
          if (temp > maxADC)
            maxADC = temp;
        }
        hMaxADCMean[coboIdx][asadIdx][iAget][iCh] -> Fill(maxADC);
//        }
      }
    }
  }

  delete raHist;
  delete aHist;

  for (Int_t iCobo = 0; iCobo < 12; iCobo++) {
    for (Int_t iAsad = 0; iAsad < 4; iAsad++) {
      for (Int_t iAget = 0; iAget < 4; iAget++) {
        for (Int_t iCh = 0; iCh < 68; iCh++) {
          Double_t mean = 0;
          if (hMean[iCobo][iAsad][iAget][iCh] -> GetEntries()) {
  //          hMean[iAsad][iAget][iCh] -> Fit("gaus", "0Q");
  //          mean = ((TF1 *) hMean[iAsad][iAget][iCh] -> GetFunction("gaus")) -> GetParameter(1);
            mean = hMean[iCobo][iAsad][iAget][iCh] -> GetMean();
          }

          Double_t sigma = 0;
          if (hSigma[iCobo][iAsad][iAget][iCh] -> GetEntries()) {
  //          hSigma[iAsad][iAget][iCh] -> Fit("gaus", "0Q");
  //          sigma = ((TF1 *) hSigma[iAsad][iAget][iCh] -> GetFunction("gaus")) -> GetParameter(2);
            sigma = hSigma[iCobo][iAsad][iAget][iCh] -> GetRMS();
          }

          Double_t sigmaWOFPN = 0;
          if (hSigmaWOFPN[iCobo][iAsad][iAget][iCh] -> GetEntries()) {
  //          hSigmaWOFPN[iAsad][iAget][iCh] -> Fit("gaus", "0Q");
  //          sigmaWOFPN = ((TF1 *) hSigmaWOFPN[iAsad][iAget][iCh] -> GetFunction("gaus")) -> GetParameter(2);
            sigmaWOFPN = hSigmaWOFPN[iCobo][iAsad][iAget][iCh] -> GetRMS();
          }

          Double_t maxADCMean = 0;
          Double_t maxADCSigma = 0;
          if (hMaxADCMean[iCobo][iAsad][iAget][iCh] -> GetEntries()) {
  //          hMaxADCMean[iAsad][iAget][iCh] -> Fit("gaus", "0Q");
  //          maxADCMean = ((TF1 *) hMaxADCMean[iAsad][iAget][iCh] -> GetFunction("gaus")) -> GetParameter(1);
  //          maxADCSigma = ((TF1 *) hMaxADCMean[iAsad][iAget][iCh] -> GetFunction("gaus")) -> GetParameter(2);
            maxADCMean = hMaxADCMean[iCobo][iAsad][iAget][iCh] -> GetMean();
            maxADCSigma = hMaxADCMean[iCobo][iAsad][iAget][iCh] -> GetRMS();
          }

          // M-CoBo
          Print(outFile, outTree, iCobo, iAsad, iAget, iCh, mean, sigma, sigmaWOFPN, maxADCMean, maxADCSigma);

          // R-CoBo
  //        Print(outFile, outTree, iAsad, 0, iAget, iCh, mean, sigma, sigmaWOFPN, maxADCMean, maxADCSigma);
        }
      }
    }
  }

  outRootFile -> Write();
}

void Print(ofstream &outFile, TTree *outTree, Int_t coboIdx, Int_t asadIdx, Int_t agetIdx, Int_t chIdx, Double_t mean, Double_t sigma, Double_t sigmaWOFPN, Double_t maxADCMean, Double_t maxADCSigma)
{
  outFile << setw(6) << coboIdx << setw(6) << asadIdx << setw(6) << agetIdx;
  outFile << setw(4) << chIdx << setw(20) << mean << setw(20) << sigma;
  outFile << setw(20) << sigmaWOFPN << setw(20) << maxADCMean << setw(20) << maxADCSigma << endl;

  wCoboIdx = coboIdx;
  wAsadIdx = asadIdx;
  wAgetIdx = agetIdx;
  wChIdx = chIdx;
  wMean = mean;
  wSigma = sigma;
  wSigmaWOFPN = sigmaWOFPN;
  wMaxADCMean = maxADCMean;
  wMaxADCSigma = maxADCSigma;

  outTree -> Fill();
}

#ifndef __CINT__
Int_t main(Int_t argc, Char_t **argv) {
  if (argc < 4) {
    cout << "Usage: " << argv[0] << " NUMTBS  POLARITY(+:1, -:0)  FILENAME.graw  [FILENAME2.graw ....]" << endl;

    return kFALSE;
  }

  Int_t numTbs = atoi(argv[1]);
  Bool_t positivePolarity = (Bool_t)atoi(argv[2]);
  TString **grawFile = new TString*[100];
  Int_t numData = argc - 3;
  for (Int_t iFile = 0; iFile < numData; iFile++)
    grawFile[iFile] = new TString(argv[3 + iFile]);
  makeSummary(numData, grawFile, numTbs, positivePolarity);

  return kFALSE;
}
#endif
