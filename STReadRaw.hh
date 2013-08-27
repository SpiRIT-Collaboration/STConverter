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

class STEvent;
class STMap;
class STPedestal;

class STReadRaw : public TObject {
  public:
    STReadRaw() { Initialize(); }
    STReadRaw(Char_t *filename) { SetRawfile(filename); Initialize(); }
    ~STReadRaw() {}

    void Initialize();

    // Setters
    void SetRawfile(Char_t *filename);
    void SetPedestalData(Char_t *filename);

    // Getters
    STEvent *GetEvent();

  private:
    std::ifstream rawfile;
    STEvent *anEvent;
    STMap *mapper;
    STPedestal *pedestal;

    Bool_t usePedestalData;

  ClassDef(STReadRaw, 1);
};

#endif
