/* Fragment shader for shadowMap. Based on the one defined at 
 * OpenGL Shading Language Book. */

uniform sampler2DShadow ShadowMap;

const float Epsilon = 1.0;

varying vec4 ShadowCoord;

float lookup(float x, float y)
{
   float depth = shadow2DProj(ShadowMap, 
                              ShadowCoord + vec4(x,y,0,0) * Epsilon).x;
   return( (depth != 1.0)?0.75:1.0 );
}

void main()
{
   float shadeFactor = lookup(0.0, 0.0);
   gl_FragColor = vec4(shadeFactor * gl_Color.rgb, gl_Color.a);
}


