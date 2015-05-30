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
    float _z;    // z coordinate
    float _vx, _vy, _vz; // veloicty
    
    // Construct particle
    // x, y, z: coordinates of particle
    // xv, vy, xz: velocity of particle
    Particle(float x, float y, float z, float vx, float vy, float vz) :
    Entity(x, y),
    _z(z),
    _vx(vx), _vy(vy), _vz(vz)
    { _modelName = "brick"; }
};

typedef std::list<Particle*> ParticleList;


#endif
