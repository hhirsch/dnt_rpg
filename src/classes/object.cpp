/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "object.h"
#include "weapon.h"
#include "money.h"

#include "../engine/util.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
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
int getObjectTypeId(string type)
{
   /* Translates a type string to the object type Id */
   if(type == "scenery")
   {
      return(OBJECT_TYPE_SCENERY);
   }
   else if(type == "weapon")
   {
      return(OBJECT_TYPE_WEAPON);
   }
   else if(type == "armor")
   {
      return(OBJECT_TYPE_ARMOR);
   }
   else if(type == "heal")
   {
      return(OBJECT_TYPE_HEAL);
   }
   else if( (type == "ammo") || (type == "munition") )
   {
      return(OBJECT_TYPE_AMMO);
   }
   else if(type == "explosive")
   {
      return(OBJECT_TYPE_EXPLOSIVE);
   }
   else if(type == "narcotic")
   {
      return(OBJECT_TYPE_NARCOTIC);
   }
   else if(type == "book")
   {
      return(OBJECT_TYPE_BOOK);
   }
   else if(type == "money")
   {
      return(OBJECT_TYPE_MONEY);
   }
   else
   {
      cerr << "Warning: Unknow object type: '" << type << "'" << endl;
   }

   return(OBJECT_TYPE_GENERIC);
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(string path, string curMap): thing()
{
   modelList mdlList;
   string cal3DFile = "";
   int aux;
   dirs dir;
   int diceId;
   int numberOfDices;
   int sumNumber;

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
         cal3DFile = value;
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
   if(!cal3DFile.empty())
   {
      model3D = mdlList.addModel(cal3DFile,"", staticScenery);
      model3D->incUsed();
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
   model3D = obj->model3D;
   model3D->incUsed();

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
   fileName = "";
   relatedInfo = "";
   model2dName = "";
   model2d = NULL;
   model3D = NULL;
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
   /* Decrement Model3D Usage */
   model3D->decUsed();

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
 *                            draw                            *
 **************************************************************/
void object::draw(bool inverted)
{
   /* Draw the defined model */
   glEnable(GL_COLOR_MATERIAL);
   glPushMatrix();
      glTranslatef(xPosition, (inverted?-yPosition:yPosition), zPosition);
      glRotatef(orientation,0,1,0);
      if(inverted)
      {
         glScalef(1.0, -1.0, 1.0);
      }
      model3D->update(WALK_UPDATE);
      model3D->draw();
   glPopMatrix();
   glDisable(GL_COLOR_MATERIAL);
}

/**************************************************************
 *                        equippedMods                        *
 **************************************************************/
void object::equippedTransforms(int type)
{
   if(type == 1)
   {
      glTranslatef(eqTrans1[0], eqTrans1[1], eqTrans1[2]);
   }
   else
   {
      glTranslatef(eqTrans2[0], eqTrans2[1], eqTrans2[2]);
   }

   glRotatef(eqAngleX, 1,0,0);
   glRotatef(eqAngleZ, 0,0,1);
   glRotatef(eqAngleY, 0,1,0);
}

/**************************************************************
 *                        renderEquipped                      *
 **************************************************************/
void object::renderEquipped(int type, float pX, float pY, 
      float pZ, float angle, float angleXY, float angleYZ,
      bool reflexion, bool shadow, 
      GLfloat* shadowMatrix, float shadowAlpha)
{
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
         model3D->renderReflexion(0,0,0,0);
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
 *                       getBoundingBox                       *
 **************************************************************/
boundingBox object::getBoundingBox()
{
   return(model3D->getBoundingBox());
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
 *                            get3dModel                             *
 *********************************************************************/
model3d* object::get3dModel()
{
   return(model3D);
}

/*********************************************************************
 *                         addRenderPosition                         *
 *********************************************************************/
void object::addRenderPosition(float x, float y, float z, float angle)
{
   if( (staticScenery) && (model3D))
   {
      model3D->addPosition(x,y,z,angle);
   }
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
bool object::depthCollision(GLfloat angle, GLfloat pX, GLfloat pY, GLfloat pZ,
                            GLfloat colMin[3], GLfloat colMax[3])
{
   if(model3D)
   {
      return(model3D->depthCollision(angle,pX,pY,pZ,colMin,colMax));
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
         if(cur->isStaticScenery())
         {
            /* static sceneries always use the same object pointer,
             * so it is found! */
            return(cur);
         }
         else if( (cur->xPosition == posX) && 
                  (cur->yPosition == posY) &&
                  (cur->zPosition == posZ) )
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


