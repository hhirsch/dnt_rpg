#include "classes.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include <iostream>
#include <fstream>

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
classe::classe()
{
   image = NULL;
   name = "";
   description = "";
   totalModifiers = 0;
   classModifiers = NULL;
   totalFeats = 0;
   classFeats = NULL;
   totalSkills = 0;
   classSkills = NULL;
   reqLevel = NULL;
   previous = NULL;
   next = NULL;
}

/******************************************************************
 *                             Destructor                         *
 ******************************************************************/
classe::~classe()
{
   if(image)
   {
      SDL_FreeSurface(image);
   }
   if(classModifiers)
   {
      delete [] classModifiers;
      classModifiers = NULL;
   }
   if(classFeats)
   {
      delete[] classFeats;
      classFeats = NULL;
   }
   if(reqLevel)
   {
      delete[] reqLevel;
      reqLevel = NULL;
   }
   if(classSkills)
   {
      delete[] classSkills;
      classSkills = NULL;
   }
}

/******************************************************************
 *                          applySkillCosts                       *
 ******************************************************************/
void classe::applySkillCosts(skills* sk)
{
   int i;
   skill* s = NULL;
   for(i=0; i<totalSkills; i++)
   {
      s = sk->getSkillByString(classSkills[i]);
      if(s != NULL)
      {
         s->mod = 1;
      }
      else
      {
         cerr << "Warn: Unknow class Skill: " << classSkills[i] << endl;
      }
   }
}

/******************************************************************
 *                          getAttModifiers                       *
 ******************************************************************/
void classe::getAttModifiers(int mods[6], bool sum, skills* sk)
{
   int i, n;
   /* If isn't to sum, zero's to the mods */
   if(!sum)
   {
      for(i=0; i<6; i++)
      {
         mods[i] = 0;
      }
   }
   
   /* Now Apply all attributes modifiers */
   for(i=0; i < totalModifiers; i++)
   {
      if(classModifiers[i].cause.type == MOD_TYPE_ATT)
      {
         n = sk->getSkillIntByString(classModifiers[i].cause.id);
         if((n > 0) && (n <=6 ) )
         {
            mods[n-1] += classModifiers[i].mod;
         }
         else
         {
            cerr << "Unknow Attribute:" << classModifiers[i].cause.id << endl;
         }
      }
   }

}

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
void classes::init(string directory, string fileListName)
{
   std::ifstream file;
   string aux;
   char arqName[255];
   char imgFile[255];
   char idStr[255];
   int idInt;
   int total = 0;
   int i;

   totalClasses = 0;
   first = NULL;

   file.open(fileListName.c_str(), ios::in | ios::binary);

   if(!file)
   {
      cerr << "Error opening class list file: " << fileListName << endl;
      return;
   }

   getline(file, aux);
   sscanf(aux.c_str(), "%d", &total);

   for(i = 0; i < total; i++)
   {
      getline(file, aux);
      sscanf(aux.c_str(),"%d %s %s %s", &idInt, &arqName[0], &imgFile[0], 
             &idStr[0]);
      insertClass(directory+arqName, imgFile, idStr, idInt);
   }

   
   file.close();
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
void classes::finish()
{
   int i;
   classe* aux;
   for(i=0; i< totalClasses; i++)
   {
      aux = first;
      first = first->next;
      delete(aux);
   }
}

/******************************************************************
 *                            insertClass                         *
 ******************************************************************/
void classes::insertClass(string fileName, string imgFile, string idString,
                          int idInt)
{
   dirs dir;
   std::ifstream file;
   string str;
   int i;
   char buf1[128], buf2[128], buf3[128], buf4[128];
   classe* ins = new(classe);

   file.open(fileName.c_str(), ios::in | ios::binary);

   if(!file)
   {
      cerr << "Error while opening class file: " << fileName << endl;
      return;
   }

   /* Indentifiers */
   ins->strID = idString;
   ins->intID = idInt;

   /* Name */
   getline(file, ins->name);

   /* Translate Name */
   ins->name = translateDataString(ins->name);

   /* Citation */
   getline(file, ins->citation);

   /* Translate Citation */
   ins->citation = translateDataString(ins->citation);

   /* Description */
   getline(file, ins->description);

   /* Translate Description */
   ins->description = translateDataString(ins->description);

   /* Life Dice */
   getline(file, str);
   sscanf(str.c_str(), "d%d",&ins->lifeDiceID);

   ins->image = IMG_Load(dir.getRealFile(imgFile).c_str());
   if(!ins->image)
   {
      cerr << "Error while opening image class file: " << imgFile << endl;
   }

   /* Modifiers */
   getline(file, str);
   sscanf(str.c_str(),"%d",&ins->totalModifiers);

   if(ins->totalModifiers > 0)
   {
      ins->classModifiers = new modifier[ins->totalModifiers];
   }
   else
   {
      ins->classModifiers = NULL;
   }

   for(i=0; i<ins->totalModifiers;i++)
   {
      /* Modifier */
      getline(file,str);
      sscanf(str.c_str(),"%d %s %s %s %s",&ins->classModifiers[i].mod, &buf1[0],
                                          &buf2[0], &buf3[0], &buf4[0]);
      ins->classModifiers[i].cause.type = buf1;
      ins->classModifiers[i].cause.id = buf2;
      ins->classModifiers[i].target.type = buf3;
      ins->classModifiers[i].target.id = buf4;
      /* Modifier Description */
      getline(file, ins->classModifiers[i].description);
      ins->classModifiers[i].description = translateDataString(
                                           ins->classModifiers[i].description);
   }

   /* Feats */
   getline(file, str);
   sscanf(str.c_str(),"%d",&ins->totalFeats);

   if(ins->totalFeats > 0)
   {
      ins->classFeats = new string[ins->totalFeats];
      ins->reqLevel = new int[ins->totalFeats];
   }
   else
   {
      ins->classFeats = NULL;
      ins->reqLevel = NULL;
   }

   for(i=0; i < ins->totalFeats; i++)
   {
      getline(file, str);
      sscanf(str.c_str(), "%d %s", &ins->reqLevel[i], &buf1[0]);
      ins->classFeats[i] = buf1;
   }

   /* Skills */
   getline(file, str);
   sscanf(str.c_str(),"%d",&ins->totalSkills);

   if(ins->totalSkills > 0)
   {
      ins->classSkills = new string[ins->totalSkills];
   }
   else
   {
      ins->classSkills = NULL;
   }

   for(i=0; i < ins->totalSkills; i++)
   {
      getline(file, ins->classSkills[i]);
   }

   /* Skill Points per Level */
   getline(file, str);
   sscanf(str.c_str(), "( %d %s %s )x%d", &ins->firstLevelSP.sum, &buf1[0], 
                                        &buf2[0], &ins->firstLevelSP.mult);
   ins->firstLevelSP.attID = buf2;
   if(buf1[0] == '-')
   {
      ins->firstLevelSP.signal = SIGNAL_DEC;
   }
   else
   {
      ins->firstLevelSP.signal = SIGNAL_SUM;
   }
   getline(file, str);
   sscanf(str.c_str(), "( %d %s %s )x%d", &ins->otherLevelsSP.sum, &buf1[0], 
                                        &buf2[0], &ins->otherLevelsSP.mult);
   ins->otherLevelsSP.attID = buf2;
   if(buf1[0] == '-')
   {
      ins->otherLevelsSP.signal = SIGNAL_DEC;
   }
   else
   {
      ins->otherLevelsSP.signal = SIGNAL_SUM;
   }

   /* Points and Attacks per Level */
   for(i=0; i<MAX_DEFINED_LEVEL; i++)
   {
      getline(file, str);
      sscanf(str.c_str(), "%d,%d/%d/%d/%d/%d/%d,%d/%d/%d",
             &ins->bonus[i].level,
             &ins->bonus[i].baseAttackBonus[0],
             &ins->bonus[i].baseAttackBonus[1],
             &ins->bonus[i].baseAttackBonus[2],
             &ins->bonus[i].baseAttackBonus[3],
             &ins->bonus[i].baseAttackBonus[4],
             &ins->bonus[i].baseAttackBonus[5],
             &ins->bonus[i].fortSave,
             &ins->bonus[i].refSave, 
             &ins->bonus[i].willSave);
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

   totalClasses++;
}

/******************************************************************
 *                         getClassByInteger                      *
 ******************************************************************/
classe* classes::getClassByInteger(int id)
{
   classe* ret = first;
   int i;
   for(i = 0; i<totalClasses; i++)
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
 *                          getClassByString                      *
 ******************************************************************/
classe* classes::getClassByString(string id)
{
   classe* ret = first;
   int i;
   for(i = 0; i<totalClasses; i++)
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
 *                          getTotalClasses                       *
 ******************************************************************/
int classes::getTotalClasses()
{
   return(totalClasses);
}

/******************************************************************
 *                          static members                        *
 ******************************************************************/
int classes::totalClasses = 0;
classe* classes::first = NULL;

