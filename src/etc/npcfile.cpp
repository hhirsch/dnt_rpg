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
   FILE* arq;
   int i, totalNpcs;
   char name[50];
   char npcFile[255];
   GLfloat posX, posZ, angle;
   dirs dir;
   npcFileName = fileName;

   arq = fopen(dir.getRealFile(fileName).c_str(),"r");
   if(arq == NULL)
   {
      cerr << "Can't open NPCs File: " << fileName << endl;
      return(false);
   }

   /* Get total Npcs defined on the file */
   fscanf(arq,"%d",&totalNpcs);
   for(i = 0; i < totalNpcs; i++)
   {
      /* Get each npc definition */
      fscanf(arq,"%s %s %f %f %f",&name[0],&npcFile[0],&posX,&posZ,&angle);
      insertCharacter(name, npcFile, posX, posZ, angle);
   }

   /* Close file */
   fclose(arq);
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

