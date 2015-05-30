//
//  Obstacle.h
//  Flamework
//
//  Created by Alexander Hofmann on 27/04/15.
//
//

#ifndef __Flamework__Obstacle__
#define __Flamework__Obstacle__

#include "Entity.h"
#include "Ball.h"

// An obstacle (i.e. brick, wall or paddle)
class Obstacle : public Entity
{
public:
    
    // Create obstacle
    // x, y: position
    Obstacle(float x, float y);
    
    
    // Detect collision with ball
    virtual bool detectCollision(BallPtr ball) = 0;
    
    // returns true if obstacle should be destroyed on collision
    virtual bool destroyOnCollision();
    
    // returns true if game round should end on collision
    virtual bool endRoundOnCollision();
    
    // destruct obstacle
    virtual ~Obstacle();
};

#endif /* defined(__Flamework__Obstacle__) */
