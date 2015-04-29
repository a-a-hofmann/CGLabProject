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


#endif /* defined(__Flamework__Cuboid__) */
