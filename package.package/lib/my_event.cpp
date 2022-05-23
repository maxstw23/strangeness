#include "my_event.h"
#include "TObject.h"
#include "./my_particle.h"

my_event::my_event(std::vector<my_particle> _particles)
{
    particles = _particles;
}

std::vector<my_particle> my_event::GetParticles() {return particles; }

void my_event::push_back(my_particle _par) { particles.push_back(_par); }