/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "object.h"
#include "../engine/util.h"
#include "../etc/dirs.h"

/**************************************************************
 *                        getAfterEqual                       *
 **************************************************************/
string object::getAfterEqual(string s)
{
   unsigned int i = 0;

   /* First, delete the \n character at the end of the string,
    * if there is one there. */
   if(s[s.length()-1] == '\n')
   {
      s.erase(s.length()-1,1);
   }

   /* Goes to the equal character */
   while( (i < s.length()) && (s[i] != '='))
   {
      i++;
   }
   i++;

   /* Remove all spaces after the equal */
   while( (i < s.length()) && (s[i] == ' '))
   {
      i++;
   }

   if(i < s.length())
   {
      return(s.substr(i));
   }
   printf("Error parsing %s\n", s.c_str());
   return("");
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(string path, modelList& mdlList): thing()
{
   dirs dir;
   FILE* file;
   char buffer[512];
   string token, token2;
   int aux;
   string cal3DFile = "";

   cleanValues();

   if(!(file=fopen(dir.getRealFile(path).c_str(),"r")))
   {
       printf("Error on open object %s\n", 
              dir.getRealFile(path).c_str());
       return;
   }

   fileName = path;

   while(fscanf(file, "%s", buffer) != EOF)
   {
      token = buffer;

      /* eat up the rest of line */
      fgets(buffer, sizeof(buffer), file);
      token2 = getAfterEqual(buffer);

      if(token2 == "")
      {
         printf("at file: %s\n",path.c_str());
      }

      /* Tokenize the first token */
      if(token == "name")
      {
         name = token2;
      }
      else if(token == "cal3d")
      {
         cal3DFile = token2;
      }
      else if(token == "inventory_sizes")
      {
         sscanf(token2.c_str(),"%d %d",&inventSizeX, &inventSizeY);
      }
      else if(token == "inventory_texture")
      {
         model2dName = token2;
         model2d = IMG_Load(dir.getRealFile(model2dName).c_str());
         if(!model2d)
         {
            printf("Can't open image: %s\nWill Crash Soon!\n", 
                   dir.getRealFile(model2dName).c_str());
         }
      }
      else if(token == "life_points")
      {
         sscanf(token2.c_str(),"%d",&maxLifePoints);
         lifePoints = maxLifePoints;
      }
      else if(token == "fortitude")
      {
         sscanf(token2.c_str(),"%d",&fortitude);
      }
      else if(token == "reflex")
      {
         sscanf(token2.c_str(),"%d",&reflex);
      }
      else if((token == "will") || (token == "i_am_not_a_fool") )
      {
         sscanf(token2.c_str(),"%d",&iAmNotAFool);
      }
      else if(token == "displacement")
      {
         sscanf(token2.c_str(),"%d",&displacement);
      }
      else if(token == "armature_class")
      {
         sscanf(token2.c_str(),"%d",&armatureClass);
      }
      else if(token == "size_modifier")
      {
         sscanf(token2.c_str(),"%d",&sizeModifier);
      }
      else if(token =="cost") 
      {
         sscanf(token2.c_str(),"%f",&cost);
      }
      else if(token == "static_scenery")
      {
         sscanf(token2.c_str(),"%d", &aux);
         staticScenery = (aux);
      }
      else if((token == "weight_value") || (token == "weight"))
      {
         sscanf(token2.c_str(),"%f",&weight);
      }
      else
      {
         printf("Warning: Unknow token '%s' at %s\n", token.c_str(), 
                                                      path.c_str());
      }
   }

   fclose(file);

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
 *                              canGet                               *
 *********************************************************************/
bool object::canGet()
{
   int x, y;
   getInventorySize(x,y);
   return( (x != 0) && (y != 0) && (!staticScenery));
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

