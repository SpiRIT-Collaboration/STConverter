gSystem -> Load("libSTConverter.so");

class STCore;
class STRawEvent;
class STPad;

TCanvas *cvs;
TH2D *hist[12];

STCore *core;

void Draw();

void layerPlot(TString filename) {
  gSystem -> Load("libSTConverter.so");

  cvs = new TCanvas("cvs", "", 1400, 1000);
  cvs -> Divide(4, 3);

  for (Int_t iLayer = 0; iLayer < 12; iLayer++)
    hist[iLayer] = new TH2D(Form("Layer_%d", iLayer), Form("Layer_%d", iLayer), 64, 0, 64, 512, 0, 512);

  core = new STCore(filename);
  core -> SetAGETMap("AGET.map");
  core -> SetUAMap("UnitAsAd.map");
  core -> SetFPNPedestal();
  core -> SetData(0);
}

void Draw() {
  for (Int_t iLayer = 0; iLayer < 12; iLayer++)
    hist[iLayer] -> Reset(); 

  STRawEvent *event = core -> GetRawEvent();

  for (Int_t iLayer = 0; iLayer < 12; iLayer++) {
    for (Int_t iRow = 0; iRow < 64; iRow++) {
      STPad *pad = event -> GetPad(iRow + 16, iLayer);
      Double_t *adc = pad -> GetADC();

      for (Int_t iTb = 0; iTb < 512; iTb++)
        hist[iLayer] -> SetBinContent(iRow, iTb, adc[iTb]);
    }

    cvs -> cd(iLayer + 1);
    hist[iLayer] -> Draw("colz");
  }

  cvs -> Draw();
}
