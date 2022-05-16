#ifndef my_particle_hh
#define my_particle_hh

#include "TObject.h"
#include "TVector3.h"

class my_particle : public TObject
{
public:
    my_particle();
    my_particle(float _px, float _py, float _pz, int _PID);
    TVector3 GetMomentum();
    int GetPID();
private:
    float px, py, pz;
    int PID;

ClassDef (my_particle, 1)
};

#endif