#ifndef NpartNormalizer_hh
#define NpartNormalizer_hh
#include "TString.h"

class NpartNormalizer
{
private:
    static const int lowest_npp = 101;
    static const float ratio_ox[100];
    static const float ratio_oxb[100];
    static const float ratio_obx[100];
    static const float ratio_obxb[100];

    static const float ratio_x[100];
    static const float ratio_xb[100];
    static const float ratio_o[100];
    static const float ratio_ob[100];
    
public:
    NpartNormalizer() {}
    virtual ~NpartNormalizer() {}
    float weight(int npp, TString compare_case);

ClassDef(NpartNormalizer, 1)
};

#endif
