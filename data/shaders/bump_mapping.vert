
attribute vec3 tangent;
varying vec3 lightVec;
varying float fog;
varying vec4 lightSpecular;
varying vec4 lightDiffuse;
varying vec4 lightAmbient;
varying float lightW;


void main()
{
   gl_TexCoord[0] =  gl_MultiTexCoord0;

   vec3 n = normalize (gl_NormalMatrix * gl_Normal);
   vec3 t = normalize (gl_NormalMatrix * tangent);
   vec3 b = cross (n, t);

   vec3 vertexPosition = vec3(gl_ModelViewMatrix *  gl_Vertex);
   vec3 lightDir;
   vec3 v;

   /* Verify each light (indoor is using LIGHT_1) */
   int lightNumber = 0;
   if(gl_LightSource[1].position.w != 2.0)
   {
      lightNumber = 1;
   }

   /* Only calculate for enabled lights */
   if(gl_LightSource[lightNumber].position.w != 2.0)
   {
      if(gl_LightSource[lightNumber].position.w != 0.0)
      {
         /* Positional Light */
         lightDir = normalize(gl_LightSource[lightNumber].position.xyz - 
               vertexPosition);
      }
      else
      {
         /* Directional Light */
         lightDir = normalize(gl_LightSource[lightNumber].position.xyz);
      }

      v.x = dot(lightDir, t);
      v.y = dot(lightDir, b);
      v.z = dot(lightDir, n);
      lightVec = normalize(v);
      lightDiffuse = gl_LightSource[lightNumber].diffuse;
      lightSpecular = gl_LightSource[lightNumber].specular;
      lightAmbient = gl_LightSource[lightNumber].ambient;
      lightW = gl_LightSource[lightNumber].position.w;
   }
   else
   {
      lightW = 2.0;
   }
  
   gl_Position = ftransform();
   fog = (gl_Fog.end - gl_Position.z)  * gl_Fog.scale;
   fog = clamp(fog, 0.0, 1.0);
}

