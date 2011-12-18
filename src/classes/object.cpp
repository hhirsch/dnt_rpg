/*
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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


#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#include <SDL/SDL_opengl.h>
#include "object.h"
#include "weapon.h"
#include "money.h"

#include "../engine/util.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include "../etc/scene.h"
#include "../lang/translate.h"

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               Object                                   //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


/**************************************************************
 *                       getObjectTypeId                      *
 **************************************************************/
int object::getObjectTypeId(string t)
{
   /* Translates a type string to the object type Id */
   if(t == "scenery")
   {
      return(OBJECT_TYPE_SCENERY);
   }
   else if(t == "weapon")
   {
      return(OBJECT_TYPE_WEAPON);
   }
   else if(t == "armor")
   {
      return(OBJECT_TYPE_ARMOR);
   }
   else if(t == "heal")
   {
      return(OBJECT_TYPE_HEAL);
   }
   else if( (t == "ammo") || (t == "munition") )
   {
      return(OBJECT_TYPE_AMMO);
   }
   else if(t == "explosive")
   {
      return(OBJECT_TYPE_EXPLOSIVE);
   }
   else if(t == "narcotic")
   {
      return(OBJECT_TYPE_NARCOTIC);
   }
   else if(t == "book")
   {
      return(OBJECT_TYPE_BOOK);
   }
   else if(t == "money")
   {
      return(OBJECT_TYPE_MONEY);
   }
   else
   {
      cerr << "Warning: Unknow object type: '" << t << "'" << endl;
   }

   return(OBJECT_TYPE_GENERIC);
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(string path, string curMap): thing()
{
   scene curScene;
   int aux;
   dirs dir;
   int diceId;
   int numberOfDices;
   int sumNumber;
   cal3dName = "";

   /* Initial Values */
   cleanValues();

   /* Parse the values */
   defParser parser;
   if(!parser.load(path))
   {
       cerr << "Error loading object file " <<  dir.getRealFile(path) << endl;
      return;
   }

   fileName = path;

   /* Define each attribute */
   string key = "", value = "";
   while(parser.getNextTuple(key, value))
   {
      if(key == "name")
      {
         name = translateDataString(value);
      }
      else if(key == "description")
      {
         description = translateDataString(value);
      }
      else if(key == "dialog")
      {
         setConversationFile(value);
         createConversation(curMap);
      }
      else if(key == "related_info")
      {
         relatedInfo = value;
      }
      else if(key == "cal3d")
      {
         cal3dName = value;
      }
      else if(key == "type")
      {
         type = getObjectTypeId(value);
      }
      else if(key == "state")
      {
         sscanf(value.c_str(), "%d", &state);
      }
      else if(key == "inventory_sizes")
      {
         sscanf(value.c_str(),"%d %d",&inventSizeX, &inventSizeY);
      }
      else if(key == "inventory_texture")
      {
         model2dName = value;
         model2d = IMG_Load(dir.getRealFile(model2dName).c_str());
         if(!model2d)
         {
            cerr << "Couldn't open image: " <<  dir.getRealFile(model2dName)
                 << "! Will crash soon." << endl;
         }
      }
      else if(key == "life_points")
      {
         sscanf(value.c_str(),"%d",&maxLifePoints);
         lifePoints = maxLifePoints;
      }
      else if(key == "fortitude")
      {
         sscanf(value.c_str(),"%d",&curBonusAndSaves.fortitude);
      }
      else if( (key == "reflex") || (key == "reflexes"))
      {
         sscanf(value.c_str(),"%d",&curBonusAndSaves.reflexes);
      }
      else if((key == "will") || (key == "i_am_not_a_fool") )
      {
         sscanf(value.c_str(),"%d",&curBonusAndSaves.iAmNotAFool);
      }
      else if(key == "displacement")
      {
         sscanf(value.c_str(),"%d",&displacement);
      }
      else if(key == "armature_class")
      {
         sscanf(value.c_str(),"%d",&armatureClass);
      }
      else if(key == "size_modifier")
      {
         sscanf(value.c_str(),"%d",&sizeModifier);
      }
      else if(key =="cost")
      {
         sscanf(value.c_str(),"%f",&cost);
      }
      else if(key == "eqAngles")
      {
         sscanf(value.c_str(),"%f,%f,%f",&eqAngleX, &eqAngleY, &eqAngleZ);
      }
      else if(key == "eqTrans1")
      {
         sscanf(value.c_str(),"%f,%f,%f",
                &eqTrans1[0], &eqTrans1[1], &eqTrans1[2]);
      }
      else if(key == "eqTrans2")
      {
         sscanf(value.c_str(),"%f,%f,%f",
                &eqTrans2[0], &eqTrans2[1], &eqTrans2[2]);
      }
      else if(key == "static_scenery")
      {
         sscanf(value.c_str(),"%d", &aux);
         staticScenery = (aux);
      }
      else if((key == "weight_value") || (key == "weight"))
      {
         sscanf(value.c_str(),"%f",&weight);
      }
      else if(key == "base_dice")
      {
         sscanf(value.c_str(),"%d*d%d+%d",&numberOfDices, &diceId, &sumNumber);
         diceInfo.baseDice.setNumberOfDices(numberOfDices);
         diceInfo.baseDice.setType(diceId);
         diceInfo.baseDice.setSumNumber(sumNumber);
      }
      else if(key == "aditional_dice")
      {
         sscanf(value.c_str(),"%d*d%d+%d",&numberOfDices, &diceId, &sumNumber);
         diceInfo.aditionalDice.setNumberOfDices(numberOfDices);
         diceInfo.aditionalDice.setType(diceId);
         diceInfo.aditionalDice.setSumNumber(sumNumber);
      }
      else
      {
         cerr << "Warning: Unknow key '" << key << "' at '"
              << path << "'" << endl;
      }
   }

   /* Load/Get Cal3D Model */
   if(!cal3dName.empty())
   {
      scNode = curScene.createSceneNode(staticScenery, cal3dName, 0,0,0,0,0,0);
   }
   else
   {
      cerr << "Error: 3D Model not defined for " << name << endl;
   }

   /* Add the object to the list */
   objectsList::insert(this);
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(object* obj): thing()
{
   scene curScene;

   /* First, clean */
   cleanValues();

   /* Define type */
   type = obj->type;

   /* Define Inventory things */
   inventSizeX = obj->inventSizeX;
   inventSizeY = obj->inventSizeY;

   /* Define Object Things */
   name = obj->name;
   fileName = obj->fileName;
   relatedInfo = obj->relatedInfo;

   /* Define 2D model things */
   model2dName = obj->model2dName;
   model2d = IMG_Load(model2dName.c_str());

   /* Define 3D Model */
   staticScenery = obj->staticScenery;
   cal3dName = obj->cal3dName;
   scNode = curScene.createSceneNode(staticScenery, cal3dName, 0,0,0,0,0,0);

   /* Define some Points */
   maxLifePoints = obj->maxLifePoints;
   lifePoints = maxLifePoints;
   curBonusAndSaves.fortitude = obj->curBonusAndSaves.fortitude;
   curBonusAndSaves.reflexes = obj->curBonusAndSaves.reflexes;
   curBonusAndSaves.iAmNotAFool = obj->curBonusAndSaves.iAmNotAFool;
   armatureClass = obj->armatureClass;
   sizeModifier = obj->sizeModifier;

   /* Add the object to the list */
   objectsList::insert(this);
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(string path): thing()
{
   cleanValues();

   /* Set the fileName */
   fileName = path;

   /* Add the object to the list */
   objectsList::insert(this);
}

/**************************************************************
 *                       removeSceneNode                      *
 **************************************************************/
void object::removeSceneNode()
{
   scene curScene;

   if(scNode)
   {
      curScene.deleteSceneNode(scNode);
      scNode = NULL;
   }
}

/**************************************************************
 *                       createSceneNode                      *
 **************************************************************/
void object::createSceneNode(float pX, float pY, float pZ,
                             float aX, float aY, float aZ)
{
   scene curScene;
   /* Create or update the current scene node */
   if(scNode)
   {
      /* Just update it */
      scNode->set(pX, pY, pZ, aX, aY, aZ);
   }
   else
   {
      scNode = curScene.createSceneNode(staticScenery, cal3dName, 0,0,0,0,0,0);
   }
}

/**************************************************************
 *                         cleanValues                        *
 **************************************************************/
void object::cleanValues()
{
   state = 0;
   inventSizeX = 0;
   inventSizeY = 0;
   eqAngleX = 0.0f;
   eqAngleY = 0.0f;
   eqAngleZ = 0.0f;
   eqTrans1[0] = 0.0f;
   eqTrans1[1] = 0.0f;
   eqTrans1[2] = 0.0f;
   eqTrans2[0] = 0.0f;
   eqTrans2[1] = 0.0f;
   eqTrans2[2] = 0.0f;
   name = "";
   description = "";
   fileName = "";
   relatedInfo = "";
   model2dName = "";
   model2d = NULL;
   scNode = NULL;
   type = OBJECT_TYPE_GENERIC;
   maxLifePoints = 0;
   lifePoints = 0;
   curBonusAndSaves.fortitude = 0;
   armatureClass = 0;
   sizeModifier = 0;
   cost = 0;
   staticScenery = false;
   thingType = THING_TYPE_OBJECT;
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
object::~object()
{
   /* Remove the scene node, if any */
   removeSceneNode();

   /* Delete the model 2D used
    * TODO something like model3d for model2d */
   if(model2d)
   {
      SDL_FreeSurface(model2d);
   }

   /* Remove the object from the list */
   objectsList::remove(this);
}

/**************************************************************
 *                           getNext                          *
 **************************************************************/
object* object::getNext()
{
   return(next);
}

/**************************************************************
 *                         getPrevious                        *
 **************************************************************/
object* object::getPrevious()
{
   return(previous);
}

/**************************************************************
 *                           setNext                          *
 **************************************************************/
void object::setNext(object* o)
{
   next = o;
}

/**************************************************************
 *                         setPrevious                        *
 **************************************************************/
void object::setPrevious(object* o)
{
   previous = o;
}

/**************************************************************
 *                     seEquippedPosition                     *
 **************************************************************/
void object::setEquipedPosition(int type, float pX, float pY, float pZ,
                                float aY)
{
   if(type == 1)
   {
      pX += eqTrans1[0];
      pY += eqTrans1[1];
      pZ += eqTrans1[2];
   }
   else
   {
      pX += eqTrans2[0];
      pY += eqTrans2[1];
      pZ += eqTrans2[2];
   }
   if(scNode)
   {
      scNode->set(pX, pY, pZ, eqAngleX, eqAngleY, eqAngleZ);
      scNode->setRotationLast(aY, 0.0f, 1.0f, 0.0f);
   }
}

/**************************************************************
 *                        renderEquipped                      *
 **************************************************************/
#if 0
void object::renderEquipped(int type, float pX, float pY,
      float pZ, float angle, float angleXY, float angleYZ,
      bool reflexion, bool shadow,
      GLfloat* shadowMatrix, float shadowAlpha)
{
   /* FIXME! */

   /* Update model (if animated) */
   model3D->update(WALK_UPDATE);

   /* Put it at graphic's memory */
   model3D->loadToGraphicMemory();

   /* Draw the defined model */
   glEnable(GL_COLOR_MATERIAL);
   glPushMatrix();
      glTranslatef(pX, pY, pZ);
      glRotatef(angle, 0,1,0);
      equippedTransforms(type);
      model3D->renderFromGraphicMemory();
   glPopMatrix();
   glDisable(GL_COLOR_MATERIAL);

   /* Render its reflexion */
   if(reflexion)
   {
       glPushMatrix();
         glScalef(1.0f, -1.0f, 1.0f);
         glTranslatef(pX, pY, pZ);
         glRotatef(angle, 0,1,0);
         equippedTransforms(type);
         model3D->renderReflexion(0,0,0,0,0,0);
      glPopMatrix();
   }

   /* Render its shadow */
   if(shadow)
   {
      /* FIXME: some workaround to use the renderShadow,
       * instead of redoing it here (the problem is with the equipped
       * transforms). */
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_TEXTURE_2D);
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_EQUAL, 1, 0xffffffff);
      glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
      glPolygonOffset(-2.0f,-1.0f);
      glEnable(GL_POLYGON_OFFSET_FILL);
      glDisable(GL_LIGHTING);
      glColor4f(0.0f, 0.0f, 0.0f, shadowAlpha);

      glPushMatrix();
         glMultMatrixf(shadowMatrix);
         glPushMatrix();
            glTranslatef(pX, pY, pZ);
            glRotatef(angle, 0,1,0);
            equippedTransforms(type);
            model3D->renderFromGraphicMemory();
         glPopMatrix();
      glPopMatrix();

      glEnable(GL_LIGHTING);
      glDisable(GL_POLYGON_OFFSET_FILL);
      glDisable(GL_STENCIL_TEST);
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_BLEND);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   }

   /* Remove it from graphic's memory */
   model3D->removeFromGraphicMemory();

}
#endif

/**************************************************************
 *                           draw2D                           *
 **************************************************************/
void object::draw2D(int x, int y, SDL_Surface* surface)
{
   SDL_Rect ret;
   ret.x = x;
   ret.y = y;
   ret.w = model2d->w;
   ret.h = model2d->h;
   SDL_BlitSurface(model2d, NULL, surface, &ret);
}

/**************************************************************
 *                           getName                          *
 **************************************************************/
string object::getName()
{
   return(name);
}

/**************************************************************
 *                         getFileName                        *
 **************************************************************/
string object::getFileName()
{
   return(fileName);
}

/**************************************************************
 *                      getDescription                        *
 **************************************************************/
string object::getDescription()
{
   return(description);
}


/**************************************************************
 *                        getRelatedInfo                      *
 **************************************************************/
string object::getRelatedInfo()
{
   return(relatedInfo);
}

/**************************************************************
 *                      getInventorySize                      *
 **************************************************************/
void object::getInventorySize(int &x, int &y)
{
   x = inventSizeX;
   y = inventSizeY;
}

/*********************************************************************
 *                            getDiceInfo                            *
 *********************************************************************/
diceThing object::getDiceInfo()
{
   return(diceInfo);
}

/*********************************************************************
 *                              canGet                               *
 *********************************************************************/
bool object::canGet()
{
   int x, y;
   getInventorySize(x,y);
   return( (x != 0) && (y != 0) && (!staticScenery));
}

/*********************************************************************
 *                             isUsable                              *
 *********************************************************************/
bool object::isUsable()
{
   return( (type == OBJECT_TYPE_WEAPON) ||
           (type == OBJECT_TYPE_ARMOR) ||
           (type == OBJECT_TYPE_HEAL) ||
           (type == OBJECT_TYPE_EXPLOSIVE) ||
           (type == OBJECT_TYPE_BOOK) ||
           (type == OBJECT_TYPE_AMMO) ||
           (type == OBJECT_TYPE_NARCOTIC));
}

/*********************************************************************
 *                               getType                             *
 *********************************************************************/
int object::getType()
{
   return(type);
}

/*********************************************************************
 *                               setState                            *
 *********************************************************************/
void object::setState(int st)
{
   state = st;
}

/*********************************************************************
 *                               getState                            *
 *********************************************************************/
int object::getState()
{
   return(state);
}

/*********************************************************************
 *                            isStaticScenery                        *
 *********************************************************************/
bool object::isStaticScenery()
{
   return(staticScenery);
}

/*********************************************************************
 *                            get2dModel                             *
 *********************************************************************/
SDL_Surface* object::get2dModel()
{
   return(model2d);
}

/*********************************************************************
 *                          get2dModelName                           *
 *********************************************************************/
string object::get2dModelName()
{
   return(model2dName);
}

/*********************************************************************
 *                           callDeadAnimation                       *
 *********************************************************************/
void object::callDeadAnimation()
{
   //TODO
   //setState(STATE_DIE);
}

/*********************************************************************
 *                          callAttackAnimation                      *
 *********************************************************************/
void object::callAttackAnimation()
{
   //TODO
   //setState(STATE_ATTACK_MEELE);
}

/*********************************************************************
 *                           callIdleAnimation                       *
 *********************************************************************/
void object::callIdleAnimation()
{
   //TODO
   //setState(STATE_IDLE);
}

/*********************************************************************
 *                            depthCollision                         *
 *********************************************************************/
bool object::depthCollision(GLfloat angleX, GLfloat angleY, GLfloat angleZ,
      GLfloat pX, GLfloat pY, GLfloat pZ, boundingBox colBox)
{
   if(scNode)
   {
      return(scNode->getModel()->depthCollision(angleX,angleY,angleZ,
               pX,pY,pZ,colBox));
   }
   /* If no model, no collision =^P */
   return(false);
}


////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                            ObjectsList                                 //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/*********************************************************************
 *                               insert                              *
 *********************************************************************/
void objectsList::insert(object* o)
{
   /* Redefine the object's pointers */
   if(first)
   {
      o->setNext(first);
      o->setPrevious(first->getPrevious());
      o->getPrevious()->setNext(o);
      o->getNext()->setPrevious(o);
   }
   else
   {
      o->setNext(o);
      o->setPrevious(o);
   }

   /* Set it as the initial one */
   first = o;
   total++;
}

/*********************************************************************
 *                               remove                              *
 *********************************************************************/
void objectsList::remove(object* o)
{
   /* Redefine the first, if needed */
   if(first == o)
   {
      first = o->getNext();
   }

   /* Redefine the neighboor pointers */
   o->getNext()->setPrevious(o->getPrevious());
   o->getPrevious()->setNext(o->getNext());

   /* Decrement the counter */
   total--;

   /* Nullify the first, if needed */
   if(total <= 0)
   {
      first = NULL;
   }
}

/*********************************************************************
 *                       removeStaticSceneries                       *
 *********************************************************************/
void objectsList::removeStaticSceneries()
{
   int i;
   int curTotal = total;
   object* cur = first;
   object* oth = NULL;

   /* Look at all list */
   for(i=0; i<curTotal; i++)
   {
      oth = cur;
      cur = cur->getNext();

      /* verify if is static scenery */
      if(oth->isStaticScenery())
      {
         /* Just delete, because the destructor will call
          * the remove(oth) for us. */
         deleteObject(oth);
      }
   }
}

/*********************************************************************
 *                             removeAll                             *
 *********************************************************************/
void objectsList::removeAll()
{
   int i;
   int curTotal = total;
   object* cur = first;
   object* oth = NULL;

   /* Look at all list */
   for(i=0; i<curTotal; i++)
   {
      oth = cur;
      cur = cur->getNext();

      /* Just delete, because the destructor will call
       * the remove(oth) for us. */
      deleteObject(oth);
   }

   total = 0;
   first = NULL;
}

/*********************************************************************
 *                              search                               *
 *********************************************************************/
object* objectsList::search(string fileName, GLfloat posX, GLfloat posY,
                           GLfloat posZ)
{
   int i;
   object* cur = first;

   /* Search all the list */
   for(i=0; i<total; i++)
   {
      if(cur->getFileName() == fileName)
      {
         if(!cur->scNode)
         {
            /* FIXME: !!
            cerr << "Without scNode: " << fileName << " "
                 << posX << " " << posY << " " << posZ << " " << endl; */
         }
         else if( (cur->scNode->getPosX() == posX) &&
                  (cur->scNode->getPosY() == posY) &&
                  (cur->scNode->getPosZ() == posZ) )
         {
            /* same name, at the same position, it's the same! */
            return(cur);
         }
      }
      cur = cur->getNext();
   }

   /* Not found! */
   return(NULL);
}

/*********************************************************************
 *                              search                               *
 *********************************************************************/
object* objectsList::search(sceneNode* scNode)
{
   int i;
   object* cur = first;

   /* Search all the list */
   for(i=0; i<total; i++)
   {
      if(cur->scNode == scNode)
      {
         return(cur);
      }

      cur = cur->getNext();
   }

   /* Not found! */
   return(NULL);

}

/*********************************************************************
 *                          static members                           *
 *********************************************************************/
object* objectsList::first = NULL;
int objectsList::total = 0;

/********************************************************************
 *                         deleteObject                             *
 ********************************************************************/
void deleteObject(object* obj)
{
   switch(obj->getType())
   {
      case OBJECT_TYPE_WEAPON:
      {
         weapon* wp = (weapon*)obj;
         delete(wp);
      }
      break;
      case OBJECT_TYPE_MONEY:
      {
         money* m = (money*)obj;
         delete(m);
      }
      break;
      default:
      {
         delete(obj);
      }
      break;
   }
}

/********************************************************************
 *                         createObject                             *
 ********************************************************************/
object* createObject(string arquivo, string mapFileName)
{
   object* novo = NULL;

   if(arquivo == DNT_MONEY_OBJECT)
   {
      /* It's a money! */
      novo = (money*) new money();
   }

   else
   {
      string::size_type loc = arquivo.find( ".dcc", 0 );
      if( loc != string::npos )
      {
         /* It's an Object *.dcc */
         novo = (object*) new object(arquivo, mapFileName);
      }
      else
      {
         loc = arquivo.find( ".wcc", 0 );
         if( loc != string::npos )
         {
            /* It's an weapon Object *.wcc */
            novo = (object*) new weapon(arquivo);
         }
      }
   }

   /* verify if created the pointer */
   if(novo == NULL)
   {
      cerr << "Error, cannot define the type of " << arquivo << endl;
      return(NULL);
   }

   return(novo);
}


