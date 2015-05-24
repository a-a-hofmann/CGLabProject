//
//  TextureData.h
//  Framework
//
//  Created by David Steiner.
//
//

#ifndef FRAMEWORK_GEOMETRY_DATA_H
#define FRAMEWORK_GEOMETRY_DATA_H

#include <vector>
#include <unordered_map>
#include "Framework_GL.h"
#include "vector.hpp"
#include "MaterialData.h"
#include "Texture.h"

class GeometryDataImpl;

struct IndexData
{
    GLushort vertexIndex;
    GLushort texCoordsIndex;
    GLushort normalIndex;
    
    IndexData()
    : vertexIndex(0)
    , texCoordsIndex(0)
    , normalIndex(0)
    {}
};

struct TexCoord
{
    GLfloat s;
    GLfloat t;
};

struct Color
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct Point3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Vector3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Vertex
{
    Vertex()
    {
        position.x = 0;
        position.y = 0;
        position.z = 0;
        normal.x = 0;
        normal.y = 0;
        normal.z = 0;
        tangent.x = 0;
        tangent.y = 0;
        tangent.z = 0;
        bitangent.x = 0;
        bitangent.y = 0;
        bitangent.z = 0;
        texCoord.s = 0;
        texCoord.t = 0;
    }
    
    Vertex(GLfloat pX, GLfloat pY, GLfloat pZ, GLfloat tS, GLfloat tT)
    {
        position.x = pX;
        position.y = pY;
        position.z = pZ;
        normal.x = 0;
        normal.y = 0;
        normal.z = 0;
        tangent.x = 0;
        tangent.y = 0;
        tangent.z = 0;
        bitangent.x = 0;
        bitangent.y = 0;
        bitangent.z = 0;
        texCoord.s = tS;
        texCoord.t = tT;
    }
    
    Point3      position;
    Vector3     normal;
    Vector3     tangent;
    Vector3     bitangent;
    TexCoord    texCoord;
};

typedef GLushort Index;

struct GeometryData
{
    typedef std::vector< Vertex >   VboVertices;
    typedef std::vector< GLushort > VboIndices;
    
    std::vector< IndexData > indices;
    
    VboVertices vboVertices;
    VboIndices  vboIndices;
    
    MaterialData materialData;
};

typedef std::shared_ptr< GeometryData > GeometryDataPtr;

#endif
