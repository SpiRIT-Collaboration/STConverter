void plot03(Char_t *filename) {
  if (filename == "") {
    cout << "Run 'root \'plot03.cc(\"/path/to/GRAWFILE.graw\")\'" << endl;
    return;
  }

  gSystem -> Load("../libGETDecoder.so");

  GETDecoder *decoder = new GETDecoder("GRAWFILE.graw");
  GETPlot *plot = decoder -> GetGETPlot();

  GETFrame *frame = decoder -> GetFrame(0);

  Int_t coboID = frame -> GetCoboID();
  Int_t asadID = frame -> GetAsadID();
  Int_t frameID = frame -> GetFrameID();

  // If the first argument is -1 or all the argumets are ignored,
  // the method automatically read the next frame and draws plot.
  // If the second to fifth arguments are ignored, the method plots all the channels.
  //
  // Pedestal is calculated using 20 time buckets from 10th time bucket.
  // You can change the start position and the number of time buckets used in the calculation
  // by changing the second (starting point) and the third (number of channels) arguments.
  //
  // The fourth argument is number of channels to draw (+) or to exclude (-)
  // The fifth argument is a channel list.
  // This rule is also used in ShowRawFrame() and ShowAverage() methods.
  //
  // example)
  // TCanvas *cvs = plot -> ShowFrame(2); // Plot all the signals of frame ID 2
  //
  // Plot the signals of channels in the list in frame ID 2
  // Int_t list[5] = {11, 22, 45, 56, 67};
  // TCanvas *cvs = plot -> ShowFrame(2, 10, 20, 5, list);
  //
  // Plot the signals of channels except those in the list in frame ID 2
  // Int_t list[5] = {11, 22, 45, 56, 67};
  // TCanvas *cvs = plot -> ShowFrame(2, 10, 20, -5, list);
  TCanvas *cvs = plot -> ShowFrame();

  cvs -> SaveAs(Form("CoBo%d-AsAd%d-RawFrame%d.png", coboID, asadID, frameID));
}
