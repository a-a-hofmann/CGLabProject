//
//  ParticleSystem.cpp
//  Flamework
//
//  Created by Adrian Gasparini on 13.05.15.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(unit x, unit y, unit z) :
_particles(),
_x(x), _y(y), _z(z),
step(0)
{
    unit maxV = 0.03;
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        _particles.insert(_particles.begin(), new Particle(_x, _y, _z,
                    rand()*2*maxV/RAND_MAX - maxV, rand()*2*maxV/RAND_MAX - maxV, rand()*2*maxV/RAND_MAX));
    }
}

ParticleSystem::~ParticleSystem()
{
    for(Particle* particle : _particles)
        delete particle;
    _particles.clear();
}

bool ParticleSystem::moveParticles()
{
    if(step >= NUM_STEPS) return false;
        
    step++;
    
    for(Particle* particle : _particles)
    {
        particle->_x += particle->_vx;
        particle->_y += particle->_vy;
        particle->_z += particle->_vz;
    }
    
    return true;
}