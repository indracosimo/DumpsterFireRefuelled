#version 330 core
#define MAX_LIGHTS 16

// Global ambient
uniform vec4 global_ambient = vec4(0.2, 0.2, 0.2, 1.0);

// Lighting uniforms
uniform int activeLights = 1;
uniform int light_type[MAX_LIGHTS];
uniform vec3 light_position[MAX_LIGHTS];
uniform vec3 light_direction[MAX_LIGHTS];
uniform int light_enabled[MAX_LIGHTS];
uniform vec4 light_diffuse[MAX_LIGHTS];
uniform vec4 light_specular[MAX_LIGHTS];
uniform vec3 light_attenuation[MAX_LIGHTS];
uniform float light_spotCutoff[MAX_LIGHTS];
uniform int light_spotExponent[MAX_LIGHTS];

// Material
uniform vec4 materialAmbient = vec4(1.0);
uniform vec4 materialDiffuse = vec4(1.0);
uniform vec4 materialSpecular = vec4(1.0);
uniform float materialShininess = 32.0;

// Shadow mapping
uniform sampler2DShadow shadowMap;
uniform mat4 shadowMapMatrix;
uniform vec2 shadowMapTexelSize = vec2(1.0 / 2048.0, 1.0 / 2048.0);
uniform float shadowCalcBias = 0.005;
uniform bool bUseShadowMap = false;

// Textures
uniform sampler2D albedoMap;
uniform sampler2D specularMap;

in vec3 v_normal;
in vec2 UV_Coord;
in vec3 fragPosition;
in vec3 vecToEye;

out vec4 FragColor;

float CalcShadowWithPCF()
{
    if (!bUseShadowMap)
        return 0.0;

    vec4 shadowSpace = shadowMapMatrix * vec4(fragPosition, 1.0);
    shadowSpace.xyz /= shadowSpace.w;

    if (shadowSpace.x < 0.0 || shadowSpace.x > 1.0 ||
        shadowSpace.y < 0.0 || shadowSpace.y > 1.0 ||
        shadowSpace.z < 0.0 || shadowSpace.z > 1.0)
        return 0.0;

    float visibility = 0.0;
    vec2 texelSize = shadowMapTexelSize;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            vec2 uv = shadowSpace.xy + texelSize * vec2(x, y);
            float refDepth = shadowSpace.z - shadowCalcBias;
            visibility += texture(shadowMap, vec3(uv, refDepth));
        }
    }

    visibility /= 9.0;     
    return 1.0 - visibility; 
}

void main()
{
    vec4 albedoTexel = texture(albedoMap, UV_Coord);
    vec3 specularTexel = texture(specularMap, UV_Coord).rgb;

    vec3 normal = normalize(v_normal);
    vec3 viewDir = normalize(vecToEye);

    FragColor = vec4(albedoTexel.rgb * materialAmbient.rgb * global_ambient.rgb, albedoTexel.a);

    for (int i = 0; i < activeLights; ++i)
    {
        if (light_enabled[i] == 0)
            continue;

        vec3 lightDir;
        float attenuation = 1.0;

        if (light_type[i] == 0) // point
        {
            vec3 lightVec = light_position[i] - fragPosition;
            float dist = length(lightVec);
            lightDir = normalize(lightVec);
            attenuation = 1.0 / (light_attenuation[i].x +
                                 light_attenuation[i].y * dist +
                                 light_attenuation[i].z * dist * dist);
        }
        else if (light_type[i] == 1) // directional
        {
            lightDir = normalize(-light_direction[i]);
        }
        else if (light_type[i] == 2) // spot
        {
            vec3 lightVec = light_position[i] - fragPosition;
            float dist = length(lightVec);
            lightDir = normalize(lightVec);
            attenuation = 1.0 / (light_attenuation[i].x +
                                 light_attenuation[i].y * dist +
                                 light_attenuation[i].z * dist * dist);

            vec3 lightToFrag = normalize(fragPosition - light_position[i]);
            vec3 spotForward = normalize(-light_direction[i]); 
            
            float spotDot = dot(spotForward, lightToFrag);
            if (spotDot < light_spotCutoff[i])
                attenuation = 0.0;
            else
                attenuation *= pow(spotDot, float(light_spotExponent[i]));
        }

        if (attenuation <= 0.0)
            continue;

        float diff = max(dot(lightDir, normal), 0.0);
        if (diff <= 0.0)
            continue;

        float shadow = 0.0;
        if (bUseShadowMap && light_type[i] == 1)
            shadow = CalcShadowWithPCF();

        vec3 diffuseLight = (1.0 - shadow) * diff * light_diffuse[i].rgb * attenuation;

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
        vec3 specLight = (1.0 - shadow) * spec * light_specular[i].rgb * materialSpecular.rgb * attenuation;

        FragColor.rgb += albedoTexel.rgb * diffuseLight + specLight * specularTexel;
    }
}