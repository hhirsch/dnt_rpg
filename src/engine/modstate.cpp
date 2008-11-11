/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "modstate.h"
#include "character.h"

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               modAction                                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
modAction::modAction(int act, string obj, string mapFile,
                     GLfloat xPos, GLfloat yPos, GLfloat zPos)
{
   next = NULL;
   previous = NULL;
   action = act;
   target = obj;
   mapFileName = mapFile;
   x = xPos;
   y = yPos;
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
 *                       getPosition                        *
 ************************************************************/
void modAction::getPosition(GLfloat &posX, GLfloat& posY, GLfloat& posZ)
{
   posX = x;
   posY = y;
   posZ = z;
}

/************************************************************
 *                       setPosition                        *
 ************************************************************/
void modAction::setPosition(GLfloat posX, GLfloat posY, GLfloat posZ)
{
   x = posX;
   y = posY;
   z = posZ;
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
mapCharacterModAction::mapCharacterModAction(int act, string character, 
                                             string mapFile,
                                             GLfloat xPos, GLfloat yPos,
                                             GLfloat zPos, 
                                             GLfloat orientation,  
                                             GLfloat initialX, 
                                             GLfloat initialZ):
                        modAction(act, character, mapFile, xPos, yPos, zPos)
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

/************************************************************
 *                      getOrientation                      *
 ************************************************************/
GLfloat mapCharacterModAction::getOrientation()
{
   return(oriAngle);
}

/************************************************************
 *                       getInitialX                        *
 ************************************************************/
GLfloat mapCharacterModAction::getInitialX()
{
   return(initX);
}

/************************************************************
 *                       getInitialZ                        *
 ************************************************************/
GLfloat mapCharacterModAction::getInitialZ()
{
   return(initZ);
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
                                       GLfloat xPos, GLfloat yPos, 
                                       GLfloat zPos): 
                    modAction(act, obj, mapFile, xPos, yPos, zPos)
{
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
mapObjectModAction::~mapObjectModAction()
{
}

/************************************************************
 *                          getValue                        *
 ************************************************************/
int mapObjectModAction::getValue()
{
   return(value);
}

/************************************************************
 *                          setValue                        *
 ************************************************************/
void mapObjectModAction::setValue(int v)
{
   value = v;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                          mapTalkModAction                              //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
mapTalkModAction::mapTalkModAction(int act, string character, string mapFile,
                                   int talkValue): 
                  modAction(act, character, mapFile, 0, 0, 0)
{
   value = talkValue;
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
mapTalkModAction::~mapTalkModAction()
{
}

/************************************************************
 *                          getValue                        *
 ************************************************************/
int mapTalkModAction::getValue()
{
   return(value);
}

/************************************************************
 *                          setValue                        *
 ************************************************************/
void mapTalkModAction::setValue(int v)
{
   value = v;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                            modInventory                                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
modInventory::modInventory(inventory* inv, string owner, string mapFile)
             : modAction(MODSTATE_INVENTORY, owner, mapFile, -1, -1, -1)
{
   /* Default Initial Values */
   objects = NULL;
   totalObjects = 0;

   /* Create the list from the inventory */
   create(inv);
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
modInventory::~modInventory()
{
   /* clear the list */
   clear();
}

/************************************************************
 *                            flush                         *
 ************************************************************/
void modInventory::flush(Map* curMap, inventory* inv,
                         modelList& models,weaponTypes& wTypes)
{
   modInvObj* invObj = objects;
   object* curObj = NULL;
   int i;

   /* Insert all objects at its previous positions */
   for(i = 0; i < totalObjects; i++)
   {
      /* Try get object from map object list */
      curObj = objectsList::search(invObj->fileName, -1, -1, -1);
      if(!curObj)
      {
         /* It's not on the list, so must insert it */
         curObj = curMap->insertObject(invObj->fileName, models, wTypes);
      }

      /* Now insert it at the inventory */
      if(curObj)
      {
         if(!inv->addObject(curObj, invObj->x, invObj->y, invObj->invNumber))
         {
            cerr << "Error: Can't put object '" << invObj->fileName << "' "
                 << "at inventory position '" << invObj->x << "," << invObj->y
                 << " of invNumber: " << invObj->invNumber << endl;
         }
      }
      else
      {
         /* Can't get the object, must show error */
         cerr << "Error: Can't load object " << invObj->fileName << endl;
      }

      /* Let's do the next */
      invObj = invObj->next;
   }
}

/************************************************************
 *                           create                         *
 ************************************************************/
void modInventory::create(inventory* inv)
{
   int x=0, y=0;
   object* obj = NULL;
   int curInv;
   modInvObj* invObj = NULL;

   /* Clear the current one */
   clear();

   /* For all inventories */
   for(curInv = 0; curInv < INVENTORY_PER_CHARACTER; curInv++)
   {
      obj = inv->getFirstObject(x,y, curInv);
      while(obj != NULL)
      {
         /* Create the object representation */
         invObj = new modInvObj();
         invObj->x = x;
         invObj->y = y;
         invObj->invNumber = curInv;
         invObj->fileName = obj->getFileName();

         /* Insert it at the modInventory */
         insert(invObj);
         
         /* Try to get next object */
         obj = inv->getNextObject(x,y, curInv);
      }
   }
}

/************************************************************
 *                           insert                         *
 ************************************************************/
void modInventory::insert(modInvObj* obj)
{
   /* Set next/previous pointers */
   if(objects == NULL)
   {
      obj->next = obj;
      obj->previous = obj;
   }
   else
   {
      obj->next = objects;
      obj->previous = objects->previous;
      obj->next->previous = obj;
      obj->previous->next = obj;
   }

   /* Set it as the initial node on list */
   objects = obj;
   totalObjects++;
}

/************************************************************
 *                            clear                         *
 ************************************************************/
void modInventory::clear()
{
   int i;
   modInvObj* cur = objects;
   modInvObj* aux = NULL;

   /* Free all objects */
   for(i = 0; i < totalObjects; i++)
   {
      aux = cur;
      cur  = cur->next;
      delete(aux);
   }

   totalObjects = 0;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                modMap                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
modMap::modMap(string fileName)
{
   mapFileName = fileName;
   next = NULL;
   previous = NULL;
   modActionsList = NULL;
   totalModActions = 0;
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
modMap::~modMap()
{
   clear();
}

/************************************************************
 *                          getNext                         *
 ************************************************************/
modMap* modMap::getNext()
{
   return(next);
}

/************************************************************
 *                         getPrevious                      *
 ************************************************************/
modMap* modMap::getPrevious()
{
   return(previous);
}

/************************************************************
 *                         setNext                          *
 ************************************************************/
void modMap::setNext(modMap* n)
{
   next = n;
}

/************************************************************
 *                       setPrevious                        *
 ************************************************************/
void modMap::setPrevious(modMap* p)
{
   previous = p;
}
/************************************************************
 *                      getMapFileName                      *
 ************************************************************/
string modMap::getMapFileName()
{
   return(mapFileName);
}

/************************************************************
 *                           clear                          *
 ************************************************************/
void modMap::clear()
{
   int i;
   int total = totalModActions;

   /* Free all map objects from list */
   for(i = 0; i < total; i++)
   {
      if(modActionsList)
      {
         removeAction(modActionsList);
      }
   }
   modActionsList = NULL;
   totalModActions = 0;
}

/************************************************************
 *                         addAction                        *
 ************************************************************/
void modMap::addAction(modAction* act)
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
void modMap::mapObjectAddAction(int action, string target, 
                                  string mapFileName, 
                                  GLfloat xPos, GLfloat yPos, GLfloat zPos,
                                  int value)
{
   mapObjectModAction* n;

   /* Object Add or Object Remove Actions */
   if( (action == MODSTATE_ACTION_OBJECT_REMOVE) ||
       (action == MODSTATE_ACTION_OBJECT_ADD) )
   {
      if(removeInverseObjectAction(action, target, mapFileName, 
                                   xPos, yPos, zPos))
      {
         /*! Add is similar to remove inverse, so it is added! */
         return;
      }
      else
      {
         n = new mapObjectModAction(action, target, mapFileName, 
                                    xPos, yPos, zPos);
         addAction(n);
      }
   }
   /* Change State Action */
   else if(action == MODSTATE_ACTION_OBJECT_CHANGE_STATE)
   {
      /* search for already created one */
      n = (mapObjectModAction*)search(action, target, xPos, yPos, zPos);

      /* not found, so must create a new one */
      if(n == NULL)
      {
         n = new mapObjectModAction(action, target, mapFileName, 
                                    xPos, yPos, zPos);
         addAction(n);
      }

      /* Define the new state :value */
      n->setValue(value);
   }
   /* Invalid Action  */
   else
   {
      cerr << "Invalid modification object action: " <<  action << endl;
   }
}

/************************************************************
 *                  mapCharacterAddAction                   *
 ************************************************************/
void modMap::mapCharacterAddAction(int act, string character, string mapFile,
                                   GLfloat xPos, GLfloat yPos, GLfloat zPos, 
                                   GLfloat orientation, GLfloat initialX, 
                                   GLfloat initialZ)
{
   if( (act != MODSTATE_ACTION_CHARACTER_DEAD) && 
       (act != MODSTATE_ACTION_CHARACTER_MOVE))
   {
      cerr << "Invalid modification character action: " <<  act << endl;
   }

   /* Search for a previous modAction with same info */
   mapCharacterModAction* n=(mapCharacterModAction*)search(act, character,
                                                           initialX, 0, 
                                                           initialZ);
   if(n != NULL)
   {
      /* Just update the one found */
      n->setPosition(xPos, yPos, zPos);
   }
   else
   {
      /* None found, must create a new one */
      n = new mapCharacterModAction(act, character, mapFile, xPos, yPos, zPos,
                                    orientation, initialX, initialZ);
      addAction(n);
   }
}

/************************************************************
 *                      mapTalkAddAction                    *
 ************************************************************/
void modMap::mapTalkAddAction(int act, string character, string mapFile,
                              int value)
{
   if(act == MODSTATE_TALK_ENTER_VALUE)
   {
      /* Search List for a previous value */
      mapTalkModAction* n = (mapTalkModAction*)search(act, character);

      if(n != NULL)
      {
         /* Use the one found */
         n->setValue(value);
      }
      else
      {
         /* Create a new one */
         n = new mapTalkModAction(act, character, mapFile, value);
         addAction(n);
      }
   }
   else
   {
      cerr << "Invalid Talk Action: " << act << endl;
   }
}

/************************************************************
 *                      mapInventoryAdd                     *
 ************************************************************/
void modMap::mapInventoryAdd(inventory* inv, string owner)
{
   /* Search for a previous modInventory here */
   modInventory* modInv = (modInventory*)search(MODSTATE_INVENTORY, owner);

   if(modInv != NULL)
   {
      /* Found, so just update this one */
      modInv->create(inv);
   }
   else
   {
      /* None found, so must create a new one */
      modInv = new modInventory(inv, owner, mapFileName);
      addAction(modInv);
   }
}

/************************************************************
 *                           search                         *
 ************************************************************/
modAction* modMap::search(int action, string target, 
                          GLfloat xPos, GLfloat yPos, GLfloat zPos)
{
   int i;
   modAction* mod = modActionsList;
   for(i = 0; (i < totalModActions); i++)
   {
      if( (mod->getAction() == action) && 
          (mod->getTarget() == target) )
      {
         if((xPos != -1) && (zPos != -1))
         {
            /* verify the initial or current position */
            GLfloat pX=0, pY=0, pZ=0;


            if( (action == MODSTATE_ACTION_CHARACTER_DEAD) ||
                (action == MODSTATE_ACTION_CHARACTER_MOVE) )
            {
               /* For Character Ones, verify the initial position */
               mapCharacterModAction* charAct = (mapCharacterModAction*)mod;
               pX = charAct->getInitialX();
               pZ = charAct->getInitialZ();
            }
            else
            {
               /* For all other types, verify the current position */
               mod->getPosition(pX, pY, pZ);
            }

            /* Verify the position */
            if( (pX == xPos) && (pZ == zPos))
            {
               /* Found it at the desired position */
               return(mod);
            }
         }
         else
         {
            /* No Position Verification, so found it! */
            return(mod);
         }
      }
      mod = mod->getNext();
   }

   /* No ModActions found */
   return(NULL);
}

/************************************************************
 *                       removeAction                       *
 ************************************************************/
void modMap::removeAction(modAction* act)
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
      else if( (act->getAction() == MODSTATE_TALK_ENTER_VALUE) )
      {
         /* Talk One */
         delete((mapTalkModAction*)act);
      }
      else if( (act->getAction() == MODSTATE_INVENTORY) )
      {
         /* Inventory One */
         delete((modInventory*)act);
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
bool modMap::removeInverseObjectAction(int action, string target, 
                                       string mapFileName, 
                                       GLfloat xPos, GLfloat yPos, GLfloat zPos)
{

   if( (action != MODSTATE_ACTION_OBJECT_REMOVE) && 
       (action != MODSTATE_ACTION_OBJECT_ADD) )
   {
      /* Invalid action: not an object one! */
      return(false);
   }

   modAction* tmp = search(!action, target, xPos, yPos, zPos);
   if(tmp != NULL)
   {
      /* Found the Inverse, so remove it */
      removeAction(tmp);
      return(true);
   }
   return(false);
}

/************************************************************
 *                    doMapModifications                    *
 ************************************************************/
void modMap::doMapModifications(Map* actualMap, void* NPCs, 
                                modelList& mdlList, weaponTypes& wTypes)
{
   int i;
   characterList* npcs = (characterList*) NPCs;
   character* ch  = NULL;
   GLfloat x=0, y=0, z=0;
   modAction* tmpMobj = modActionsList;
   for(i = 0; i < totalModActions; i++)
   {
      /* If the information is from the loaded map, apply it! */
      if(tmpMobj->getMapFileName() == actualMap->getFileName())
      {
         tmpMobj->getPosition(x,y,z);
         
         /* Object Remove */
         if(tmpMobj->getAction() == MODSTATE_ACTION_OBJECT_REMOVE)
         {
            /* Get the object from the list */
            object* obj = objectsList::search(tmpMobj->getTarget(), x, y, z);

            if(obj)
            {
               /* Remove it from the map */
               actualMap->removeObject(obj);
               /* And remove it from game! */
               delete(obj);
            }
            else
            {
               cerr << "Error: Unknow object: " << tmpMobj->getTarget() 
                    << " to remove at modState" << endl;
            }
         }

         /* Object Add */
         else if(tmpMobj->getAction() == MODSTATE_ACTION_OBJECT_ADD)
         {
            object* obj = objectsList::search(tmpMobj->getTarget(),
                                              x, actualMap->getHeight(x,z),z);
            if(obj == NULL)
            {
               /* Load it to the map */
               obj = actualMap->insertObject(tmpMobj->getTarget(), mdlList, 
                                             wTypes);
            }
            /* Insert the Object  */
            actualMap->insertObject(x, actualMap->getHeight(x,z), z, 
                                    0, obj, 0);
         }

         /* Object Change State */
         else if(tmpMobj->getAction() == MODSTATE_ACTION_OBJECT_CHANGE_STATE)
         {
            /* Get the object from the list */
            mapObjectModAction* act = (mapObjectModAction*)tmpMobj;
            object* obj = objectsList::search(act->getTarget(), x, y, z);

            if(obj)
            {
               /* Change its state */
               obj->setState(act->getValue());
            }
            else
            {
               cerr << "Error: Unknow object: " << act->getTarget() 
                    << " to change at modState" << endl;
            }
         }

         /* Character Dead */
         else if(tmpMobj->getAction() == MODSTATE_ACTION_CHARACTER_DEAD)
         {
            /* Get The character Pointer */
            mapCharacterModAction* charAct = (mapCharacterModAction*)tmpMobj;
            ch = npcs->getCharacter(charAct->getTarget());
            bool done = false;
            while( (ch != NULL) && (!done) )
            {
               if( (ch->xPosition == charAct->getInitialX()) &&
                   (ch->zPosition == charAct->getInitialZ()) )
               {
                  /* Put it as dead at the position */
                  ch->instantKill();
                  ch->orientation = charAct->getOrientation();
                  charAct->getPosition(ch->xPosition, ch->yPosition, 
                                       ch->zPosition);
                  done = true;
               }
               else
               {
                  /* Not the one, get the next */
                  ch = npcs->getNextSameCharacter(ch);
               }
            }
         }

         /* Character Move */
         else if(tmpMobj->getAction() == MODSTATE_ACTION_CHARACTER_MOVE)
         {
            //TODO
         }

         /* Talk Initial Dialog */
         else if(tmpMobj->getAction() == MODSTATE_TALK_ENTER_VALUE)
         {
            /* Get the character pointer */
            ch = npcs->getCharacter(tmpMobj->getTarget());
            if(ch)
            {
               mapTalkModAction* mTalk = (mapTalkModAction*)tmpMobj;
               ch->setInitialConversation(mTalk->getValue());
            }
         }

         /* Inventory */
         else if(tmpMobj->getAction() == MODSTATE_INVENTORY)
         {
            /* Get the modified inventory */
            modInventory* mInv = (modInventory*)tmpMobj;
            /* Get the character */
            ch = npcs->getCharacter(tmpMobj->getTarget());
            if(ch)
            {
               mInv->flush(actualMap, ch->inventories, mdlList, wTypes);
            }
         }

         /* Unknow */
         else
         {
            printf("Unknow saved action: %d, at %d element!\n", 
                   tmpMobj->getAction(), i);
         }
      }
      
      tmpMobj = tmpMobj->getNext();
   }

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
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
modState::~modState()
{
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
/*   int i;
   GLfloat x=0, z=0;
   modAction* tmpMod = modActionsList;
   for(i = 0; i < totalModActions; i++)
   {
      tmpMod->getPosition(x,z);
      printf("Map: %s, Action: %d\n\ttarget: %s\n\tx:%.3f z:%.3f\n",
             tmpMod->getMapFileName().c_str(), tmpMod->getAction(),
             tmpMod->getTarget().c_str(), x, z);
      tmpMod = tmpMod->getNext();
   }*/

   return(true);
}

/************************************************************
 *                        findModMap                        *
 ************************************************************/
modMap* modState::findModMap(string fileName)
{
  int i;
  modMap* mod = modMapList;
  for(i = 0; i < totalModMaps; i++)
  {
     if(mod->getMapFileName() == fileName)
     {
        /* Found it! */
        return(mod);
     }
     mod = mod->getNext();
  }
  /* None found, must create one */
  return(createModMap(fileName));
}

/************************************************************
 *                       createModMap                       *
 ************************************************************/
modMap* modState::createModMap(string fileName)
{
   modMap* m = new modMap(fileName);

   /* Set next/previous pointers */
   if(modMapList == NULL)
   {
      m->setNext(m);
      m->setPrevious(m);
   }
   else
   {
      m->setNext(modMapList);
      m->setPrevious(modMapList->getPrevious());
      m->getNext()->setPrevious(m);
      m->getPrevious()->setNext(m);
   }

   /* Define as the list head */
   modMapList = m;

   /* And increment elements counter */
   totalModMaps++;

   return(m);
}

/************************************************************
 *                    mapObjectAddAction                    *
 ************************************************************/
void modState::mapObjectAddAction(int action, string target, 
                                  string mapFileName, 
                                  GLfloat xPos, GLfloat yPos, GLfloat zPos,
                                  int value)
{
   modMap* mod = findModMap(mapFileName);

   if(mod != NULL)
   {
      mod->mapObjectAddAction(action, target, mapFileName, 
                              xPos, yPos, zPos, value);
   }
}

/************************************************************
 *                  mapCharacterAddAction                   *
 ************************************************************/
void modState::mapCharacterAddAction(int act, string character, string mapFile,
                                     GLfloat xPos, GLfloat yPos, GLfloat zPos, 
                                     GLfloat orientation,
                                     GLfloat initialX, GLfloat initialZ)
{
   modMap* mod = findModMap(mapFile);
   if(mod != NULL)
   {
      mod->mapCharacterAddAction(act, character, mapFile, xPos, yPos, zPos,
                                 orientation, initialX, initialZ);
   }
}

/************************************************************
 *                      mapTalkAddAction                    *
 ************************************************************/
void modState::mapTalkAddAction(int act, string character, string mapFile,
                                int value)
{
   modMap* mod = findModMap(mapFile);
   if(mod != NULL)
   {
      mod->mapTalkAddAction(act, character, mapFile, value);
   }
}

/************************************************************
 *                       mapInventoryAdd                    *
 ************************************************************/
void modState::mapInventoryAdd(inventory* inv, string owner, string mapFile)
{
   modMap* mod = findModMap(mapFile);
   if(mod != NULL)
   {
      mod->mapInventoryAdd(inv, owner);
   }
}

/************************************************************
 *                    doMapModifications                    *
 ************************************************************/
void modState::doMapModifications(Map* actualMap, 
                                  void* NPCs, 
                                  modelList& mdlList, 
                                  weaponTypes& wTypes)
{
   modMap* mod = findModMap(actualMap->getFileName());
   if(mod != NULL)
   {
      mod->doMapModifications(actualMap, NPCs, mdlList, wTypes);
   } 
}

/************************************************************
 *                           clear                          *
 ************************************************************/
void modState::clear()
{
   int i;
   modMap* m = modMapList;
   modMap* aux = NULL;

   /* Delete each map List */
   for(i = 0; i < totalModMaps; i++)
   {
      aux = m;
      m = m->getNext();
      delete(aux);
   }

   modMapList = NULL;
   totalModMaps = 0;
}

/************************************************************
 *                      static fields                       *
 ************************************************************/
modMap* modState::modMapList = NULL;
int modState::totalModMaps = 0;

