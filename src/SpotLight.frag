varying vec4 ambientGlobal, ecPos;
varying vec3, normal;
uniform sampler2D myTexture;

varying vec2 vTexCoord;

#define LIGHT_COUNT 4

vec4 mod289(vec4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec4 permute(vec4 x)
{
    return mod289(((x*34.0)+1.0)*x);
}
vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}
vec2 fade(vec2 t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec2 P)
{
    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
    Pi = mod289(Pi); // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;
    vec4 i = permute(permute(ix) + iy);
    vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
    vec4 gy = abs(gx) - 0.5 ;
    vec4 tx = floor(gx + 0.5);
    gx = gx - tx;
    vec2 g00 = vec2(gx.x,gy.x);
    vec2 g10 = vec2(gx.y,gy.y);
    vec2 g01 = vec2(gx.z,gy.z);
    vec2 g11 = vec2(gx.w,gy.w);
    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;
    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));
    vec2 fade_xy = fade(Pf.xy);
    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3 * n_xy;
}

float noise(vec2 co){
    float A = 0.5, B = 0.3, C = 2.0, D = 0.01;
    return cnoise(co)*cos(10.1*cnoise(co)*length(co));
}

void main() {
    vec3 n, halfV;
    float NdotL, NdotHV;
    vec4 color = ambientGlobal * texture2D(myTexture, vTexCoord).rgba + 0.05*noise(vTexCoord.st);
    float att, spotEffect;

    /* a fragment shader can't write a verying variable, hence we need
    a new variable to store the normalized interpolated normal */
    n = normalize(normal);

    for (int i = 0; i < LIGHT_COUNT; i++) {

        ///* Compute the diffuse, ambient and globalAmbient terms */
        vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse;
        vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[i].ambient;

        /* Normalize the halfVector to pass it to the fragment shader */
        vec3 halfVector = gl_LightSource[i].halfVector.xyz;

        // Compute the ligt direction
        vec3 lightDir = vec3(gl_LightSource[i].position - ecPos);

        /* compute the distance to the light source to a varying variable*/
        float dist = length(lightDir);

        /* compute the dot product between normal and ldir */
        NdotL = max(dot(n, normalize(lightDir)), 0.0);

        if (NdotL > 0.0) {

            spotEffect = dot(normalize(gl_LightSource[i].spotDirection), normalize(-lightDir));
            if (spotEffect > 0.999 || i == 2 /*HACK*/) {
                spotEffect = pow(spotEffect, gl_LightSource[i].spotExponent);
                att = spotEffect / (gl_LightSource[i].constantAttenuation +
                        gl_LightSource[i].linearAttenuation * dist +
                        gl_LightSource[i].quadraticAttenuation * dist * dist);

                color += att * (diffuse * NdotL + ambient);


                halfV = normalize(halfVector);
                NdotHV = max(dot(n, halfV), 0.0);
                color += att * gl_FrontMaterial.specular * gl_LightSource[i].specular * pow(NdotHV, gl_FrontMaterial.shininess);
            }
        }
    }

    // Linear fog
    //float fogFactor = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;
    // Exp fog
    //float fogFactor = exp(-gl_Fog.density * gl_FogFragCoord);
    // Exp2 fog
    float fogFactor = exp(-pow((gl_Fog.density * gl_FogFragCoord), 2.0));

    fogFactor = clamp(fogFactor, 0.0, 1.0);
    gl_FragColor = mix(gl_Fog.color, color, fogFactor);
}