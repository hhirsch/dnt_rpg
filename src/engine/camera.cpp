/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "camera.h"
#include "util.h"
#include "options.h"

#define CAMERA_ATENUATION    0.1  /**< The atenuation value of aceleration */


/******************************************************************
 *                         Constructor                            *
 ******************************************************************/
camera::camera()
{
   theta = 45.0f;
   phi = 75.75f;
   d = 320.0f;
   centerX = 0.0f;
   centerZ = 0.0f;
   centerY = 30.0f;
   deltaY = 0.0f;
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   middleMouse = false;
   type = CAMERA_TYPE_NORMAL;
   zoomAc = 0.0f;
   phiAc = 0.0f;
   thetaAc = 0.0f;
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
   options opt;

   /* Keys to Camera Moviments */
   if(keys[opt.getKey(DNT_KEY_CAMERA_ZOOM_UP)])
   {
      zoomAc = -(2*varCamera);
   }
   if(keys[opt.getKey(DNT_KEY_CAMERA_ZOOM_DOWN)])
   {
      zoomAc = (2*varCamera);
   }
   if(keys[opt.getKey(DNT_KEY_CAMERA_ROTATE_RIGHT)]) 
   {
      phiAc = -varCamera;
   }
   if(keys[opt.getKey(DNT_KEY_CAMERA_ROTATE_LEFT)]) 
   {
      phiAc = varCamera;
   }
   if((keys[opt.getKey(DNT_KEY_CAMERA_ROTATE_UP)]) || (keys[SDLK_KP9]))
   {
      thetaAc = varCamera;
   }
   if((keys[opt.getKey(DNT_KEY_CAMERA_ROTATE_DOWN)]) || (keys[SDLK_KP3]))
   {
      thetaAc = -varCamera;
   }
   if((keys[opt.getKey(DNT_KEY_CAMERA_MAX_ZOOM)]) || (keys[SDLK_KP7]))
   {
      zoomAc = -20;
   }
   if((keys[opt.getKey(DNT_KEY_CAMERA_MIN_ZOOM)]) || (keys[SDLK_KP1]))
   {
      zoomAc = 20;
   }   
   if((keys[opt.getKey(DNT_KEY_CAMERA_TOP)]) || (keys[SDLK_KP0]))
   {
      thetaAc = 10;
   }
   if(keys[opt.getKey(DNT_KEY_CAMERA_BOTTOM)])
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
         theta += (y - prevY) / 10.0f;
         phi += (prevX - x) / 10.0f;  // X axis is inverted :~p
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

   /* Calculate Current Camera Position  */
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));

   /* Verify if it is inner terrain  */
   if(acMap != NULL)
   {
      height = acMap->getHeight(cameraX, cameraZ);

      if(cameraY < (height+10))
      {
         cameraY = height+10;
      }
      
      /* Verify if it is inner object's bounding box  */
      int sqX = (int)floor(cameraX / acMap->squareSize());
      int sqZ = (int)floor(cameraZ / acMap->squareSize());
      Square* ocSquare = acMap->relativeSquare(sqX, sqZ);
      if(ocSquare)
      {
         float sumY = 0;
         int ob = 0;
         boundingBox bounding;
         
         /* Set camera bounding box */
         boundingBox cameraBox;
         cameraBox.setMin(cameraX-4, cameraY-4, cameraZ-4);
         cameraBox.setMax(cameraX+4, cameraY+4, cameraZ+4);
         
         objSquare* sobj = ocSquare->getFirstObject();
         while( (ob < ocSquare->getTotalObjects()))
         {
            if(sobj->colision)
            {
               bounding = sobj->obj->scNode->getBoundingBox();
               if(cameraBox.intercepts(bounding))
               {
                  if( (bounding.y2 + sobj->y+10) > sumY)
                  {
                     sumY = bounding.y2 + sobj->y+10;
                  }
               }
            }
            ob++;
            sobj = (objSquare*)sobj->getNext();
         }

         /* Put the camera at the maximun SumY got, if one  */
         if(sumY != 0)
         {
            cameraY = sumY;
         }
      }

   }

   /* Finnaly, look to the defined position  */
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
 *                               setPhi                           *
 ******************************************************************/
void camera::setPhi(GLfloat f)
{
   phi = f;
}

/******************************************************************
 *                             setTheta                           *
 ******************************************************************/
void camera::setTheta(GLfloat f)
{
   theta = f;
}

/******************************************************************
 *                            setDeltaY                           *
 ******************************************************************/
void camera::setDeltaY(GLfloat f)
{
   deltaY = f;
}

/******************************************************************
 *                              setD                              *
 ******************************************************************/
void camera::setD(GLfloat f)
{
   d = f;
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

