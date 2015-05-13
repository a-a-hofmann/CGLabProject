//
//  ParticleSystem.h
//  Flamework
//
//  Created by Adrian Gasparini on 13.05.15.
//
//

#ifndef Flamework_ParticleSystem_h
#define Flamework_ParticleSystem_h

#include <list>
#include "Particle.h"

// A particle system
class ParticleSystem
{
public:
    const int NUM_PARTICLES = 20;   // number of particles
    const int NUM_STEPS = 10;   // number of movement steps
    unit _x, _y, _z;    // coordinates of particle system (center)
    ParticleList _particles;    // the particles
    int step;   // current movement step
    
    // Create particle system
    // x, y, z: coordinates of center
    ParticleSystem(unit x, unit y, unit z);
    
    ~ParticleSystem();
    
    // Move particles
    // returns false if particle system is to be removed
    bool moveParticles();
};

#endif
