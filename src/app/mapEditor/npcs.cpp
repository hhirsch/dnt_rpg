#include "npcs.h"

#define NPCS_STATE_NONE    0
#define NPCS_STATE_ADD     1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
npcs::npcs(Map* acMap, characterList* NPCsList, featsList* lFeats)
{
   actualMap = acMap;
   state = NPCS_STATE_NONE;
   actualNpc = NULL;
   npcFile = "";
   intList = new(characterList);
   NPCs = NPCsList;
   features = lFeats;
}

/******************************************************
 *                       Destructor                   *
 ******************************************************/
npcs::~npcs()
{
   actualMap = NULL;
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void npcs::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                           Uint8 mButton, int mouseXw, int mouseYw,
                           int tool, GLdouble proj[16], 
                           GLdouble modl[16], GLint viewPort[4])
{
   if(actualNpc == NULL)
   {
      return;
   }

   if(tool == TOOL_NPC_ADD)
   {
      state = NPCS_STATE_ADD;

      if( (mX != mouseXw) || (mY != mouseYw))
      {
         mX = mouseXw;
         mY = mouseYw;
         actualNpc->xPosition = mouseX;
         actualNpc->zPosition = mouseZ;
         actualNpc->yPosition = actualMap->getHeight(mouseX, mouseZ);
      }
      
      if( (mButton & SDL_BUTTON(1)) && (actualNpc != NULL) )
      {
         insertNpc(mouseX, mouseZ, actualNpc, 
                   (int)(mouseX / actualMap->squareSize()), 
                   (int)(mouseZ / actualMap->squareSize()));
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
   }
   else
   {
      state = NPCS_STATE_NONE;
   }
}

/******************************************************************
 *                         drawTemporary                          *
 ******************************************************************/
void npcs::drawTemporary()
{
   if(actualNpc)
   {
      actualNpc->update(WALK_UPDATE/*seconds*/);   
      actualNpc->calculateBoundingBox();
      glPushMatrix();
         glTranslatef(actualNpc->xPosition, 
                      actualNpc->yPosition,
                      actualNpc->zPosition);
         glRotatef(actualNpc->orientation,0,1,0);
         actualNpc->render();
      glPopMatrix();
   }
}

/******************************************************************
 *                           insertNpc                            *
 ******************************************************************/
void npcs::insertNpc(GLfloat xReal, GLfloat zReal,
                     character* npc, int qx, int qz)
{
   character* per;
   per = NPCs->insertCharacter(npcFile,features, NULL, "");
   per->xPosition = xReal;
   per->zPosition = zReal;
   per->yPosition = actualMap->getHeight(xReal, zReal);
   per->update(0); 
   per->calculateBoundingBox();
}

/******************************************************************
 *                       defineActualObject                       *
 ******************************************************************/
void npcs::defineActualNpc(string fileName)
{
   if(npcFile != fileName)
   {
      delete(intList);
      intList = new(characterList);
      actualNpc = intList->insertCharacter(fileName, features, NULL, "");
      actualNpc->update(0); 
      actualNpc->calculateBoundingBox();
      npcFile = fileName;
   }
}

/******************************************************************
 *                       getObjectFileName                        *
 ******************************************************************/
string npcs::getNpcFileName()
{
   return(npcFile);
}

/******************************************************************
 *                             saveFile                           *
 ******************************************************************/
bool npcs::saveFile(string fileName)
{
   FILE* arq;
   if(!(arq = fopen(fileName.c_str(),"w")))
   {
      return(false);
   }
   else
   {
      int npc;
      fprintf(arq,"%d\n",NPCs->getTotal());
      character* per = (character*) NPCs->first->next;
      for(npc = 0; npc < NPCs->getTotal(); npc++)
      {
         fprintf(arq,"%s %s %f %f\n",per->name.c_str(),
                 per->getCharacterFile().c_str(),
                 per->xPosition,per->zPosition);
         per = (character*) per->next;
       }
       fclose(arq);
   }
   return(true);
}

