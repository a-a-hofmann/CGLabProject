
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
    
    _camera = std::make_shared<Camera>();
    _cameraAlt = std::make_shared<Camera>();

    _modelMatrixStack.push(vmml::mat4f::IDENTITY);

    loadModel("ball.obj", true, true);
    loadModel("brick.obj", true, true);
    loadModel("paddle.obj", true, true);
    loadModel("walls.obj", true, true);
    loadModel("quad.obj", true, true);
    loadModel("rfloor.obj", true, true);
    loadModel("skydome.obj", true, true);
    loadModel("skybox.obj", true, true);
    
    loadModel("debug.obj", true, true);
    
    _modelMatrix = vmml::mat4f::IDENTITY;
}


void DemoSceneManager::drawModel(const std::string &name, GLenum mode, bool isReflection)
{
    Model::GroupMap &groups = getModel(name)->getGroups();
    for (auto i = groups.begin(); i != groups.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader.get())
        {
            shader->setUniform("ProjectionMatrix", _currentCamera->getProjectionMatrix());
            shader->setUniform("ViewMatrix", _currentCamera->getViewMatrix());
            shader->setUniform("ModelMatrix", _modelMatrixStack.top());
            
            vmml::mat3f normalMatrix;
            vmml::compute_inverse(vmml::transpose(vmml::mat3f(_modelMatrixStack.top())), normalMatrix);
            shader->setUniform("NormalMatrix", normalMatrix);
        
            shader->setUniform("EyePos", _currentCamera->getPosition());
            shader->setUniform("LightPos", _lightPos);
            shader->setUniform("Ia", vmml::vec3f(1.3f));
            shader->setUniform("Id", vmml::vec3f(1.3f));
            shader->setUniform("Is", vmml::vec3f(1.3f));
            
            if (isReflection)
            {
                shader->setUniform("OverrideColor", vmml::vec3f(1.67f, 1.9f, 1.9f));
            }
            else
            {
                shader->setUniform("OverrideColor", vmml::vec3f(0.0f));
            }
        }
        else
        {
            util::log("No shader available.", util::LM_WARNING);
        }
        geometry.draw(mode);
    }
}

void DemoSceneManager::drawSkyModel(const std::string &name, GLenum mode)
{
    Model::GroupMap &groups = getModel(name)->getGroups();
    for (auto i = groups.begin(); i != groups.end(); ++i) {
        Geometry & geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader.get()) {
            shader->setUniform("ProjectionMatrix", _currentCamera->getProjectionMatrix());
            shader->setUniform("ViewMatrix", _currentCamera->getViewMatrix());
            shader->setUniform("ModelMatrix", _modelMatrixStack.top());
        }
        else{
            util::log("No skybox shader available", util::LM_WARNING);
        }
        geometry.draw(mode);
    }
}

void DemoSceneManager::drawOutlinedModel(const std::string &name, float isOutlined, GLenum mode)
{
    float outlineFactor = 5.0;
    Model::GroupMap &groups = getModel(name)->getGroups();
    for (auto i = groups.begin(); i != groups.end(); ++i)
    {
        Geometry &geometry = i->second;
        GeometryData::VboVertices &vertexData = geometry.getVertexData();
        
        if (isOutlined) {
            extrudeVertex(vertexData, outlineFactor);
            geometry.updateVertexBuffer();
        }
        
        
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader.get())
        {
            shader->setUniform("ProjectionMatrix", _currentCamera->getProjectionMatrix());
            shader->setUniform("ViewMatrix", _currentCamera->getViewMatrix());
            shader->setUniform("ModelMatrix", _modelMatrixStack.top());
            
            vmml::mat3f normalMatrix;
            vmml::compute_inverse(vmml::transpose(vmml::mat3f(_modelMatrix)), normalMatrix);
            shader->setUniform("NormalMatrix", normalMatrix);
            
            shader->setUniform("EyePos", _currentCamera->getPosition());
            
            shader->setUniform("LightPos", _lightPos);
            shader->setUniform("Outlined", isOutlined);
            shader->setUniform("Ia", vmml::vec3f(1.f));
            shader->setUniform("Id", vmml::vec3f(1.f));
            shader->setUniform("Is", vmml::vec3f(1.f));
        }
        else
        {
            util::log("No shader available.", util::LM_WARNING);
        }
        geometry.draw(mode);
        
        
        
        if (isOutlined) {
            resetVertex(vertexData, outlineFactor);
            geometry.updateVertexBuffer();
        }
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
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glFrontFace(GL_CW);
  

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    
    
    Gyro *gyro = Gyro::getInstance();
    gyro->read();
    
    vmml::mat3f rotation = vmml::create_rotation(gyro->getRoll() * -M_PI_F, vmml::vec3f::UNIT_Y) *
                                vmml::create_rotation(gyro->getPitch() * -M_PI_F, vmml::vec3f::UNIT_X);
    
    
    // set second camera
    vmml::vec3f pos = getPaddlePos() + vmml::vec3f(0.0, -2.0, 1.0);
    _cameraAlt->moveCamera(pos);
    _cameraAlt->rotateCamera(rotation);
    _cameraAlt->lookAt(vmml::vec3f::UNIT_Y + vmml::vec3f(pos.x(), pos.y(), 0.0));
    
    // set first camera
    _camera->moveCamera(vmml::vec3f(0.0, -5.0, 7.0));
    _camera->rotateCamera(rotation);
    _camera->lookAt(vmml::vec3f::UNIT_Y);
    
    // set Projection matrix
    _camera->setProjection();
    
    // set active camera
    _currentCamera = _camera;
    
    
    _lightPos = vmml::vec4f(10.0, -3.0, 15.0, 1.0);
    
    startGame();
}

vmml::vec3f DemoSceneManager::getPaddlePos() const
{
    return vmml::vec3f(_game._paddle->_x, _game._paddle->_y, 0.0);
}

void DemoSceneManager::drawObstacles()
{
    for (Cuboid *obstacle : _game._obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle -> _x, obstacle -> _y, 0.5)));
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
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game._ball._vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game._ball._vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    drawModel("ball");
    popModelMatrix();
}

void DemoSceneManager::drawPaddle()
{
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(getPaddlePos()));
    drawModel("paddle");
    popModelMatrix();
}

void DemoSceneManager::drawParticleSystems()
{
    for(ParticleSystem* particleSystem : _game._particleSystems)
    {
        for(Particle* particle : particleSystem->_particles)
        {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(particle->_x, particle->_y, particle->_z)));
            transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.1)));
            drawModel(particle->getModelName());
            popModelMatrix();
        }
    }
}

void DemoSceneManager::drawSkybox()
{
    glDisable(GL_DEPTH_TEST);
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(5.0f)));
    drawSkyModel("skybox");
    popModelMatrix();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void DemoSceneManager::drawSkydome()
{
    glDisable(GL_DEPTH_TEST);
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(10.0f)));
    drawSkyModel("skydome");
    popModelMatrix();
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
}

void DemoSceneManager::extrudeVertex(GeometryData::VboVertices &vertexData, float outlineFactor)
{
    for (Vertex &v : vertexData)
    {
        Point3 &p = v.position;
        p.x += v.normal.x/outlineFactor;
        p.y += v.normal.y/outlineFactor;
        p.z += v.normal.z/outlineFactor;
    }
}

void DemoSceneManager::resetVertex(GeometryData::VboVertices &vertexData, float outlineFactor)
{
    for (Vertex &v : vertexData)
    {
        Point3 &p = v.position;
        p.x -= v.normal.x/outlineFactor;
        p.y -= v.normal.y/outlineFactor;
        p.z -= v.normal.z/outlineFactor;
    }
}

void DemoSceneManager::drawDebug(vmml::vec3f position)
{
//    pushModelMatrix();
//    transformModelMatrix(vmml::create_translation(position));
//    drawModel("debug");
//    popModelMatrix();
    
    glCullFace(GL_FRONT);
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(position));
    drawOutlinedModel("debug", 1.0);
    glCullFace(GL_BACK);
    drawOutlinedModel("debug", 0.0);
    popModelMatrix();
}

void DemoSceneManager::startGame()
{
    drawSkydome();
    
    drawObstacles();
    
//    drawDebug(vmml::vec3f(0.0, 0.0, 5.0));
    
    if(_game._playing)
    {
    
        _game.movePaddle(_game._ball._x < _game._paddle->_x);

        // touch controls
//        float touchDx = _scrolling.x();
//        _game.movePaddle(-touchDx);

        drawPaddle();

        _game.moveBall();
        drawBall();

        _game.moveParticles();
        drawParticleSystems();
    }
    else
    {
        if(_game.isWon())
            std::cout << "WON!!!" << std::endl;
        else
            std::cout << "LOST!" << std::endl;
    }

    
    drawMirrorFloor();

    drawFloorReflections();
    
    drawMirrorWall();
    
    drawWallReflections();
}

void DemoSceneManager::drawMirrorFloor()
{
    glEnable(GL_STENCIL_TEST);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(0.0, 0.0, -1.5)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(2.0, 1.0, 1.0)));
    drawModel("quad");
    popModelMatrix();
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(0, 0.0, -0.5)));
    transformModelMatrix(vmml::create_rotation(0.5f * M_PI_F, vmml::vec3f::UNIT_X));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.67f, 1.0f, 1.0f)));
    drawModel("rfloor");
    popModelMatrix();
    
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    
    glDepthMask(GL_TRUE);
}

void DemoSceneManager::drawMirrorWall()
{
    glEnable(GL_STENCIL_TEST);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(5, 4, 4)));
    transformModelMatrix(vmml::create_rotation(M_PI_F, vmml::vec3f::UNIT_Y));
    drawModel("walls");
    popModelMatrix();
    
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    
    glDepthMask(GL_TRUE);
}

void DemoSceneManager::drawFloorReflections()
{
    float angle = _time;
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x, _game._ball._y, -1)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game._ball._vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game._ball._vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    drawModel("ball", GL_TRIANGLES, true);
    popModelMatrix();
    
    for (Cuboid *obstacle : _game._obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle -> _x, obstacle -> _y, -1.5)));
            drawModel(obstacle->getModelName(), GL_TRIANGLES, true);
            popModelMatrix();
        }
    }
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._paddle->_x, _game._paddle->_y, -1)));
    drawModel(_game._paddle->getModelName(), GL_TRIANGLES, true);
    popModelMatrix();
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(5, 4, 4)));
    transformModelMatrix(vmml::create_rotation(M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_translation(vmml::vec3f(0, 0, 0.5)));
    drawModel("walls", GL_TRIANGLES, true);
    popModelMatrix();
    
    glDisable(GL_STENCIL_TEST);
}

void DemoSceneManager::drawWallReflections()
{
    float angle = _time;
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x - 1.4, _game._ball._y - 0.7, 0.5)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game._ball._vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game._ball._vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    drawModel("ball", GL_TRIANGLES, true);
    popModelMatrix();
    
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._paddle->_x - 1.5, _game._paddle->_y, 0.5)));
    drawModel(_game._paddle->getModelName(), GL_TRIANGLES, true);
    popModelMatrix();
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x + 1.5, _game._ball._y -2.5, 0.5)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game._ball._vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game._ball._vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    drawModel("ball", GL_TRIANGLES, true);
    popModelMatrix();
    
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._paddle->_x + 1.5, _game._paddle->_y, 0.5)));
    drawModel(_game._paddle->getModelName(), GL_TRIANGLES, true);
    popModelMatrix();

    
    glDisable(GL_STENCIL_TEST);
}
