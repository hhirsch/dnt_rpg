
#include "comicbox.h"

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
comicBox::comicBox()
{
   int i;

   /* Set some default values */
   next = NULL;
   previous = NULL;
   scaleFactorX = 1.0;
   scaleFactorY = 1.0;
   status = COMIC_BOX_STATUS_INACTIVE;
   effectType = COMIC_BOX_EFFECT_NONE;

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
void comicBox::setVertex(int vertNumber, GLfloat x, GLfloat y)
{
   if( (vertNumber >= 0) && (vertNumber <= 3) )
   {
      vertex[vertNumber][0] = x;
      vertex[vertNumber][1] = y;
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
 *                                setEffect                            *
 ***********************************************************************/
void comicBox::setEffect(int type)
{
   effectType = type;
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
   /* Set the current status to running */
   status = COMIC_BOX_STATUS_RUNNING;

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
   switch(effectType)
   {
      /* No Effect */
      case COMIC_BOX_EFFECT_NONE:
      default:
         status = COMIC_BOX_STATUS_DONE;
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
   //TODO
}

