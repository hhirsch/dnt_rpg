#include "race.h"
#include <iostream>
#include <fstream>

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
race::race()
{
   image = NULL;
   name = "";
   description = "";
   totalModifiers = 0;
   raceModifiers = NULL;
   totalFeats = 0;
   raceFeats = NULL;
   totalSkills = 0;
   raceSkills = NULL;
   raceSkillsJustify = NULL;
   previous = NULL;
   next = NULL;
}

/******************************************************************
 *                             Destructor                         *
 ******************************************************************/
race::~race()
{
   if(image)
   {
      SDL_FreeSurface(image);
   }
   if(raceModifiers)
   {
      delete [] raceModifiers;
      raceModifiers = NULL;
   }
   if(raceFeats)
   {
      delete[] raceFeats;
      raceFeats = NULL;
   }
   if(raceSkills)
   {
      delete[] raceSkills;
      raceSkills = NULL;
   }
   if(raceSkillsJustify)
   {
      delete[] raceSkillsJustify;
      raceSkillsJustify = NULL;
   }
}

/******************************************************************
 *                          applySkillCosts                       *
 ******************************************************************/
void race::applySkillCosts(skills* sk)
{
   int i;
   skill* s = NULL;
   for(i=0; i<totalSkills; i++)
   {
      s = sk->getSkillByString(raceSkills[i]);
      if(s != NULL)
      {
         s->mod = 1;
      }
      else
      {
         printf("Warn: Unknow race Skill: %s\n",raceSkills[i].c_str());
      }
   }
}

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
races::races(string directory, string fileListName)
{
   std::ifstream file;
   string aux;
   char arqName[255];
   char imgFile[255];
   char idStr[255];
   int idInt;
   int total = 0;
   int i;

   totalRaces = 0;
   first = NULL;

   file.open(fileListName.c_str(), ios::in | ios::binary);

   if(!file)
   {
      printf("Error opening race list file: %s\n",fileListName.c_str());
      return;
   }

   getline(file, aux);
   sscanf(aux.c_str(), "%d", &total);

   for(i = 0; i < total; i++)
   {
      getline(file, aux);
      sscanf(aux.c_str(),"%d %s %s %s",&idInt,&arqName[0],&imgFile[0],
             &idStr[0]);
      insertRace(directory+arqName, imgFile, idStr, idInt);
   }

   
   file.close();
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
races::~races()
{
   int i;
   race* aux;
   for(i=0; i< totalRaces; i++)
   {
      aux = first;
      first = first->next;
      delete(aux);
   }
}

/******************************************************************
 *                            insertRace                         *
 ******************************************************************/
void races::insertRace(string fileName,string imgFile,string idString,int idInt)
{
   std::ifstream file;
   string str;
   int i;
   char buf1[128], buf2[128], buf3[128], buf4[128];
   race* ins = new(race);

   file.open(fileName.c_str(), ios::in | ios::binary);

   if(!file)
   {
      printf("Error while opening race file: %s\n",fileName.c_str());
      return;
   }

   /* Indentifiers */
   ins->strID = idString;
   ins->intID = idInt;

   /* Name */
   getline(file, ins->name);

   /* Citation */
   getline(file, ins->citation);

   /* Description */
   getline(file, ins->description);

   /* Image */
   ins->image = IMG_Load(imgFile.c_str());
   if(!ins->image)
   {
      printf("Error while opening image racement file: %s\n",str.c_str());
   }

   /* Modifiers */
   getline(file, str);
   sscanf(str.c_str(),"%d",&ins->totalModifiers);

   if(ins->totalModifiers > 0)
   {
      ins->raceModifiers = new modifier[ins->totalModifiers];
   }
   else
   {
      ins->raceModifiers = NULL;
   }

   for(i=0; i<ins->totalModifiers;i++)
   {
      /* Modifier */
      getline(file,str);
      sscanf(str.c_str(),"%d %s %s %s %s", &ins->raceModifiers[i].mod, &buf1[0], 
                                           &buf2[0], &buf3[0], &buf4[0]);
      ins->raceModifiers[i].cause.type = buf1;
      ins->raceModifiers[i].cause.id = buf2;
      ins->raceModifiers[i].target.type = buf3;
      ins->raceModifiers[i].target.id = buf4;
      /* Modifier Description */
      getline(file, ins->raceModifiers[i].description);
   }

   /* Feats */
   getline(file, str);
   sscanf(str.c_str(),"%d",&ins->totalFeats);

   if(ins->totalFeats > 0)
   {
      ins->raceFeats = new string[ins->totalFeats];
   }
   else
   {
      ins->raceFeats = NULL;
   }

   for(i=0; i < ins->totalFeats; i++)
   {
      getline(file, ins->raceFeats[i]);
   }

   /* Skills */
   getline(file, str);
   sscanf(str.c_str(),"%d",&ins->totalSkills);

   if(ins->totalSkills > 0)
   {
      ins->raceSkills = new string[ins->totalSkills];
      ins->raceSkillsJustify = new string[ins->totalSkills];
   }
   else
   {
      ins->raceSkills = NULL;
      ins->raceSkillsJustify = NULL;
   }

   for(i=0; i < ins->totalSkills; i++)
   {
      getline(file, ins->raceSkills[i]);
      getline(file, ins->raceSkillsJustify[i]);
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

   totalRaces++;
}

/******************************************************************
 *                          getRaceByInteger                      *
 ******************************************************************/
race* races::getRaceByInteger(int id)
{
   race* ret = first;
   int i;
   for(i = 0; i<totalRaces; i++)
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
 *                          getRaceByString                       *
 ******************************************************************/
race* races::getRaceByString(string id)
{
   race* ret = first;
   int i;
   for(i = 0; i<totalRaces; i++)
   {
      if(ret->strID == id)
      {
         return(ret);
      }
      ret = ret->next;
   }

   return(NULL);
}

