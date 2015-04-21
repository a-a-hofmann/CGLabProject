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

class Entity {
protected:
    const char* _modelName = "";
    
public:
    unit _x, _y;
    
    Entity(unit x, unit y) : _x(x), _y(y) {}
    char* getModelName() { return (char*)_modelName; }
};

class Ball : public Entity
{
public:
    unit _r, _vx, _vy;
    //unit _oldX, _oldY;
    
    Ball(unit x, unit y, unit r, unit vx, unit vy);
};

class Cuboid;

class Obstacle : public Entity
{
public:
    Obstacle(unit x, unit y);
    virtual bool detectCollision(Ball& ball) = 0;
    virtual bool detectCollision(Cuboid& cuboid) = 0;
    virtual bool destroyOnCollision();
    virtual bool endRoundOnCollision();
    virtual ~Obstacle();
};

class Cuboid : public Obstacle
{
public:
    unit _width, _height;
    
    Cuboid(unit x, unit y, unit width, unit height);
    bool detectCollision(Ball& ball);
    bool detectCollision(Cuboid& cuboid);
};

class Brick : public Cuboid
{
public:
    Brick(unit x, unit y, unit width, unit height);
    bool destroyOnCollision();
};

class Wall : public Cuboid
{
protected:
    bool _endRoundOnCollision;
    
public:
    Wall(unit x, unit y, unit width, unit height, bool horizontal, bool endRoundOnCollision = false);
    bool endRoundOnCollision();
};

class Paddle : public Cuboid
{
public:
    unit _dvx;
    unit _vx;
    
    Paddle(unit x, unit y, unit width, unit height, unit dvx);
    bool detectCollision(Ball& ball);
};

class Game
{
protected:
    int _velocityDivisor;
    void moveBall2();
    
public:
    std::list<Obstacle*> _obstacles;
    //std::list<std::shared_ptr<Obstacle> > _obstacles;
    Paddle _paddle;
    Ball _ball;
    bool _playing;
    
    Game();
    ~Game();
    void moveBall();
    void movePaddle(bool left);
    bool isWon();
};

#endif /* defined(__Flamework__GameLogic__) */
