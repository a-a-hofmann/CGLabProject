
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;


uniform sampler2D DiffuseMap;

varying mediump vec4 texCoordVarying;


void main()
{
    lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.st);
    gl_FragColor = color;
    
}
