
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform mediump float Ns;   // specular material exponent (shininess)

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

uniform lowp vec3 OverrideColor;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec4 TexCoord;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
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
    
    
//    
    // Ambient component
    ambientVarying = vec4(Ka * Ia, 1.0);
    
    // Diffuse component
    mediump vec3 l = normalize(LightPos.xyz - posVarying.xyz);
    lowp float intensity = dot(normalVarying, l);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    diffuseVarying = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    // Specular component
    specularVarying = vec4(0.0);
    if (intensity > 0.0)
    {
        mediump vec3 eyeVec = normalize(EyePos - posVarying).xyz;
        mediump vec3 h = normalize(l + eyeVec);
        mediump vec3 specular = Ks * pow(max(0.0, dot( normalVarying, h )), Ns) * Is;
        specularVarying = vec4(clamp(specular, 0.0, 1.0), 1.0);
    }
}
