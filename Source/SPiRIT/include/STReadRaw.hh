// =================================================
//  STReadRaw Class
// 
//  Description:
//    Read the raw file from CoBo and process it
//    into STEvent class.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 16
// =================================================

#ifndef STPROCESSRAW_H
#define STPROCESSRAW_H

#include <fstream>

#include "TROOT.h"
#include "TObject.h"
#include "TString.h"

class STGraw;
class STEvent;
class STMap;
class STPedestal;

class STReadRaw : public TObject {
  public:
    STReadRaw();
    STReadRaw(Char_t *filename);
    ~STReadRaw() {}

    void Initialize();
    Bool_t NextFile();

    // Setters
    void SetRawfile(Char_t *filename);
    void SetPedestalData(Char_t *filename);

    // Getters
    STEvent *GetEvent();
    STGraw *GetGraw();

  private:
    TString datafile;

    std::ifstream rawfile;

    STGraw *aGraw;

    STEvent *anEvent;
    STMap *mapperPtr;
    STPedestal *pedestalPtr;

    Bool_t firstFlag;
    Bool_t usePedestalData;
    UInt_t prevEventIdx;

  ClassDef(STReadRaw, 1);
};

#endif
