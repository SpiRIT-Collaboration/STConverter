class GETDecoder;
class GETFrame;

TCanvas *cvs;
TGraph *gAget[4];
GETDecoder *readraw = NULL;

Int_t minTb = 0;
Int_t maxTb = 276;

void showGraw(void);
void showGraw(Int_t frameNo = 0);
void showChannel(Int_t agetIdx, Int_t chIdx);
void showSum(Bool_t isConnected = 0, Int_t frameNo = 0);

void PrepareCanvas() {
  gStyle -> SetOptStat(0);
  gStyle -> SetTitleSize(0.05, "x");
  gStyle -> SetTitleSize(0.05, "y");
  gStyle -> SetLabelSize(0.05, "x");
  gStyle -> SetLabelSize(0.04, "y");
  gStyle -> SetTitleOffset(1.0, "x");
  gStyle -> SetTitleOffset(1.2, "y");
  gStyle -> SetPadLeftMargin(0.12);
  gStyle -> SetPadRightMargin(0.03);

  cvs = new TCanvas("cvs", "", 1200, 1000);
  cvs -> Divide(2, 2);

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    gAget[iAget] = new TGraph();
    gAget[iAget] -> SetTitle(Form("AGET %d", iAget));
    gAget[iAget] -> SetMarkerStyle(2);
    gAget[iAget] -> SetPoint(0, -10, -20);
    gAget[iAget] -> SetPoint(1, 520, 4200);

    cvs -> cd(iAget + 1);
    gAget[iAget] -> Draw("AP");
    gAget[iAget] -> GetXaxis() -> SetTitle("Time Bucket");
    gAget[iAget] -> GetXaxis() -> CenterTitle();
    gAget[iAget] -> GetXaxis() -> SetRangeUser(minTb - 1, maxTb + 1);
    gAget[iAget] -> GetYaxis() -> SetTitle("ADC Channel");
    gAget[iAget] -> GetYaxis() -> CenterTitle();
    gAget[iAget] -> GetYaxis() -> SetRangeUser(-10, 4106);
  }
}

void PrepareOneCanvas(Int_t agetIdx, Int_t chIdx) {
  gStyle -> SetOptStat(0);
  gStyle -> SetPadLeftMargin(0.15);
  gStyle -> SetPadRightMargin(0.05);
  gStyle -> SetTitleSize(0.05, "x");
  gStyle -> SetTitleSize(0.05, "y");
  gStyle -> SetLabelSize(0.05, "x");
  gStyle -> SetLabelSize(0.04, "y");
  gStyle -> SetTitleOffset(1.0, "x");
  gStyle -> SetTitleOffset(1.5, "y");

  cvs = new TCanvas("cvs", "", 600, 500);

  gAget[0] = new TGraph();
  gAget[0] -> SetTitle(Form("AGET %d - Channel %d", agetIdx, chIdx));
  gAget[0] -> SetMarkerStyle(2);
  gAget[0] -> SetPoint(0, -20, -20);
  gAget[0] -> SetPoint(1, 532, 4200);

  gAget[0] -> Draw("AP");
  gAget[0] -> GetXaxis() -> SetTitle("Time Bucket");
  gAget[0] -> GetXaxis() -> CenterTitle();
  gAget[0] -> GetXaxis() -> SetRangeUser(-1, 513);
  gAget[0] -> GetYaxis() -> SetTitle("ADC Channel");
  gAget[0] -> GetYaxis() -> CenterTitle();
  gAget[0] -> GetYaxis() -> SetRangeUser(-10, 4106);
}

void showGraw() {
  gSystem -> Load("libGETDecoder");

  if (readraw != NULL)
    delete readraw;
  
  // If you want to run the macro without inputting manually, fill the blank below.
  TString filename = "";
  while (filename.EqualTo("")) {
    Char_t input[1000];
    cout << "filename: ";
    cin >> input;
    filename = input;
  }

  readraw = new GETDecoder(filename.Data());

  Int_t allOrSingle = -1;
  while (allOrSingle <= -1 || allOrSingle >= 4) {
    cout << "Single Channel(0), All Channels(1), Summed Signal(2)): ";
    cin >> allOrSingle;
  }

  if (allOrSingle == 0) {
    Int_t agetIdx = -1;
    Int_t chIdx = -1;
    while (agetIdx <= -1 || agetIdx >= 4 || chIdx <= -1 || chIdx >= 68) {
      cout << "Type agetIdx and chIdx with space(ex. 3 50): ";
      cin >> agetIdx >> chIdx;
    }

    showChannel(agetIdx, chIdx);
  } else if (allOrSingle == 1) {
    showGraw(0);
  } else if (allOrSingle == 2) {
    Int_t isConnected = -1;
    while (isConnected <= -1 || isConnected > 2) {
      cout << "Not connected(0) or Connected(1): ";
      cin >> isConnected;
    }

    showSum((Bool_t) isConnected, 0);
  }

}

void showGraw(Int_t frameNo) {
  PrepareCanvas();

  gSystem -> Load("libGETDecoder");

  GETFrame *aGraw = NULL;

  aGraw = readraw -> GetFrame(frameNo);

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    for (Int_t iCh = 0; iCh < 68; iCh++) {
      Int_t *rawadc = NULL;
      rawadc = aGraw -> GetRawADC(iAget, iCh);
      
      Int_t numSamples = 0;
      Int_t tb[512] = {0};
      Int_t charge[512] = {0};

      for (Int_t iTb = 0; iTb < 512; iTb++) {
        if (rawadc[iTb] == 0)
          continue;
        
        tb[numSamples] = iTb;
        charge[numSamples] = rawadc[iTb];
        numSamples++;
      }

      Float_t r = 0, g = 0, b = 0;
      Float_t h = iCh/67.*360, s = 0.5, l = 0.5;
      TColor::HLS2RGB(h, l, s, r, g, b);
      Int_t markerColor = TColor::GetColor(r, g, b);

      TGraph *graph = new TGraph(numSamples, tb, charge);
      graph -> SetMarkerColor(markerColor);
      graph -> SetLineColor(markerColor);
      graph -> SetMarkerStyle(2);
      graph -> SetMarkerSize(0.3);
      cvs -> cd(iAget + 1);
      graph -> Draw("PL same");
    }
  }
}

void showChannel(Int_t agetIdx, Int_t chIdx) {
  PrepareOneCanvas(agetIdx, chIdx);

  gSystem -> Load("libGETDecoder");

  GETFrame *aGraw = NULL;

  while (aGraw = readraw -> GetFrame()) {
    Int_t *rawadc = NULL;
    rawadc = aGraw -> GetRawADC(agetIdx, chIdx);

    Int_t numSamples = 0;
    Int_t tb[512] = {0};
    Int_t charge[512] = {0};

    for (Int_t iTb = 0; iTb < 512; iTb++) {
      if (rawadc[iTb] == 0)
        continue;

      tb[numSamples] = iTb;
      charge[numSamples] = rawadc[iTb];
      numSamples++;
    }

    Float_t r = 0, g = 0, b = 0;
    Float_t h = (aGraw -> GetEventID())%360, s = 0.5, l = 0.5;
    TColor::HLS2RGB(h, l, s, r, g, b);
    Int_t markerColor = TColor::GetColor(r, g, b);

    TGraph *graph = new TGraph(numSamples, tb, charge);
    graph -> SetMarkerColor(markerColor);
    graph -> SetLineColor(markerColor);
    graph -> SetMarkerStyle(2);
    graph -> SetMarkerSize(0.3);
    cvs -> cd();
    graph -> Draw("PL same");
  }
}

void showSum(Bool_t isConnected, Int_t frameNo) {
  PrepareCanvas();

  gSystem -> Load("libGETDecoder");

  GETFrame *aGraw = NULL;

  Double_t tb[4][512] = {0};
  Double_t charge[4][512] = {0};

  aGraw = readraw -> GetFrame(frameNo);

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    for (Int_t iCh = 0; iCh < 68; iCh++) {
      if (isConnected == 1) {
        if (iCh == 11 || iCh == 22 || iCh == 45 || iCh == 56 || iCh == 67)
          continue;
      } else {
        if (iCh != 11 && iCh != 22 && iCh != 45 && iCh != 56)
          continue;
      }

      Int_t *rawadc = NULL;
      rawadc = aGraw -> GetRawADC(iAget, iCh);

      Int_t numSamples = 0;

      for (Int_t iTb = 0; iTb < 512; iTb++) {
        //          if (rawadc[iTb] == 0)
        //            continue;

        tb[iAget][numSamples] = (Double_t) iTb;
        charge[iAget][numSamples] += (Double_t) rawadc[iTb];
        numSamples++;
      }
    }
  }

  Double_t divider = 0;
  if (isConnected == 1)
    divider = 63;
  else 
    divider = 4;

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    for (Int_t iSample = 0; iSample < 512; iSample++) {
      charge[iAget][iSample] /= divider;
    }
  }

  for (Int_t iAget = 0; iAget < 4; iAget++) {
    TGraph *graph = new TGraph(512, tb[iAget], charge[iAget]);
    graph -> SetMarkerColor(2);
    graph -> SetLineColor(2);
    graph -> SetMarkerStyle(2);
    graph -> SetMarkerSize(0.3);
    cvs -> cd(iAget + 1);
    graph -> Draw("PL same");

    Double_t min = 5000;
    Double_t max = -1000;
    for (Int_t iSample = 0; iSample < 512; iSample++) {
      if (charge[iAget][iSample] < min) min = charge[iAget][iSample];
      if (charge[iAget][iSample] > max) max = charge[iAget][iSample];
    }

    gAget[iAget] -> GetYaxis() -> SetRangeUser(min - 10, max + 10);
  }
}
