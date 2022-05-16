#ifndef NpartNormalizer_hh
#define NpartNormalizer_hh
#include "TString.h"

class NpartNormalizer
{
private:
    bool IsUnitWeight;
    int mode;
    static const int lowest_np = 301;
    static const float ratio_s_ox[80];
    static const float ratio_s_oxb[80];
    static const float ratio_s_obx[80];
    static const float ratio_s_obxb[80];

    static const float ratio_s_x[80];
    static const float ratio_s_xb[80];
    static const float ratio_s_o[80];
    static const float ratio_s_ob[80];

    static const float ratio_s_woallx_for_wx[80];
    static const float ratio_s_woallx_for_wxb[80];
    static const float ratio_s_woallo_for_wo[80];
    static const float ratio_s_woallo_for_wob[80];
    
public:
    NpartNormalizer(int _mode) {IsUnitWeight = false; mode = _mode;}
    virtual ~NpartNormalizer() {}
    void unit_weight() {IsUnitWeight = true; }
    float weight(int np, TString compare_case, int mode);

ClassDef(NpartNormalizer, 1)
};

#endif
