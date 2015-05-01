//
//  GameLogic.h
//  Flamework
//
//  Created by Adrian Gasparini on 15.04.15.
//
//

#ifndef __Flamework__GameLogic__
#define __Flamework__GameLogic__

#include <iostream>
#include <list>
#include "Entity.h"
#include "Ball.h"
#include "Obstacle.h"
#include "Cuboid.h"
#include "Wall.h"
#include "Brick.h"
#include "Paddle.h"

typedef std::list<Cuboid*> ObstacleList;
// The game
class Game
{
protected:
    int _velocityDivisor;   // if the ball can be fast compared to the dimensions this should be increased in order to move the ball in several smaller steps
    void moveBall2();   // moves the ball with velocity devided by _velocityDivisor
    
public:
    ObstacleList _obstacles;    // list of obstacles (i.e. bricks and walls)
    Paddle _paddle; // the paddle
    Ball _ball;     // the ball
    bool _playing;  // true if the game round has not ended yet
    
    // Construct game
    Game();
    
    // Destruct game
    ~Game();
    
    // Move ball
    void moveBall();
    
    // Move paddle: Autopilot
    // left: true if move to the left, false if to the right
    void movePaddle(bool left);
    
    // Move paddle: touchscreen
    void movePaddle(float dx);
    
    // returns true if the game is won (i.e. all bricks have been destroyed)
    bool isWon();
};

#endif /* defined(__Flamework__GameLogic__) */
