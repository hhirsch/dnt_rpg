/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "modellist.h"
#include "../engine/culling.h"
#include "../engine/util.h"


/*************************************************************************
 *                               model3D                                 *
 *************************************************************************/

/********************************************************
 *                     Constructor                      *
 ********************************************************/
model3d::model3d(string path, string texturePath, bool staticScenery)
{
   fileName = path;
   loadModel(path);
   usedFlag = 0;
   staticFlag = staticScenery;
   positions = NULL;
   totalPositions = 0;
}

/********************************************************
 *                     Destructor                       *
 ********************************************************/
model3d::~model3d()
{
   int i;

   /* Clear Positions List */
   sceneryRenderPosition* aux;
   for(i=0; i < totalPositions; i++)
   {
      aux = positions;
      positions = positions->next;
      delete(aux);
   }
   positions = NULL;
}

/********************************************************
 *                   isStaticScenery                    *
 ********************************************************/
bool model3d::isStaticScenery()
{
   return(staticFlag);
}

/********************************************************
 *                      addPosition                     *
 ********************************************************/
void model3d::addPosition(float x, float y, float z, float angle)
{
   /* Create Position */
   sceneryRenderPosition* pos = new sceneryRenderPosition();
   pos->x = x;
   pos->y = y;
   pos->z = z;
   pos->angle = angle;

   /* Add it to the list */
   if(positions == NULL)
   {
      pos->next = pos;
      pos->previous = pos;
   }
   else
   {
      pos->next = positions;
      pos->previous = positions->previous;
      pos->next->previous = pos;
      pos->previous->next = positions;
   }

   positions = pos;
   totalPositions++;
}

/********************************************************
 *                        draw                          *
 ********************************************************/
void model3d::draw(GLfloat matriz[6][4], bool inverted)
{
   GLfloat min[3], max[3];
   GLfloat X[4], Z[4];
   boundingBox bound;

   if(positions)
   {
      int i;
      sceneryRenderPosition* pos = positions;

      /* Load Model To Graphic Card Memory */
      loadToGraphicMemory();

      /* Render All Models */
      for(i=0; i<totalPositions; i++)
      {
         /* Do View Frustum Culling */
         bound = getBoundingBox();
         X[0] = bound.x1;
         Z[0] = bound.z1;
         X[1] = bound.x1;
         Z[1] = bound.z2; 
         X[2] = bound.x2;
         Z[2] = bound.z2;
         X[3] = bound.x2;
         Z[3] = bound.z1;
         rotTransBoundingBox(pos->angle, X, Z, pos->x, 
                             pos->y+bound.y1, pos->y+bound.y2, 
                             pos->z, min, max );
         if(visibleCube(min[0],min[1],min[2],max[0],max[1],max[2],
                           matriz))
         {
            /* Is visible, so render */
            glPushMatrix();
               glTranslatef(pos->x, pos->y, pos->z);
               glRotatef(pos->angle, 0,1,0);
               renderFromGraphicMemory();
            glPopMatrix();
         }

         /* Reflected Draw */
         if(inverted)
         {
            /* Do Clulling */
            rotTransBoundingBox(pos->angle, X, Z, pos->x, 
                                pos->y-bound.y2, pos->y-bound.y1, 
                                pos->z, min, max);
            if(visibleCube(min[0],min[1],min[2],max[0],max[1],max[2],
                           matriz))
            {
               /* Is visible, so render the reflexion */
               glEnable(GL_STENCIL_TEST);
               glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
               glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
               glEnable(GL_NORMALIZE);
               glPushMatrix();
                  glTranslatef(pos->x, pos->y, pos->z);
                  glRotatef(pos->angle, 0,1,0);
                  glScalef(1.0,-1.0,1.0);
                  renderFromGraphicMemory();
               glPopMatrix();
               glDisable(GL_NORMALIZE);
               glDisable(GL_STENCIL_TEST);
            }
         }

         pos = pos->next;
      }

      /* remove Model from graphic card memory */
      removeFromGraphicMemory();
   }
}

/********************************************************
 *                        draw                          *
 ********************************************************/
void model3d::draw()
{
   render();
   if(staticFlag)
   {
      //printf("Warning: Rendering Static as Non Static!\n");
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
   while(first)
   {
      removeModel(first);
   }
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
model3d* modelList::addModel(string path, string texturePath,
                             bool staticScenery)
{
   model3d* mdl = findModel(path);
   if(mdl)
   {
      /* Already Inserted! */
      return(mdl);
   }

   /* Create model */
   mdl = new model3d(path, texturePath, staticScenery);


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
   model3d* aux = NULL;
   int i, total;

   total = totalModels;
   
   for(i = 0; i < total; i++)
   {
      if(mdl->getUsedFlag() <= 0)
      {
         aux = mdl;
         mdl = mdl->next;
         removeModel(aux);
      }
      else
      {
         mdl = mdl->next;
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

