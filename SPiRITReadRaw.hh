// =================================================
//  SPiRITReadRaw Class
// 
//  Description:
//    Read the raw file from CoBo and process it
//    into SPiRITEvent class.
// 
//  Genie Jhang ( geniejhang@majimak.com )
//  2013. 08. 16
// =================================================

#ifndef SPIRITPROCESSRAW_H
#define SPIRITPROCESSRAW_H

#include "TROOT.h"
#include <fstream>

class SPiRITEvent;
class SPiRITMap;
class SPiRITPedestal;

class SPiRITReadRaw {
  public:
    SPiRITReadRaw() { Initialize(); }
    SPiRITReadRaw(Char_t *filename) { SetRawfile(filename); Initialize(); }
    ~SPiRITReadRaw() {}

    void Initialize();

    // Setters
    void SetRawfile(Char_t *filename);
    void SetPedestalData(Char_t *filename);

    // Getters
    SPiRITEvent *GetEvent();

  private:
    std::ifstream rawfile;
    SPiRITEvent *anEvent;
    SPiRITMap *mapper;
    SPiRITPedestal *pedestal;

  ClassDef(SPiRITReadRaw, 1);
};

#endif
