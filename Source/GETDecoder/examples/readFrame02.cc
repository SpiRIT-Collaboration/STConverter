void readFrame02() {
  gSystem -> Load("../libGETDecoder.so");

  GETDecoder *decoder = new GETDecoder("GRAWFILE.graw");
  
  GETFrame *frame = NULL;
  while ((frame = decoder -> GetFrame())) { 
    UInt_t eventID = frame -> GetEventID();
    Int_t coboID = frame -> GetCoboID();
    Int_t asadID = frame -> GetAsadID();

    // The line below calls pedestal subtracting method
    // It selects 20 time buckets from 10th time bucket
    // and calculates the average and RMS value.
    // Finally, it uses them as a pedestal for the channel.
    frame -> CalcPedestal(10, 20);

    Double_t *adc = NULL;
    for (Int_t iAget = 0; iAget < 4; iAget++) {
      for (Int_t iCh = 0; iCh < 68; iCh++) {
        adc = frame -> GetRawADC(iAget, iCh);

        // Implement what you want to do with a frame here.
      }
    }
  }
}
