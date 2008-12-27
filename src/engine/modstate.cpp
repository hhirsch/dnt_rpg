/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "modstate.h"
#include "character.h"
#include "../etc/npcfile.h"


#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
#endif



#define MODSTATE_TOKEN_MAP                  "map"
#define MODSTATE_TOKEN_MAP_END              "mapEnd"
#define MODSTATE_TOKEN_VERSION              "version"
#define MODSTATE_TOKEN_CHARACTER_MOD_ACTION "characterModAction"
#define MODSTATE_TOKEN_OBJECT_MOD_ACTION    "objectModAction"
#define MODSTATE_TOKEN_TALK_MOD_ACTION      "talkModAction"
#define MODSTATE_TOKEN_MOD_INVENTORY        "modInventory"
#define MODSTATE_TOKEN_INVENTORY_ITEM       "inventoryItem"

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
 *                       Constructor                        *
 ************************************************************/
mapCharacterModAction::mapCharacterModAction(string s)
                      :modAction(0,"","",0,0,0)
{
   fromString(s);
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

/************************************************************
 *                         toString                         *
 ************************************************************/
string mapCharacterModAction::toString()
{
   char buf[64];

   /* Definition */
   string res = MODSTATE_TOKEN_CHARACTER_MOD_ACTION;
   res += " = ";

   /* Action Type */
   sprintf(buf, " %d ", action);
   res += buf; 

   /* Target */
   res += target;

   /* Position */
   sprintf(buf, " %.3f %.3f %.3f ", x,y,z);
   res += buf;

   /* Orientation and Initial Position  */
   sprintf(buf, " %.3f  %.3f %.3f ", oriAngle, initX, initZ);
   res += buf;

   /* Done */
   return(res);
}

/************************************************************
 *                       fromString                         *
 ************************************************************/
void mapCharacterModAction::fromString(string s)
{
   char buf[256];

   sscanf(s.c_str(),"%d %s %f %f %f %f %f %f", &action, &buf[0],
                    &x, &y, &z, &oriAngle, &initX, &initZ);
   target = buf;
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
 *                       Constructor                        *
 ************************************************************/
mapObjectModAction::mapObjectModAction(string s)
                   :modAction(0, "", "", 0,0,0)
{
   fromString(s);
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

/************************************************************
 *                         toString                         *
 ************************************************************/
string mapObjectModAction::toString()
{
   char buf[64];

   /* Definition */
   string res = MODSTATE_TOKEN_OBJECT_MOD_ACTION;
   res += " = ";

   /* Action Type */
   sprintf(buf, " %d ", action);
   res += buf; 

   /* Target */
   res += target;

   /* Position */
   sprintf(buf, " %.3f %.3f %.3f ", x,y,z);
   res += buf;

   /* Value  */
   sprintf(buf, " %d ", value);
   res += buf;

   /* Done */
   return(res);
}

/************************************************************
 *                       fromString                         *
 ************************************************************/
void mapObjectModAction::fromString(string s)
{
   char buf[256];

   sscanf(s.c_str(),"%d %s %f %f %f %d", &action, &buf[0],
                    &x, &y, &z, &value);
   target = buf;
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
 *                       Constructor                        *
 ************************************************************/
mapTalkModAction::mapTalkModAction(string s)
                   :modAction(0, "", "", 0,0,0)
{
   fromString(s);
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

/************************************************************
 *                         toString                         *
 ************************************************************/
string mapTalkModAction::toString()
{
   char buf[64];

   /* Definition */
   string res = MODSTATE_TOKEN_TALK_MOD_ACTION;
   res += " = ";

   /* Action Type */
   sprintf(buf, " %d ", action);
   res += buf; 

   /* Target */
   res += target;

   /* Value  */
   sprintf(buf, " %d ", value);
   res += buf;

   /* Done */
   return(res);
}

/************************************************************
 *                       fromString                         *
 ************************************************************/
void mapTalkModAction::fromString(string s)
{
   char buf[256];

   sscanf(s.c_str(),"%d %s %d", &action, &buf[0], &value);
   target = buf;
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
   if(inv != NULL)
   {
      create(inv);
   }
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
void modInventory::flush(Map* curMap, inventory* inv)
{
   modInvObj* invObj = objects;
   object* curObj = NULL;
   int i, invNumber;
   bool res;

   /* Insert all objects at its previous positions */
   for(i = 0; i < totalObjects; i++)
   {
      /* Try get object from map object list */
      curObj = objectsList::search(invObj->fileName, -1, -1, -1);
      if(!curObj)
      {
         /* It's not on the list, so must insert it */
         curObj = createObject(invObj->fileName, curMap->getFileName());
      }

      /* Now insert it at the inventory */
      if(curObj)
      {
         res = true;

         if(invObj->invNumber >= 0)
         {
            /* Insert at inventory */
            res = inv->addObject(curObj, invObj->x, invObj->y, 
                                 invObj->invNumber);
         }
         else
         {
            /* Insert at equipped place */
            invNumber = invObj->invNumber + 1;
            invNumber *= -1;
            res = inv->equipObject(curObj, invNumber);
         }
         
         if(!res)
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

   /* Add All Equipped Places */
   for(curInv = 0; curInv < INVENTORY_TOTAL_PLACES; curInv++)
   {
      obj = inv->getFromPlace(curInv);
      if(obj)
      {
         /* Insert it with -1-curInv to designate a place, not a position */
         invObj =  new modInvObj();
         invObj->x = 0;
         invObj->y = 0;
         invObj->invNumber = -1-curInv;
         invObj->fileName = obj->getFileName();

         /* Insert it at the modInventory */
         insert(invObj);
      }
   }

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

/************************************************************
 *                           save                           *
 ************************************************************/
bool modInventory::load(string file)
{
   defParser def;
   string key="", value="";
   
   if(!def.load(file))
   {
      cerr << "Error: Can't load the inventory file: " << file << endl;
      return(false);
   }

   /* Parse all the file */
   while(def.getNextTuple(key, value))
   {
      if(key == MODSTATE_TOKEN_MOD_INVENTORY)
      {
         /* The Owner */
         target = value;
      }
      else if(key == MODSTATE_TOKEN_INVENTORY_ITEM)
      {
         /* Insert the item! */
         insert(value);
      }
      else
      {
         cerr << "Unknow token: '" << key << "' at modInventory load!" << endl;
      }
   }

   return(true);
}

/************************************************************
 *                           save                           *
 ************************************************************/
void modInventory::save(ofstream* file)
{
   int i;
   modInvObj* obj;

   /* Define the owner (target) */
   *file << MODSTATE_TOKEN_MOD_INVENTORY << " = " << target << endl;

   /* Save all objects here */
   obj = objects;
   for(i = 0; i < totalObjects; i++)
   {
      *file << MODSTATE_TOKEN_INVENTORY_ITEM << " = " 
            << obj->fileName << " " << obj->x << " " 
            << obj->y << " " << obj->invNumber << endl;
      obj = obj->next;
   }
}

/************************************************************
 *                         insert                           *
 ************************************************************/
void modInventory::insert(string s)
{
   modInvObj* obj = new modInvObj();
   char fName[256];

   /* Get Values */
   sscanf(s.c_str(), "%s %d %d %d", &fName[0], &obj->x, &obj->y, 
          &obj->invNumber);
   obj->fileName = fName;

   /* Insert it! */
   insert(obj);
}

/************************************************************
 *                         toString                         *
 ************************************************************/
string modInventory::toString()
{
   /* Not used for modInventories */
   return("");
}

/************************************************************
 *                       fromString                         *
 ************************************************************/
void modInventory::fromString(string s)
{
   /* Not used for modInventories */
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
 *                            load                          *
 ************************************************************/
void modMap::load(defParser* def)
{
   string key="", value="";
   modInventory* modInv = NULL;

   while(def->getNextTuple(key, value))
   {
      if(key == MODSTATE_TOKEN_MAP_END)
      {
         /* Done with the current modMap, so exit! */
         return;
      }
      else if(key == MODSTATE_TOKEN_CHARACTER_MOD_ACTION)
      {
         /* Parse and add Character ModAction */
         mapCharacterModAction* mChar = new mapCharacterModAction(value);
         addAction(mChar);
      }
      else if(key == MODSTATE_TOKEN_OBJECT_MOD_ACTION)
      {
         /* Parse and add Object ModAction */
         mapObjectModAction* mObj = new mapObjectModAction(value);
         addAction(mObj);
      }
      else if(key == MODSTATE_TOKEN_TALK_MOD_ACTION)
      {
         /* Parse and add Talk ModAction */
         mapTalkModAction* mTalk = new mapTalkModAction(value);
         addAction(mTalk);
      }
      else if(key == MODSTATE_TOKEN_MOD_INVENTORY)
      {
         /* Add the modInventory */
         modInv = new modInventory(NULL, value, mapFileName);
         addAction(modInv);
      }
      else if(key == MODSTATE_TOKEN_INVENTORY_ITEM)
      {
         /* Add item to the current mod Inventory */
         if(modInv)
         {
            modInv->insert(value);
         }
         else
         {
            cerr << "Warning: Item without modInventory!" << endl;
         }
      }
   }
}

/************************************************************
 *                            save                          *
 ************************************************************/
void modMap::save(ofstream* file)
{
   int i;
   modAction* act;
   modInventory* inv;

   /* Save the map fileName */
   *file << MODSTATE_TOKEN_MAP << " = " << mapFileName << endl;

   /* Now save each modAction */
   act = modActionsList;
   for(i = 0; i < totalModActions; i++)
   {
      /* Save normal actions */
      if(act->getAction() != MODSTATE_INVENTORY)
      {
         *file << act->toString() << endl;
      }

      /* Save a modInventory */
      else
      {
         inv = (modInventory*)act;
         inv->save(file);
      }
      act = act->getNext();
   }

   /* Mark end of map */
   *file << MODSTATE_TOKEN_MAP_END << " = " << mapFileName << endl;
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
void modMap::doMapModifications(Map* actualMap, void* NPCs) 
{
   int i;
   characterList* npcs = (characterList*) NPCs;
   character* ch  = NULL;
   GLfloat x=0, y=0, z=0;
   modAction* tmpMobj = modActionsList;
   for(i = 0; i < totalModActions; i++)
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
            obj = createObject(tmpMobj->getTarget(), actualMap->getFileName());
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

         if(!done)
         {
            cerr << "Not found Character: " << charAct->getTarget() << endl;
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
            mInv->flush(actualMap, ch->inventories);
         }
      }

      /* Unknow */
      else
      {
         printf("Unknow saved action: %d, at %d element!\n", 
               tmpMobj->getAction(), i);
      }
      
      tmpMobj = tmpMobj->getNext();
   }

}

/************************************************************
 *                     allCharacterDead                     *
 ************************************************************/
bool modMap::allCharactersDead(string npcFileName)
{
   npcFile npcs;
   string name="", arq ="";
   GLfloat posX=0, posZ=0, angle=0;

   if(!npcs.load(npcFileName))
   {
      /* No File, so always false */
      return(false);
   }

   /* Try to search for all characters dead definition here */
   while(npcs.getNextCharacter(name, arq, posX, posZ, angle))
   {
      /* If one isn't found, not all character are dead, so exit! */
      if(search(MODSTATE_ACTION_CHARACTER_DEAD, arq, posX, 0, posZ) == NULL)
      {
         /* Not dead, so false! */
         return(false);
      }
   }

   /*  All found (or no one defined), so all dead! */
   return(true);
}

/************************************************************
 *                    allCharacterAlive                     *
 ************************************************************/
bool modMap::allCharactersAlive(string npcFileName)
{
   npcFile npcs;
   string name="", arq ="";
   GLfloat posX=0, posZ=0, angle=0;

   if(!npcs.load(npcFileName))
   {
      /* No File, so always false */
      return(false);
   }

   /* Try to search for all characters dead definition here */
   while(npcs.getNextCharacter(name, arq, posX, posZ, angle))
   {
      /* If one is found, not all character are alive, so exit! */
      if(search(MODSTATE_ACTION_CHARACTER_DEAD, arq, posX, 0, posZ) != NULL)
      {
         /* Not dead, so false! */
         return(false);
      }
   }

   /*  No one found (or no one defined), so all alive! */
   return(true);
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
   defParser def;
   string key, value;
   modMap* mod = NULL;

   /* Clear current modState */
   clear();

   /* Try to open the modState file */
   if(!def.load(file))
   {
      cerr << "Can't open modstate file: " << file << endl;
      return(false);
   }

   /* Get version */
   def.getNextTuple(key, value);
   if(key == MODSTATE_TOKEN_VERSION)
   {
      //TODO do some latter version check
   }
   else
   {
      cerr << "Invalid modState File: " << file << endl;
      return(false);
   }

   /* Now parse the file */
   while(def.getNextTuple(key, value))
   {
      if(key == MODSTATE_TOKEN_MAP)
      {
         /* Get the modMap */
         mod = findModMap(value);

         /* And load things for it! */
         mod->load(&def);
      }
      else
      {
         cerr << "Unexpected token '" << key << "' at file " << file << endl;
      }
   }

   return(true);
}

/************************************************************
 *                         saveState                        *
 ************************************************************/
bool modState::saveState(string file)
{
   ofstream f;
   int i;
   modMap* cur;

   /* Create the file */
   f.open(file.c_str(), ios::out | ios::binary);
   if(!f)
   {
      cerr << "Can't save file: " << file << endl;
      return(false);
   }

   /* Save the version */
   f << MODSTATE_TOKEN_VERSION << " = " << VERSION << endl;

   /* Now call to save each modMap */
   cur = modMapList;
   for(i = 0; i < totalModMaps; i++)
   {
      cur->save(&f);
      cur = cur->getNext();
   }

   /* Close the file and done! */
   f.close();
   return(true);
}

/************************************************************
 *                          haveMap                         *
 ************************************************************/
bool modState::haveMap(string fileName)
{
   int i;
   modMap* mod = modMapList;
   for(i = 0; i < totalModMaps; i++)
   {
      if(mod->getMapFileName() == fileName)
      {
         /* Found it! */
         return(true);
      }
      mod = mod->getNext();
   }

   return(false);
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
void modState::doMapModifications(Map* actualMap, void* NPCs)
{
   modMap* mod = findModMap(actualMap->getFileName());
   if(mod != NULL)
   {
      mod->doMapModifications(actualMap, NPCs);
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
 *                    allCharactersDead                     *
 ************************************************************/
bool modState::allCharactersDead(string npcFileName)
{
   modMap* mod;   
   /* FIXME: trick, removing .npc and getting map Name! */
   string mapFile = npcFileName;
   mapFile.erase(mapFile.length()-4,4);

   /* Find its modMap */
   mod = findModMap(mapFile);
   if(mod)
   {
      return(mod->allCharactersDead(npcFileName));
   }

   /* No Mod, so all alive */
   return(false);
}


/************************************************************
 *                    allCharactersAlive                    *
 ************************************************************/
bool modState::allCharactersAlive(string npcFileName)
{
   modMap* mod;

   /* FIXME: trick, removing .npc and getting map Name! */
   string mapFile = npcFileName;
   mapFile.erase(mapFile.length()-4,4);

   /* Find its modMap */
   mod = findModMap(mapFile);
   if(mod)
   {
      return(mod->allCharactersAlive(npcFileName));
   }

   /* No Mod, so all alive */
   return(true);
}

/************************************************************
 *                      static fields                       *
 ************************************************************/
modMap* modState::modMapList = NULL;
int modState::totalModMaps = 0;

