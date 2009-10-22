/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#include <math.h>
#include <libintl.h>
#include <iostream>

using namespace std;

#include "skills.h"
#include "defs.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include "../engine/briefing.h"

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                         SkillsDefinitions                             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************
 *                          Init                             *
 *************************************************************/
void skillsDefinitions::init()
{
   dirs dir;
   string strBuffer;
   char buf[128];
   char buf2[128];
   int num;
   ifstream desc;
   string descFile, imgFile;

   defParser def;
   string key="", value="";

   /* Load the definitions file */ 
   if(!def.load("skills/skills.skl"))
   {
      cerr << "Error: Can't parse skills.skl file!" << endl;
      return;
   }

   /* Define total skills */
   def.getNextTuple(key, value);
   if(key != "totalSkills")
   {
      cerr << "Error: Invalid skills.skl file!" << endl;
      return;
   }
   sscanf(value.c_str(), "%d", &totalSkills);

   /* Create the skills space */
   if(totalSkills > 0)
   {
      skillsDefs = new skillDefinition[totalSkills];
   }
   else
   {
      skillsDefs = NULL;
   }

   /* Now define and load all skills */ 
   while(def.getNextTuple(key, value))
   {
      /* Break the value */
      sscanf(value.c_str(),"%d %s %s", &num, &buf[0], &buf2[0]);

      /* Define the skill */
      skillsDefs[num].idString = key;
      descFile = dir.getRealFile("skills/") + buf;
      imgFile = buf2;
   
      /* Read the Skill Description */
      desc.open(descFile.c_str(), ios::in | ios::binary);
      if(!desc)
      {
         cerr << "Error: Can't open skill file: " << descFile << endl;
         return;
      }
      getline(desc, strBuffer);
      skillsDefs[num].name = translateDataString(strBuffer);
      getline(desc, strBuffer);
      skillsDefs[num].description = translateDataString(strBuffer);
      getline(desc, strBuffer);
      sscanf(strBuffer.c_str(),"%d",&skillsDefs[num].baseAttribute);
      skillsDefs[num].image = IMG_Load(dir.getRealFile(imgFile).c_str());
      if(!skillsDefs[num].image)
      {
         cout << "Can't open skill image: " << imgFile << endl;
      }
      desc.close();
      /* Define as Attribute if needed */
      skillsDefs[num].isAttribute = num <= 6;
   }
}

/*************************************************************
 *                         Finish                            *
 *************************************************************/
void skillsDefinitions::finish()
{
  int aux;
  if(skillsDefs)
  {
     for(aux = 0; aux < totalSkills; aux++)
     {
        if(skillsDefs[aux].image)
        {
           SDL_FreeSurface(skillsDefs[aux].image);
        }
     }
     delete [] skillsDefs;
  }
}

/*************************************************************
 *                      getTotalSkills                       *
 *************************************************************/
int skillsDefinitions::getTotalSkills()
{
   return(totalSkills);
}

/*************************************************************
 *                  getSkillDefinition                       *
 *************************************************************/
skillDefinition* skillsDefinitions::getSkillDefinition(string idString)
{
   int i;
   for(i=0; i < totalSkills; i++)
   {
      if(skillsDefs[i].idString == idString)
      {
         return(&skillsDefs[i]);
      }
   }

   return(NULL);
}

/*************************************************************
 *                  getSkillDefinition                       *
 *************************************************************/
skillDefinition* skillsDefinitions::getSkillDefinition(int i)
{
   if( (i >= 0) && (i < totalSkills) )
   {
      return(&skillsDefs[i]);
   }

   return(NULL);
}

/*************************************************************
 *                      static members                       *
 *************************************************************/
skillDefinition* skillsDefinitions::skillsDefs = NULL;
int skillsDefinitions::totalSkills = 0;


///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                Skills                                 //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************
 *                      Constructor                          *
 *************************************************************/
skills::skills()
{
   int i;
   skillsDefinitions skDef;

   /* Set Values */
   avaiblePoints = 0;
   totalSkills = skDef.getTotalSkills();

   /* Define Skills */
   if(totalSkills > 0)
   {
      /* Alloc Vector */
      skillsVector = new skill[totalSkills];

      /* Define initial values and definitions */
      for(i = 0; i < totalSkills; i++)
      {
         skillsVector[i].points = 0;
         skillsVector[i].prevPoints = 0;
         skillsVector[i].mod = 2;
         skillsVector[i].definition = skDef.getSkillDefinition(i);
      }
   }
   else
   {
      skillsVector = NULL;
      totalSkills = 0;
   }
}

/*************************************************************
 *                       Destructor                          *
 *************************************************************/
skills::~skills()
{
   if(skillsVector != NULL)
   {
      delete[] skillsVector;
   }
}


/*************************************************************
 *                    getSkillByString                       *
 *************************************************************/
skill* skills::getSkillByString(string idString)
{
   int i;
   for(i=0; i<totalSkills; i++)
   {
      if(skillsVector[i].definition != NULL)
      {
         if(idString == skillsVector[i].definition->idString)
         {
            return(&skillsVector[i]);
         }
      }
   }

   return(NULL);
}

/*************************************************************
 *                    getSkillByString                       *
 *************************************************************/
skill* skills::getSkillByInt(int i)
{
   if( (i >= 0) && (i < totalSkills) )
   {
      return(&skillsVector[i]);
   }

   return(NULL);
}

/*************************************************************
 *                   getSkillIntByString                     *
 *************************************************************/
int skills::getSkillIntByString(string idString)
{
   int i;
   for(i=0; i<totalSkills; i++)
   {
      if(skillsVector[i].definition != NULL)
      {
         if(idString == skillsVector[i].definition->idString)
         {
            return(i);
         }
      }
   }

   return(-1);
   
}

/*************************************************************
 *                       doSkillCheck                        *
 *************************************************************/
bool skills::doSkillCheck(string idString, int difficulty)
{
   return(doSkillCheck(getSkillByString(idString), difficulty));
}

/*************************************************************
 *                       doSkillCheck                        *
 *************************************************************/
bool skills::doSkillCheck(skill* sk, int difficulty)
{
   int res = 0;
   dice d20(DICE_D20);
   skill* att;
   int mod = 0;
   char buffer[512];
   int value = 0;
   briefing brief;

   if(sk != NULL)
   {
      if(sk->definition->baseAttribute > 0)
      {
          att = getSkillByInt(sk->definition->baseAttribute);
          /* Define the attribute modifier */
          if(att)
          {
             mod = (int)floor((att->points-10) / 2.0);
          }
      }
      else
      {
          att = getSkillByInt(-sk->definition->baseAttribute);
          /* Define the inverse attribute modifier */
          if(att)
          {
             mod = -(int)floor((att->points-10) / 2.0);
          }
      }

      /* Finally, roll the dices */
      if(sk->definition->isAttribute)
      {
         /* Is an attribute, the roll is with the modifier then */
         value = d20.roll() + mod;
      }
      else
      {
         /* Is an skill, do the normal roll */
         value = d20.roll() + sk->points + mod;
      }
   }

   res = (value > difficulty);

   /* Brief the result */
   sprintf(&buffer[0], "%s: %d x %d: %s.",
         sk->definition->name.c_str(), value, difficulty,
         res?gettext("Success"):gettext("Failure"));

   if(res)
   {
      /* With blue color */
      brief.addText(buffer, 27, 169, 245);
   }
   else
   {
      /* With red color */
      brief.addText(buffer, 233, 0, 5);
   }

   /* Done */
   return(res);
}

/*************************************************************
 *                   setAvaiblePoints                        *
 *************************************************************/
void skills::setAvaiblePoints(int points)
{
   avaiblePoints = points;
}

/*************************************************************
 *                   getAvaiblePoints                        *
 *************************************************************/
int skills::getAvaiblePoints()
{
   return(avaiblePoints);
}

/*************************************************************
 *                   getTotalSkills                          *
 *************************************************************/
int skills::getTotalSkills()
{
   return(totalSkills);
}

/*************************************************************
 *                      clearSkills                          *
 *************************************************************/
void skills::clear()
{
   int aux;
   /* Clear only skills */
   for(aux = ATT_SKILL_FIRST; aux < totalSkills; aux++)
   {
      skillsVector[aux].points = 0;
      skillsVector[aux].prevPoints = 0;
      skillsVector[aux].mod = 2;
   }
}

