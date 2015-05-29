
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
uniform sampler2D EnvironmentMap;


varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space
varying mediump vec3 tangentVarying;

lowp vec4 ambientV;
lowp vec4 diffuseV;
lowp vec4 specularV;


void main()
{
    // Phong Shading (per-fragment lighting)
    mediump vec4 p = posVarying;
    mediump vec3 n = normalize(normalVarying);
    mediump vec3 l = normalize(LightPos.xyz - p.xyz);
    mediump vec3 eyeVec = normalize(EyePos.xyz - p.xyz);
    mediump vec3 t = normalize(tangentVarying);
    
    
    // Environment mapping
    // "stretch" coefficient - to better simulate the sky being infinitely far
    mediump vec3 stretch = vec3(0.0, 0.0, 8.0);
    mediump vec3 e = normalize(p.xyz - stretch);
    mediump vec3 r = normalize(reflect(e, n));
    r.z += 1.0;
    mediump float m = 2.0 * sqrt(dot(r, r));
    mediump vec2 reflection = r.xy/m + vec2(0.5);
    
    lowp vec4 rColor = texture2D(EnvironmentMap, -reflection);
    

    
    // Ambient component
    ambientV = vec4(Ka * Ia, 1.0);
    
    // Diffuse component
    lowp float intensity = dot(n, l);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    diffuseV = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    // Specular component
    specularV = vec4(0.0);
    if (intensity > 0.0)
    {
        mediump vec3 h = normalize(l + eyeVec);
        mediump vec3 specular = Ks * pow(max(0.0, dot( n, h )), Ns) * Is;
        specularV = vec4(clamp(specular, 0.0, 1.0), 1.0);
    }

    
    lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.st);
    
    lowp vec4 phongColor = (ambientV + diffuseV) * color + specularV;
    phongColor = vec4(phongColor.x, phongColor.y, phongColor.z, 0.45);
    
    gl_FragColor = vec4(mix(phongColor, rColor, 0.4).rgb, 1.0);
}
