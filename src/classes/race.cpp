#include "race.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"

#include <iostream>
#include <fstream>
using namespace std;

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
         cerr << "Warn: Unknow race Skill: '" << raceSkills[i] << "'" << endl;
      }
   }
}

/******************************************************************
 *                          getAttModifiers                       *
 ******************************************************************/
void race::getAttModifiers(int mods[6], bool sum, skills* sk)
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
      if(raceModifiers[i].cause.type == MOD_TYPE_ATT)
      {
         n = sk->getSkillIntByString(raceModifiers[i].cause.id);
         if((n > 0) && (n <=6 ) )
         {
            mods[n-1] += raceModifiers[i].mod;
         }
         else
         {
            cerr << "Unknow Attribute: " << raceModifiers[i].cause.id  << endl;
         }
      }
   }

}

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
void races::init()
{
   defParser def;
   int idInt=0;
   char raceFile[256], imgFile[256];
   string key="", value="";

   totalRaces = 0;
   first = NULL;

   /* Open the list */
   if(!def.load("races/races.lst"))
   {
      cerr << "Error opening race list file!" << endl;
      return;
   }

   /* Get and Insert All Races */
   while(def.getNextTuple(key, value))
   {
      /* Break Value */
      sscanf(value.c_str(),"%d %s %s",&idInt,&raceFile[0],&imgFile[0]);
      insertRace(raceFile, imgFile, key, idInt);
   }
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
void races::finish()
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
   defParser def;
   dirs dir;
   race* ins;
   string key="", value="";
   char buf1[128], buf2[128], buf3[128], buf4[128];
   int curMod=-1, curTalent=-1, curSkill=-1;

   /* Read the file */
   if(!def.load("races/" + fileName))
   {
      cerr << "Error while opening race file: " << fileName << endl;
      return;
   }

   /* Define Indentifiers */
   ins = new race();
   ins->strID = idString;
   ins->intID = idInt;

   /* Define Image */
   ins->image = IMG_Load(dir.getRealFile(imgFile).c_str());
   if(!ins->image)
   {
      cerr << "Error while opening image race image: " << imgFile << endl;
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
      /* Total Modifiers */
      else if(key == "totalModifiers")
      {
         sscanf(value.c_str(), "%d", &ins->totalModifiers);
         if(ins->totalModifiers > 0)
         {
            ins->raceModifiers = new modifier[ins->totalModifiers];
         }
      }
      /* Modifier */
      else if(key == "modifier")
      {
         curMod++;
         sscanf(value.c_str(),"%d %s %s %s %s", &ins->raceModifiers[curMod].mod,
                &buf1[0], &buf2[0], &buf3[0], &buf4[0]);
         ins->raceModifiers[curMod].cause.type = buf1;
         ins->raceModifiers[curMod].cause.id = buf2;
         ins->raceModifiers[curMod].target.type = buf3;
         ins->raceModifiers[curMod].target.id = buf4;
      }
      /* Modifier Text */
      else if(key == "modifierText")
      {
         ins->raceModifiers[curMod].description = translateDataString(value);
      }
      /* TotalFeats */
      else if(key == "totalTalents")
      {
         sscanf(value.c_str(),"%d",&ins->totalFeats);
         if(ins->totalFeats > 0)
         {
            ins->raceFeats = new string[ins->totalFeats];
         }
      }
      /* Feat */
      else if(key == "talent")
      {
         curTalent++;
         ins->raceFeats[curTalent] = value;
      }
      /* Total Skills */
      else if(key == "totalSkills")
      {
         sscanf(value.c_str(),"%d",&ins->totalSkills);
         if(ins->totalSkills > 0)
         {
            ins->raceSkills = new string[ins->totalSkills];
            ins->raceSkillsJustify = new string[ins->totalSkills];
         }
      }
      /* Skill  */
      else if(key == "skill")
      {
         curSkill++;
         ins->raceSkills[curSkill] = value;
      }
      /* Skill Text */
      else if(key == "skillText")
      {
         ins->raceSkillsJustify[curSkill] = value;
      }
      else
      {
         cerr << "Unknow Token '" << key << "' at race: " << fileName << endl;
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

/******************************************************************
 *                          getTotalRaces                         *
 ******************************************************************/
int races::getTotalRaces()
{
   return(totalRaces);
}

/******************************************************************
 *                          Static Members                        *
 ******************************************************************/
int races::totalRaces = 0; 
race* races::first = NULL;

