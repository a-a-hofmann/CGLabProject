
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
    
    // Instanciate two cameras
    _cameras.push(std::make_shared<Camera>());
    _cameras.push(std::make_shared<Camera>());

    _modelMatrixStack.push(vmml::mat4f::IDENTITY);

    
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
    
}


void DemoSceneManager::drawModel(const std::string &name, bool isOutlined, bool isReflection, float outlineFactor, GLenum mode)
{

    Model::GroupMap &groups = getModel(name)->getGroups();
    for (auto i = groups.begin(); i != groups.end(); ++i)
    {
        Geometry &geometry = i->second;
        
        // Don't delete this!
        // Classic outlining. Extrude normals and draw the backfaces then draw over it normally
        GeometryData::VboVertices &vertexData = geometry.getVertexData();
//        if (isOutlined) {
//            extrudeVertex(vertexData, outlineFactor);
//            geometry.updateVertexBuffer();
//        }
        
        
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader.get())
        {
            shader->setUniform("ProjectionMatrix", _cameras.front()->getProjectionMatrix());
            shader->setUniform("ViewMatrix", _cameras.front()->getViewMatrix());
            shader->setUniform("ModelMatrix", _modelMatrixStack.top());
            
            vmml::mat3f normalMatrix;
            vmml::compute_inverse(vmml::transpose(vmml::mat3f(_modelMatrixStack.top())), normalMatrix);
            shader->setUniform("NormalMatrix", normalMatrix);
        
            shader->setUniform("EyePos", _cameras.front()->getPosition());
            shader->setUniform("LightPos", _lightPos);
            shader->setUniform("Ia", vmml::vec3f(1.3f));
            shader->setUniform("Id", vmml::vec3f(1.3f));
            shader->setUniform("Is", vmml::vec3f(1.3f));
            
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
}


// Shortened drawModel method for the sky-box/dome
void DemoSceneManager::drawSkyModel(const std::string &name, GLenum mode)
{
    Model::GroupMap &groups = getModel(name)->getGroups();
    for (auto i = groups.begin(); i != groups.end(); ++i) {
        Geometry & geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader.get()) {
            shader->setUniform("ProjectionMatrix", _cameras.front()->getProjectionMatrix());
            shader->setUniform("ViewMatrix", _cameras.front()->getViewMatrix());
            shader->setUniform("ModelMatrix", _modelMatrixStack.top());
        }
        else{
            util::log("No skybox shader available", util::LM_WARNING);
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

void DemoSceneManager::swapCameras()
{
    // sets current camera to last in queue and "selects" the newest camera
    if (_cameras.size() > 1) {
        _cameras.push(_cameras.front());
        _cameras.pop();
    }
}

void DemoSceneManager::draw(double deltaT)
{
    _time += deltaT;
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    
    glFrontFace(GL_CW);
  
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    
    setCameras();
    
    _lightPos = vmml::vec4f(10.0, -3.0, 15.0, 1.0);
    
    getSound("theme")->play();
//    swapCameras();
    startGame();
}

void DemoSceneManager::setCameras()
{
    Gyro *gyro = Gyro::getInstance();
    gyro->read();
    
    vmml::mat3f rotation = vmml::create_rotation(gyro->getRoll() * -M_PI_F, vmml::vec3f::UNIT_Y) *
    vmml::create_rotation(gyro->getPitch() * -M_PI_F, vmml::vec3f::UNIT_X);
    rotation = vmml::mat3f::IDENTITY;

    // set first camera
    _cameras.front()->moveCamera(vmml::vec3f(0.0, -5.0, 7.0));
    _cameras.front()->rotateCamera(rotation);
    _cameras.front()->lookAt(vmml::vec3f::UNIT_Y);
    _cameras.front()->setProjection();
    
    // set second camera
    vmml::vec3f pos = getPaddlePos() + vmml::vec3f(0.0, -9.0, 3.0);
    _cameras.back()->moveCamera(pos);
    _cameras.back()->rotateCamera(rotation);
    _cameras.back()->lookAt(vmml::vec3f::UNIT_Y * 10.f + vmml::vec3f(pos.x(), pos.y(), 0.0));
    _cameras.back()->setProjection(0.6f);
}

vmml::vec3f DemoSceneManager::getPaddlePos() const
{
    return vmml::vec3f(_game._paddle->_x, _game._paddle->_y, 0.0);
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
    for (Cuboid *obstacle : _game._obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle -> _x, obstacle -> _y, 0.5)));
            transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.1, 1.1, 1.)));
            drawModel(obstacle->getModelName(), true);
            popModelMatrix();
        }
    }
    glCullFace(GL_BACK);
    for (Cuboid *obstacle : _game._obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle -> _x, obstacle -> _y, 0.5)));
            drawModel(obstacle->getModelName(), false);
            popModelMatrix();
        }
    }
}

void DemoSceneManager::drawBall()
{
    float angle = _time * M_PI_F;
    float rotationFactor = 3.0f;
    
    float xRotation = _game._ball._vx * angle * rotationFactor;
    float yRotation = _game._ball._vy * angle * rotationFactor;
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x, _game._ball._y, 0)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(xRotation , vmml::vec3f::UNIT_X));
    transformModelMatrix(vmml::create_rotation(yRotation, vmml::vec3f::UNIT_Y));
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.2, 1.2, 1.2)));
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
    transformModelMatrix(vmml::create_translation(getPaddlePos()));
    
    drawModel("paddle", false);
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
    transformModelMatrix(vmml::create_translation(_cameras.front()->getPosition()));
    drawSkyModel("skydome");
    popModelMatrix();
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
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

void DemoSceneManager::startGame()
{
    drawSkydome();
    
    drawObstacles();
    
    if(_game._playing)
    {
        // Demo
        _game.movePaddle(_game._ball._x < _game._paddle->_x);

        // touch controls
//        float touchDx = _scrolling.x();
//        _game.movePaddle(touchDx);
        
        // Gyro controls
//        _game.movePaddle(Gyro::getInstance());

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
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x, _game._ball._y, -1)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game._ball._vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game._ball._vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.2, 1.2, 1.0)));
    glCullFace(GL_FRONT);
    drawModel("ball", true, true);
    
    popModelMatrix();
    glCullFace(GL_BACK);
    drawModel("ball", false, true);
    popModelMatrix();
    
    
    // Draw Bricks reflection and outline
    glCullFace(GL_FRONT);
    for (Cuboid *obstacle : _game._obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle -> _x, obstacle -> _y, -1.5)));
            transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.1, 1.1, 1.0)));
            drawModel(obstacle->getModelName(), true, true);
            popModelMatrix();
        }
    }
    glCullFace(GL_BACK);
    for (Cuboid *obstacle : _game._obstacles){
        if (strcmp(obstacle->getModelName(), "field")) {
            pushModelMatrix();
            transformModelMatrix(vmml::create_translation(vmml::vec3f(obstacle -> _x, obstacle -> _y, -1.5)));
            drawModel(obstacle->getModelName(), false, true);
            popModelMatrix();
        }
    }
    
    
    // Draw paddle reflection
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._paddle->_x, _game._paddle->_y, -1)));
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
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x - 1.4, _game._ball._y - 0.7, 0.5)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game._ball._vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game._ball._vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.2, 1.2, 1.0)));
    glCullFace(GL_FRONT);
    drawModel("ball", true, true);
    popModelMatrix();
    
    glCullFace(GL_BACK);
    drawModel("ball", false, true);
    popModelMatrix();
    
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._paddle->_x - 1.5, _game._paddle->_y, 0.5)));
    drawModel(_game._paddle->getModelName(), false, true);
    popModelMatrix();
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._ball._x + 1.5, _game._ball._y -2.5, 0.5)));
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(0.5)));
    transformModelMatrix(vmml::create_rotation(_game._ball._vx * angle * M_PI_F, vmml::vec3f::UNIT_Y));
    transformModelMatrix(vmml::create_rotation(_game._ball._vy * angle * M_PI_F, vmml::vec3f::UNIT_X));
    
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_scaling(vmml::vec3f(1.2, 1.2, 1.0)));
    glCullFace(GL_FRONT);
    drawModel("ball", true, true);
    popModelMatrix();
    
    glCullFace(GL_BACK);
    drawModel("ball", false, true);
    popModelMatrix();
    
    
    pushModelMatrix();
    transformModelMatrix(vmml::create_translation(vmml::vec3f(_game._paddle->_x + 1.5, _game._paddle->_y, 0.5)));
    drawModel(_game._paddle->getModelName(), false, true);
    popModelMatrix();

    
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
}
