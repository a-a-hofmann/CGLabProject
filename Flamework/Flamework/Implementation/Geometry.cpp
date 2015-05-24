//
//  Geometry.cpp
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#include "Geometry.h"
#include "Framework_GL.h"


void Geometry::initialize(GeometryDataPtr geometryData)
{
    copyVertexData(geometryData->vboVertices);
    copyIndexData(geometryData->vboIndices);
    initializeVertexBuffer();
}

void Geometry::copyVertexData(const GeometryData::VboVertices &arg)
{
    _vertexData = arg;
}

void Geometry::copyIndexData(const GeometryData::VboIndices &arg)
{
    _indexData = arg;
}

void Geometry::initializeVertexBuffer()
{
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.size() * sizeof(Vertex), &_vertexData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::updateVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.size() * sizeof(Vertex), &_vertexData[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::draw(GLenum mode)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    
#ifndef USE_GL_ES2
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoord)));
    glNormalPointer(GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
#endif
    if (_material) _material->bind();
    
    glDrawElements(mode, _indexData.size(), GL_UNSIGNED_SHORT, &_indexData[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
#ifndef USE_GL_ES2
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
#endif
}
