
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;


uniform sampler2D DiffuseMap;

varying mediump vec4 texCoordVarying;


void main()
{
    gl_FragColor = texture2D(DiffuseMap, texCoordVarying.st);    
}
