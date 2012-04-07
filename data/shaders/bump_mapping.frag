
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

#define MAX_LIGHTS  4

varying vec3 lightVec[MAX_LIGHTS];
varying float fog;

/*! Calculate lamber factor for bidirectional light */
float calcBidirectionalLamber(vec3 v, vec3 normal)
{
   float factor = dot(normal, v);
   if(factor < 0.0)
   {
      factor = -factor;
   }
   return factor;
}

void main()
{
   vec4 colorTexture = texture2D(diffuseTexture, gl_TexCoord[0].st);

   /* Get normal from normal map */
   vec3 normal = 2.0 * texture2D (normalTexture, gl_TexCoord[0].st).rgb - 1.0;
   normal = normalize(normal);

   gl_FragColor = vec4(0.0,0.0,0.0,0.0);
   int i;
   for(i=0; i < MAX_LIGHTS; i++)
   {
      /* Only calculate for enabled lights */
      if(gl_LightSource[i].position.w != 2.0)
      {
         /* Calculate lambert factor */
         float lamberFactor;

         if(gl_LightSource[i].position.w != 0.0)
         {
            /* Positional Light */
            lamberFactor = max(dot(normal, lightVec[i]), 0.0);
         }
         else
         {
            /* Directional Light (sun), must illuminate all sides. */
            lamberFactor = calcBidirectionalLamber(normal, lightVec[i]);
            lamberFactor += 0.5*calcBidirectionalLamber(normal, 
                  vec3(lightVec[i].z, lightVec[i].y, lightVec[i].x));
            lamberFactor /= 1.5;
         }

         /* Apply Texture and single light */
         float shininess = lamberFactor * gl_FrontMaterial.shininess;

         /* Diffuse light */
         gl_FragColor += colorTexture * gl_LightSource[i].diffuse * 
            gl_FrontMaterial.diffuse * lamberFactor * 1.6;

         /* Specular Light */
         gl_FragColor += gl_LightSource[i].specular * 
            gl_FrontMaterial.specular * shininess;

         /* Ambient Light */
         gl_FragColor += gl_LightSource[i].ambient * colorTexture * 
            gl_FrontMaterial.ambient;
      }
      else if(i != 0)
      {
         /* Done with lights */
         i = MAX_LIGHTS;
      }
   }

   /* Apply Fog */
   gl_FragColor = vec4(mix(vec3(gl_Fog.color), vec3(gl_FragColor), fog), 
         gl_FragColor.a);

}


