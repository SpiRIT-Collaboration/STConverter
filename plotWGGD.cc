void plotWGGD(Int_t layer) {
  const Int_t numData = 3;

  TString filename[numData] = {"CoBo_AsAdX_2014-11-14T04:45:29.root",
                               "CoBo_AsAdX_2014-11-14T04:40:08.root",
                               "CoBo_AsAdX_2014-11-14T03:02:24.root"};

  TFile *file[numData];
  TTree *tree[numData];
  TH1D *hist[numData];
  for (Int_t iFile = 0; iFile < numData; iFile++) {
    file[iFile] = new TFile(filename[iFile]);
    tree[iFile] = (TTree *) file[iFile] -> Get("peak");
    hist[iFile] = new TH1D(Form("hist_%d", iFile), "", 1024, 0, 4096);
    tree[iFile] -> Project(Form("hist_%d", iFile), "adc", Form("layer==%d", layer), "");
    Double_t scale = 1/(Double_t)tree[iFile] -> GetEntries(Form("layer==%d", layer));
    hist[iFile] -> Scale(scale);
  }

  gStyle -> SetOptStat(0);
  gStyle -> SetPadLeftMargin(0.12);
  gStyle -> SetPadRightMargin(0.07);
  gStyle -> SetPadBottomMargin(0.12);
  gStyle -> SetPadTopMargin(0.08);

  TCanvas *cvs = new TCanvas("cvs", "", 800, 600);
  cvs -> cd();
  hist[1] -> SetLineColor(2);
  hist[1] -> GetXaxis() -> SetTitle(Form("Max. ADC in layer %d", layer));
  hist[1] -> GetXaxis() -> SetTitleOffset(1.5);
  hist[1] -> GetXaxis() -> CenterTitle();
  hist[1] -> GetXaxis() -> SetRangeUser(0, 800);
  hist[1] -> GetXaxis() -> SetTitleSize(0.05);
  hist[1] -> GetXaxis() -> SetLabelSize(0.05);
  hist[1] -> GetXaxis() -> SetTitleOffset(1.20);
  hist[1] -> GetYaxis() -> SetTitle("Normalized counts");
  hist[1] -> GetYaxis() -> CenterTitle();
  hist[1] -> GetYaxis() -> SetRangeUser(0, 0.06);
  hist[1] -> GetYaxis() -> SetTitleSize(0.05);
  hist[1] -> GetYaxis() -> SetLabelSize(0.05);
  hist[1] -> GetYaxis() -> SetTitleOffset(1.25);
  hist[1] -> Draw();
  hist[0] -> Draw("same");
  hist[2] -> SetLineColor(3);
  hist[2] -> Draw("same");

  TLegend *legend = new TLegend(0.20, 0.85, 0.45, 0.60, "Beam Intensity");
  legend -> SetEntrySeparation(0.1);
  legend -> SetMargin(0.4);
  legend -> AddEntry(hist[0], "10^{3}", "l");
  legend -> AddEntry(hist[1], "10^{4}", "l");
  legend -> AddEntry(hist[2], "10^{5}", "l");
  legend -> SetTextSize(0.05);
  legend -> SetFillStyle(0);
  legend -> SetLineColor(1);
  legend -> Draw("same");
}
