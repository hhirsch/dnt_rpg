#include "camera.h"
#include "util.h"

#define CAMERA_ATENUATION    0.1  /**< The atenuation value of aceleration */


/******************************************************************
 *                         Constructor                            *
 ******************************************************************/
camera::camera()
{
   theta=45;
   phi=75.75;
   d=320;
   centerX = centerZ = 0;
   centerY = 30;
   deltaY = 0;
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   middleMouse = false;
   type = CAMERA_TYPE_NORMAL;
   zoomAc = 0.0;
   phiAc = 0.0;
   thetaAc = 0.0;
   minZoom = ZOOM_MIN;
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
   /* Keys to Camera Moviments */
   if(keys[SDLK_UP])  // Increases Zoom
   {
      zoomAc = -(2*varCamera);
   }
   if(keys[SDLK_DOWN]) // Decreases Zoom
   {
      zoomAc = (2*varCamera);
   }
   if(keys[SDLK_RIGHT]) // Rotate Camera CounterClockWise
   {
      phiAc = -varCamera;
   }
   if(keys[SDLK_LEFT]) // Rotate Camera ClockWise
   {
      phiAc = varCamera;
   }
   if((keys[SDLK_PAGEUP]) || (keys[SDLK_KP9])) // Maximize Up Camera
   {
      thetaAc = varCamera;
   }
   if((keys[SDLK_PAGEDOWN]) || (keys[SDLK_KP3])) // Minimize Up Camera
   {
      thetaAc = -varCamera;
   }
   if((keys[SDLK_HOME]) || (keys[SDLK_KP7])) // Maximize zoom
   {
      zoomAc = -20;
   }
   if((keys[SDLK_END]) || (keys[SDLK_KP1])) // Minimize zoom
   {
      zoomAc = 20;
   }   
   if((keys[SDLK_INSERT]) || (keys[SDLK_KP0])) //Up view Max
   {
      thetaAc = 10;
   }
   if(keys[SDLK_DELETE]) //Down view Max
   {
      thetaAc = -10;
   }

   /* Mouse to move Camera */

   /* Left Edge */
   if(x <= 5)  // Turn Clockwise
   {
      phiAc = varCamera; 
   }
   /* Right Edge */
   if(x >= SCREEN_X-5) // Turn CounterClockWise
   {
      phiAc = -varCamera; 
   }

   /* Middle Mouse Button Rotation Control */
   if(mBotao & SDL_BUTTON(2))
   {
      if(middleMouse)
      {
         theta += (y - prevY) / 10.0;
         phi += (prevX - x) / 10.0;  // X axis is inverted :~p
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

   /* Apply the Zoom accelerations */
   if(zoomAc < 0)
   {
      d += zoomAc;
      zoomAc += CAMERA_ATENUATION;
      if(zoomAc > 0)
      {
         zoomAc = 0;
      }
   }
   else if(zoomAc > 0)
   {
      d += zoomAc;
      zoomAc -= CAMERA_ATENUATION;
      if(zoomAc < 0)
      {
         zoomAc = 0;
      }
   }

   /* Apply the Phi accelerations */
   if(phiAc < 0)
   {
      phi += phiAc;
      phiAc += CAMERA_ATENUATION;
      if(phiAc > 0)
      {
         phiAc = 0;
      }
   }
   else if(phiAc > 0)
   {
      phi += phiAc;
      phiAc -= CAMERA_ATENUATION;
      if(phiAc < 0)
      {
         phiAc = 0;
      }
   }

   /* Apply the theta accelerations */
   if(thetaAc < 0)
   {
      theta += thetaAc;
      thetaAc += CAMERA_ATENUATION;
      if(thetaAc > 0)
      {
         thetaAc = 0;
      }
   }
   else if(thetaAc > 0)
   {
      theta += thetaAc;
      thetaAc -= CAMERA_ATENUATION;
      if(thetaAc < 0)
      {
         thetaAc = 0;
      }
   }


   /* Verify Limits */

   /* Verify Theta Limits */
   if(theta > 89) 
   {
      theta = 89;
   }
   else if(theta < 0)
   {
      theta = 0;
   }

   /* Verify D limits */
   if(d > minZoom)
   {
      d = minZoom;
   }
   else if(d < ZOOM_MAX)
   {
      d = ZOOM_MAX;
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

   return(true);
}

/******************************************************************
 *                           lookAt                               *
 ******************************************************************/
void camera::lookAt(Map* acMap)
{
   GLfloat height = 0;

   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));

   if(acMap != NULL)
   {
      height = acMap->getHeight(cameraX, cameraZ);

      if(cameraY < (height+10))
      {
         cameraY = height+10;
      }
   }

   gluLookAt(cameraX,cameraY,cameraZ, centerX, centerY, centerZ, 0,1,0);
}

/******************************************************************
 *                          getCameraX                            *
 ******************************************************************/
GLfloat camera::getCameraX()
{
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
   if (d < ZOOM_MAX) 
   {
      d = ZOOM_MAX;
   }
   else if (d > ZOOM_MIN)
   {
      d = ZOOM_MIN;
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
 *                           defineMinZoom                        *
 ******************************************************************/
void camera::defineMinZoom(GLfloat value)
{
   minZoom = value;
}

/******************************************************************
 *                          updateCamera                          *
 ******************************************************************/
void camera::updateCamera(GLfloat characterX, GLfloat characterY, 
                          GLfloat characterZ, GLfloat characterOrientation)
{
   centerX = characterX;
   centerZ = characterZ;
   centerY = characterY + 30;

   if( (type == CAMERA_TYPE_DRIVE) && (characterOrientation != phi) )
   {
      /* Make the camera follow character rotations */
      GLfloat variation = fabs(characterOrientation - phi);
      GLfloat othVariation = fabs(360-variation);
      if( (variation <= DELTA_CAMERA) || (othVariation <= DELTA_CAMERA) )
      {
         phi = characterOrientation;
      }
      else if(phi > characterOrientation)
      {
         if(variation < othVariation)
         {
            phi -= DELTA_CAMERA;
         }
         else
         {
            phi += DELTA_CAMERA;
         }
      }
      else
      {
         if(variation < othVariation)
         {
            phi += DELTA_CAMERA;
         }
         else
         {
            phi -= DELTA_CAMERA;
         }
      }
   }
}

