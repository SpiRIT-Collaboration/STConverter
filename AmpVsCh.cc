TCanvas *cvs;
TH2D *hAsad[4];

void PrepareCanvas(Int_t numAsads = 4) {
  gStyle -> SetOptStat(0);
  gStyle -> SetPadLeftMargin(0.04);
  gStyle -> SetPadRightMargin(0.04);
  gStyle -> SetTitleSize(0.05, "x");
  gStyle -> SetTitleSize(0.05, "y");
  gStyle -> SetLabelSize(0.05, "x");
  gStyle -> SetLabelSize(0.05, "y");
  gStyle -> SetTitleOffset(0.5, "x");
  gStyle -> SetTitleOffset(0.4, "y");
  gStyle -> SetTickLength(0.01, "y");
  gStyle -> SetPalette(55);
  gStyle -> SetNumberContours(100);

  cvs = new TCanvas("cvs", "", 2200, numAsads*400);
  cvs -> Divide(1, numAsads);

  hAsad[0] = new TH2D("hAsad0", "AsAd 1", 272, -0.5, 271.5, 8192, -0.5, 4095.5);
  hAsad[0] -> GetXaxis() -> SetTitle("Channel");
  hAsad[0] -> GetXaxis() -> CenterTitle();
  hAsad[0] -> GetYaxis() -> SetTitle("Amplitude");
  hAsad[0] -> GetYaxis() -> CenterTitle();
  hAsad[0] -> Fill(-10, -10);

  cvs -> cd(1);
  hAsad[0] -> Draw("colz");
  gPad -> Update();
  gPad -> SetLogz();
  
  TPaletteAxis *axis = (TPaletteAxis *) hAsad[0] -> GetListOfFunctions() -> FindObject("palette");
  axis -> SetX2NDC(0.98);
  axis -> GetAxis() -> SetTickSize(0.01);
  axis -> SetLabelSize(0.08);
  axis -> SetLabelOffset(-0.01);

  for (Int_t i = 1; i < numAsads; i++) {
    hAsad[i] = (TH2D *) hAsad[0] -> Clone(Form("hAsad%d", i + 1));
    hAsad[i] -> SetTitle(Form("AsAd %d", i + 1));
    hAsad[i] -> Fill(-10, -10);

    cvs -> cd(i + 1);
    hAsad[i] -> Draw("colz");
    gPad -> Update();
    gPad -> SetLogz();

    axis = (TPaletteAxis *) hAsad[i] -> GetListOfFunctions() -> FindObject("palette");
    axis -> SetX2NDC(0.98);
    axis -> GetAxis() -> SetTickSize(0.01);
    axis -> SetLabelSize(0.08);
    axis -> SetLabelOffset(-0.01);
  }
}

void AmpVsCh(TString date, TString data, TString file0, TString file1, TString file2 = "", TString file3 = "", Int_t cut = 0) {
  Int_t numData = 2;

  PrepareCanvas(numData);

  gSystem -> Load("libSPiRIT");
  STReadRaw *a[4] = {0};

  /*
  TString date = "20130911";
  TString data = "ramp_1pC_512samples_10to0w11steps_50MHz";
  TString file0 = "CoBo_AsAd0_2013-09-11T11-31-19.116_0000.graw";
  TString file1 = "CoBo_AsAd1_2013-09-11T11-31-19.126_0000.graw";
  TString file2 = "CoBo_AsAd2_2013-09-11T11-31-19.136_0000.graw";
  TString file3 = "CoBo_AsAd3_2013-09-11T11-31-19.157_0000.graw";
  */

  /*
  TString date = "20130912";
  TString data = "ramp_240fC_256samples_8to0w8steps_50MHz";
  TString file0 = "CoBo_AsAd0_2013-09-12T11-18-45.611_0000.graw";
  TString file1 = "CoBo_AsAd1_2013-09-12T11-18-45.617_0000.graw";
  TString file2 = "CoBo_AsAd2_2013-09-12T11-18-45.622_0000.graw";
  TString file3 = "CoBo_AsAd3_2013-09-12T11-18-45.633_0000.graw";
  */

  /*
  TString date = "20130912";
  TString data = "ramp_240fC_256samples_8to0w9steps_100MHz";
  TString file0 = "CoBo_AsAd0_2013-09-12T12-07-49.657_0000.graw";
  TString file1 = "CoBo_AsAd1_2013-09-12T12-07-49.663_0000.graw";
  TString file2 = "CoBo_AsAd2_2013-09-12T12-07-49.668_0000.graw";
  TString file3 = "CoBo_AsAd3_2013-09-12T12-07-49.679_0000.graw";
  */

  /*
  TString date = "20130912";
  TString data = "ramp_120fC_512samples_6to0w7steps_50MHz";
  TString file0 = "CoBo_AsAd0_2013-09-12T11-26-32.747_0000.graw";
  TString file1 = "CoBo_AsAd1_2013-09-12T11-26-32.757_0000.graw";
  TString file2 = "CoBo_AsAd2_2013-09-12T11-26-32.767_0000.graw";
  TString file3 = "CoBo_AsAd3_2013-09-12T11-26-32.788_0000.graw";
  */

  /*
  TString date = "20130912";
  TString data = "ramp_1pC_128samples_10to0w11steps_25MHz";
  TString file0 = "CoBo_AsAd0_2013-09-12T12-28-31.714_0000.graw";
  TString file1 = "CoBo_AsAd1_2013-09-12T12-28-31.717_0000.graw";
  TString file2 = "CoBo_AsAd2_2013-09-12T12-28-31.720_0000.graw";
  TString file3 = "CoBo_AsAd3_2013-09-12T12-28-31.723_0000.graw";
  */

  /*
  TString date = "20130912";
  TString data = "ramp_1pC_256samples_10to0w11steps_50MHz";
  TString file0 = "CoBo_AsAd0_2013-09-12T10-45-28.178_0000.graw";
  TString file1 = "CoBo_AsAd1_2013-09-12T10-45-28.184_0000.graw";
  TString file2 = "CoBo_AsAd2_2013-09-12T10-45-28.190_0000.graw";
  TString file3 = "CoBo_AsAd3_2013-09-12T10-45-28.200_0000.graw";
  */

  a[0] = new STReadRaw(Form("/Volumes/SPiRIT_data/test_%s/%s/%s", date.Data(), data.Data(), file0.Data()));
  a[1] = new STReadRaw(Form("/Volumes/SPiRIT_data/test_%s/%s/%s", date.Data(), data.Data(), file1.Data()));
  if (numData > 2)
    a[2] = new STReadRaw(Form("/Volumes/SPiRIT_data/test_%s/%s/%s", date.Data(), data.Data(), file2.Data()));
  if (numData > 3)
    a[3] = new STReadRaw(Form("/Volumes/SPiRIT_data/test_%s/%s/%s", date.Data(), data.Data(), file3.Data()));
  
  STGraw *ea[4] = {0};

  for (Int_t j = 0; j < numData; j++) {
    while (ea[j] = a[j] -> GetGraw()) {
      if (ea[j] == NULL) break;
      for (Int_t i = 0; i < 68; i++) {
        hAsad[j] -> Fill(i, (Double_t) ea[j] -> GetMaxADC(0, i));
        hAsad[j] -> Fill(i + 68, (Double_t) ea[j] -> GetMaxADC(1, i));
        hAsad[j] -> Fill(i + 68*2, (Double_t) ea[j] -> GetMaxADC(2, i));
        hAsad[j] -> Fill(i + 68*3, (Double_t) ea[j] -> GetMaxADC(3, i));
      }
    }

    cvs -> cd(j + 1);
    hAsad[j] -> Draw("colz");
    hAsad[j] -> GetYaxis() -> SetRangeUser(0, 4096);
  }

  cvs -> SaveAs(Form("%s.pdf", data.Data()));
  cvs -> SaveAs(Form("%s.png", data.Data()));
  cvs -> SaveAs(Form("%s.root", data.Data()));

  if (cut != 0) {
    for (Int_t j = 0; j < numData; j++) {
      cvs -> cd(j + 1);
      hAsad[j] -> GetYaxis() -> SetRangeUser(0, cut);
      hAsad[j] -> Draw("colz");
    }

    cvs -> SaveAs(Form("%s_%d.pdf", data.Data(), cut));
    cvs -> SaveAs(Form("%s_%d.png", data.Data(), cut));
    cvs -> SaveAs(Form("%s_%d.root", data.Data(), cut));
  }
}
