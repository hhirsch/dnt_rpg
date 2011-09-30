//varying vec4 diffuse,ambientGlobal,ambient;
//varying vec4 normal,light,

void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_TexCoord[1] = gl_MultiTexCoord1;

   gl_FogFragCoord = gl_FogCoord;

   gl_Position = ftransform();
}


