#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

#include "TString.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

#include "STCore.hh"
#include "STMap.hh"
#include "STRawEvent.hh"
#include "STPad.hh"
#include "STEvent.hh"
#include "STHit.hh"

//#define DEBUG
//#define DEBUG2

void makeSTEvent(TString grawFile, TString agetMap, TString uaMap, Bool_t fIsGainCalib, Int_t fNumTbs, Double_t fPeakTH, Double_t fPeakPercMin, Double_t fPeakPercMax, Int_t fMinPoints, Int_t fTb2ns, Int_t fShapeTime, Double_t sigmaThreshold, Double_t driftv);
Double_t GetSidePadADC(TH2D *layerHist, Int_t deleteMode, Int_t numTbs, Int_t peakPad, Int_t peakTb, Int_t tolerance);
void LSLFit(Int_t numPoints, Double_t *x, Double_t *y, Double_t &constant, Double_t &slope);

void makeSTEvent(TString grawFile, TString agetMap, TString uaMap, Bool_t fIsGainCalib, Int_t fNumTbs, Double_t fPeakTH, Double_t fPeakPercMin, Double_t fPeakPercMax, Int_t fMinPoints, Int_t fTb2ns, Int_t fShapeTime, Double_t sigmaThreshold, Double_t driftv)
{
  Int_t tolerance = ((Int_t)(fShapeTime/fTb2ns))*2;

  Int_t frontTbs = tolerance;
  Int_t backTbs = tolerance*2;

  STCore *core = new STCore(grawFile);
  core -> SetAGETMap(agetMap);
  core -> SetUAMap(uaMap);
  core -> SetFPNPedestal(sigmaThreshold);
  core -> SetNumTbs(fNumTbs);
  core -> SetData(0);
  if (fIsGainCalib == 1) {
    core -> SetGainCalibrationData("gainCalibration_pulser_HIMAC.recalib.root", "nf");
    core -> SetGainBase((Int_t)17, (Int_t)10);
  }

  TClonesArray *fEventArray = new TClonesArray("STEvent", 1000);
  Int_t storedEvent = 0;

  TH2D *layerHist = new TH2D("layerHist", "", 64, 0, 64, 512, 0, 512);
  STRawEvent *rawEvent;
  while ((rawEvent = core -> GetRawEvent())) {

    if (storedEvent%500 == 0)
      cout << "Processing event ID: " << storedEvent << endl;

    STEvent *event = new ((*fEventArray)[storedEvent]) STEvent();
    Int_t hitID = 0;

    event -> SetEventID(storedEvent);
#ifdef DEBUG
    cout << "Processing event ID: " << storedEvent << endl;
#endif

    for (Int_t iLayer = 0; iLayer < 12; iLayer++) {
#ifdef DEBUG
      cout << " Processing layer: " << iLayer << endl;
#endif
      layerHist -> Reset();

      for (Int_t iRow = 0; iRow < 64; iRow++) {
        STPad *pad = rawEvent -> GetPad(iRow + 16, iLayer);

        if (pad == NULL)
          continue;

        Double_t *adc = pad -> GetADC();

        for (Int_t iTb = 0; iTb < fNumTbs; iTb++)
          layerHist -> SetBinContent(iRow, iTb, adc[iTb]);
      }

      while (1) {
#ifdef DEBUG2
        cout << "  First while!" << endl;
#endif
        Int_t peakPad, peakTb, dummy;
        layerHist -> GetMaximumBin(peakPad, peakTb, dummy);

        Double_t adcSum[5] = {0};

        Double_t peakADC = layerHist -> GetBinContent(peakPad, peakTb);
        if (peakADC < fPeakTH)
          break;
        
        Double_t adc[100] = {0};
        Double_t tb[100] = {0};

        Int_t selectedTb = peakTb - 1;
        Int_t numPoints = 0;
        Int_t maxPoints = fShapeTime/fTb2ns + 2;
        while (1) {
#ifdef DEBUG2
          cout << "  Second while!" << endl;
#endif
          if (selectedTb < 1)
            break;

          Double_t slopeADC = layerHist -> GetBinContent(peakPad, selectedTb);

          if (slopeADC > peakADC*fPeakPercMax) {
            selectedTb--;
            continue;
          }

          if (slopeADC < peakADC*fPeakPercMin)
            break;

          adc[numPoints] = slopeADC;
          tb[numPoints] = selectedTb;

          selectedTb--;
          numPoints++;

          if (numPoints > maxPoints)
            break;
        }

        if (numPoints < fMinPoints) {
          GetSidePadADC(layerHist, 0, fNumTbs, peakPad, peakTb, tolerance);
//          GetSidePadADC(layerHist, 1, fNumTbs, peakPad, peakTb);
          if (peakPad == 1) {
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 1, peakTb, tolerance);
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 2, peakTb, tolerance);
          } else if (peakPad == 2) {
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 1, peakTb, tolerance);
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 1, peakTb, tolerance);
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 2, peakTb, tolerance);
          } else if (peakPad == 64) {
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 1, peakTb, tolerance);
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 2, peakTb, tolerance);
          } else if (peakPad == 63) {
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 1, peakTb, tolerance);
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 2, peakTb, tolerance);
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 1, peakTb, tolerance);
          }

          continue;
        }

/*
        TGraph graph(numPoints, tb, adc);
        Int_t fitFail = 1;
        Double_t fitConst, fitSlope;
        graph.LeastSquareLinearFit(numPoints, fitConst, fitSlope, fitFail);

        if (fitFail) {
          GetSidePadADC(layerHist, 1, fNumTbs, peakPad, peakTb);
          if (peakPad != 1)
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 1, peakTb);
          if (peakPad != 64)
            GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 1, peakTb);

          continue;
        }
*/
        Double_t fitConst, fitSlope;
        LSLFit(numPoints, tb, adc, fitConst, fitSlope);
        adcSum[0] = peakADC;

        Double_t hitTime = -fitConst/fitSlope;
        Int_t tbMin = (peakTb - frontTbs < 1 ? 1 : peakTb - frontTbs);
        Int_t tbMax = (peakTb + backTbs > fNumTbs ? fNumTbs : peakTb + backTbs);
        for (Int_t iTb = tbMin; iTb < tbMax; iTb++) {
//          adcSum[0] += layerHist -> GetBinContent(peakPad, iTb);
          layerHist -> SetBinContent(peakPad, iTb, 0);
        }

        Double_t posX = adcSum[0]*peakPad;
        if (peakPad == 1) {
          adcSum[1] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 1, peakTb, tolerance);
          posX += adcSum[1]*(peakPad + 1);
          adcSum[2] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 2, peakTb, tolerance);
          posX += adcSum[2]*(peakPad + 2);
        } else if (peakPad == 2) {
          adcSum[1] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 1, peakTb, tolerance);
          posX += adcSum[1]*(peakPad - 1);
          adcSum[2] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 1, peakTb, tolerance);
          posX += adcSum[2]*(peakPad + 1);
          adcSum[3] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 2, peakTb, tolerance);
          posX += adcSum[3]*(peakPad + 2);
        } else if (peakPad == 63) {
          adcSum[1] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 1, peakTb, tolerance);
          posX += adcSum[1]*(peakPad - 1);
          adcSum[2] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 2, peakTb, tolerance);
          posX += adcSum[2]*(peakPad - 2);
          adcSum[3] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 1, peakTb, tolerance);
          posX += adcSum[3]*(peakPad + 1);
        } else if (peakPad == 64) {
          adcSum[1] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 1, peakTb, tolerance);
          posX += adcSum[1]*(peakPad - 1);
          adcSum[2] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 2, peakTb, tolerance);
          posX += adcSum[2]*(peakPad - 2);
        } else {
          adcSum[1] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 1, peakTb, tolerance);
          adcSum[2] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad - 2, peakTb, tolerance);
          adcSum[3] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 1, peakTb, tolerance);
          adcSum[4] = GetSidePadADC(layerHist, 0, fNumTbs, peakPad + 2, peakTb, tolerance);

          posX += adcSum[1]*(peakPad - 1);
          posX += adcSum[2]*(peakPad - 2);
          posX += adcSum[3]*(peakPad + 1);
          posX += adcSum[4]*(peakPad + 2);
        }

        posX /= adcSum[0] + adcSum[1] + adcSum[2] + adcSum[3] + adcSum[4];
        posX = (posX + 15.5)*3.9 - 187.2;
        Double_t posZ = iLayer*30. + 15.;
        Double_t posY = -hitTime*fTb2ns*driftv; // mm

        if (hitTime < -10 || hitTime > fNumTbs)
          continue;

        STHit *hit = new STHit(hitID, posX, posY, posZ, adcSum[0]);
        hitID++;
        event -> AddHit(hit);
        delete hit;
      }

#ifdef DEBUG
      cout << " hitID: " << hitID << endl;
#endif
    }

    storedEvent++;
  }

  delete layerHist;

  grawFile.ReplaceAll(".graw", ".event.5.root");
  TFile *writeFile = new TFile(grawFile, "recreate");
  TTree *writeTree = new TTree("tree", "TClonesArray containing STEvent");
  writeTree -> Branch("eventBranch", "TClonesArray", &fEventArray);
  writeTree -> Fill();
  writeFile -> Write();
}

Double_t GetSidePadADC(TH2D *layerHist, Int_t deleteMode, Int_t numTbs, Int_t peakPad, Int_t peakTb, Int_t tolerance)
{
  Double_t sumADC = 0;

//  Int_t maxTb = (deleteMode == 1 ? peakTb : 0);
  Int_t maxTb = peakTb;
  Double_t maxADC = 0;

  Int_t tbMin = 0;
  Int_t tbMax = 0;

/*
  if (!deleteMode) {
    tbMin = (peakTb - 2 < 1 ? 1 : peakTb - 2);
    tbMax = (peakTb + 2 > numTbs ? numTbs : peakTb + 2);
    for (Int_t iTb = tbMin; iTb < tbMax + 1; iTb++) {
      Double_t adc = layerHist -> GetBinContent(peakPad, iTb);
      if (adc > maxADC) {
        maxADC = adc;
        maxTb = iTb;
      }
    }
  }

  sumADC = maxADC;
*/
  sumADC = layerHist -> GetBinContent(peakPad, peakTb);

  Int_t frontTbs = tolerance;
  Int_t backTbs = tolerance*2;

  tbMin = (maxTb - frontTbs < 1 ? 1 : maxTb - frontTbs);
  tbMax = (maxTb + backTbs > numTbs ? numTbs : maxTb + backTbs);
  for (Int_t iTb = tbMin; iTb < tbMax + 1; iTb++) {
//    sumADC += layerHist -> GetBinContent(peakPad, iTb);
    layerHist -> SetBinContent(peakPad, iTb, 0);
  }

  return sumADC;
}

void LSLFit(Int_t numPoints, Double_t *x, Double_t *y, Double_t &constant, Double_t &slope)
{
  Double_t sumXY = 0, sumX = 0, sumY = 0, sumX2 = 0;
  for (Int_t i = 0; i < numPoints; i++) {
    sumXY += x[i]*y[i];
    sumX += x[i];
    sumY += y[i];
    sumX2 += x[i]*x[i];
  }

  slope = (numPoints*sumXY - sumX*sumY)/(numPoints*sumX2 - sumX*sumX);
  constant = (sumX2*sumY - sumX*sumXY)/(numPoints*sumX2 - sumX*sumX);
}

Int_t main(Int_t argc, Char_t **argv)
{
  if (argc < 10) {
    cout << "Usage: " << argv[0] << " FILENAME.graw AGET.map UnitAsad.map GAIN_CALIB NUM_TBS PEAK_THRESHOLD PEAK_PERC_MIN PEAK_PREC_MAX MIN_PNTS [SAMPLERATE(25 MHz) SHAPINGTIME(232 ns) SIGMATH(5)]" << endl;

    return false;
  }

  //     GAIN_CALIB : Whether use gain calibration data or not
  //        NUM_TBS : Number of time buckets in one channel data
  // PEAK_THRESHOLD : Peak threshold value for finding peak
  //  PEAK_PERC_MIN : Minimum percentage of peak value for finding points for hit time fit
  //  PEAK_PERC_MAX : Maximum percentage of peak value for finding points for hit time fit
  //       MIN_PNTS : Minimum points for fitting hit time
  //     SAMPLERATE : Sampling rate (default: 25 MHz)
  //      SHAPETIME : Shaping time (default: 232 ns)

  Int_t samplingRate = 0;
  Int_t shapingTime = 0;
  Double_t sigmaThreshold = 0;
  Double_t driftv = 0;

  if (argc == 10) {
    samplingRate = 25;
    shapingTime = 232;
    sigmaThreshold = 5;
    driftv = 0.0995;
  } else if (argc == 10) {
    samplingRate = atoi(argv[10]);
    shapingTime = 232;
    sigmaThreshold = 5;
    driftv = 0.0995;
  } else if (argc == 12) {
    samplingRate = atoi(argv[10]);
    shapingTime = atoi(argv[11]);
    sigmaThreshold = 5;
    driftv = 0.0995;
  } else if (argc == 13) {
    samplingRate = atoi(argv[10]);
    shapingTime = atoi(argv[11]);
    sigmaThreshold = atof(argv[12]);
    driftv = 0.0995;
  } else {
    samplingRate = atoi(argv[10]);
    shapingTime = atoi(argv[11]);
    sigmaThreshold = atof(argv[12]);
    driftv = atof(argv[13]);
  }

  Int_t tb2ns = (1./((Double_t)samplingRate))*1000.;
  Bool_t gainCalib = atoi(argv[4]);

  TString filename = argv[1];

  cout << "============================================================================================" << endl;
  cout << "            GRAW file: " << argv[1] << endl;
  cout << "             AGET map: " << argv[2] << endl;
  cout << "               UA map: " << argv[3] << endl;
  cout << "               NumTbs: " << argv[5] << endl;
  cout << "       Peak Threshold: " << argv[6] << endl;
  cout << "       Peak % Minimum: " << atof(argv[7]) << " %" << endl;
  cout << "       Peak % Maximum: " << atof(argv[8]) << " %" << endl;
  cout << "       Minimum points: " << atoi(argv[9]) << endl;
  cout << "  FPN Sigma Threshold: " << sigmaThreshold << endl;
  cout << "        Sampling rate: " << samplingRate << " MHz" << endl;
  cout << "         Shaping time: " << shapingTime << " ns" << endl;
  cout << "                  1tb: " << tb2ns << " ns" << endl;
  cout << "       Drift velocity: " << driftv << " mm/ns" << endl;
  cout << "     Gain Calibration: " << (gainCalib == 1 ? "YES" : "NO") << endl;
  cout << "============================================================================================" << endl;
  cout << endl;

  makeSTEvent(argv[1], argv[2], argv[3], gainCalib, atoi(argv[5]), atof(argv[6]), atof(argv[7])/100, atof(argv[8])/100, atoi(argv[9]), tb2ns, shapingTime, sigmaThreshold, driftv);

  return false;
}
