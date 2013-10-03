class GETDecoder;
class GETFrame;

TCanvas *cvs;
TGraph *gAget[4];
GETDecoder *decoder = NULL;

Int_t minTb = 0;
Int_t maxTb = 276;

void showGraw(void);
void showGraw(Int_t frameNo = -1);
void showChannel(Int_t agetIdx, Int_t chIdx, Int_t frameNo = -1);
void showSum(Bool_t isConnected = 0, Int_t frameNo = -1);

void showGraw() {
  gSystem -> Load("libGETDecoder");

  if (decoder != NULL)
    delete decoder;
  
  // If you want to run the macro without inputting manually, fill the blank
  // below.
  TString filename = "";
  while (filename.EqualTo("")) {
    Char_t input[1000];
    cout << "filename: ";
    cin >> input;
    filename = input;
  }

  decoder = new GETDecoder(filename.Data());

  Int_t allOrSingle = -1;
  while (allOrSingle <= -1 || allOrSingle >= 4) {
    cout << "Single Channel(0), All Channels(1), Summed Signal(2)): ";
    cin >> allOrSingle;
  }

  if (allOrSingle == 0) {
    Int_t agetIdx = -1;
    Int_t chIdx = -1;
    while (agetIdx <= -1 || agetIdx >= 4 || chIdx <= -1 || chIdx >= 68) {
      cout << "Type agetIdx and chIdx with space(ex. 3 50): ";
      cin >> agetIdx >> chIdx;
    }

    showChannel(agetIdx, chIdx);
  } else if (allOrSingle == 1) {
    showGraw(0);
  } else if (allOrSingle == 2) {
    Int_t isConnected = -1;
    while (isConnected <= -1 || isConnected > 2) {
      cout << "Not connected(0) or Connected(1): ";
      cin >> isConnected;
    }

    showSum((Bool_t) isConnected, 0);
  }

}

void showRaw(Int_t frameNo) {
  GETPlot *plot = decoder -> GetGETPlot();

  if (frameNo == -1)
    cvs = plot -> ShowRawFrame();
  else
    cvs = plot -> ShowRawFrame(frameNo);
}

void showChannel(Int_t agetIdx, Int_t chIdx, Int_t frameNo) {
  gSystem -> Load("libGETDecoder");

  GETPlot *plot = decoder -> GetGETPlot();
  Int_t list[1] = {chIdx};

  cvs = plot -> ShowRawFarme(frameNo, 1, list);
}

void showSum(Bool_t isConnected, Int_t frameNo) {
  gSystem -> Load("libGETDecoder");

  GETPlot *plot = decoder -> GetGETPlot();

  Int_t list[68] = {0};
  Int_t ch = 0;
  for (Int_t i = 0; i < 68; i++) {
    if (i == 11 || i == 22 || i == 45 || i == 56 || i == 67)
      continue;

    list[ch] = i;
    ch++;
  }

  cvs = plot -> ShowAverage((isConnected == 0 ? -63 : 63), list, frameNo);
}

