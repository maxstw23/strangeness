#ifndef CenMaker_hh
#define CenMaker_hh
#include "TString.h"

class CenMaker
{
private:
    static const int cent9_19GeV_sm[9];
    static const int cent9_14GeV_sm[9];
    static const int cent9_14GeV_default[9];
    static const int cent9_14GeV_urqmd[9];
    static const int cent16_19GeV_sm[16]; 
    static const int cent16_14GeV_sm[16];
    static const int cent16_14GeV_default[16];
    static const int cent16_14GeV_urqmd[16];
public:
    CenMaker() {}
    virtual ~CenMaker() {}
    int cent9(int refmult, TString energy, int AMPTmode); //AMPTmode: 2=urqmd, 1=string_melting, 0=default
    int cent16(int refmult, TString energy, int AMPTmode);

ClassDef(CenMaker, 1)
};

#endif
