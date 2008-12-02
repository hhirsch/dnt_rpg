#include "classes.h"

#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"

#include <iostream>
#include <fstream>
using namespace std;

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
         cerr << "Warn: Unknow class Skill: '" << classSkills[i] << "'" << endl;
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
void classes::init()
{
   defParser def;
   int idInt=0;
   char classFile[256], imgFile[256];
   string key="", value="";

   totalClasses = 0;
   first = NULL;

   /* Open the list */
   if(!def.load("classes/classes.lst"))
   {
      cerr << "Error opening class list file!" << endl;
      return;
   }

   /* Get and Insert All Races */
   while(def.getNextTuple(key, value))
   {
      /* Break Value */
      sscanf(value.c_str(),"%d %s %s",&idInt,&classFile[0],&imgFile[0]);
      insertClass(classFile, imgFile, key, idInt);
   }
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
   defParser def;
   dirs dir;
   classe* ins;
   string key="", value="";
   char buf1[128], buf2[128], buf3[128], buf4[128];
   int tmpBonus=0;
   int curBonus=-1, curMod=-1, curTalent=-1, curSkill=-1;

   /* Read the file */
   if(!def.load("classes/" + fileName))
   {
      cerr << "Error while opening race file: " << fileName << endl;
      return;
   }

   /* Define Indentifiers */
   ins = new classe();
   ins->strID = idString;
   ins->intID = idInt;

   /* Define Image */
   ins->image = IMG_Load(dir.getRealFile(imgFile).c_str());
   if(!ins->image)
   {
      cerr << "Error while opening image class image: " << imgFile << endl;
   }

   /* Now parse all tokens got */
   while(def.getNextTuple(key, value))
   {
      /* Name */
      if(key == "name")
      {
         ins->name = translateDataString(value);
      }
      /* Epigraph */
      else if(key == "epigraph")
      {
         ins->citation = translateDataString(value);
      }
      /* Description */
      else if(key == "description")
      {
         ins->description = translateDataString(value);
      }
      /* LifeDice */
      else if(key == "lifeDice")
      {
         sscanf(value.c_str(), "d%d",&ins->lifeDiceID);
      }
      /* Total Modifiers */
      else if(key == "totalModifiers")
      {
         sscanf(value.c_str(), "%d", &ins->totalModifiers);
         if(ins->totalModifiers > 0)
         {
            ins->classModifiers = new modifier[ins->totalModifiers];
         }
      }
      /* Modifier */
      else if(key == "modifier")
      {
         curMod++;
         sscanf(value.c_str(),"%d %s %s %s %s", 
               &ins->classModifiers[curMod].mod,
               &buf1[0], &buf2[0], &buf3[0], &buf4[0]);
         ins->classModifiers[curMod].cause.type = buf1;
         ins->classModifiers[curMod].cause.id = buf2;
         ins->classModifiers[curMod].target.type = buf3;
         ins->classModifiers[curMod].target.id = buf4;
      }
      /* Modifier Text */
      else if(key == "modifierText")
      {
         ins->classModifiers[curMod].description = translateDataString(value);
      }
      /* TotalFeats */
      else if(key == "totalTalents")
      {
         sscanf(value.c_str(),"%d",&ins->totalFeats);
         if(ins->totalFeats > 0)
         {
            ins->classFeats = new string[ins->totalFeats];
            ins->reqLevel = new int[ins->totalFeats];
         }
      }
      /* Feat */
      else if(key == "talent")
      {
         curTalent++;
         sscanf(value.c_str(), "%d %s", &ins->reqLevel[curTalent], &buf1[0]);
         ins->classFeats[curTalent] = buf1;
      }
      /* Total Skills */
      else if(key == "totalSkills")
      {
         sscanf(value.c_str(),"%d",&ins->totalSkills);
         if(ins->totalSkills > 0)
         {
            ins->classSkills = new string[ins->totalSkills];
         }
      }
      /* Skill  */
      else if(key == "skill")
      {
         curSkill++;
         ins->classSkills[curSkill] = value;
      }
      /* First Skill Points */
      else if(key == "firstSkillPoints")
      {
         sscanf(value.c_str(), "( %d %s %s )x%d", &ins->firstLevelSP.sum, 
               &buf1[0], &buf2[0], &ins->firstLevelSP.mult);
         ins->firstLevelSP.attID = buf2;
         if(buf1[0] == '-')
         {
            ins->firstLevelSP.signal = SIGNAL_DEC;
         }
         else
         {
            ins->firstLevelSP.signal = SIGNAL_SUM;
         }
      }
      /* Next Skill Points */
      else if(key == "nextSkillPoints")
      {
         sscanf(value.c_str(), "( %d %s %s )x%d", &ins->otherLevelsSP.sum, 
               &buf1[0], &buf2[0], &ins->otherLevelsSP.mult);
         ins->otherLevelsSP.attID = buf2;
         if(buf1[0] == '-')
         {
            ins->otherLevelsSP.signal = SIGNAL_DEC;
         }
         else
         {
            ins->otherLevelsSP.signal = SIGNAL_SUM;
         }
      }
      /* Bonus Attack and Mods Per Level */
      else if(key == "bonus")
      {
         curBonus++;
         sscanf(value.c_str(), "%d,%d,%d/%d/%d",
               &ins->bonus[curBonus].level,
               &tmpBonus,
               &ins->bonus[curBonus].fortitude,
               &ins->bonus[curBonus].reflex, 
               &ins->bonus[curBonus].iAmNotAFool);
         ins->bonus[curBonus].baseAttackBonus = attackBonus(tmpBonus);
      }
      else
      {
         cerr << "Unknow token: '" << key << "' for class: " 
              << fileName << endl;
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

