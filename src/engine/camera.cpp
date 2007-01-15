#include "camera.h"
#include "util.h"

/******************************************************************
 *                         Constructor                            *
 ******************************************************************/
camera::camera()
{
   theta=25;
   phi=0.05;
   d=150;
   centerX = centerZ = 0;
   centerY = 30;
   deltaY = 0;
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   middleMouse = false;
   type = CAMERA_TYPE_NORMAL;
}

/******************************************************************
 *                          Destructor                            *
 ******************************************************************/
camera::~camera()
{
}

/******************************************************************
 *                             doIO                               *
 ******************************************************************/
bool camera::doIO(Uint8 *keys, Uint8 mBotao, int x, int y, GLfloat varCamera)
{
   bool modify = false;
   /* Keys to Camera Moviments */
   if(keys[SDLK_UP])  // Increases Zoom
   {
       if (d>ZOOMMAXIMO)
       {
          d-= varCamera;
          modify = true;
       }
   }
   if(keys[SDLK_DOWN]) // Decreases Zoom
   {
      if(d<ZOOMMINIMO)
      {
          d+= varCamera; 
          modify = true;
      }
   }
   if(keys[SDLK_RIGHT]) // Rotate Camera CounterClockWise
   {
       phi -= varCamera;  
       modify = true;
   }
   if(keys[SDLK_LEFT]) // Rotate Camera ClockWise
   {
      phi += varCamera;
      modify = true;
   }
   if(keys[SDLK_PAGEUP]) // Maximize Up Camera
   {
      theta += varCamera;
      modify = true;
   }
   if(keys[SDLK_PAGEDOWN]) // Minimize Up Camera
   {
      theta -= varCamera;
      modify = true;
   }
   if (keys[SDLK_HOME]) // Maximize zoom
   {
      d = ZOOMMAXIMO;
      modify = true;
   }
   if(keys[SDLK_END]) // Minimize zoom
   {
      d = ZOOMMINIMO;
      modify = true;
   }   
   if(keys[SDLK_INSERT]) //Up view Max
   {
      theta = 89;
      modify = true;
   }
   if(keys[SDLK_DELETE]) //Down view Max
   {
      theta = 0;
      modify = true;
   }

   /* Mouse to move Camera */

   /* Left Edge */
   if(x == 0)  // Turn Clockwise
   {
      phi += 2; 
      modify = true;  
   }
   /* Right Edge */
   if(x == SCREEN_X-1) // Turn CounterClockWise
   {
     phi -= 2; 
     modify = true;
   }

   /* Middle Mouse Button Rotation Control */
   if(mBotao & SDL_BUTTON(2))
   {
      if(middleMouse)
      {
         theta += (y - prevY) / 10.0;
         phi += (prevX - x) / 10.0;  // X axis is inverted :~p
         modify = true;
      }
      else
      {
         middleMouse = true;  
      }
      prevX = x;
      prevY = y;
   }
   else
   {
      middleMouse = false;
   }

   if(modify)
   {
      /* Verify Theta Limits */
      if(theta > 89) 
      {
         theta = 89;
      }
      else if(theta < 0)
      {
         theta = 0;
      }

      /* Put Phi on [0-360) range */
      if(phi < 0)
      {
         phi += 360;
      }
      else if(phi >= 360)
      {
         phi = phi - 360;
      }
   }

   return(modify);
}

/******************************************************************
 *                           lookAt                               *
 ******************************************************************/
void camera::lookAt()
{
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centerX, centerY, centerZ, 0,1,0);
}

/******************************************************************
 *                          getCameraX                            *
 ******************************************************************/
GLfloat camera::getCameraX(){
   return(cameraX);
}

/******************************************************************
 *                          getCameraY                            *
 ******************************************************************/
GLfloat camera::getCameraY()
{
   return(cameraY);
}

/******************************************************************
 *                          getCameraZ                            *
 ******************************************************************/
GLfloat camera::getCameraZ()
{
   return(cameraZ);
}

/******************************************************************
 *                           sumTheta                             *
 ******************************************************************/
void camera::sumTheta(GLfloat f)
{
   theta += f;
   if(theta > 89) 
   {
      theta = 89;
   }
   else if(theta < 0)
   {
      theta = 0;
   }
}

/******************************************************************
 *                            sumPhi                              *
 ******************************************************************/
void camera::sumPhi(GLfloat f)
{
   phi += f;
   if(phi < 0)
   {
      phi += 360;
   }
   else if(phi >= 360)
   {
      phi = phi - 360;
   }
}

/******************************************************************
 *                             sumD                               *
 ******************************************************************/
void camera::sumD(GLfloat f)
{
   d += f;
   if (d < ZOOMMAXIMO) 
   {
      d = ZOOMMAXIMO;
   }
   else if (d > ZOOMMINIMO)
   {
      d = ZOOMMINIMO;
   }
}

/******************************************************************
 *                             getPhi                             *
 ******************************************************************/
GLfloat camera::getPhi()
{
   return(phi);
}

/******************************************************************
 *                            getTheta                            *
 ******************************************************************/
GLfloat camera::getTheta()
{
   return(theta);
}


/******************************************************************
 *                              getD                              *
 ******************************************************************/
GLfloat camera::getD()
{
   return(d);
}


/******************************************************************
 *                           getDeltaY                            *
 ******************************************************************/
GLfloat camera::getDeltaY()
{
   return(deltaY);
}


/******************************************************************
 *                           getCenterX                           *
 ******************************************************************/
GLfloat camera::getCenterX()
{
   return(centerX);
}

/******************************************************************
 *                           getCenterY                           *
 ******************************************************************/
GLfloat camera::getCenterY()
{
   return(centerY);
}

/******************************************************************
 *                           getCenterZ                           *
 ******************************************************************/
GLfloat camera::getCenterZ()
{
   return(centerZ);
}

/******************************************************************
 *                          defineCameraType                      *
 ******************************************************************/
void camera::defineCameraType(int value)
{
   type = value;
}

/******************************************************************
 *                        actualizeCamera                         *
 ******************************************************************/
void camera::actualizeCamera(GLfloat characterX, GLfloat characterY, 
                             GLfloat characterZ, GLfloat characterOrientation)
{
   centerX = characterX;
   centerZ = characterZ;
   //centerY = characterY;
   
   if( (type == CAMERA_TYPE_DRIVE) && (characterOrientation != phi) )
   {
      /* Make the camera follow character rotations */
      GLfloat variation = fabs(characterOrientation - phi);
      GLfloat othVariation = fabs(360-variation);
      if( (variation <= DELTACAMERA) || (othVariation <= DELTACAMERA) )
      {
         phi = characterOrientation;
      }
      else if(phi > characterOrientation)
      {
         if(variation < othVariation)
         {
            phi -= DELTACAMERA;
         }
         else
         {
            phi += DELTACAMERA;
         }
      }
      else
      {
         if(variation < othVariation)
         {
            phi += DELTACAMERA;
         }
         else
         {
            phi -= DELTACAMERA;
         }
      }
   }
}

