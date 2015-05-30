//
//  Ball.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 27/04/15.
//
//

#include "Ball.h"

Ball::Ball() : Entity(-1.0, 0.0)
{
    _r = 0.5;
    _vx = 0.1;
    _vy = -0.2;
    _oldX = 0.0;
    _oldY = 0.0;
    _modelName = "sphere";
}

Ball::Ball(float x, float y, float r, float vx, float vy) : Entity(x, y)
{
    _r = r;
    _vx = vx;
    _vy = vy;
    _oldX = x;
    _oldY = y;
    _modelName = "sphere";
}

Ball::~Ball()
{
    
}