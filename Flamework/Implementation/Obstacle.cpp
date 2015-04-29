//
//  Obstacle.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 27/04/15.
//
//

#include "Obstacle.h"

Obstacle::Obstacle(float x, float y) : Entity(x, y)
{
}

Obstacle::~Obstacle()
{
}

bool Obstacle::destroyOnCollision()
{
    return false;
}

bool Obstacle::endRoundOnCollision()
{
    
    return false;
}