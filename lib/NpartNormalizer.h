#ifndef NpartNormalizer_hh
#define NpartNormalizer_hh
#include "TString.h"

class NpartNormalizer
{
private:
    bool IsUnitWeight;
    int mode;
     
    static const int lowest_np_s = 301;
    static const int lowest_np_d = 321;
    static const int lowest_np_u = 251;
    static const int lowest_nss_s = 81;
    static const int lowest_nss_d = 121;

    static const int num_np_s = 80;
    static const int num_np_d = 60;
    static const int num_np_u = 100;
    static const int num_nss_s = 70;
    static const int num_nss_d = 60;

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
    static const float ssratio_s_woallx_for_wx[70];
    static const float ssratio_s_woallx_for_wxb[70];

    static const float ratio_d_ox[60];
    static const float ratio_d_oxb[60];
    static const float ratio_d_obx[60];
    static const float ratio_d_obxb[60];
    static const float ratio_d_x[60];
    static const float ratio_d_xb[60];
    static const float ratio_d_o[60];
    static const float ratio_d_ob[60];
    static const float ratio_d_woallx_for_wx[60];
    static const float ratio_d_woallx_for_wxb[60];
    static const float ratio_d_woallo_for_wo[60];
    static const float ratio_d_woallo_for_wob[60];
    static const float ssratio_d_woallx_for_wx[60];
    static const float ssratio_d_woallx_for_wxb[60];

    static const float ratio_u_ox[100];
    static const float ratio_u_oxb[100];
    static const float ratio_u_obx[100];
    static const float ratio_u_obxb[100];
    static const float ratio_u_x[100];
    static const float ratio_u_xb[100];
    static const float ratio_u_o[100];
    static const float ratio_u_ob[100];
    static const float ratio_u_woallx_for_wx[100];
    static const float ratio_u_woallx_for_wxb[100];
    static const float ratio_u_woallo_for_wo[100];
    static const float ratio_u_woallo_for_wob[100];
    
public:
    NpartNormalizer(int _mode) {IsUnitWeight = false; mode = _mode;}
    virtual ~NpartNormalizer() {}
    void unit_weight() {IsUnitWeight = true; }
    float weight(int np, TString compare_case);

ClassDef(NpartNormalizer, 1)
};

#endif
