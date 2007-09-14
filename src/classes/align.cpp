#include "align.h"
#include "../lang/translate.h"
#include <iostream>
#include <fstream>

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
aligns::aligns(string directory, string fileListName)
{
   std::ifstream file;
   string aux;
   char arqName[255];
   char idStr[255];
   int idInt;
   int total = 0;
   int i;

   totalAlignments = 0;
   first = NULL;

   file.open(fileListName.c_str(), ios::in | ios::binary);

   if(!file)
   {
      printf("Error on opening alignment list file: %s\n",fileListName.c_str());
      return;
   }

   getline(file, aux);
   sscanf(aux.c_str(), "%d", &total);

   for(i = 0; i < total; i++)
   {
      getline(file, aux);
      sscanf(aux.c_str(), "%d %s %s", &idInt, &arqName[0], &idStr[0]);
      insertAlign(directory+arqName, idStr, idInt);
   }

   
   file.close();
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
aligns::~aligns()
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
   std::ifstream file;
   string str;
   align* ins = new(align);

   file.open(fileName.c_str(), ios::in | ios::binary);

   if(!file)
   {
      printf("Error while opening alignment file: %s\n",fileName.c_str());
      return;
   }

   /* Indentifiers */
   ins->strID = idString;
   ins->intID = idInt;

   /* Name */
   getline(file, ins->name);

   /* Translate Name */
   ins->name = translateDataString(ins->name);

   /* Description */
   getline(file, ins->description);

   /* Translate Description */
   ins->description = translateDataString(ins->description);

   /* Image */
   getline(file, str);

   ins->image = IMG_Load(str.c_str());
   if(!ins->image)
   {
      printf("Error while opening image alignment file: %s\n",str.c_str());
   }
   
   file.close();
   
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
 *                            Constructor                         *
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

