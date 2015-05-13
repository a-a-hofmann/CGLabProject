//
//  GameLogic.cpp
//  Flamework
//
//  Created by Adrian Gasparini on 15.04.15.
//
//

#include "GameLogic.h"


Game::Game() :
_obstacles(),
_particleSystems(),
_ball(0.0, 0.0, 0.3, 0.0, -0.3),
_paddle(0.0, -4.5, 4.5, 0.3, 0.5),
_velocityDivisor(2),
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
    
    
    for(unit x = -4.0; x <= 4.0; x += 2.0)
        for(unit y = 4.0; y >= 2.0; y -= 1.0)
            _obstacles.insert(_obstacles.begin(), new Brick(x, y, 2.0, 1.0));

}

Game::~Game()
{
    for(ObstacleList::iterator it = _obstacles.begin(); it != _obstacles.end(); ++it) {
        delete *it;
    }
    _obstacles.clear();
    
    for(ParticleSystem* particleSystem : _particleSystems)
    {
        delete particleSystem;
    }
    _particleSystems.clear();
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
    
    ObstacleList toDestroy;
    
    
    for(Cuboid* obstacle : _obstacles) {
        if(obstacle -> detectCollision(_ball)) {
            if(obstacle -> endRoundOnCollision()) {
                _playing = false;
                return;
            }
            else if(obstacle -> destroyOnCollision()) {
                toDestroy.insert(toDestroy.begin(), obstacle);
                _particleSystems.insert(_particleSystems.begin(), new ParticleSystem(obstacle->_x, obstacle->_y, 0.0));
            }
        }
    }
    
    for(ObstacleList::iterator it2 = toDestroy.begin(); it2 != toDestroy.end(); ++it2) {
        _obstacles.remove(*it2);
        delete *it2;
    }
    
}

void Game::movePaddle(bool left)
{
    unit paddleOldX = _paddle._x;
    if(left)
        _paddle._vx -= _paddle._dvx;
    else
        _paddle._vx += _paddle._dvx;
        
    _paddle._x += _paddle._vx;
    
    for (Cuboid* obstacle : _obstacles){
        if (obstacle -> detectCollision(_paddle)) {
            _paddle._x = paddleOldX;
            break;
        }
    }
}

void Game::movePaddle(float dx){
    unit paddleOldX = _paddle._x;
    _paddle._x -= dx;
    _paddle._vx += _paddle._dvx;
    
    for (Cuboid* obstacle : _obstacles){
        if (obstacle -> detectCollision(_paddle)) {
            _paddle._x = paddleOldX;
            break;
        }
    }
}

void Game::moveParticles()
{
    ParticleSystemList toDestroy;
    for(ParticleSystem* particleSystem : _particleSystems)
    {
        if(!particleSystem->moveParticles())
        {
            toDestroy.insert(toDestroy.begin(), particleSystem);
        }
    }
    
    for(ParticleSystem* particleSystem : toDestroy)
    {
        _particleSystems.remove(particleSystem);
        delete particleSystem;
    }
}

bool Game::isWon()
{
    return _obstacles.size() == 4;   // only walls left
}
