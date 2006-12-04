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
      if(theta > 89) 
         theta = 89;
      else if(theta < 0)
         theta = 0;
      
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


GLfloat camera::getCameraX(){
   return(cameraX);
}

GLfloat camera::getCameraY()
{
   return(cameraY);
}

GLfloat camera::getCameraZ()
{
   return(cameraZ);
}

void camera::sumTheta(GLfloat f)
{
   theta += f;
   if(theta > 89) 
      theta = 89;
   else if(theta < 0)
      theta = 0;
}

void camera::sumPhi(GLfloat f)
{
   phi += f;
}

void camera::sumD(GLfloat f)
{
   d += f;
   if (d<1) d = 1;
   else if (d>300) d = 300;
}

GLfloat camera::getPhi()
{
   return(phi);
}

GLfloat camera::getTheta()
{
   return(theta);
}

GLfloat camera::getD()
{
   return(d);
}

GLfloat camera::getDeltaY()
{
   return(deltaY);
}

