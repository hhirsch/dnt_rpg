/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "modellist.h"


/*************************************************************************
 *                               model3D                                 *
 *************************************************************************/

/********************************************************
 *                     Constructor                      *
 ********************************************************/
model3d::model3d(string path, string texturePath)
{
   fileName = path;
   model = glmReadOBJ(path.c_str(), texturePath.c_str(), 0);
   if(!model)
   {
      model = NULL;
      printf("Error on Openning Model: %s\n", path.c_str());
      printf("Maybe will crash soon!\n");
   }
   usedFlag = 0;
}

/********************************************************
 *                     Destructor                       *
 ********************************************************/
model3d::~model3d()
{
   if(model)
   {
      glmDelete(model);
   }
   else
   {
      printf("Model: %s can't remove Model!\n", fileName.c_str());
   }
}

/********************************************************
 *                        draw                          *
 ********************************************************/
void model3d::draw()
{
   if(model)
   {
      glmDraw(model);
   }
}

/********************************************************
 *                      incUsed                         *
 ********************************************************/
void model3d::incUsed()
{
   usedFlag++;
}

/********************************************************
 *                       decUsed                        *
 ********************************************************/
void model3d::decUsed()
{
   usedFlag--;
   if(usedFlag < 0)
   {
      printf("Used flag for model %s is underflow!\n", fileName.c_str());
   }
}

/********************************************************
 *                     getUsedFlag                      *
 ********************************************************/
int model3d::getUsedFlag()
{
   return(usedFlag);
}

/********************************************************
 *                     getFileName                      *
 ********************************************************/
string model3d::getFileName()
{
   return(fileName);
}

/********************************************************
 *                    getBoundingBox                    *
 ********************************************************/
boundingBox model3d::getBoundingBox()
{
   boundingBox b;
   b.x1 = model->x1;
   b.x2 = model->x2;
   b.z1 = model->z1;
   b.z2 = model->z2;
   b.y1 = model->y1;
   b.y2 = model->y2;
   return(b);
}


/*************************************************************************
 *                              modelList                                *
 *************************************************************************/

/********************************************************
 *                     Constructor                      *
 ********************************************************/
modelList::modelList()
{
   first = NULL;
   totalModels = 0;
}

/********************************************************
 *                     Destructor                       *
 ********************************************************/
modelList::~modelList()
{
   model3d* mdl;
   int i;
   for(i=0; i<totalModels; i++)
   {
      mdl = first;
      first = first->next;
      delete(mdl);
   }
   first = NULL;
   totalModels = 0;
}

/********************************************************
 *                     findModel                       *
 ********************************************************/
model3d* modelList::findModel(string path)
{
   model3d* mdl = first;
   int i;
   for(i=0; i<totalModels;i++)
   {
      if(mdl->getFileName() == path)
      {
         return(mdl);
      }
      mdl = mdl->next;
   }
   return(NULL);
}

/********************************************************
 *                       addModel                       *
 ********************************************************/
model3d* modelList::addModel(string path, string texturePath)
{
   model3d* mdl = findModel(path);
   if(mdl)
   {
      /* Already Inserted! */
      return(mdl);
   }

   mdl = new model3d(path, texturePath);


   if(totalModels == 0)
   {
      first = mdl;
      mdl->next = first;
      mdl->previous = first;
   }
   else
   {
      mdl->next = first;
      mdl->previous = first->previous;
      mdl->previous->next = mdl;
      mdl->next->previous = mdl;
      first = mdl; 
   }

   totalModels++;

   return(mdl);
}

/********************************************************
 *                     removeModel                      *
 ********************************************************/
void modelList::removeModel(model3d* mdl)
{
   if(mdl != NULL)
   {

      if(mdl == first)
      {
         first = mdl->next;
      }
      mdl->next->previous = mdl->previous;
      mdl->previous->next = mdl->next;
      delete(mdl);
      totalModels--;
      if(totalModels == 0)
      {
         first = NULL;
      }
   }
}

/********************************************************
 *                       getFirst                       *
 ********************************************************/
model3d* modelList::getFirst()
{
   return(first);
}

/********************************************************
 *                  removeUnusedModels                  *
 ********************************************************/
void modelList::removeUnusedModels()
{
   model3d* mdl = first;
   model3d* oth;

   if(first)
   {
      while( (first != NULL) && (mdl->next != first) )
      {
         oth = mdl;
         mdl = mdl->next;
         if(oth->getUsedFlag() <= 0)
         {
            printf("Will remove: %s\n", oth->getFileName().c_str());
            removeModel(oth);
         }
      }
      if((first != NULL) && (first->getUsedFlag() <= 0))
      {
         removeModel(first);
      }
   }
}

/********************************************************
 *                       printAll                       *
 ********************************************************/
void modelList::printAll()
{
   int i;
   model3d* mdl = first;
   printf("*****************************************************\n");
   printf("*              Current Models on List               *\n");
   for(i = 0; i < totalModels; i++)
   {
      printf("* Obj: %s\n*\tUsed for: %d\n", mdl->getFileName().c_str(), 
                                             mdl->getUsedFlag());
      mdl = mdl->next;
   }
   printf("*****************************************************\n\n");
}

