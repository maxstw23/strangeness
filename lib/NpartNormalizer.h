#ifndef NpartNormalizer_hh
#define NpartNormalizer_hh
#include "TString.h"
#include <map>

class NpartNormalizer
{
private:
    static const const std::map<int, float> ratio_ox;
    static const const std::map<int, float> ratio_oxb;
    static const const std::map<int, float> ratio_obx;
    static const const std::map<int, float> ratio_obxb;
    
public:
    NpartNormalizer() {}
    virtual ~NpartNormalizer() {}
    float weight(int npp, TString case);

ClassDef(NpartNormalizer, 1)
};

#endif
