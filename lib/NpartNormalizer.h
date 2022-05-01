#ifndef NpartNormalizer_hh
#define NpartNormalizer_hh
#include "TString.h"
#include <map>

class NpartNormalizer
{
private:
    static std::map<int, float> ratio_ox;
    static std::map<int, float> ratio_oxb;
    static std::map<int, float> ratio_obx;
    static std::map<int, float> ratio_obxb;

    static std::map<int, float> ratio_x;
    static std::map<int, float> ratio_xb;
    static std::map<int, float> ratio_o;
    static std::map<int, float> ratio_ob;
    
public:
    NpartNormalizer() {}
    virtual ~NpartNormalizer() {}
    float weight(int npp, TString compare_case);

ClassDef(NpartNormalizer, 1)
};

#endif
