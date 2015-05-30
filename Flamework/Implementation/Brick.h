//
//  Brick.h
//  Flamework
//
//  Created by Alexander Hofmann on 29/04/15.
//
//

#ifndef __Flamework__Brick__
#define __Flamework__Brick__

#include "Cuboid.h"

// A brick
class Brick : public Cuboid
{
public:
    // Construct brick
    // x, y: position (i.e. center of brick)
    // width, height: width and height of brick
    Brick(float x, float y, float width, float height);
    
    // see Obstacle
    bool destroyOnCollision();
};


#endif /* defined(__Flamework__Brick__) */
