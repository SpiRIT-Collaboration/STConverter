{
  gSystem -> Load("libSTConverter.so");
  STCore *c = new STCore("../Intensity_1M_small.graw");
  c -> SetAGETMap("AGET.map");
  c -> SetUAMap("UnitAsAd.map");
  c -> SetFPNPedestal();
  c -> SetData(0);
  STPlot *p = new STPlot();
  STRawEvent *event;
  event = c -> GetRawEvent(); p -> SetEvent(event); p -> DrawPadplane();

  cout << "To draw next event, type \"event = c -> GetRawEvent(); p -> SetEvent(event); p -> DrawPadplane();\"" << endl;
}
