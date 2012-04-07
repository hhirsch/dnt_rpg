
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

varying vec3 lightVec;
varying float fog;

void main()
{
   /* Get normal from normal map */
   vec3 normal = 2.0 * texture2D (normalTexture, gl_TexCoord[0].st).rgb - 1.0;
   normal = normalize(normal);

   /* Calculate lambert factor */
   float lamberFactor= max(dot(normal, lightVec), 0.0) ;
   /*float lamberFactor= dot(lightVec, normal);
   if(lamberFactor < 0.0)
   {
      lamberFactor = -lamberFactor;
   }*/

   /* Apply Texture and single light */
   vec4 colorTexture = texture2D(diffuseTexture, gl_TexCoord[0].st);
   float shininess = lamberFactor * gl_FrontMaterial.shininess;

   /* Diffuse light */
   gl_FragColor = colorTexture * gl_LightSource[0].diffuse * 
                  gl_FrontMaterial.diffuse * lamberFactor * 1.6;

   /* Specular Light */
   gl_FragColor += gl_LightSource[0].specular * 
                   gl_FrontMaterial.specular * shininess;

   /* Ambient Light */
   gl_FragColor += gl_LightSource[0].ambient * /*gl_FrontMaterial.ambient * */
                   colorTexture;

   /* Apply Fog */
   gl_FragColor = vec4(mix(vec3(gl_Fog.color), vec3(gl_FragColor), fog), 
         gl_FragColor.a);

}


