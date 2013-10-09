void plot02(Char_t *filename) {
  if (filename == "") {
    cout << "Run 'root \'plot02.cc(\"/path/to/GRAWFILE.graw\")\'" << endl;
    return;
  }
  gSystem -> Load("../libGETDecoder.so");

  GETDecoder *decoder = new GETDecoder("GRAWFILE.graw");
  GETPlot *plot = decoder -> GetGETPlot();

  GETFrame *frame = decoder -> GetFrame(0);

  Int_t coboID = frame -> GetCoboID();
  Int_t asadID = frame -> GetAsadID();
  Int_t frameID = frame -> GetFrameID();

  // If the first argument is -1 or all the arguments are ignored
  // the method automatically read the next frame and draws plot.
  //
  // If the second and third arguments are ignored, the method plots all the channels.
  // The second argument is number of channels to draw (+) or to exclude (-)
  // The third argument is a channel list.
  // This rule is also used in ShowFrame() and ShowAverage() methods.
  //
  // example)
  // TCanvas *cvs = plot -> ShowRawFrame(2); // Plot all the signals of frame ID 2
  //
  // Plot the signals of channels in the list in frame ID 2
  // Int_t list[5] = {11, 22, 45, 56, 67};
  // TCanvas *cvs = plot -> ShowRawFrame(2, 5, list);
  //
  // Plot the signals of channels except those in the list in frame ID 2
  // Int_t list[5] = {11, 22, 45, 56, 67};
  // TCanvas *cvs = plot -> ShowRawFrame(2, -5, list);
  TCanvas *cvs = plot -> ShowRawFrame();

  cvs -> SaveAs(Form("CoBo%d-AsAd%d-RawFrame%d.png", coboID, asadID, frameID));
}
