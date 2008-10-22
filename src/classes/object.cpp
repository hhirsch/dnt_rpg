/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "object.h"
#include "../engine/util.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"

/**************************************************************
 *                       getObjectTypeId                      *
 **************************************************************/
int getObjectTypeId(string type)
{
   /* Translates a type string to the object type Id */
   if(type == "weapon")
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
   else if(type == "ammo")
   {
      return(OBJECT_TYPE_AMMO);
   }
   else if(type == "explosive")
   {
      return(OBJECT_TYPE_EXPLOSIVE);
   }

   return(OBJECT_TYPE_GENERIC);
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(string path, modelList& mdlList): thing()
{
   string cal3DFile = "";
   int aux;
   dirs dir;

   /* Initial Values */
   cleanValues();

   /* Parse the values */
   defParser parser;
   if(!parser.load(path))
   {
       printf("Error loading object file '%s'\n", 
              dir.getRealFile(path).c_str());
      return;
   }

   fileName = path;

   /* Define each attribute */
   string key = "", value = "";
   while(parser.getNextTuple(key, value))
   {
      if(key == "name")
      {
         name = value;
      }
      else if(key == "cal3d")
      {
         cal3DFile = value;
      }
      else if(key == "type")
      {
         type = getObjectTypeId(value);
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
            printf("Can't open image: %s\nWill Crash Soon!\n", 
                   dir.getRealFile(model2dName).c_str());
         }
      }
      else if(key == "life_points")
      {
         sscanf(value.c_str(),"%d",&maxLifePoints);
         lifePoints = maxLifePoints;
      }
      else if(key == "fortitude")
      {
         sscanf(value.c_str(),"%d",&fortitude);
      }
      else if(key == "reflex")
      {
         sscanf(value.c_str(),"%d",&reflex);
      }
      else if((key == "will") || (key == "i_am_not_a_fool") )
      {
         sscanf(value.c_str(),"%d",&iAmNotAFool);
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
      else if(key == "static_scenery")
      {
         sscanf(value.c_str(),"%d", &aux);
         staticScenery = (aux);
      }
      else if((key == "weight_value") || (key == "weight"))
      {
         sscanf(value.c_str(),"%f",&weight);
      }
      else
      {
         printf("Warning: Unknow key '%s' at %s\n", key.c_str(), 
                                                    path.c_str());
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
      printf("Error: 3D Model not defined for %s!\n", name.c_str());
   }
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(object* obj): thing()
{
   /* Define type */
   usedFlag = 0;
   type = obj->type;

   /* Define Inventory things */
   inventSizeX = obj->inventSizeX;
   inventSizeY = obj->inventSizeY;

   /* Define Object Things */
   name = obj->name;
   fileName = obj->fileName;

   /* Define 2D model things */
   model2dName = obj->model2dName;
   model2d = IMG_Load(model2dName.c_str());

   /* Define 3D Model */
   model3D = obj->model3D;
   model3D->incUsed();

   /* Define some Points */
   maxLifePoints = obj->maxLifePoints;
   lifePoints = maxLifePoints;
   fortitude = obj->fortitude;
   armatureClass = obj->armatureClass;
   sizeModifier = obj->sizeModifier;
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(): thing()
{
   cleanValues();
}

/**************************************************************
 *                         cleanValues                        *
 **************************************************************/
void object::cleanValues()
{
   usedFlag = 0;
   inventSizeX = 0;
   inventSizeY = 0;
   name = "";
   fileName = "";
   model2dName = "";
   model2d = NULL;
   model3D = NULL;
   next = NULL;
   previous = NULL;
   type = OBJECT_TYPE_GENERIC;
   maxLifePoints = 0;
   lifePoints = 0;
   fortitude = 0;
   armatureClass = 0;
   sizeModifier = 0;
   cost = 0;
   staticScenery = false;
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
object::~object()
{
   model3D->decUsed();
   if(model2d)
   {
      SDL_FreeSurface(model2d);
   }
}

/**************************************************************
 *                            draw                            *
 **************************************************************/
void object::draw(float x, float z, GLfloat dist, float orientation, 
                  bool inverted)
{
   /* Draw the defined model */
   glEnable(GL_COLOR_MATERIAL);
   glPushMatrix();
      glTranslatef(x, 0 ,z);
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
           (type == OBJECT_TYPE_EXPLOSIVE) );
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

