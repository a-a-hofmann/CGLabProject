//
//  Ball.h
//  Flamework
//
//  Created by Alexander Hofmann on 27/04/15.
//
//

#ifndef __Flamework__Ball__
#define __Flamework__Ball__

#include "Entity.h"

// The ball
class Ball : public Entity
{
public:
    unit _r;        // redius
    unit _vx, _vy;  // velocity
    
    // Construct ball
    // x, y: position (center of circle)
    // r: radius
    // vx, vy: initial velocity
    Ball(unit x, unit y, unit r, unit vx, unit vy);
};

#endif /* defined(__Flamework__Ball__) */
