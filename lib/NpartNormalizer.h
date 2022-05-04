#ifndef NpartNormalizer_hh
#define NpartNormalizer_hh
#include "TString.h"

class NpartNormalizer
{
private:
    static const int lowest_npp = 151;
    static const float ratio_ox[40];
    static const float ratio_oxb[40];
    static const float ratio_obx[40];
    static const float ratio_obxb[40];

    static const float ratio_x[40];
    static const float ratio_xb[40];
    static const float ratio_o[40];
    static const float ratio_ob[40];
    
public:
    NpartNormalizer() {}
    virtual ~NpartNormalizer() {}
    float weight(int npp, TString compare_case);

ClassDef(NpartNormalizer, 1)
};

#endif
