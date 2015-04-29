uniform sampler2D DiffuseMap;

varying lowp vec4 colorVarying;
varying lowp vec4 texCoordVarying;

void main()
{
    lowp vec3 tc = texture2DProj( DiffuseMap, texCoordVarying ).xyz;
    lowp vec4 color = vec4(tc, 0.0);
    gl_FragColor = color;
}
