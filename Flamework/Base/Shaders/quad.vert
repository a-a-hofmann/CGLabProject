
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec4 TexCoord;

varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space
varying mediump vec3 tangentVarying;


void main()
{
    posVarying = ModelMatrix * Position;
    normalVarying = normalize(NormalMatrix * Normal);
    tangentVarying = normalize(NormalMatrix * Tangent);
    texCoordVarying = TexCoord;
    
    gl_Position = ProjectionMatrix * ViewMatrix * posVarying;

}
