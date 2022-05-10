#ifndef NpartNormalizer_hh
#define NpartNormalizer_hh
#include "TString.h"

class NpartNormalizer
{
private:
    bool IsUnitWeight;
    static const int lowest_np = 151;
    static const float ratio_ox[40];
    static const float ratio_oxb[40];
    static const float ratio_obx[40];
    static const float ratio_obxb[40];

    static const float ratio_x[40];
    static const float ratio_xb[40];
    static const float ratio_o[40];
    static const float ratio_ob[40];

    static const float ratio_woallx_for_wx[40];
    static const float ratio_woallx_for_wxb[40];
    static const float ratio_woallo_for_wo[40];
    static const float ratio_woallo_for_wob[40];
    
public:
    NpartNormalizer() {IsUnitWeight = false;}
    virtual ~NpartNormalizer() {}
    void unit_weight() {IsUnitWeight = true; }
    float weight(int np, TString compare_case);

ClassDef(NpartNormalizer, 1)
};

#endif
