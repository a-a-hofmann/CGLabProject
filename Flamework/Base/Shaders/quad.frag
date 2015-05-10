
uniform highp vec4 LightPos;
uniform highp vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform mediump float Ns;   // specular material exponent (shininess)

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying highp vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space
varying mediump vec3 tangentVarying;


void main()
{
    // Phong Shading (per-fragment lighting)
    mediump vec4 p = normalize(posVarying);
    mediump vec3 n = normalize(normalVarying);
    mediump vec3 l = normalize(LightPos - p).xyz;
    mediump vec3 t = normalize(tangentVarying);
    
    t = normalize(t - n * dot(n, t));
    
    mediump vec3 b = normalize(cross(n, t));
    mediump mat3 tbn = mat3(t, b, n);
    mediump vec3 bumpNormal = texture2D(NormalMap, texCoordVarying.st).rgb;
    bumpNormal = 2.0 * bumpNormal - 1.0;
    
    n = tbn * bumpNormal;
    
    
    // Ambient component
    lowp vec4 ambientV = vec4(Ka * Ia, 1.0);
    
    // Diffuse component
    lowp float intensity = dot(n, l);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    lowp vec4 diffuseV = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    // Specular component
    lowp vec4 specularV = vec4(0.0);
    if (intensity > 0.0)
    {
        mediump vec3 eyeVec = normalize(EyePos - p).xyz;
        mediump vec3 h = normalize(l + eyeVec);
        mediump vec3 specular = Ks * pow(max(0.0, dot( n, h )), Ns) * Is;
        specularV = vec4(clamp(specular, 0.0, 1.0), 1.0);
    }
    
    lowp vec4 color = texture2D(DiffuseMap , texCoordVarying.st);
    
    lowp vec4 gouraudColor = (ambientVarying + diffuseVarying) * color + specularVarying;
    lowp vec4 phongColor = (ambientV + diffuseV) * color + specularV;
    
    gl_FragColor = phongColor;
}
