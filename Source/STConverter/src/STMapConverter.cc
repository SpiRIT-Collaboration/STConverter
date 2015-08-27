// =================================================
//  STMapConverter Class
//
//  Description:
//    Convert mapping data
//    between STConverter and get-config-wizard
//
//  Genie Jhang ( geniejhang@majimak.com )
//  2014. 10. 31
// =================================================

#include "STMapConverter.hh"

ClassImp(STMapConverter)

STMapConverter::STMapConverter()
{
  fMap = new STMap();

  fIsSTMap = kFALSE;
  fIsGETMap = kFALSE;
}

Bool_t
STMapConverter::LoadSTMap(TString uaMap, TString agetMap)
{
  fIsSTMap = kTRUE;
  fIsSTMap &= fMap -> SetUAMap(uaMap);
  fIsSTMap &= fMap -> SetAGETMap(agetMap);
  
  return fIsSTMap;
}

Bool_t
STMapConverter::LoadGETMap(TString filename)
{
  ifstream inFile(filename.Data());

  if (!inFile.is_open()) {
    cout << filename << " is not loaded! Check the existance of the file!" << endl;

    fIsGETMap = kFALSE;

    return fIsGETMap;
  }

  Char_t dummy[200];
  inFile.getline(dummy, 200);

  Int_t cobo = -1, asad = -1, aget = -1, ch = -1;
  Double_t padX = -9999, padZ = -9999;

  while (!(inFile.eof())) {
    inFile >> cobo >> asad >> aget >> ch >> padX >> padZ;

    Int_t padRow = (padX + 432 - 4)/8.;
    Int_t padLayer = (padZ - 6)/12.;

    Int_t uaLayer = padLayer/(7*4);
    Int_t uaRow = padRow/9;

    Int_t uaIdx = uaLayer*100 + uaRow;

    fMap -> SetUAMap(uaIdx, cobo, asad);

    if (uaRow < 6)
      fMap -> SetAGETMap(ch, padRow%9, padLayer%7);
    else
      fMap -> SetAGETMap(ch, 8 - padRow%9, 6 - padLayer%7);
  }

  inFile.close();

  fIsGETMap = kTRUE;

  return fIsGETMap;
}

void
STMapConverter::SaveSTMap(TString uaMap, TString agetMap)
{
  if (!fIsSTMap && !fIsGETMap) {
    cout << "== [STMapConvrter] Mapping file isn't Loaded!" << endl;
    
    return;
  }

  ofstream outFile(uaMap.Data());

  outFile << "# UAIdx CoBoIdx AsAdIdx" << endl;

  for (Int_t iCobo = 0; iCobo < 12; iCobo++) {
    for (Int_t iAsad = 0; iAsad < 4; iAsad++) {
      Int_t uaIdx = fMap -> GetUAIdx(iCobo, iAsad);

      if (uaIdx == -1)
        continue;

      outFile << setw(7) << Form("%03d", uaIdx) << setw(8) << iCobo << setw(8) << iAsad << endl;
    }
  }

  outFile.close();

  outFile.open(agetMap.Data());

  outFile << "# Ch PadLayer PadRow" << endl;
  for (Int_t iCh = 0; iCh < 68; iCh++) {
    Int_t padRow = -2, padLayer = -2;
    fMap -> GetAGETMap(iCh, padRow, padLayer);

    padLayer %= 7;

    outFile << setw(4) << iCh << setw(9) << padLayer << setw(7) << padRow << endl;
  } 

  outFile.close();
}

void
STMapConverter::SaveGETMap(TString filename)
{
  if (!fIsSTMap && !fIsGETMap) {
    cout << "== [STMapConvrter] Mapping file isn't Loaded!" << endl;
    
    return;
  }

  ofstream outFile(filename.Data());

  outFile << "# CoBo  AsAd  AGET  Channel    Row    Column" << endl;

  for (Int_t iCobo = 0; iCobo < 12; iCobo++) {
    for (Int_t iAsad = 0; iAsad < 4; iAsad++) {
      Int_t uaIdx = fMap -> GetUAIdx(iCobo, iAsad);

      if (uaIdx == -1)
        continue;

      for (Int_t iAget = 0; iAget < 4; iAget++) {
        for (Int_t iCh = 0; iCh < 68; iCh++) {
          Int_t padRow = -2;
          Int_t padLayer = -2;

          fMap -> GetRowNLayer(iCobo, iAsad, iAget, iCh, padRow, padLayer);

          if (padRow == -2 || padLayer == -2)
            continue;

          Double_t padX = (padRow*8. + 4.) - 432;
          Double_t padZ = padLayer*12. + 6;

          outFile << setw(6) << iCobo << setw(6) << iAsad;
          outFile << setw(6) << iAget << setw(9) << iCh;
          outFile << setw(7) << padX << setw(10) << padZ << endl;
        }
      }
    }
  }

  outFile.close();
}
