varying vec4 ambientGlobal, ecPos;
varying vec3 normal;
varying vec2 vTexCoord;
 
void main()
{
    vTexCoord = gl_MultiTexCoord0.xy;

    vec3 aux;
     
    /* first transform the normal into eye space and normalize the result */
    normal = normalize(gl_NormalMatrix * gl_Normal);
 
    /* compute the vertex position  in camera space. */
    ecPos = gl_ModelViewMatrix * gl_Vertex;

    /* Compute the globalAmbient terms */
    ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;


    gl_Position = ftransform();
} 