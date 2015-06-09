precision mediump float;

uniform vec4 LightPos;
uniform vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform float Ns;   // specular material exponent (shininess)

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

uniform sampler2D DiffuseMap;
uniform sampler2D EnvironmentMap;


varying lowp vec4 texCoordVarying;

varying vec4 posVarying;       // pos in world space
varying vec3 normalVarying;    // normal in world space
varying vec3 tangentVarying;

lowp vec4 ambientV;
lowp vec4 diffuseV;
lowp vec4 specularV;


// Toon shading
const float levels = 5.0;
const float scaleFactor = 1.0/levels;

float compute_intesity(vec3 n, vec3 l)
{
    float df = max(0.0, dot(l, n));
    return floor(df * levels) * scaleFactor;
}


// rim lighting
const mediump float gamma = 1.0/0.8;
const lowp vec3 rimColor = vec3(0.5);

mediump vec3 compute_rim(vec3 n, vec3 v)
{
    float f = 1.0 - max(dot(n, v), 0.0);
    f = pow(f, 5.0);
    f = smoothstep(0.6, 1.0, f);
    
    return f * rimColor;
}


void main()
{
    // Phong Shading (per-fragment lighting)
    vec4 p = posVarying;
    vec3 n = normalize(normalVarying);
    vec3 l = normalize(LightPos.xyz - p.xyz);
    vec3 eyeVec = normalize(EyePos.xyz - p.xyz);
    vec3 h = normalize(l + eyeVec);
    vec3 t = normalize(tangentVarying);
    
    
//     Environment mapping
    // "stretch" coefficient - to better simulate the sky being infinitely far
    vec3 stretch = vec3(0.0, 0.0, 10.0);
    vec3 e = normalize(p.xyz  - EyePos.xyz - stretch);
    vec3 r = normalize(reflect(e, n));
    r.z += 1.0;
    float m = 2.0 * sqrt(dot(r, r));
    vec2 reflection = -(r.xy/m + vec2(0.5));
    
    lowp vec4 rColor = texture2D(EnvironmentMap, reflection);
    

    // Rim lightning
    lowp vec4 rim = vec4(compute_rim(n, eyeVec), 1.0);
    
    // Ambient component
    ambientV = vec4(Ka * Ia, 1.0);
    
    // Diffuse component    
    float df = compute_intesity(n, l);
    lowp vec3 diffuse = Kd * clamp(df, 0.0, 1.0) * Id;
    diffuseV = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    // Specular component
    float sf = max(0.0, dot(n, h));
    sf = step(0.5, pow(sf, Ns));
    lowp vec4 specularResult = vec4(0.0);
    if (df > 0.0) {
        vec3 specular = Ks * clamp(sf, 0.0, 1.0) * Is;
        specularV = vec4(clamp(specular, 0.0, 1.0), 1.0);
    }


    
    lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.st);
    lowp vec4 phongColor = (rim + ambientV + diffuseV) * color + specularV;
    
    
    phongColor = vec4(phongColor.xyz, 0.85);
    
    lowp vec4 finalColor = vec4(pow(phongColor.r, gamma),
                                pow(phongColor.g, gamma),
                                pow(phongColor.b, gamma),
                                phongColor.a);
    
    
    gl_FragColor = vec4(mix(finalColor, rColor, 0.4).rgb, phongColor.a);
}
