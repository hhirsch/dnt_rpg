
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

// New bumpmapping
varying vec3 lightVec;
varying vec3 halfVec;
varying float fog;

void main()
{
   // lookup normal from normal map, move from [0,1] to  [-1, 1] range, normalize
   vec3 normal = 2.0 * texture2D (normalTexture, gl_TexCoord[0].st).rgb - 1.0;
   //vec3 normal = texture2D(normalTexture, gl_TexCoord[0].st).rgb;
   normal = normalize(normal);

   // compute diffuse lighting
   //float lamberFactor= max(dot(lightVec, normal), 0.0) ;
   float lamberFactor= dot(lightVec, normal);
   if(lamberFactor < 0.0)
   {
      lamberFactor = -lamberFactor;
   }

   vec4 diffuseMaterial = vec4(0.0);
   vec4 diffuseLight  = vec4(0.0);

   // compute specular lighting
   vec4 specularMaterial ;
   vec4 specularLight ;
   float shininess ;

   // compute ambient
   vec4 ambientLight = gl_LightSource[0].ambient * gl_FrontMaterial.ambient; 

   gl_FragColor = vec4(0);
   if(lamberFactor > 0.0)
   {
      diffuseMaterial = texture2D(diffuseTexture, gl_TexCoord[0].st);
      diffuseLight = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;

      // In doom3, specular value comes from a texture 
      specularMaterial =  gl_FrontMaterial.specular;
      specularLight = gl_LightSource[0].specular;
      shininess = pow(max (dot (halfVec, normal), 0.0), 4.0);
      shininess *= gl_FrontMaterial.shininess;

      gl_FragColor = diffuseMaterial * diffuseLight * lamberFactor * 1.8;
      gl_FragColor += specularMaterial * specularLight * shininess;
   }

   gl_FragColor += ambientLight * diffuseMaterial * lamberFactor;

   /* Apply Fog */
   gl_FragColor = vec4(mix(vec3(gl_Fog.color), vec3(gl_FragColor), fog), 
         gl_FragColor.a);

   //gl_FragColor = texture2D (diffuseTexture, gl_TexCoord[0].st);

}


