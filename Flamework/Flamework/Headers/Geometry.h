//
//  Geometry.h
//  Framework
/*
 Stores geometry data for one model group(or part).
 Initializes the buffer and renders the geometry.
 */
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#ifndef FRAMEWORK_GEOMETRY_H
#define FRAMEWORK_GEOMETRY_H

#include <string>
#include <fstream>
#include "GeometryData.h"
#include "Material.h"
#include "Texture.h"

#include "Framework_GL.h"


class Geometry
{
public:
    typedef std::shared_ptr< Vertex >   VertexDataPtr;
    typedef std::shared_ptr< GLushort > IndexDataPtr;
    
    void draw(GLenum mode = GL_TRIANGLES);
    
    void initialize(GeometryDataPtr geometryData);
    
    void initializeVertexBuffer();
    
    void updateVertexBuffer();

    void copyVertexData(const GeometryData::VboVertices &arg);
    void copyIndexData(const GeometryData::VboIndices &arg);
    
    template <class InputIterator>
    void copyVertexData(InputIterator first, InputIterator last)
    {
        _vertexData.assign(first, last);
    }
    
    template <class InputIterator>
    void copyIndexData(InputIterator first, InputIterator last)
    {
        _indexData.assign(first, last);
    }

    GeometryData::VboVertices   &getVertexData()        { return _vertexData;   }
    GeometryData::VboIndices    &getIndexData()         { return _indexData;    }
    
    MaterialPtr     getMaterial()                       { return _material;     }
    void            setMaterial(MaterialPtr arg)        { _material     = arg;  }
    
    size_t          getNumVertices()                    { return _vertexData.size(); }
    size_t          getNumIndices()                     { return _indexData.size();  }
    
private:
    GLuint _indexBuffer, _vertexBuffer;
    
    GeometryData::VboVertices   _vertexData;
    GeometryData::VboIndices    _indexData;
    
    MaterialPtr _material;
};

typedef std::shared_ptr<Geometry> GeometryPtr;

#endif
