/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <libintl.h>
#include <iostream>

using namespace std;

#include "skills.h"
#include "defs.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"

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
   char buffer[1024];
   char buf[128];
   char buf2[128];
   int num;
   FILE* desc;
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
      if(! (desc = fopen(descFile.c_str(), "r")))
      {
         cerr << "Error: Can't open skill file: " << descFile << endl;
         return;
      }
      fgets(buffer, sizeof(buffer), desc);
      skillsDefs[num].name = translateDataString(buffer);
      fgets(buffer, sizeof(buffer), desc);
      skillsDefs[num].description = translateDataString(buffer);
      fscanf(desc,"%d",&skillsDefs[num].baseAttribute);
      skillsDefs[num].image = IMG_Load(dir.getRealFile(imgFile).c_str());
      if(!skillsDefs[num].image)
      {
         cout << "Can't open skill image: " << imgFile << endl;
      }
      fclose(desc);
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
int skills::doSkillCheck(string idString)
{
   return(doSkillCheck(getSkillByString(idString)));
}

/*************************************************************
 *                       doSkillCheck                        *
 *************************************************************/
int skills::doSkillCheck(skill* sk)
{
   if(sk != NULL)
   {
      dice d20(DICE_D20);

      //TODO apply modifiers (manly attribute)!
      return(d20.roll() + sk->points);
   }
   return(0);
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

