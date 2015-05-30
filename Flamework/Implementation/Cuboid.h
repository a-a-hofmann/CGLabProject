//
//  Cuboid.h
//  Flamework
//
//  Created by Alexander Hofmann on 29/04/15.
//
//

#ifndef __Flamework__Cuboid__
#define __Flamework__Cuboid__

#include "Obstacle.h"

// A cuboid, (i.e. brick wall or paddle)
class Cuboid : public Obstacle
{
public:
    float _width, _height;   // width and height of cuboid
    
    // Construct cuboid
    // x, y: position (i.e. center of cuboid)
    // width, height: width and height of cuboid
    Cuboid(float x, float y, float width, float height);
    
    // see Obstacle
    bool detectCollision(BallPtr ball);
    
    // see Obstacle
    bool detectCollision(Cuboid& cuboid);
};

typedef std::shared_ptr<Cuboid> CuboidPtr;

#endif /* defined(__Flamework__Cuboid__) */
