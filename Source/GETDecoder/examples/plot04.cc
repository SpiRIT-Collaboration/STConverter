void plot03(Char_t *filename) {
  if (filename == "") {
    cout << "Run 'root \'plot04.cc(\"/path/to/GRAWFILE.graw\")\'" << endl;
    return;
  }

  gSystem -> Load("../libGETDecoder.so");

  GETDecoder *decoder = new GETDecoder("GRAWFILE.graw");
  GETPlot *plot = decoder -> GetGETPlot();

  GETFrame *frame = decoder -> GetFrame(0);

  Int_t coboID = frame -> GetCoboID();
  Int_t asadID = frame -> GetAsadID();
  Int_t frameID = frame -> GetFrameID();

  // The first argument is number of channels to draw (+) or to exclude (-)
  // The second argument is a channel list.
  // This rule is also used in ShowRawFrame() and ShowFrame() methods.
  //
  // If the third argument is -1, the method automatically read the next frame and draws plot.
  // The third argument can be ignored.
  Int_t list[5] = {11, 22, 45, 56, 67};
  TCanvas *cvs = plot -> ShowAverage(-5, list);

  cvs -> SaveAs(Form("CoBo%d-AsAd%d-RawFrame%d.png", coboID, asadID, frameID));
}
