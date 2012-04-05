
attribute vec3 tangent;
varying vec3 lightVec;
varying vec3 halfVec;
varying float fog;

void main()
{
   gl_TexCoord[0] =  gl_MultiTexCoord0;

   gl_Position = ftransform();

   vec3 n = normalize (gl_NormalMatrix * gl_Normal);
   vec3 t = normalize (gl_NormalMatrix * tangent);
   vec3 b = cross (n, t);

   //vec3 vertexPosition = vec3(gl_ModelViewMatrix *  gl_Vertex);
   vec3 lightDir = normalize(gl_LightSource[0].position.xyz - gl_Position.xyz);

   vec3 v;
   v.x = dot (lightDir, t);
   v.y = dot (lightDir, b);
   v.z = dot (lightDir, n);
   lightVec = normalize (v);

   //vertexPosition = normalize(vertexPosition);

   vec3 halfVector = normalize(vec3(gl_Position) + lightDir);
   v.x = dot (halfVector, t);
   v.y = dot (halfVector, b);
   v.z = dot (halfVector, n);
   halfVec = v ;

   fog = (gl_Fog.end - gl_Position.z)  * gl_Fog.scale;
   fog = clamp(fog, 0.0, 1.0);
}

