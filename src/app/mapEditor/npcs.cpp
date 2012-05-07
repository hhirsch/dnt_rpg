/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "npcs.h"

#include "../../engine/util.h"

#define NPCS_STATE_NONE    0
#define NPCS_STATE_ADD     1

using namespace std;
using namespace dntMapEditor;

/******************************************************
 *                      Constructor                   *
 ******************************************************/
Npcs::Npcs(Map* acMap, characterList* NPCsList, featsList* lFeats)
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
Npcs::~Npcs()
{
   actualMap = NULL;
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void Npcs::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
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

      /* Changed Character Position */
      if( (mX != mouseXw) || (mY != mouseYw))
      {
         /* Set the new position */
         mX = mouseXw;
         mY = mouseYw;
         actualNpc->scNode->setPosition(mouseX,
               actualMap->getHeight(mouseX, mouseZ),mouseZ); 
      }

      /* Rotate the NPC + */
      if( (mButton & SDL_BUTTON(2)) && (actualNpc != NULL) )
      {
         actualNpc->scNode->setAngle(0.0f, 
               ((int)(actualNpc->scNode->getAngleY()+1))%360,0.0f);
      }

      /* Rotate the NPC - */
      if( (mButton & SDL_BUTTON(3)) && (actualNpc != NULL) )
      {
         actualNpc->scNode->setAngle(0.0f, 
               ((int)(actualNpc->scNode->getAngleY()-1))%360,0.0f);
      }
     
      /* Insert the NPC */
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
void Npcs::drawTemporary()
{
}

/******************************************************************
 *                           insertNpc                            *
 ******************************************************************/
void Npcs::insertNpc(GLfloat xReal, GLfloat zReal,
                     character* npc, int qx, int qz)
{
   character* per;
   per = NPCs->insertCharacter(npcFile,features, NULL, "");
   per->scNode->set(xReal, actualMap->getHeight(xReal, zReal), zReal,
      0.0f, npc->scNode->getAngleY(), 0.0f);
}

/******************************************************************
 *                        defineActualNpc                         *
 ******************************************************************/
void Npcs::defineActualNpc(string fileName)
{
   if(npcFile != fileName)
   {
      delete(intList);
      intList = new(characterList);
      actualNpc = intList->insertCharacter(fileName, features, NULL, "");
      npcFile = fileName;
   }
}

/******************************************************************
 *                            deleteNpcs                          *
 ******************************************************************/
void Npcs::deleteNpc()
{
   if(actualNpc)
   {
      intList->removeCharacter(actualNpc);
      npcFile = "";
      actualNpc = NULL;
   }
}

/******************************************************************
 *                       getObjectFileName                        *
 ******************************************************************/
string Npcs::getNpcFileName()
{
   return(npcFile);
}

/******************************************************************
 *                             saveFile                           *
 ******************************************************************/
bool Npcs::saveFile(string fileName)
{
   FILE* arq;
   string saveName;

   if(!(arq = fopen(fileName.c_str(),"w")))
   {
      return(false);
   }
   else
   {
      int npc;
      fprintf(arq,"%d\n",NPCs->getTotal());
      character* per = (character*) NPCs->getFirst();
      for(npc = 0; npc < NPCs->getTotal(); npc++)
      {
         saveName = replaceSpaces(per->name);
         fprintf(arq,"%s %s %.3f %.3f %.3f %d\n", saveName.c_str(),
                 per->getCharacterFile().c_str(),
                 per->scNode->getPosX(),
                 per->scNode->getPosZ(), 
                 per->scNode->getAngleY(),
                 per->getPsychoState());
         per = (character*) per->getNext();
       }
       fclose(arq);
   }
   return(true);
}

