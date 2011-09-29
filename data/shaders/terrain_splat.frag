
uniform sampler2D alphaMap; /* R for 0, G for 1, B for 2 and A for 3  */
uniform sampler2D texture0; /* First texture */
uniform sampler2D texture1; /* Second texture */
uniform sampler2D texture2; /* Third texture */
uniform sampler2D texture3; /* Forth texture */

void main()
{
   vec4 alpha = texture2D(alphaMap, gl_TexCoord[0].st);
   vec3 t0 = vec3(texture2D(texture0, gl_TexCoord[1].st));
   vec3 t1 = vec3(texture2D(texture1, gl_TexCoord[1].st));
   vec3 t2 = vec3(texture2D(texture2, gl_TexCoord[1].st));
   vec3 t3 = vec3(texture2D(texture3, gl_TexCoord[1].st));

   t0 *= alpha.r;
   t1 = mix(t0,t1,alpha.g);
   t2 = mix(t1,t2,alpha.b);
   vec3 outColor = mix(t2,t3,alpha.a);

   gl_FragColor = vec4(outColor, 1.0);

   //gl_FragColor = texture2D(alphaMap, gl_TexCoord[0].st);
   //gl_FragColor = texture2D(texture0, gl_TexCoord[1].st);
}


