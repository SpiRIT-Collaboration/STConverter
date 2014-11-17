void peakFinder(TString filename) {
  gSystem -> Load("libSTConverter.so");

  STCore *core = new STCore(filename, 512);
  core -> SetAGETMap("AGET.map");
  core -> SetUAMap("UnitAsAd.map");
  core -> SetFPNPedestal();
  core -> SetData(0);

  Int_t layer;
  Int_t row;
  Double_t maxADC;

  filename.ReplaceAll(".graw", ".root");
  TFile *file = new TFile(filename, "recreate");
  TTree *tree = new TTree("peak", "");
  tree -> Branch("layer", &layer, "layer/I");
  tree -> Branch("row", &row, "row/I");
  tree -> Branch("adc", &maxADC, "adc/D");

  STRawEvent *event = NULL;
  while ((event = core -> GetRawEvent())) {
    Int_t eventID = event -> GetEventID();
    if (eventID != 0 && eventID%500 == 0)
      cout << "Event ID: " << eventID << endl;

    for (Int_t iLayer = 0; iLayer < 12; iLayer++) {
      maxADC = -9999;
      layer = -1;
      row = -1;
      for (Int_t iRow = 16; iRow < 80; iRow++) {
        STPad *aPad = event -> GetPad(iRow, iLayer);
        Double_t *adc = aPad -> GetADC();

        for (Int_t iTb = 0; iTb < 512; iTb++) {
          if (maxADC < adc[iTb]) {
            maxADC = adc[iTb];
            layer = iLayer;
            row = iRow;
          }
        }
      }

      tree -> Fill();
    }
  }

  file -> Write();
}
