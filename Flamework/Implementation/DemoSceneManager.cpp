//
//  DemoSceneManager.cpp
//  Framework
//
//  Created by David Steiner on 4/28/14.
//
//

#include "DemoSceneManager.h"
#include "addendum.hpp"

#include "Application.h"
#include "TextureData.h"
#include "ModelData.h"
#include "ShaderData.h"
#include "Framework_GL.h"
#include "Util.h"

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

#define SCROLL_SPEED    0.002f
#define SCALE_SPEED     0.008f


DemoSceneManager::DemoSceneManager(Application *application)
    : SceneManager(application)
    , _time(12)
    , _scaling(1, 1)
    , _scrolling(0, 0.25)
{
    
    
}

DemoSceneManager::~DemoSceneManager() {}

void DemoSceneManager::onTouchBegan(float x, float y)
{
    util::log("onTouchBegan");
    vmml::vec2f cScrollPos(x, y);
    _lScrollPos = cScrollPos;
    
    getSound("test")->play();
}

void DemoSceneManager::onTouchMoved(float x, float y)
{
    util::log("onTouchMoved");
    vmml::vec2f cScrollPos(x, y);
    vmml::vec2f scrollDelta(cScrollPos - _lScrollPos);
    _scrolling += scrollDelta * SCROLL_SPEED;
    _lScrollPos = cScrollPos;
}

void DemoSceneManager::onTouchEnded(float x, float y, int tapCount)
{
    util::log("onTouchEnded");
}

void DemoSceneManager::onScaleBegan(float x, float y)
{
    util::log("onScaleBegan");
    vmml::vec2f cScale(-x, y);
    _lScale = cScale;
}

void DemoSceneManager::onScaleMoved(float x, float y)
{
    util::log("onScaleModev");
    vmml::vec2f cScale(-x, y);
    vmml::vec2f scaleDelta(cScale - _lScale);
    _scaling += scaleDelta * SCALE_SPEED;
    _lScale = cScale;
}

void DemoSceneManager::onScaleEnded(float x, float y)
{
    vmml::vec2f cScale(-x, y);
    vmml::vec2f scaleDelta(cScale - _lScale);
    _scaling += scaleDelta * SCALE_SPEED;
    _lScale = cScale;
}

void DemoSceneManager::initialize(size_t width, size_t height)
{
    getApplication()->addTouchHandler(this);
    getApplication()->addScaleHandler(this);

    _modelMatrixStack.push(vmml::mat4f::IDENTITY);

    loadModel("sphere.obj", true, true);
    loadModel("brick.obj", true, true);
    loadModel("paddle.obj", true, true);
    loadModel("horizontalwall.obj", true, true);
    loadModel("verticalwall.obj", true, true);
//    loadSound("test.mp3");
}

vmml::mat4f lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up)
{
    vmml::vec3f zaxis = vmml::normalize(eye - target);
    vmml::vec3f xaxis = vmml::normalize(vmml::cross<3>(up, zaxis));
    vmml::vec3f yaxis = vmml::cross<3>(zaxis, xaxis);
    
    vmml::mat4f view;
    view.set_row(0, vmml::vec4f(xaxis.x(), xaxis.y(), xaxis.z(), -vmml::dot(xaxis, eye)));
    view.set_row(1, vmml::vec4f(yaxis.x(), yaxis.y(), yaxis.z(), -vmml::dot(yaxis, eye)));
    view.set_row(2, vmml::vec4f(zaxis.x(), zaxis.y(), zaxis.z(), -vmml::dot(zaxis, eye)));
    view.set_row(3, vmml::vec4f(0, 0, 0, 1.0));
    
    return view;
}

void DemoSceneManager::drawModel(const std::string &name, GLenum mode)
{
    Model::GroupMap &groups = getModel(name)->getGroups();
    for (auto i = groups.begin(); i != groups.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader.get())
        {
            shader->setUniform("ProjectionMatrix", vmml::create_scaling(vmml::vec3f(.1f)));
            shader->setUniform("ModelViewMatrix", _viewMatrix * _modelMatrix);
            
            vmml::mat3f normalMatrix;
            vmml::compute_inverse(vmml::transpose(vmml::mat3f(_modelMatrix)), normalMatrix);
            shader->setUniform("NormalMatrix", normalMatrix);
        }
        else
        {
            util::log("No shader available.", util::LM_WARNING);
        }
        geometry.draw(mode);
    }
}

void DemoSceneManager::pushModelMatrix()
{
    // TODO: implement
    _modelMatrixStack.push(_modelMatrix);
}

void DemoSceneManager::popModelMatrix()
{
    // TODO: implement
    _modelMatrix = _modelMatrixStack.top();
    _modelMatrixStack.pop();
}

void DemoSceneManager::transformModelMatrix(const vmml::mat4f &t)
{
    // TODO: implement
    _modelMatrix = _modelMatrix * t;
}

// draw sphere at origin of current reference frame
void DemoSceneManager::drawSphere()
{
    drawModel("sphere");
}

Game _game;

void DemoSceneManager::draw(double deltaT)
{
    _time += deltaT;
    float angle = _time * .1;   // .1 radians per second
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    Gyro *gyro = Gyro::getInstance();
    gyro->read();

    vmml::mat4f translation = vmml::create_translation(vmml::vec3f(_scrolling.x(), -_scrolling.y(), 0));
    vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(.12f, .12f, .12f));
    
    vmml::mat3f rotation = vmml::create_rotation(gyro->getRoll() * -M_PI_F, vmml::vec3f::UNIT_Y) *
    vmml::create_rotation(gyro->getPitch() * -M_PI_F, vmml::vec3f::UNIT_X);
    vmml::vec3f eyePos(0, 0, 0.25);
    vmml::vec3f eyeUp = vmml::vec3f::UP;
    _viewMatrix = lookAt(rotation * eyePos, vmml::vec3f::ZERO, rotation * eyeUp);

    _modelMatrix = vmml::mat4f::IDENTITY;
    
    if(_game._playing)
    {
        _game.movePaddle(_game._ball._x < _game._paddle._x);
        _game.moveBall();
    }
    else
    {
        if(_game.isWon())
            std::cout << "WON!!!" << std::endl;
        else
            std::cout << "LOST!" << std::endl;
    }
    
    for(std::list<Obstacle*>::iterator it = _game._obstacles.begin(); it != _game._obstacles.end(); ++it) {
        pushModelMatrix();
        transformModelMatrix(vmml::create_translation(vmml::vec3f((*it)->_x, (*it)->_y, 0)));
        drawModel((*it)->getModelName());
        popModelMatrix();
    }
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._paddle._x, _game._paddle._y, 0)));
    drawModel(_game._paddle.getModelName());
    popModelMatrix();
    
    if(_game._playing) {
        pushModelMatrix();
        transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x, _game._ball._y, 0)));
        drawModel(_game._ball.getModelName());
        popModelMatrix();
    }
}
