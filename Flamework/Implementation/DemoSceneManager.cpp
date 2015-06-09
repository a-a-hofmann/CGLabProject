
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
//    util::log("onTouchBegan");
    vmml::vec2f cScrollPos(x, y);
    _lScrollPos = cScrollPos;
    
    
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
    
    // init cameras
    initCameras();

    // push identity to stack
    _modelMatrixStack.push(vmml::mat4f::IDENTITY);
    
    // Create game
    _game = std::make_shared<Game>();
    
    // Gyro
    _gyro = Gyro::getInstance();
    
    // Sky
    initSky();
    
    // Light
    _lightPos = vmml::vec4f(10.0, -3.0, 15.0, 1.0);

    
    // Load objects
    loadModel("ball.obj", true, true);
    loadModel("brick.obj", true, true);
    loadModel("paddle.obj", true, true);
    loadModel("walls.obj", true, true);
    loadModel("quad.obj", true, true);
    loadModel("rfloor.obj", true, true);
    loadModel("skydome.obj", true, true);
    loadModel("skybox.obj", true, true);
//    loadModel("debug.obj", true, true);
    
    // Load music/sound effects
    loadSound("test.mp3");
    loadSound("theme.mp3");
    
    
    // init shaders static uniforms
    initShaders();
    
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
}

void DemoSceneManager::initShaders()
{
    setUpShader("ball");
    setUpShader("brick");
    setUpShader("paddle");
    setUpShader("walls");
    setUpShader("quad");
    setUpShader("rfloor");
    setUpSkyShader(_sky->getName());
}

void DemoSceneManager::initSky()
{
    _sky = std::make_shared<Sky>("skydome");
    _sky->transformModelMatrix(vmml::create_translation(_cameras.front()->getPosition()));
}

void DemoSceneManager::initCameras()
{
    vmml::vec3f position = vmml::vec3f(0.0, -5.0, 7.0);
    
    _cameras.push(std::make_shared<Camera>(position));
    _cameras.push(std::make_shared<Camera>());
    
    // set first camera
    _cameras.front()->moveCamera(vmml::vec3f(0.0, -5.0, 7.0));
    _cameras.front()->lookAt(vmml::vec3f::UNIT_Y + vmml::vec3f::UNIT_Z);
    _cameras.front()->setProjection();
    
}

void DemoSceneManager::setSecondCamera()
{
    vmml::vec3f pos = _game->_paddle->getPosition3f() + vmml::vec3f(0.0, -9.0, 3.0);
    _cameras.front()->moveCamera(pos);
    _cameras.front()->lookAt(vmml::vec3f::UNIT_Y * 10.f + vmml::vec3f(pos.x(), pos.y(), 0.0));
    _cameras.front()->setProjection(0.6f);
    
    initShaders();
}

void DemoSceneManager::setThirdCamera()
{
    _cameras.push(std::make_shared<Camera>());
    _cameras.back()->moveCamera(vmml::vec3f(0.0, -23.0, 13.0));
    _cameras.back()->lookAt(vmml::vec3f(0.0, 10.0, 10.0));
    _cameras.back()->setProjection(1.2);
    
    initShaders();
}

void DemoSceneManager::swapCameras()
{
    // sets current camera to last in queue and "selects" the newest camera
    if (_cameras.size() > 1) {
        _cameras.push(_cameras.front());
        _cameras.pop();
    }
}


// Model matrix stack methods
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


// Set up these uniforms only once in the init: These don't change inbetween frames
void DemoSceneManager::setUpSkyShader(const std::string &name)
{
    Model::GroupMap &groups = getModel(name)->getGroups();
    Geometry &geometry = groups.begin()->second;
    MaterialPtr material = geometry.getMaterial();
    ShaderPtr shader = material->getShader();
    
    if (shader.get())
    {
        shader->setUniform("ProjectionMatrix", _cameras.front()->getProjectionMatrix());
        shader->setUniform("ViewMatrix", _cameras.front()->getViewMatrix());
        shader->setUniform("ModelMatrix", _sky->getModelMatrix());
        
    }
    else
    {
        util::log("No shader available.", util::LM_WARNING);
    }
    

    
}

void DemoSceneManager::setUpShader(const std::string &name)
{
    Model::GroupMap &groups = getModel(name)->getGroups();
    Geometry &geometry = groups.begin()->second;
    MaterialPtr material = geometry.getMaterial();
    ShaderPtr shader = material->getShader();
    
    if (shader.get())
    {
        shader->setUniform("ProjectionMatrix", _cameras.front()->getProjectionMatrix());
        shader->setUniform("ViewMatrix", _cameras.front()->getViewMatrix());
        shader->setUniform("EyePos", _cameras.front()->getPosition());
        shader->setUniform("LightPos", _lightPos);
        shader->setUniform("Ia", vmml::vec3f(1.3f));
        shader->setUniform("Id", vmml::vec3f(1.3f));
        shader->setUniform("Is", vmml::vec3f(1.3f));
        
    }
    else
    {
        util::log("No shader available.", util::LM_WARNING);
    }

    
}


// Draw methods
void DemoSceneManager::drawModel(const std::string &name, bool isOutlined, bool isReflection, float outlineFactor, GLenum mode)
{
    
    Model::GroupMap &groups = getModel(name)->getGroups();
    Geometry &geometry = groups.begin()->second;
    
    // Don't delete this!
    // Classic outlining. Extrude normals and draw the backfaces then draw over it normally
//        GeometryData::VboVertices &vertexData = geometry.getVertexData();
//        if (isOutlined) {
//            extrudeVertex(vertexData, outlineFactor);
//            geometry.updateVertexBuffer();
//        }
    
    
    MaterialPtr material = geometry.getMaterial();
    ShaderPtr shader = material->getShader();
    if (shader.get())
    {

        shader->setUniform("ModelMatrix", _modelMatrixStack.top());
        
        vmml::mat3f normalMatrix;
        vmml::compute_inverse(vmml::transpose(vmml::mat3f(_modelMatrixStack.top())), normalMatrix);
        shader->setUniform("NormalMatrix", normalMatrix);
    
        // string::compare return 0 iff str1 == str2
        // iff obj to draw is NOT a quad or rfloor
        if (name.compare("quad") && name.compare("rfloor"))
        {
            shader->setUniform("isOutlined", isOutlined);

            if (isReflection)
            {
                shader->setUniform("OverrideColor", vmml::vec3f(1.67f, 1.9f, 1.9f));
            }
            else
            {
                shader->setUniform("OverrideColor", vmml::vec3f(0.0f));
            }
        }
    }
    else
    {
        util::log("No shader available.", util::LM_WARNING);
    }
    geometry.draw(mode);
    
    
    // Don't delete this!
//        if (isOutlined) {
//            resetVertex(vertexData, outlineFactor);
//            geometry.updateVertexBuffer();
//        }

}


// Shortened drawModel method for the sky
void DemoSceneManager::drawSky(GLenum mode)
{
    glDisable(GL_DEPTH_TEST);
    
    Model::GroupMap &groups = getModel(_sky->getName())->getGroups();
    Geometry & geometry = groups.begin()->second;
    geometry.draw(mode);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}



void DemoSceneManager::draw(double deltaT)
{
    _time += deltaT;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    getSound("theme")->play();
    startGame();
}


void DemoSceneManager::startGame()
{
    drawSky();
    
    drawObstacles();
    
    if(_game->_playing)
    {
        _game->moveBall();
        drawBall();
        
        // Demo
        _game->movePaddle(_game->_ball->_x < _game->_paddle->_x);
        
        // touch controls
        //        float touchDx = _scrolling.x();
        //        _game->movePaddle(touchDx);
        
        // Gyro controls
//        _game->movePaddle(_gyro);
        
        drawPaddle();
        
        _game->moveParticles();
        drawParticleSystems();
    }
    else
    {
        if(_game->isWon())
            std::cout << "WON!!!" << std::endl;
        else
            std::cout << "LOST!" << std::endl;
    }
    
    
    drawMirrorFloor();
    
    drawFloorReflections();
    
    drawMirrorWall();
    
    drawWallReflections();
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


void DemoSceneManager::drawObstacles()
{
    glCullFace(GL_FRONT);
    for (Cuboid *obstacle : _game->_obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle->getPosition2f(), 0.5)));
            transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.1, 1.1, 1.)));
            drawModel(obstacle->getModelName(), true);
            popModelMatrix();
        }
    }
    glCullFace(GL_BACK);
    for (Cuboid *obstacle : _game->_obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle->getPosition2f(), 0.5)));
            drawModel(obstacle->getModelName(), false);
            popModelMatrix();
        }
    }
}

void DemoSceneManager::drawBall()
{
    float angle = _time * M_PI_F;
    float rotationFactor = 3.0f;
    
    float xRotation = _game->_ball->_vx * angle * rotationFactor;
    float yRotation = _game->_ball->_vy * angle * rotationFactor;
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(_game->_ball->getPosition3f()));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(xRotation , vmml::vec3f::UNIT_X));
    transformModelMatrix(vmml::create_rotation(yRotation, vmml::vec3f::UNIT_Y));
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.3f)));
    glCullFace(GL_FRONT);
    drawModel("ball", true);
    popModelMatrix();
    
    glCullFace(GL_BACK);
    drawModel("ball", false);
    popModelMatrix();
}

void DemoSceneManager::drawPaddle()
{
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(_game->_paddle->getPosition3f()));
    
    drawModel("paddle", false);
    popModelMatrix();
}

void DemoSceneManager::drawParticleSystems()
{
    for(ParticleSystem* particleSystem : _game->_particleSystems)
    {
        for(Particle* particle : particleSystem->_particles)
        {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(particle->getPosition3f()));
            transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.1)));
            drawModel(particle->getModelName());
            popModelMatrix();
        }
    }
}

void DemoSceneManager::drawDebug(vmml::vec3f position)
{
    glCullFace(GL_FRONT);
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(position));
        transformModelMatrix(vmml::create_rotation(M_PI_F/2.0f, vmml::vec3f::UNIT_X));
    drawModel("debug", true);
    glCullFace(GL_BACK);
    drawModel("debug", true);
    popModelMatrix();
}


void DemoSceneManager::drawMirrorFloor()
{
    glEnable(GL_STENCIL_TEST);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(0.0, 0.0, -1.5)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(2.0, 1.0, 1.0)));
    drawModel("quad");
    popModelMatrix();
    
    
    glEnable(GL_BLEND);
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
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(5, 4, 4)));
    transformModelMatrix(vmml::create_rotation(M_PI_F, vmml::vec3f::UNIT_Y));
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.05, 1.05, 1.0)));
    glCullFace(GL_FRONT);
    drawModel("walls", true, false);
    popModelMatrix();
    glCullFace(GL_BACK);
    drawModel("walls", false, false);
    popModelMatrix();
    
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    
    glDepthMask(GL_TRUE);
}

void DemoSceneManager::drawFloorReflections()
{
    
    // Draw ball reflection and outline
    float angle = _time;
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game->_ball->getPosition2f(), -1)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game->_ball->_vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game->_ball->_vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.2f)));
    glCullFace(GL_FRONT);
    drawModel("ball", true, true);
    
    popModelMatrix();
    glCullFace(GL_BACK);
    drawModel("ball", false, true);
    popModelMatrix();
    
    
    // Draw Bricks reflection and outline
    glCullFace(GL_FRONT);
    for (Cuboid *obstacle : _game->_obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle->getPosition2f(), -1.5)));
            transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.1, 1.1, 1.0)));
            drawModel(obstacle->getModelName(), true, true);
            popModelMatrix();
        }
    }
    glCullFace(GL_BACK);
    for (Cuboid *obstacle : _game->_obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle->getPosition2f(), -1.5)));
            drawModel(obstacle->getModelName(), false, true);
            popModelMatrix();
        }
    }
    
    
    // Draw paddle reflection
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game->_paddle->getPosition2f(), -1)));
    drawModel("paddle", false, true);
    popModelMatrix();
    
    
    // Draw walls reflection
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(5, 4, 4)));
    transformModelMatrix(vmml::create_rotation(M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_translation(vmml::vec3f(0, 0, 0.5)));
    drawModel("walls", false, true);
    popModelMatrix();
    
    glDisable(GL_STENCIL_TEST);
}

void DemoSceneManager::drawWallReflections()
{
    float angle = _time;
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(_game->_ball->getPosition3f() + vmml::vec3f(-1.4, -0.7, 0.5)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game->_ball->_vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game->_ball->_vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.2f)));
    glCullFace(GL_FRONT);
    drawModel("ball", true, true);
    popModelMatrix();
    
    glCullFace(GL_BACK);
    drawModel("ball", false, true);
    popModelMatrix();
    
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(_game->_paddle->getPosition3f() + vmml::vec3f(-1.5, 0.0, 0.5)));
    drawModel(_game->_paddle->getModelName(), false, true);
    popModelMatrix();
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game->_ball->getPosition3f() + vmml::vec3f(1.5, -2.5, 0.5))));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game->_ball->_vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game->_ball->_vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.2f)));
    glCullFace(GL_FRONT);
    drawModel("ball", true, true);
    popModelMatrix();
    
    glCullFace(GL_BACK);
    drawModel("ball", false, true);
    popModelMatrix();
    
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game->_paddle->getPosition3f() + vmml::vec3f(1.5, 0.0, 0.5))));
    drawModel(_game->_paddle->getModelName(), false, true);
    popModelMatrix();

    
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
}
