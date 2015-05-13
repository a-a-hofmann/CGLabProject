//
//  Particle.h
//  Flamework
//
//  Created by Mac on 13.05.15.
//
//

#ifndef Flamework_Particle_h
#define Flamework_Particle_h

#include "Entity.h"

// A particle of a particle system
class Particle : public Entity
{
public:
    unit _z;    // z coordinate
    unit _vx, _vy, _vz; // veloicty
    
    // Construct particle
    // x, y, z: coordinates of particle
    // xv, vy, xz: velocity of particle
    Particle(unit x, unit y, unit z, unit vx, unit vy, unit vz) :
    Entity(x, y),
    _z(z),
    _vx(vx), _vy(vy), _vz(vz)
    { _modelName = "brick"; }
};

typedef std::list<Particle*> ParticleList;


#endif
