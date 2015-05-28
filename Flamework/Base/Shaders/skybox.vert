
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

attribute vec4 Position;
attribute vec4 TexCoord;

varying lowp vec4 texCoordVarying;

void main()
{
    texCoordVarying = TexCoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
    
}
