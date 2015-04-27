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

class Cuboid;

// An obstacle (i.e. brick, wall or paddle)
class Obstacle : public Entity
{
public:
    
    // Create obstacle
    // x, y: position
    Obstacle(unit x, unit y);
    
    
    // Detect collision with ball
    virtual bool detectCollision(Ball& ball) = 0;
    
    // Detect collision with an other cuboid
    virtual bool detectCollision(Cuboid& cuboid) = 0;
    
    // returns true if obstacle should be destroyed on collision
    virtual bool destroyOnCollision();
    
    // returns true if game round should end on collision
    virtual bool endRoundOnCollision();
    
    // destruct obstacle
    virtual ~Obstacle();
};

// A cuboid, (i.e. brick wall or paddle)
class Cuboid : public Obstacle
{
public:
    unit _width, _height;   // width and height of cuboid
    
    // Construct cuboid
    // x, y: position (i.e. center of cuboid)
    // width, height: width and height of cuboid
    Cuboid(unit x, unit y, unit width, unit height);
    
    // see Obstacle
    bool detectCollision(Ball& ball);
    
    // see Obstacle
    bool detectCollision(Cuboid& cuboid);
};

// A brick
class Brick : public Cuboid
{
public:
    // Construct brick
    // x, y: position (i.e. center of brick)
    // width, height: width and height of brick
    Brick(unit x, unit y, unit width, unit height);
    
    // see Obstacle
    bool destroyOnCollision();
};

// A wall
class Wall : public Cuboid
{
protected:
    bool _endRoundOnCollision;  // true if game round should end on collision
    
public:
    // Construct wall
    // x, y: position (i.e. center of wall)
    // width, height: width and height of wall
    // horizontal: true if it is a horizontal wall, false if it is a vertical wall
    // endRoundOnCollision: true if game round should end on collision
    Wall(unit x, unit y, unit width, unit height, bool horizontal, bool endRoundOnCollision = false);
    bool endRoundOnCollision();
};

// The paddle
class Paddle : public Cuboid
{
public:
    unit _dvx;  // change of horizontal velocity on movement
    unit _vx;   // horizontal velocity
    
    // Construct paddle
    // x, y: position (i.e. center of paddle)
    // width, height: width and height of paddle
    // dvx: change of horizontal velocity on movement
    Paddle(unit x, unit y, unit width, unit height, unit dvx);
    bool detectCollision(Ball& ball);
};
#endif /* defined(__Flamework__Obstacle__) */
