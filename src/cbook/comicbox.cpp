
#include "comicbox.h"

#include "../engine/util.h"

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
comicBox::comicBox(string t)
{
   int i;

   /* Set some default values */
   next = NULL;
   previous = NULL;
   scaleFactorX = 1.0;
   scaleFactorY = 1.0;
   status = COMIC_BOX_STATUS_INACTIVE;
   effectType = COMIC_BOX_EFFECT_NONE;
   timeout = COMIC_BOX_DEFAULT_TIMEOUT;
 
   title = t;

   for(i = 0; i < 4; i++)
   {
      vertex[i][0] = 0.0;
      vertex[i][1] = 0.0;
   }

   pos[0] = 0;
   pos[1] = 0;

   scale[0] = 1.0;
   scale[1] = 1.0;

   angle[0] = 0.0;
   angle[1] = 0.0;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
comicBox::~comicBox()
{
}

/***********************************************************************
 *                                setVertex                            *
 ***********************************************************************/
void comicBox::setVertex(int vertNumber, GLfloat x, GLfloat y,
                         int imgX, int imgY)
{
   if( (vertNumber >= 0) && (vertNumber <= 3) )
   {
      texCoord[vertNumber][0] = x / (float)smallestPowerOfTwo(imgX);
      texCoord[vertNumber][1] = y / (float)smallestPowerOfTwo(imgY);
      vertex[vertNumber][0] = x * (SCREEN_Y / (float)imgY);
      vertex[vertNumber][1] = SCREEN_Y - (y * (SCREEN_Y / (float)imgY));
   }
}

/***********************************************************************
 *                                setNext                              *
 ***********************************************************************/
void comicBox::setNext(comicBox* box)
{
   next = box;
}
 
/***********************************************************************
 *                              setPrevious                            *
 ***********************************************************************/
void comicBox::setPrevious(comicBox* box)
{
   previous = box;
}

/***********************************************************************
 *                                getNext                              *
 ***********************************************************************/
comicBox* comicBox::getNext()
{
   return(next);
}

/***********************************************************************
 *                              getPrevious                            *
 ***********************************************************************/
comicBox* comicBox::getPrevious()
{
   return(previous);
}

/***********************************************************************
 *                               getTitle                              *
 ***********************************************************************/
string comicBox::getTitle()
{
   return(title);
}

/***********************************************************************
 *                                setEffect                            *
 ***********************************************************************/
void comicBox::setEffect(int type)
{
   effectType = type;
}

/***********************************************************************
 *                             setTimeout                              *
 ***********************************************************************/
void comicBox::setTimeout(int value)
{
   timeout = value;
}

/***********************************************************************
 *                               getStatus                             *
 ***********************************************************************/
int comicBox::getStatus()
{
   return(status);
}

/***********************************************************************
 *                                activate                             *
 ***********************************************************************/
void comicBox::activate()
{
   int i;

   /* Set the current status to running */
   status = COMIC_BOX_STATUS_RUNNING;
   timer = SDL_GetTicks();

   /* Calculate Center Positions */
   center[0] = (vertex[3][0] - vertex[0][0]) / 2.0;
   center[1] = (vertex[2][1] - vertex[0][1]) / 2.0;

   /* Apply Center Positions */
   for(i = 0; i < 4; i++)
   {
      vertex[i][0] -= center[0];
      vertex[i][1] -= center[1];
   }

   /* Set the initial Position, scales and angles, based on effect type */
   switch(effectType)
   {
      /* No Effect */
      case COMIC_BOX_EFFECT_NONE:
      default:
         pos[0] = 0;
         pos[1] = 0;
         angle[0] = 0;
         angle[1] = 0;
         scale[0] = 1.0;
         scale[1] = 1.0;
      break;
   }
}

/***********************************************************************
 *                                 update                              *
 ***********************************************************************/
void comicBox::update()
{
   Uint32 now = SDL_GetTicks();

   switch(effectType)
   {
      /* No Effect */
      case COMIC_BOX_EFFECT_NONE:
      default:
         if(now - timer > timeout)
         {
            status = COMIC_BOX_STATUS_DONE;
         }
      break;
   }
}

/***********************************************************************
 *                                 render                              *
 ***********************************************************************/
void comicBox::render()
{
   /* First update to the new position */
   if(status == COMIC_BOX_STATUS_RUNNING)
   {
      update();
   }

   /* Now Render it at the desired one */
   glPushMatrix();
      glScalef(scale[0],scale[1],1.0);
      glTranslatef(pos[0],pos[1],0.0);
      glTranslatef(center[0], center[1], 0.0);
      glRotatef(angle[0],1,0,0);
      glRotatef(angle[1],0,1,0);
      glBegin(GL_QUADS);
         glTexCoord2fv(texCoord[0]);
         glVertex2fv(vertex[0]);

         glTexCoord2fv(texCoord[1]);
         glVertex2fv(vertex[1]);

         glTexCoord2fv(texCoord[2]);
         glVertex2fv(vertex[2]);

         glTexCoord2fv(texCoord[3]);
         glVertex2fv(vertex[3]);
      glEnd();
   glPopMatrix();
}

