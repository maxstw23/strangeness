#include "my_particle.h"
#include "TVector3.h"

my_particle::my_particle() {px = py = pz = PID = -999;}

my_particle::my_particle(float _px, float _py, float _pz, int _PID) 
    : px(_px), py(_py), pz(_pz), PID(_PID) {}

TVector3 my_particle::GetMomentum() {return TVector3(px, py, pz);}

int my_particle::GetPID() {return PID;}