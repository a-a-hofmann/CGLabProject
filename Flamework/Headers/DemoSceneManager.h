//
//  DemoSceneManager.h
//  Framework
//
//  Created by David Steiner on 4/28/14.
//
//

#ifndef FRAMEWORK_DEMO_SCENE_MANAGER_H
#define FRAMEWORK_DEMO_SCENE_MANAGER_H

#include "SceneManager.h"
#include "ITouchHandler.h"
#include "IScaleHandler.h"

#include <stack>
#include <queue>

#include "GameLogic.h"

#include <glm/glm/glm.hpp>


class Application;

class DemoSceneManager: public SceneManager, public ITouchHandler, public IScaleHandler
{
public:
    typedef std::stack< vmml::mat4f >   MatrixStack;
    typedef std::queue<CameraPtr>       CameraQueue;
    typedef std::shared_ptr<Game> GamePtr;


    DemoSceneManager(Application *application);
    virtual ~DemoSceneManager();
    
    virtual void onTouchBegan(float x, float y);
    virtual void onTouchMoved(float x, float y);
    virtual void onTouchEnded(float x, float y, int tapCount);
    
    virtual void onScaleBegan(float x, float y);
    virtual void onScaleMoved(float x, float y);
    virtual void onScaleEnded(float x, float y);
    
    virtual void initialize(size_t width, size_t height);
    virtual void draw(double deltaT);
    
    
    void drawModel(const std::string &name, bool isOutlined = false, bool isReflection = false, float outlineFactor = 7.0, GLenum mode = GL_TRIANGLES);
    void drawSkyboxModel(GLenum mode = GL_TRIANGLES);

    void pushModelMatrix();
    void popModelMatrix();
    void transformModelMatrix(const vmml::mat4f &t);
    
    void drawSkyModel(const std::string &name, GLenum mode = GL_TRIANGLES);
    void drawSphere();
    void drawObstacles();
    void drawBall();
    void drawPaddle();
    void drawParticleSystems();
    void drawSkybox();
    void drawSkydome();
    void drawDebug(vmml::vec3f position);
    
    void drawMirrorFloor();
    void drawFloorReflections();
    void drawMirrorWall();
    void drawWallReflections();

    void extrudeVertex(GeometryData::VboVertices &vertexData, float outlineFactor);
    void resetVertex(GeometryData::VboVertices &vertexData, float outlineFactor);
    
    void startGame();
    
    void setCameras();
    void swapCameras();
    
    vmml::vec3f getPaddlePos() const;
    
private:
    double _time;
    int _tapCount;

    vmml::vec2f _scrolling;
    vmml::vec2f _lScrollPos;
    vmml::vec2f _scaling;
    vmml::vec2f _lScale;
    
    vmml::vec3f _eyePos;
    vmml::vec4f _lightPos;
    
    MatrixStack _modelMatrixStack;
    vmml::mat4f _modelMatrix;
    vmml::mat4f _viewMatrix;
    

    GamePtr _game;
    
    CameraQueue _cameras;
    
    Gyro* _gyro;
    
};


#endif
