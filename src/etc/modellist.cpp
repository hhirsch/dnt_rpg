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
   loadModel(path);
   usedFlag = 0;
}

/********************************************************
 *                     Destructor                       *
 ********************************************************/
model3d::~model3d()
{
}

/********************************************************
 *                        draw                          *
 ********************************************************/
void model3d::draw()
{
   render();
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
   calculateBoundingBox();
   b.x1 = min[0];
   b.x2 = max[0];
   b.z1 = min[2];
   b.z2 = max[2];
   b.y1 = min[1];
   b.y2 = max[1];
   return(b);
}

void model3d::callDeadAnimation()
{
   //TODO
}

void model3d::callAttackAnimation()
{
   //TODO
}

void model3d::callIdleAnimation()
{
   //TODO
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
   model3d* mdl;
   model3d* oth;

   if(first)
   {
      mdl = first->next;
      while( (first != NULL) && (mdl != first) )
      {
         oth = mdl;
         mdl = mdl->next;
         if(oth->getUsedFlag() <= 0)
         {
            removeModel(oth);
         }
      }
      if((first != NULL) && (first->getUsedFlag() <= 0))
      {
         removeModel(first);
      }
   }
   printAll();
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

