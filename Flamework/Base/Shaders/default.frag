
varying lowp vec4 colorVarying;
varying mediump vec3 normalVarying;

uniform lowp float Outlined;
void main()
{
    gl_FragColor = vec4(0.5 * normalVarying + 0.5, 1.0);
    if (Outlined > 0.5) {
        gl_FragColor = vec4(vec3(0.0), 1.0);
    }
}
