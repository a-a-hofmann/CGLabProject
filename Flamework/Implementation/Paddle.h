//
//  Paddle.h
//  Flamework
//
//  Created by Alexander Hofmann on 29/04/15.
//
//

#ifndef __Flamework__Paddle__
#define __Flamework__Paddle__

#include "Cuboid.h"


// The paddle
class Paddle : public Cuboid
{
public:
    float _dvx;  // change of horizontal velocity on movement
    float _vx;   // horizontal velocity
    
    // Construct paddle
    // x, y: position (i.e. center of paddle)
    // width, height: width and height of paddle
    // dvx: change of horizontal velocity on movement
    Paddle();
    Paddle(float x, float y, float width, float height, float dvx);
    ~Paddle();
    bool detectCollision(BallPtr ball);
};

typedef std::shared_ptr<Paddle> PaddlePtr;

#endif /* defined(__Flamework__Paddle__) */
