
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

attribute vec4 Position;
attribute vec3 Normal;

varying vec4 colorVarying;
varying mediump vec3 normalVarying;

void main()
{
    vec4 pos = ModelMatrix * Position;
    vec3 n = normalize(NormalMatrix * Normal);
    normalVarying = n;
    
    colorVarying = vec4(vec3(0.5) + n * 0.5, 1.0);
    
    gl_Position = ProjectionMatrix * ViewMatrix * pos;
}
