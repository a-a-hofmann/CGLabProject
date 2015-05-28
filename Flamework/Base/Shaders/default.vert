
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

attribute vec4 Position;
attribute vec3 Normal;

varying mediump vec3 normalVarying;

void main()
{
    normalVarying = normalize(NormalMatrix * Normal);
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
}
