void readFrame01() {
  gSystem -> Load("../libGETDecoder.so");

  GETDecoder *decoder = new GETDecoder("GRAWFILE.graw");
  
  GETFrame *frame = NULL;
  while ((frame = decoder -> GetFrame())) { 
    UInt_t eventID = frame -> GetEventID();
    Int_t coboID = frame -> GetCoboID();
    Int_t asadID = frame -> GetAsadID();

    Int_t *rawadc * = NULL;
    for (Int_t iAget = 0; iAget < 4; iAget++) {
      for (Int_t iCh = 0; iCh < 68; iCh++) {
        rawadc = frame -> GetRawADC(iAget, iCh);

        // Implement what you want to do with a frame here.
      }
    }
  }
}
