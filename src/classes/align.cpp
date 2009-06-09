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

#include "align.h"

#include "../lang/translate.h"
#include "../etc/defparser.h"

#include <iostream>
#include <fstream>
using namespace std;

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
void aligns::init()
{
   defParser def;
   int idInt=0;
   char alignFile[256];
   string key="", value="";

   totalAlignments = 0;
   first = NULL;

   /* Open the list */
   if(!def.load("alignment/alignment.lst"))
   {
      cerr << "Error opening alignment list file!" << endl;
      return;
   }

   /* Get and Insert All Aligns */
   while(def.getNextTuple(key, value))
   {
      /* Break Value */
      sscanf(value.c_str(),"%d %s",&idInt,&alignFile[0]);
      insertAlign(alignFile, key, idInt);
   }
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
void aligns::finish()
{
   int i;
   align* aux;
   for(i=0; i< totalAlignments; i++)
   {
      aux = first;
      first = first->next;
      delete(aux);
   }
}

/******************************************************************
 *                            insertAlign                         *
 ******************************************************************/
void aligns::insertAlign(string fileName, string idString, int idInt)
{
   dirs dir;
   defParser def;
   string key="", value="";
   align* ins;

   if(!def.load("alignment/"+fileName))
   {
      cerr << "Error while opening alignment file: " << fileName << endl;
      return;
   }

   /* Create the alignment */
   ins = new(align);

   /* Indentifiers */
   ins->strID = idString;
   ins->intID = idInt;
 
   while(def.getNextTuple(key, value))
   {
      /* Name */
      if(key == "name")
      {
         ins->name = translateDataString(value);
      }
      /* Description */
      else if(key == "description")
      {
         ins->description = translateDataString(value);
      }
      /* Image */
      else if(key == "image")
      {
         ins->image = IMG_Load(dir.getRealFile(value).c_str());
         if(!ins->image)
         {
            cerr << "Error opening image alignment file: " << value << endl;
         }
      }
   }

   /* Pointers */
   if(first == NULL)
   {
      first = ins;
      ins->next = ins;
      ins->previous = ins;
   }
   else
   {
      ins->next = first;
      ins->previous = first->previous;
      first->previous = ins;
      ins->previous->next = ins;
   }

   totalAlignments++;
}


/******************************************************************
 *                         getAlignByInteger                      *
 ******************************************************************/
align* aligns::getAlignByInteger(int id)
{
   align* ret = first;
   int i;
   for(i = 0; i<totalAlignments; i++)
   {
      if(ret->intID == id)
      {
         return(ret);
      }
      ret = ret->next;
   }

   return(NULL);
}

/******************************************************************
 *                          getAlignByString                      *
 ******************************************************************/
align* aligns::getAlignByString(string id)
{
   align* ret = first;
   int i;
   for(i = 0; i<totalAlignments; i++)
   {
      if(ret->strID == id)
      {
         return(ret);
      }
      ret = ret->next;
   }

   return(NULL);
}

/******************************************************************
 *                           Static Members                       *
 ******************************************************************/
int aligns::totalAlignments = 0;
align* aligns::first = NULL;

