
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

uniform lowp vec3 OverrideColor;
uniform lowp float isOutlined;

varying mediump vec4 posVarying;       // pos in world space
varying mediump vec3 normalVarying;    // normal in world space
varying mediump vec3 tangentVarying;
varying lowp vec4 texCoordVarying;


void main()
{
    
    if (isOutlined > 0.5)
    {
        gl_FragColor = vec4(vec3(0.0), 0.5);
    }
    
    else
    {
        mediump vec4 p = posVarying;
        mediump vec3 n = normalize(normalVarying);
        mediump vec3 l = normalize(LightPos.xyz - p.xyz);
        mediump vec3 t = normalize(tangentVarying);
        
        // Ambient component
        lowp vec4 ambientResult = vec4(Ka * Ia, 1.0);

        // Diffuse component
        lowp float intensity = dot(n, l);
        lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
        lowp vec4 diffuseResult = vec4(clamp(diffuse, 0.0, 1.0), 1.0);

        // Specular component
        lowp vec4 specularResult = vec4(0.0);
        if (intensity > 0.0)
        {
            mediump vec3 eyeVec = normalize(EyePos.xyz - p.xyz);
            mediump vec3 h = normalize(l + eyeVec);
            mediump vec3 specular = Ks * pow(max(0.0, dot(n, h)), Ns) * Is;
            specularResult = vec4(clamp(specular, 0.0, 1.0), 1.0);
        }
        
        lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.st);
        
        lowp vec4 phongColor = (ambientResult + diffuseResult) * color + specularResult;
        
        lowp float f = OverrideColor.z;
        if (f != 0.0)
        {
            phongColor = vec4(OverrideColor, 0.1) * phongColor;
            phongColor.a = 0.5;
        }
        
        gl_FragColor = phongColor;
    }
}
