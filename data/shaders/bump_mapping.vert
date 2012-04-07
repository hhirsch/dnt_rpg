
#define MAX_LIGHTS  4

attribute vec3 tangent;
varying vec3 lightVec[MAX_LIGHTS];
varying float fog;

void main()
{
   gl_TexCoord[0] =  gl_MultiTexCoord0;

   vec3 n = normalize (gl_NormalMatrix * gl_Normal);
   vec3 t = normalize (gl_NormalMatrix * tangent);
   vec3 b = cross (n, t);

   vec3 lightDir;
   vec3 v;
   int i;

   for(i=0; i < MAX_LIGHTS; i++)
   {
      /* Only calculate for enabled lights */
      if(gl_LightSource[i].position.w != 2.0)
      {
         if(gl_LightSource[i].position.w != 0.0)
         {
            /* Positional Light */
            vec3 vertexPosition = vec3(gl_ModelViewMatrix *  gl_Vertex);
            lightDir = normalize(gl_LightSource[i].position.xyz - 
                                 vertexPosition);
         }
         else
         {
            /* Directional Light */
            lightDir = normalize(gl_LightSource[i].position.xyz);
         }

         v.x = dot(lightDir, t);
         v.y = dot(lightDir, b);
         v.z = dot(lightDir, n);
         lightVec[i] = normalize(v);
      }
      else if(i != 0)
      {
         /*! Some non trivial (0) light disable, 
          * all after it is disabled too */
         i = MAX_LIGHTS;
      }
   }
  
   gl_Position = ftransform();
   fog = (gl_Fog.end - gl_Position.z)  * gl_Fog.scale;
   fog = clamp(fog, 0.0, 1.0);
}

