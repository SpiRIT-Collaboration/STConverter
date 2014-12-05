TCanvas *padplaneCvs;
TH2D *padplaneHist;
TLine *horizLine[12];
TLine *vertLine[96];

void PreparePadplaneHist();

void sigmaPlot(TString filename) {
    gSystem -> Load("libSTConverter.so");

    STMap *map = new STMap();
    map -> SetAGETMap("/home/daq/GET/release-20141001/workarea/STConverter/AGET.map");
    map -> SetUAMap("/home/daq/GET/release-20141001/workarea/STConverter/UnitAsAd.map");

    TFile *file = new TFile(filename.Data());
    TTree *tree = (TTree *) file -> Get("summary");
    
    Int_t cobo, asad, aget, ch;
    Double_t sigma;
    tree -> SetBranchAddress("cobo", &cobo);
    tree -> SetBranchAddress("asad", &asad);
    tree -> SetBranchAddress("aget", &aget);
    tree -> SetBranchAddress("ch", &ch);
    tree -> SetBranchAddress("sigmaWOFPN", &sigma);

    PreparePadplaneHist();
    
    Int_t numEntries = tree -> GetEntries();
    for (Int_t iEntry = 0; iEntry < numEntries; iEntry++) {
        tree -> GetEntry(iEntry);

        Int_t row, layer = -2;
        map -> GetRowNLayer(cobo, asad, aget, ch, row, layer);

        padplaneHist -> SetBinContent(row + 1, layer + 1, sigma);
    }

    padplaneCvs -> Modified();
    padplaneCvs -> Update();
}

void PreparePadplaneHist()
{
//    cvs = new TCanvas("Event Display", "", 1600, 1000); // For large monitor
    padplaneCvs = new TCanvas("Event Display", "", 870, 780);

    gStyle -> SetOptStat(0000);
    gStyle -> SetPadRightMargin(0.14);
    gStyle -> SetPadLeftMargin(0.11);
    gStyle -> SetPadTopMargin(0.08);
    gStyle -> SetPadBottomMargin(0.08);
    gStyle -> SetTitleOffset(1.2, "X");
    gStyle -> SetTitleOffset(1.2, "Y");

    padplaneCvs -> Draw();

/*
    timePad = new TPad("timePad", "Time Plane Display", 0.5, 0, 1, 1);
    timePad -> Divide(2, 2);
    timePad -> Draw();
*/
    
    padplaneCvs -> cd();
    padplaneHist = new TH2D("padplaneHist", "BRAHMS-TPC Pad Plane (Sigma)", 96, -187.2, 187.2, 12, 0, 360);
//        padplane -> GetYaxis() -> SetNdivisions(15, kTRUE);
    padplaneHist -> GetXaxis() -> SetTickLength(0.01);
    padplaneHist -> GetXaxis() -> SetTitle("x (mm)");
    padplaneHist -> GetXaxis() -> CenterTitle();
    padplaneHist -> GetYaxis() -> SetTickLength(0.01);
    padplaneHist -> GetYaxis() -> SetTitle("z (mm)");
    padplaneHist -> GetYaxis() -> CenterTitle();
    padplaneHist -> GetZaxis() -> SetRangeUser(1.5, 5);
    padplaneHist -> Draw("colz");

    Double_t padLX = 3.9; // mm
    Double_t padLZ = 30.; // mm

    for (Int_t i = 0; i < 11; i++) {
      horizLine[i] = new TLine(-187.2, (i + 1)*padLZ, 187.2, (i + 1)*padLZ);
      horizLine[i] -> SetLineStyle(3);
      horizLine[i] -> Draw("same");
    }

    for (Int_t i = 0; i < 95; i++) {
      vertLine[i] = new TLine((i + 1)*padLX - 187.2, 0, (i + 1)*padLX - 187.2, 360);
      vertLine[i] -> SetLineStyle(3);
      vertLine[i] -> Draw("same");
    }
}
