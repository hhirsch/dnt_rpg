/* Vertex shader for shadowMap. Based on the one defined at 
 * OpenGL Shading Language Book. */

//attribute float Accesbility;

const float Accesbility = 1.0;

varying vec4 ShadowCoord;

const float As = 1.0 / 1.5;
const float Ds = 1.0 / 3.0;


void main()
{
   vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
   vec3 ecPosition3 = (vec3(ecPosition)) / ecPosition.w;
   vec3 VP = vec3(gl_LightSource[0].position) - ecPosition3;
   VP = normalize(VP);
   vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
   float diffuse = max(0.0, dot(normal, VP));

   //float scale = min(1.0, Accessibility * As + diffuse * Ds);

   vec4 texCoord = gl_TextureMatrix[1] * gl_Vertex;
   ShadowCoord = texCoord / texCoord.w;

   gl_FrontColor = gl_Color;//vec4(scale * gl_Color.rgb, gl_Color.a);
   gl_Position = ftransform();
}


