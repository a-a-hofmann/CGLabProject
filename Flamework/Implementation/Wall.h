//
//  Wall.h
//  Flamework
//
//  Created by Alexander Hofmann on 29/04/15.
//
//

#ifndef __Flamework__Wall__
#define __Flamework__Wall__

#include "Cuboid.h"

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

#endif /* defined(__Flamework__Wall__) */
