class STMap;

STMap *map;

void fire(Int_t ch)
{
  gSystem -> Load("libSPiRIT");

  Int_t row, layer;
  map -> GetRowNLayer(0, 0, 0, ch, row, layer);
  hist -> Reset();

  hist -> SetBinContent(layer + 1, row + 1, 10);
  hist -> Draw("colz");
}

void Prepare()
{
  gSystem -> Load("libSPiRIT");

  map = new STMap();

  gStyle -> SetOptStat(0);
  TH2D *hist = new TH2D("hist", "", 7, -0.5, 6.5, 9, -0.5, 8.5);
  hist -> Draw("colz");
  c1 -> SetGrid();
}

void testAgetMap()
{
  Prepare();
  cout << "Type 'fire(ch)'" << endl;
}
