/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "modstate.h"


////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               modAction                                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
modAction::modAction(int act, string obj, string mapFile,
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
modAction::~modAction()
{
}

/************************************************************
 *                         getTarget                        *
 ************************************************************/
string modAction::getTarget()
{
   return(target);
}

/************************************************************
 *                      getMapFileName                      *
 ************************************************************/
string modAction::getMapFileName()
{
   return(mapFileName);
}

/************************************************************
 *                         getAction                        *
 ************************************************************/
int modAction::getAction()
{
   return(action);
}

/************************************************************
 *                        getPostion                        *
 ************************************************************/
void modAction::getPosition(GLfloat &posX, GLfloat& posZ)
{
   posX = x;
   posZ = z;
}

/************************************************************
 *                          getNext                         *
 ************************************************************/
modAction* modAction::getNext()
{
   return(next);
}

/************************************************************
 *                        getPrevious                       *
 ************************************************************/
modAction* modAction::getPrevious()
{
   return(previous);
}

/************************************************************
 *                          setNext                         *
 ************************************************************/
void modAction::setNext(modAction* act)
{
   next = act;
}

/************************************************************
 *                        setPrevious                       *
 ************************************************************/
void modAction::setPrevious(modAction* act)
{
   previous = act;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                         mapCharacterModAction                          //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
mapCharacterModAction::mapCharacterModAction(int act, string character, string mapFile,
                                             GLfloat xPos, GLfloat zPos, 
                                             GLfloat orientation,  GLfloat initialX, 
                                             GLfloat initialZ):
                        modAction(act, character, mapFile, xPos, zPos)
{
   oriAngle = orientation;
   initX = initialX;
   initZ = initialZ;
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
mapCharacterModAction::~mapCharacterModAction()
{
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                         mapObjectModAction                             //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
mapObjectModAction::mapObjectModAction(int act, string obj, string mapFile,
                                       GLfloat xPos, GLfloat zPos): 
                    modAction(act, obj, mapFile, xPos, zPos)
{
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
mapObjectModAction::~mapObjectModAction()
{
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               modState                                 //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
modState::modState()
{
   modActionsList = NULL;
   totalModActions = 0;
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
modState::~modState()
{
   clear();
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
   modAction* tmpMod = modActionsList;
   for(i = 0; i < totalModActions; i++)
   {
      tmpMod->getPosition(x,z);
      printf("Map: %s, Action: %d\n\ttarget: %s\n\tx:%.3f z:%.3f\n",
             tmpMod->getMapFileName().c_str(), tmpMod->getAction(),
             tmpMod->getTarget().c_str(), x, z);
      tmpMod = tmpMod->getNext();
   }

   return(true);
}

/************************************************************
 *                         addAction                        *
 ************************************************************/
void modState::addAction(modAction* act)
{
   if(!modActionsList)
   {
      /* Add the only one! */
      modActionsList = act;
      modActionsList->setNext(modActionsList);
      modActionsList->setPrevious(modActionsList);
   }
   else
   {
      /* Add at first */
      act->setNext(modActionsList);
      act->setPrevious(modActionsList->getPrevious());
      modActionsList->setPrevious((modAction*)act);
      act->getPrevious()->setNext((modAction*)act);
   }
   totalModActions++;
}

/************************************************************
 *                    mapObjectAddAction                    *
 ************************************************************/
void modState::mapObjectAddAction(int action, string target, 
                                  string mapFileName, 
                                  GLfloat xPos, GLfloat zPos)
{
   if( (action != MODSTATE_ACTION_OBJECT_REMOVE) && 
       (action != MODSTATE_ACTION_OBJECT_ADD) )
   {
      cerr << "Invalid modification object action: " <<  action << endl;
   }

   if(removeInverseObjectAction(action, target, mapFileName, xPos, zPos))
   {
      /*! Add is similar to remove inverse, so it is added! */
      return;
   }
   else
   {
      mapObjectModAction* n;
      n = new mapObjectModAction(action, target, mapFileName, xPos, zPos);
      addAction(n);
   }
}

/************************************************************
 *                  mapCharacterAddAction                   *
 ************************************************************/
void modState::mapCharacterAddAction(int act, string character, string mapFile,
                                     GLfloat xPos, GLfloat zPos, GLfloat orientation,
                                     GLfloat initialX, GLfloat initialZ)
{
   if( (act != MODSTATE_ACTION_CHARACTER_DEAD) && 
       (act != MODSTATE_ACTION_CHARACTER_MOVE) )
   {
      cerr << "Invalid modification character action: " <<  act << endl;
   }

   mapCharacterModAction* n;
   n = new mapCharacterModAction(act, character, mapFile, xPos, zPos,
                                 orientation, initialX, initialZ);
   addAction(n);
}

/************************************************************
 *                       removeAction                       *
 ************************************************************/
void modState::removeAction(modAction* act)
{
   if(act != NULL)
   {
      if(modActionsList == act)
      {
         modActionsList = act->getNext();
      }
      act->getNext()->setPrevious(act->getPrevious());
      act->getPrevious()->setNext(act->getNext());


      /* Delete the correct struct */
      if( (act->getAction() == MODSTATE_ACTION_OBJECT_REMOVE) ||
          (act->getAction() == MODSTATE_ACTION_OBJECT_ADD))
      {
         /* Object One */
         delete((mapObjectModAction*)act);
      }
      else if( (act->getAction() == MODSTATE_ACTION_CHARACTER_DEAD) ||
               (act->getAction() == MODSTATE_ACTION_CHARACTER_MOVE))
      {
         /* Character One */
         delete((mapCharacterModAction*)act);
      }
      else
      {
         /* Generic One */
         delete(act);
      }


      totalModActions--;
      if(totalModActions <= 0)
      {
         modActionsList = NULL;
      }
   }
}

/************************************************************
 *                   removeInverseAction                    *
 ************************************************************/
bool modState::removeInverseObjectAction(int action, string target, 
                                         string mapFileName, 
                                         GLfloat xPos, GLfloat zPos)
{

   if( (action != MODSTATE_ACTION_OBJECT_REMOVE) && 
       (action != MODSTATE_ACTION_OBJECT_ADD) )
   {
      /* Invalid action: not an object one! */
      return(false);
   }

   int i;
   modAction* tmp = modActionsList;
   for(i = 0; i < totalModActions; i++)
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
            removeAction(tmp);
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
   modAction* tmpMobj = modActionsList;
   for(i = 0; i < totalModActions; i++)
   {
      /* If the information is from the loaded map, apply it! */
      if(tmpMobj->getMapFileName() == actualMap->getFileName())
      {
         tmpMobj->getPosition(x,z);
         if(tmpMobj->getAction() == MODSTATE_ACTION_OBJECT_REMOVE)
         {
            actualMap->removeObject(x, z, tmpMobj->getTarget());
         }
         else if(tmpMobj->getAction() == MODSTATE_ACTION_OBJECT_ADD)
         {
            object* obj = actualMap->getObject(tmpMobj->getTarget());
            if(obj != NULL)
            {
               actualMap->insertObject(x, actualMap->getHeight(x,z), z, 
                                       0, obj, 0);
            }
            else
            {
               cerr << "Warn: Unknow object on map with filename: " << 
                       tmpMobj->getTarget() << endl;
            }
         }
         else if(tmpMobj->getAction() == MODSTATE_ACTION_CHARACTER_DEAD)
         {
            //TODO
         }
         else if(tmpMobj->getAction() == MODSTATE_ACTION_CHARACTER_MOVE)
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

/************************************************************
 *                           clear                          *
 ************************************************************/
void modState::clear()
{
   int i;
   int total = totalModActions;

   /* Free all map objects from list */
   for(i = 0; i < total; i++)
   {
      removeAction(modActionsList);
   }
   modActionsList = NULL;
   totalModActions = 0;
}

