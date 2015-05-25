
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;
uniform lowp vec3 OverrideColor;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec4 TexCoord;

varying vec4 colorVarying;
//varying vec4 texCoordVarying;
//varying mediump vec3 normalVarying;

void main()
{
    vec4 pos = ModelMatrix * Position;
    vec3 n = normalize(NormalMatrix * Normal);
//    texCoordVarying = TexCoord;
    
    colorVarying = vec4(vec3(0.5) + n * 0.5, 1.0);
    
    gl_Position = ProjectionMatrix * ViewMatrix * pos;
}
