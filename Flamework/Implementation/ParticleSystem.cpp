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

ParticleSystem::ParticleSystem(float x, float y, float z) :
_particles(),
_x(x), _y(y), _z(z),
step(0)
{
    float maxV = 0.05;
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        _particles.insert(_particles.begin(), new Particle(_x, _y, _z,
                    rand()*2*maxV/RAND_MAX - maxV, rand()*2*maxV/RAND_MAX - maxV, rand()*4*maxV/RAND_MAX));
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