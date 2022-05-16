#ifndef my_event_hh
#define my_event_hh
#include "TObject.h"
#include "./my_particle.h"

class my_event 
{
private:
    std::vector<my_particle> particles;
public:
    my_event() {}
    my_event(std::vector<my_particle> _particles);
    virtual ~my_event() {}
    std::vector<my_particle> GetParticles();
    void push_back(my_particle _par);

ClassDef(my_event,1)
};

#endif
