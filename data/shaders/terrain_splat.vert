//varying vec4 diffuse,ambientGlobal,ambient;
//varying vec4 normal,light,

varying float fog;

void main()
{
   gl_Position = ftransform();

   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_TexCoord[1] = gl_MultiTexCoord1;

   fog = (gl_Fog.end - gl_Position.z)  * gl_Fog.scale;
   fog = clamp(fog, 0.0, 1.0);
}


