{
  gSystem -> Load("libSPiRIT");
  STReadRaw *a[4] = {0};
  
  a[0] = new STReadRaw("test_20130911/ramp_1pC/CoBo_AsAd0_2013-09-11T11-31-19.116_0000.graw");
  a[1] = new STReadRaw("test_20130911/ramp_1pC/CoBo_AsAd1_2013-09-11T11-31-19.126_0000.graw");
  a[2] = new STReadRaw("test_20130911/ramp_1pC/CoBo_AsAd2_2013-09-11T11-31-19.136_0000.graw");
  a[3] = new STReadRaw("test_20130911/ramp_1pC/CoBo_AsAd3_2013-09-11T11-31-19.157_0000.graw");

  STGraw *ea[4] = {0};
  
  TH2D *hist[4] = {0};
  hist[0] = new TH2D("hist", "", 272, -0.5, 271.5, 8192, -0.5, 4095.5);
  hist[1] = (TH2D *) hist[0] -> Clone("hist2");
  hist[2] = (TH2D *) hist[0] -> Clone("hist3");
  hist[3] = (TH2D *) hist[0] -> Clone("hist4");

  for (Int_t j = 0; j < 4; j++) {
    while (ea[j] = a[j] -> GetGraw()) {
      if (ea[j] == NULL) break;
      for (Int_t i = 0; i < 68; i++) {
        hist[j] -> Fill(i, ea[j] -> GetMaxADC(0, i));
        hist[j] -> Fill(i + 68, ea[j] -> GetMaxADC(1, i));
        hist[j] -> Fill(i + 68*2, ea[j] -> GetMaxADC(2, i));
        hist[j] -> Fill(i + 68*3, ea[j] -> GetMaxADC(3, i));
      }
    }
  }

  TCanvas *cvs = new TCanvas("cvs", "", 1200, 1000);
  cvs -> Divide(1, 4);

  for (Int_t j = 0; j < 4; j++) {
    cvs -> cd(j + 1);
    hist[j] -> Draw("colz");
  }
}
