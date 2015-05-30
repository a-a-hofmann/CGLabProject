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
    float _r;        // redius
    float _vx, _vy;  // velocity
    float _oldX, _oldY;  // previous coordinates
    
    // Construct ball
    // x, y: position (center of circle)
    // r: radius
    // vx, vy: initial velocity
    Ball(float x, float y, float r, float vx, float vy);
    Ball();
    ~Ball();
};

typedef std::shared_ptr<Ball> BallPtr;

#endif /* defined(__Flamework__Ball__) */
