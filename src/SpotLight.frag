varying vec4 ambientGlobal, ecPos;
varying vec3, normal;
uniform sampler2D myTexture;

varying vec2 vTexCoord;

#define LIGHT_COUNT 4

void main() {
    vec3 n, halfV;
    float NdotL, NdotHV;
    vec4 color = ambientGlobal * texture2D(myTexture, vTexCoord).bgra;
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

    float fogFactor = exp(-pow((gl_Fog.density * gl_FogFragCoord), 2.0));
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    gl_FragColor = mix(gl_Fog.color, color, fogFactor);
}