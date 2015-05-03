
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

uniform sampler2D DiffuseMap;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space



void main()
{
    // Phong Shading (per-fragment lighting)
    mediump vec4 p = normalize(posVarying);
    mediump vec3 n = normalize(normalVarying);
    mediump vec3 l = normalize(LightPos - p).xyz;
    lowp vec4 color = texture2D(DiffuseMap , texCoordVarying.st);
    
//    // Ambient component
//    lowp vec4 ambientV = vec4(Ka * Ia, 1.0);
//    
//    
//    // Diffuse component
//    lowp float intensity = dot(n, l);
//    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
//    lowp vec4 diffuseV = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
//    
//    // Specular component
//    lowp vec4 specularV = vec4(0.0);
//    if (intensity > 0.0)
//    {
//        mediump vec3 eyeVec = normalize(EyePos.xyz - p.xyz);
//        mediump vec3 h = normalize(l + eyeVec);
//
//        mediump vec3 specular = Ks * pow(max(0.0, dot( n, h )), Ns) * Is;
//        specularV = vec4(clamp(specular, 0.0, 1.0), 1.0);
//    }
//
//    gl_FragColor = (ambientV + diffuseV) * color + specularV;
    
//     Gouraud Shading: UNCOMMENT IF NEEDED
    gl_FragColor = (ambientVarying + diffuseVarying) * color + specularVarying;
}
