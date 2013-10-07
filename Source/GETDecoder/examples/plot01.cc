void plot01() {
  gSystem -> Load("libGETDecoder");

  GETDecoder *decoder = new GETDecoder("GRAWFILE.graw");
  GETPlot *plot = decoder -> GetGETPlot();

  GETFrame *frame = decoder -> GetFrame(0);

  Int_t coboID = frame -> GetCoboID();
  Int_t asadID = frame -> GetAsadID();

  // The line below draws the summary spectra of the set file.
  // In drawing, the pedestal is calculated with 20 time buckets
  // starting from 10th time bucket.
  TCanvas *cvs = plot -> ShowSummarySpectra(10, 20);

  cvs -> SaveAs(Form("SummarySpectra-CoBo%d-AsAd%d.png", coboID, asadId));
}
