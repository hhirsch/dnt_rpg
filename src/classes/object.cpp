/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "object.h"

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(string path, string objName, modelList& mdlList): thing()
{
   FILE* arq;
   char arqModelo[128], dirTexturas[128];

   if(!(arq=fopen(path.c_str(),"r")))
   {
       printf("Error on open object %s\n", path.c_str());
       return;
   }

   name = objName;
   fileName = path;

   /* Read Models Variations */
   fscanf(arq,"%d",&deltaVar);
   fscanf(arq,"%s",&arqModelo[0]); 
   
   /* Read Texture Dir */
   fscanf(arq,"%s",&dirTexturas[0]);

   modelMax = mdlList.addModel(arqModelo,dirTexturas);
   modelMax->incUsed();

   /* If exists, read LOD models */
   if(deltaVar !=0) 
   {
      /* Medium Model */
      fscanf(arq,"%s",arqModelo);
      fscanf(arq,"%s",dirTexturas);
      modelMed = mdlList.addModel(arqModelo,dirTexturas);
      modelMed->incUsed();

      /* Max Model */
      fscanf(arq,"%s",arqModelo);
      fscanf(arq,"%s",dirTexturas);
      modelMin = mdlList.addModel(arqModelo,dirTexturas);
      modelMin->incUsed();
   }
   else 
   {
      modelMed = NULL;
      modelMin = NULL;
   }

   /* Read Inventory Sizes */
   fscanf(arq,"%d %d", &inventSizeX, &inventSizeY);

   /* Read Object 2D Model */
   fscanf(arq,"%s",&arqModelo[0]);
   model2dName = arqModelo;
   model2d = IMG_Load(arqModelo);
   if(!model2d)
   {
      printf("Can't open image: %s\nWill Crash Soon!\n", arqModelo);
   }

   /* Read Max Life Points */
   fscanf(arq,"%d",&maxLifePoints);
   lifePoints = maxLifePoints;

   /* Read Modifiers */
   fscanf(arq,"%d %d %d",&fortitude, &armatureClass, &sizeModifier);
   
   fclose(arq);
}

/**************************************************************
 *                         Constructor                        *
 **************************************************************/
object::object(object* obj): thing()
{
   name = obj->name;
   fileName = obj->fileName;
   model2dName = obj->model2dName;
   model2d = IMG_Load(model2dName.c_str());
   modelMax = obj->modelMax;
   modelMax->incUsed();
   modelMed = obj->modelMed;
   if(modelMed)
   {
      modelMed->incUsed();
   }
   modelMin = obj->modelMin;
   if(modelMin)
   {
      modelMin->incUsed();
   }
   maxLifePoints = obj->maxLifePoints;
   lifePoints = maxLifePoints;
   fortitude = obj->fortitude;
   armatureClass = obj->armatureClass;
   sizeModifier = obj->sizeModifier;
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
object::~object()
{
   modelMax->decUsed();
   if(modelMed)
   {
     modelMed->decUsed();
   }
   if(modelMin)
   {
     modelMin->decUsed(); 
   }
   SDL_FreeSurface(model2d);
}

/**************************************************************
 *                            draw                            *
 **************************************************************/
void object::draw(float x, float z, GLfloat dist, float orientation)
{
   model3d* model = modelMax;
   
   /* Do the Static LOD, based on the Distance */
   if((dist > deltaVar) && (dist < 2*deltaVar) && (modelMed != NULL))
   {
      model = modelMed;
   }
   else if( (dist > 2*deltaVar) && (modelMin != NULL))
   {
      model = modelMin;
   }

   /* Draw the defined model */
   glEnable(GL_COLOR_MATERIAL);
   glPushMatrix();
      glTranslatef(x, 0 ,z);
      glRotatef(orientation,0,1,0);
      model->draw();
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
   return(modelMax->getBoundingBox());
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

   
