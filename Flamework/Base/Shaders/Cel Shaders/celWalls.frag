precision mediump float;

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ProjectionMatrix;

uniform mat3 NormalMatrix;

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

uniform lowp vec3 OverrideColor;
uniform lowp float isOutlined;

varying lowp vec4 texCoordVarying;

varying vec4 posVarying;       // pos in world space
varying vec3 normalVarying;    // normal in world space
varying vec3 tangentVarying;

lowp vec4 ambientV;
lowp vec4 diffuseV;
lowp vec4 specularV;


// Toon shading
precision mediump float;
const float levels = 7.0;
const float scaleFactor = 1.0/levels;

float compute_intesity(vec3 n, vec3 l)
{
    float df = max(0.0, dot(l, n));
    return floor(df * levels) * scaleFactor;
}


void main()
{
    if (isOutlined > 0.5)
    {
        gl_FragColor = vec4(vec3(0.0), 1.0);
    }
    else
    {
        // Phong Shading (per-fragment lighting)
        vec4 p = posVarying;
        vec3 n = normalize(NormalMatrix * normalVarying);
        vec3 l = normalize(LightPos - p).xyz;
        vec3 t = normalize(tangentVarying);
        vec3 eyeVec = normalize(EyePos.xyz - p.xyz);
        vec3 h = normalize(l + eyeVec);
        
        // Ambient component
        ambientV = vec4(Ka * Ia, 1.0);
        
//        // Diffuse component
        float df = compute_intesity(n, l);
        lowp vec3 diffuse = Kd * clamp(df, 0.0, 1.0) * Id;
        diffuseV = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
        
        
        // Specular component
        float sf = max(0.0, dot(n, h));
        sf = step(0.5, pow(sf, Ns));
        specularV = vec4(0.0);
        if (df > 0.0) {
            vec3 specular = Ks * clamp(sf, 0.0, 1.0) * Is;
            specularV = vec4(clamp(specular, 0.0, 1.0), 1.0);
        }

        
        lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.st);
        
        lowp vec4 phongColor = (ambientV + diffuseV) * color + specularV;
        
        lowp float f = OverrideColor.z;
        if (f != 0.0)
        {
            phongColor = vec4(OverrideColor, 0.1) * phongColor;
            phongColor.a = 0.15;
        }
        
        gl_FragColor = phongColor;
    }
}
