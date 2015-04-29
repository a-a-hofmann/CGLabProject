//
//  Obstacle.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 27/04/15.
//
//

#include "Obstacle.h"

Obstacle::Obstacle(float x, float y) : Entity(x, y)
{
}

Obstacle::~Obstacle()
{
}

bool Obstacle::destroyOnCollision()
{
    return false;
}

bool Obstacle::endRoundOnCollision()
{
    
    return false;
}

Cuboid::Cuboid(float x, float y, float width, float height) : Obstacle(x, y)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

bool detectCornerCollision(Ball& ball, float x, float y)
{
    float dx = ball._x - x;
    float dy = ball._y - y;
    
    if(dx*dx + dy*dy < ball._r*ball._r)
    {
        float q = -2.0 * (ball._vx * dx + ball._vy * dy) / (ball._r*ball._r);
        ball._vx += q * dx;
        ball._vy += q * dy;
        ball._x += ball._vx;
        ball._y += ball._vy;
        
        return true;
    }
    
    return false;
}

bool Cuboid::detectCollision(Ball& ball)
{
    float _x1 = _x - _width/2;
    float _y1 = _y + _height/2;
    float _x2 = _x + _width/2;
    float _y2 = _y - _height/2;
    
    if(ball._x >= _x1 && ball._x <= _x2) {
        float ballMaxY = _y2 - ball._r;
        float ballMinY = _y1 + ball._r;
        
        if(ball._y < _y2 && ball._y > ballMaxY)
        {
            ball._y = ballMaxY;
            ball._vy = -ball._vy;
            return true;
        }
        else if(ball._y > _y1 && ball._y < ballMinY)
        {
            ball._y = ballMinY;
            ball._vy = -ball._vy;
            return true;
        }
        
        return false;
    }
    else if(ball._y >= _y2 && ball._y <= _y1) {
        float ballMaxX = _x1 - ball._r;
        float ballMinX = _x2 + ball._r;
        
        if(ball._x < _x1 && ball._x > ballMaxX)
        {
            ball._x = ballMaxX;
            ball._vx = -ball._vx;
            return true;
        }
        else if(ball._x > _x2 && ball._x < ballMinX)
        {
            ball._x = ballMinX;
            ball._vx = -ball._vx;
            return true;
        }
        
        return false;
    }
    else if(!detectCornerCollision(ball, _x1, _y1))
        if(!detectCornerCollision(ball, _x2, _y1))
            if(!detectCornerCollision(ball, _x1, _y2))
                return detectCornerCollision(ball, _x2, _y2);
    
    return true;
}

bool Cuboid::detectCollision(Cuboid& cuboid)
{
    return
    _x - _width/2 < cuboid._x + cuboid._width/2 &&
    _x + _width/2 > cuboid._x - cuboid._width/2 &&
    _y - _height/2 < cuboid._y + cuboid._height/2 &&
    _y + _height/2 > cuboid._y - cuboid._height/2;
}

Brick::Brick(float x, float y, float width, float height) : Cuboid(x, y, width, height)
{
    _modelName = "brick";
}

bool Brick::destroyOnCollision()
{
    return true;
}

Wall::Wall(float x, float y, float width, float height, bool horizontal, bool endRoundOnCollision) :
Cuboid(x, y, width, height),
_endRoundOnCollision(endRoundOnCollision)
{
    if(horizontal)
        _modelName = "horizontalwall";
    else
        _modelName = "verticalwall";
}

bool Wall::endRoundOnCollision()
{
    return _endRoundOnCollision;
}

Paddle::Paddle(float x, float y, float width, float height, float dvx) : Cuboid(x, y, width, height), _dvx(dvx)
{
    _modelName = "paddle";
}

bool Paddle::detectCollision(Ball& ball)
{
    if(Cuboid::detectCollision(ball))
    {
        ball._vx += _vx/2;
        _vx = 0;
        return true;
    }
    _vx = 0;
    
    return false;
}
