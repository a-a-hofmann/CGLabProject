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
//#include <memory>

typedef float unit;

// An object of the game
class Entity {
protected:
    const char* _modelName = "";    // name of model for drawing
    
public:
    unit _x, _y;    // position
    
    // Construct entity
    // x, y: position
    Entity(unit x, unit y) : _x(x), _y(y) {}
    
    // Get model name for drawing of model (must be loaded beforehand using loadModel)
    char* getModelName() { return (char*)_modelName; }
};

// The ball
class Ball : public Entity
{
public:
    unit _r;        // redius
    unit _vx, _vy;  // velocity
    
    // Construct ball
    // x, y: position (center of circle)
    // r: radius
    // vx, vy: initial velocity
    Ball(unit x, unit y, unit r, unit vx, unit vy);
};

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

// The game
class Game
{
protected:
    int _velocityDivisor;   // if the ball can be fast compared to the dimensions this should be increased in order to move the ball in several smaller steps
    void moveBall2();   // moves the ball with velocity devided by _velocityDivisor
    
public:
    std::list<Obstacle*> _obstacles;    // list of obstacles (i.e. bricks and walls)
    Paddle _paddle; // the paddle
    Ball _ball;     // the ball
    bool _playing;  // true if the game round has not ended yet
    
    // Construct game
    Game();
    
    // Destruct game
    ~Game();
    
    // Move ball
    void moveBall();
    
    // Move paddle
    // left: true if move to the left, false if to the right
    void movePaddle(bool left);
    
    // ADDED
    void movePaddle(unit dx);

    
    // returns true if the game is won (i.e. all bricks have been destroyed)
    bool isWon();
};

#endif /* defined(__Flamework__GameLogic__) */
