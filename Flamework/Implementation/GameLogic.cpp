//
//  GameLogic.cpp
//  Flamework
//
//  Created by Adrian Gasparini on 15.04.15.
//
//

#include "GameLogic.h"

Ball::Ball(unit x, unit y, unit r, unit vx, unit vy) :
Entity(x, y)
{
    _r = r;
    _vx = vx;
    _vy = vy;
    _modelName = "sphere";
}

Obstacle::Obstacle(unit x, unit y) :
Entity(x, y)
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

Cuboid::Cuboid(unit x, unit y, unit width, unit height) :
Obstacle(x, y)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

bool detectCornerCollision(Ball& ball, unit x, unit y)
{
    unit dx = ball._x - x;
    unit dy = ball._y - y;
    if(dx*dx + dy*dy < ball._r*ball._r)
    {
        unit q = -2.0 * (ball._vx * dx + ball._vy * dy) / (ball._r*ball._r);
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
    unit _x1 = _x - _width/2;
    unit _y1 = _y + _height/2;
    unit _x2 = _x + _width/2;
    unit _y2 = _y - _height/2;
    if(ball._x >= _x1 && ball._x <= _x2) {
        unit ballMaxY = _y2 - ball._r;
        unit ballMinY = _y1 + ball._r;
        
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
        unit ballMaxX = _x1 - ball._r;
        unit ballMinX = _x2 + ball._r;
        
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

Brick::Brick(unit x, unit y, unit width, unit height) :
Cuboid(x, y, width, height)
{
    _modelName = "brick";
}

bool Brick::destroyOnCollision()
{
    return true;
}

Wall::Wall(unit x, unit y, unit width, unit height, bool horizontal, bool endRoundOnCollision) :
Cuboid(x, y, width, height),
_endRoundOnCollision(endRoundOnCollision)
{
    _modelName = "newFieldNoGround";
    
}

bool Wall::endRoundOnCollision()
{
    return _endRoundOnCollision;
}

Paddle::Paddle(unit x, unit y, unit width, unit height, unit dvx) :
Cuboid(x, y, width, height),
_dvx(dvx)
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



Game::Game() :
_obstacles(),
_ball(0.0, 0.0, 1.0, -0.2, -0.2),
_paddle(0.0, -4.5, 10.0, 1.0, 0.1),
_velocityDivisor(1),
_playing(true)
{
    Wall* lowerWall = new Wall(0.0, -19.0, 27.5, 2.0, true);
    Wall* upperWall = new Wall(0.0, 19.0, 27.5, 2.0, false);
    Wall* leftWall = new Wall(-13.75, 0.0, 1.25, 40.0, false);
    Wall* rightWall = new Wall(13.75, 0.0, 1.25, 40.0, false);
    
    _obstacles.insert(_obstacles.begin(), lowerWall);
    _obstacles.insert(_obstacles.begin(), upperWall);
    _obstacles.insert(_obstacles.begin(), leftWall);
    _obstacles.insert(_obstacles.begin(), rightWall);
    
    

    // Add bricks at the end so the first 4 obstacles are always the wall
    for(unit x = -4.0; x <= 4.0; x += 2.0)
        for(unit y = 4.0; y >= 2.0; y -= 1.0)
            _obstacles.insert(_obstacles.end(), new Brick(x, y, 2.0, 1.0));
}

Game::~Game()
{
    for(std::list<Obstacle*>::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it) {
        delete *it;
    }
}


void Game::moveBall()
{
    for(int i = 0; i < _velocityDivisor; i++)
        moveBall2();
}


void Game::moveBall2()
{
    _ball._x += _ball._vx/_velocityDivisor;
    _ball._y += _ball._vy/_velocityDivisor;

    _paddle.detectCollision(_ball);
    
    std::list<Obstacle*> toDestroy;
    
    for(std::list<Obstacle*>::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it) {
        if((*it)->detectCollision(_ball)) {
            if((*it)->endRoundOnCollision()) {
                _playing = false;
                return;
            }
            else if((*it)->destroyOnCollision()) {
                toDestroy.insert(toDestroy.begin(), *it);
            }
        }
    }
    
    for(std::list<Obstacle*>::iterator it2 = toDestroy.begin(); it2 != toDestroy.end(); ++it2) {
        _obstacles.remove(*it2);
        delete *it2;
    }
    
    return;
}

void Game::movePaddle(bool left)
{
    unit paddleOldX = _paddle._x;
    
    if(left)
        _paddle._vx -= _paddle._dvx;
    else
        _paddle._vx += _paddle._dvx;
        
    _paddle._x += _paddle._vx;
    
    for(std::list<Obstacle*>::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it) {
        if((*it)->detectCollision(_paddle)) {
            _paddle._x = paddleOldX;
            break;
        }
    }
}



// TODO: Add boundary detection to avoid having the paddle leave the screen
// FIXME: Sometimes the ball goes through the paddle
void Game::movePaddle(unit dx)
{
 
    unit paddleOldX = _paddle._x;
    _paddle._x -= dx;
    _paddle._vx += _paddle._dvx;
    
    
    for(std::list<Obstacle*>::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it) {
        if((*it)->detectCollision(_paddle)) {
            _paddle._x = paddleOldX;
            break;
        }
    }
}

bool Game::isWon()
{
    return _obstacles.size() == 4;   // only walls left
}
