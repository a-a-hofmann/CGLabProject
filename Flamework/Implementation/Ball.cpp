//
//  Ball.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 27/04/15.
//
//

#include "Ball.h"

Ball::Ball(float x, float y, float r, float vx, float vy) : Entity(x, y)
{
    _r = r;
    _vx = vx;
    _vy = vy;
    _oldX = x;
    _oldY = y;
    _modelName = "sphere";
}