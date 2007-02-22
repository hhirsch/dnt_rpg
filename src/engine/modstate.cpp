/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "modstate.h"

/************************************************************
 *                       Constructor                        *
 ************************************************************/
mapObjectModAction::mapObjectModAction(int act, string obj, string mapFile,
                                       GLfloat xPos, GLfloat zPos)
{
   next = NULL;
   previous = NULL;
   action = act;
   target = obj;
   mapFileName = mapFile;
   x = xPos;
   z = zPos;
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
mapObjectModAction::~mapObjectModAction()
{
}

/************************************************************
 *                         getTarget                        *
 ************************************************************/
string mapObjectModAction::getTarget()
{
   return(target);
}

/************************************************************
 *                      getMapFileName                      *
 ************************************************************/
string mapObjectModAction::getMapFileName()
{
   return(mapFileName);
}

/************************************************************
 *                         getAction                        *
 ************************************************************/
int mapObjectModAction::getAction()
{
   return(action);
}

/************************************************************
 *                        getPostion                        *
 ************************************************************/
void mapObjectModAction::getPosition(GLfloat &posX, GLfloat& posZ)
{
   posX = x;
   posZ = z;
}

/************************************************************
 *                          getNext                         *
 ************************************************************/
mapObjectModAction* mapObjectModAction::getNext()
{
   return(next);
}

/************************************************************
 *                        getPrevious                       *
 ************************************************************/
mapObjectModAction* mapObjectModAction::getPrevious()
{
   return(previous);
}

/************************************************************
 *                          setNext                         *
 ************************************************************/
void mapObjectModAction::setNext(mapObjectModAction* act)
{
   next = act;
}

/************************************************************
 *                        setPrevious                       *
 ************************************************************/
void mapObjectModAction::setPrevious(mapObjectModAction* act)
{
   previous = act;
}



/************************************************************
 *                       Constructor                        *
 ************************************************************/
modState::modState()
{
   mapObjectsList = NULL;
   totalMapObjects = 0;
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
modState::~modState()
{
   int i;

   /* Free all map objects from list */
   mapObjectModAction* tmpMobj;
   for(i = 0; i < totalMapObjects; i++)
   {
      tmpMobj = mapObjectsList;
      mapObjectsList = mapObjectsList->getNext();
      delete(tmpMobj);
   }
   mapObjectsList = NULL;
   totalMapObjects = 0;
}

/************************************************************
 *                         loadState                        *
 ************************************************************/
bool modState::loadState(string file)
{
   //TODO
   printf("Not done yet!\n");
   return(true);
}

/************************************************************
 *                         saveState                        *
 ************************************************************/
bool modState::saveState(string file)
{
   //TODO. For now is only printing on screen for debug!
   int i;
   GLfloat x=0, z=0;
   mapObjectModAction* tmpMobj = mapObjectsList;
   for(i = 0; i < totalMapObjects; i++)
   {
      tmpMobj->getPosition(x,z);
      printf("Map: %s, Action: %d\n\tobject: %s\n\tx:%.3f z:%.3f\n",
             tmpMobj->getMapFileName().c_str(), tmpMobj->getAction(),
             tmpMobj->getTarget().c_str(), x, z);
      tmpMobj = tmpMobj->getNext();
   }

   return(true);
}

/************************************************************
 *                    mapObjectAddAction                    *
 ************************************************************/
void modState::mapObjectAddAction(int action, string target, 
                                  string mapFileName, 
                                  GLfloat xPos, GLfloat zPos)
{
   if(removeInverseObjectAction(action, target, mapFileName, xPos, zPos))
   {
      /*! Add is similar to remove inverse, so it is added! */
      return;
   }
   else
   {
      if(!mapObjectsList)
      {
         /* Add the only one! */
         mapObjectsList = new mapObjectModAction(action, target, mapFileName,
                                                 xPos, zPos);
         mapObjectsList->setNext(mapObjectsList);
         mapObjectsList->setPrevious(mapObjectsList);
      }
      else
      {
         /* Add at first */
         mapObjectModAction* n;
         n = new mapObjectModAction(action, target, mapFileName, xPos, zPos);
         n->setNext(mapObjectsList);
         n->setPrevious(mapObjectsList->getPrevious());
         mapObjectsList->setPrevious(n);
         n->getPrevious()->setNext(n);
      }
      totalMapObjects++;
   }
}

/************************************************************
 *                    mapObjectAddAction                    *
 ************************************************************/
bool modState::removeInverseObjectAction(int action, string target, 
                                         string mapFileName, 
                                         GLfloat xPos, GLfloat zPos)
{
   int i;
   mapObjectModAction* tmp = mapObjectsList;
   for(i = 0; i < totalMapObjects; i++)
   {
      if( (tmp->getMapFileName() == mapFileName) &&
          (tmp->getAction() == !action) &&
          (tmp->getTarget() == target) )
      {
         /* verify the position of the objects! */
         GLfloat pX=0, pZ=0;
         tmp->getPosition(pX, pZ);
         if( (pX == xPos) && (pZ == zPos))
         {
            /* Found the Inverse, so remove it */
            if(mapObjectsList == tmp)
            {
               mapObjectsList = tmp->getNext();
            }
            tmp->getNext()->setPrevious(tmp->getPrevious());
            tmp->getPrevious()->setNext(tmp->getNext());
            delete(tmp);
            totalMapObjects--;
            if(totalMapObjects <= 0)
            {
               mapObjectsList = NULL;
            }
            return(true);
         }
      }
      tmp = tmp->getNext();
   }
   return(false);
}

/************************************************************
 *                    doMapModifications                    *
 ************************************************************/
void modState::doMapModifications(Map* actualMap)
{
   int i;
   GLfloat x=0, z=0;
   mapObjectModAction* tmpMobj = mapObjectsList;
   for(i = 0; i < totalMapObjects; i++)
   {
      /* If the information is from the loaded map, apply it! */
      if(tmpMobj->getMapFileName() == actualMap->name)
      {
         tmpMobj->getPosition(x,z);
         if(tmpMobj->getAction() == MODSTATE_ACTION_MAP_REMOVE)
         {
            actualMap->removeObject(x, z, tmpMobj->getTarget());
         }
         else if(tmpMobj->getAction() == MODSTATE_ACTION_MAP_ADD)
         {
            //TODO
         }
         else
         {
            printf("Unknow saved action: %d, at %d element!\n", 
                   tmpMobj->getAction(), i);
         }
      }
      
      tmpMobj = tmpMobj->getNext();
   }

}


