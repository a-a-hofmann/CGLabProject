
#include "DemoSceneManager.h"
#include "addendum.hpp"

#include "Application.h"
#include "TextureData.h"
#include "ModelData.h"
#include "ShaderData.h"
#include "Framework_GL.h"
#include "Util.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/matrix_access.hpp>


#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

#define SCROLL_SPEED    0.002f
#define SCALE_SPEED     0.008f


DemoSceneManager::DemoSceneManager(Application *application)
    : SceneManager(application)
    , _time(12)
    , _scaling(1, 1)
    , _scrolling(0, 0.25)
{}

DemoSceneManager::~DemoSceneManager() {}

void DemoSceneManager::onTouchBegan(float x, float y)
{
    util::log("onTouchBegan");
    vmml::vec2f cScrollPos(x, y);
    _lScrollPos = cScrollPos;
    
//    getSound("test")->play();
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

    loadModel("ball.obj", true, true);
    loadModel("brick.obj", true, true);
    loadModel("paddle.obj", true, true);
    loadModel("walls.obj", true, true);
    loadModel("quad.obj", true, true);
    
    _modelMatrix = vmml::mat4f::IDENTITY;
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

vmml::mat4f getProjectionMatrix()
{
    glm::mat4 proj = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
    
    vmml::mat4f projection = vmml::mat4f::IDENTITY;
    projection.set_column(0, (vmml::vec4f(proj[0][0], proj[0][1], proj[0][2], proj[0][3])));
    projection.set_column(1, (vmml::vec4f(proj[1][0], proj[1][1], proj[1][2], proj[1][3])));
    projection.set_column(2, (vmml::vec4f(proj[2][0], proj[2][1], proj[2][2], proj[2][3])));
    
    return projection;
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
            shader->setUniform("ProjectionMatrix", getProjectionMatrix());
            shader->setUniform("ViewMatrix", _viewMatrix);
            shader->setUniform("ModelMatrix", _modelMatrixStack.top());
            
            vmml::mat3f normalMatrix;
            vmml::compute_inverse(vmml::transpose(vmml::mat3f(_modelMatrixStack.top())), normalMatrix);
            shader->setUniform("NormalMatrix", normalMatrix);
            
            shader->setUniform("EyePos", _eyePos);
//            shader->setUniform("LightPos", vmml::vec4f(0.f, 3.f, 1.f, 1.f));
            shader->setUniform("LightPos", vmml::vec4f(7.0f, 10.0f, 2.0f, 1.f));
//            shader->setUniform("LightPos", vmml::vec4f(2.0f, 2.0f, 2.0f, 1.f));
            shader->setUniform("Ia", vmml::vec3f(1.2f));
            shader->setUniform("Id", vmml::vec3f(1.0f));
            shader->setUniform("Is", vmml::vec3f(1.0f));
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
    if (_modelMatrixStack.size() != 0) {
        vmml::mat4f tmp = _modelMatrixStack.top();
        _modelMatrixStack.push(tmp);
    }
}

void DemoSceneManager::popModelMatrix()
{
    if (_modelMatrixStack.size() > 1) {
        _modelMatrixStack.pop();
    }
}

void DemoSceneManager::transformModelMatrix(const vmml::mat4f &t)
{
    _modelMatrixStack.top() =  _modelMatrixStack.top() * t;
}

// draw sphere at origin of current reference frame
void DemoSceneManager::drawSphere()
{
    drawModel("sphere");
}


void DemoSceneManager::draw(double deltaT)
{
    _time += deltaT;
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  

    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    
    Gyro *gyro = Gyro::getInstance();
    gyro->read();
    
    vmml::mat3f rotation = vmml::create_rotation(gyro->getRoll() * -M_PI_F, vmml::vec3f::UNIT_Y) *
                                vmml::create_rotation(gyro->getPitch() * -M_PI_F, vmml::vec3f::UNIT_X);
    _eyePos = vmml::vec3f(0.0, -5.0, -7.0);
    vmml::vec3f eyeUp = vmml::vec3f::UP;
    _viewMatrix = lookAt(rotation * _eyePos, vmml::vec3f::UNIT_Y, eyeUp);
    
    startGame();
}

void DemoSceneManager::drawObstacles()
{
    for (Cuboid *obstacle : _game._obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle -> _x, obstacle -> _y, 0)));
            drawModel(obstacle->getModelName());
            popModelMatrix();
        }
    }
}

void DemoSceneManager::drawBall()
{
    float angle = _time;
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x, _game._ball._y, 0)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.3)));
    transformModelMatrix(vmml::create_rotation(angle * M_PI_F, vmml::vec3f::UNIT_Z));
    drawModel("ball");
    popModelMatrix();
}

void DemoSceneManager::drawPaddle()
{
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._paddle._x, _game._paddle._y, 0)));
    drawModel(_game._paddle.getModelName());
    popModelMatrix();
}

void DemoSceneManager::drawField(){
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(0.0, 0.0, 2.0)));
    drawModel("quad");
    popModelMatrix();
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(5, 4, 4)));
    drawModel("walls");
    popModelMatrix();
}



void DemoSceneManager::startGame()
{
    drawField();

    if(_game._playing)
    {
////         Autopilot: DEMO
        _game.movePaddle(_game._ball._x < _game._paddle._x);
        
        // touch controls
//        float touchDx = _scrolling.x();
//        _game.movePaddle(touchDx);
        
        drawPaddle();

        _game.moveBall();
        drawBall();
    }
    else
    {
        if(_game.isWon())
            std::cout << "WON!!!" << std::endl;
        else
            std::cout << "LOST!" << std::endl;
    }
    
    drawObstacles();
    
}
