/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "npcfile.h"
#include "dirs.h"

#include "../engine/modstate.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
using namespace std;


/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
npcFile::npcFile()
{
   current = NULL;
   list = NULL;
   total = 0;
   npcFileName = "";
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
npcFile::~npcFile()
{
   int i;
   current = list;

   /* Delete all list elements */
   for(i=0; i < total; i++)
   {
      current = current->next;
      delete(current->previous);
   }
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool npcFile::load(string fileName)
{
   ifstream arq;
   string strBuffer;
   int i, totalNpcs;
   char name[50];
   char npcFile[255];
   GLfloat posX, posZ, angle;
   dirs dir;
   npcFileName = fileName;

   arq.open(dir.getRealFile(fileName).c_str(), ios::in | ios::binary);
   if(!arq)
   {
      cerr << "Can't open NPCs File: " << fileName << endl;
      return(false);
   }

   /* Get total Npcs defined on the file */
   getline(arq, strBuffer);
   sscanf(strBuffer.c_str(),"%d",&totalNpcs);
   for(i = 0; i < totalNpcs; i++)
   {
      /* Get each npc definition */
      getline(arq, strBuffer);
      sscanf(strBuffer.c_str(),"%s %s %f %f %f",
             &name[0],&npcFile[0],&posX,&posZ,&angle);
      insertCharacter(name, npcFile, posX, posZ, angle);
   }

   /* Close file */
   arq.close();
   return(true);
}

/***********************************************************************
 *                                save                                 *
 ***********************************************************************/
bool npcFile::save(string fileName)
{
   //TODO
   return(false);
}

/***********************************************************************
 *                          insertCharacter                            *
 ***********************************************************************/
void npcFile::insertCharacter(string name, string fileName, GLfloat posX, 
                              GLfloat posZ, GLfloat angle)
{
   npcParseStruct* npc = new npcParseStruct();
   npc->name = name;
   npc->fileName = fileName;
   npc->posX = posX;
   npc->posZ = posZ;
   npc->angle = angle;

   /* Really insert at the list */
   if(list)
   {
      npc->next = list;
      npc->previous = list->previous;
      npc->previous->next = npc;
      npc->next->previous = npc;
   }
   else
   {
      npc->next = npc;
      npc->previous = npc;
   }

   list = npc;
   total++;
}

/***********************************************************************
 *                           getNextCharacter                          *
 ***********************************************************************/
bool npcFile::getNextCharacter(string& name, string& fileName, GLfloat& posX, 
                               GLfloat& posZ, GLfloat& angle)
{
   bool res = false;
   
   if(total > 0)
   {
      if(!current)
      {
         current = list;
         res = true;
      }
      else
      {
         current = current->next;
         res = (current != list);
      }
   }

   /* If defined, so get the return values */
   if(res)
   {
      name = current->name;
      fileName = current->fileName;
      posX = current->posX;
      posZ = current->posZ;
      angle = current->angle;
   }

   return(res);
}

/***********************************************************************
 *                                killAll                              *
 ***********************************************************************/
void npcFile::killAll()
{
   npcParseStruct* npc = list;
   int i;
   string mapFileName = "";
   modState modif;

   /* FIXME: trick to get mapFile, removing the ".npc" */
   if(npcFileName.length() > 4)
   {
      mapFileName = npcFileName;
      mapFileName.erase(mapFileName.length()-4, 4);
   }

   /* Tell modState that All Elements are dead */
   for(i=0; i < total; i++)
   {
      modif.mapCharacterAddAction(MODSTATE_ACTION_CHARACTER_DEAD,
                                  npc->fileName, mapFileName,
                                  npc->posX, 0.0, npc->posZ, npc->angle,
                                  npc->posX, npc->posZ);

      npc = npc->next;
   }
}

