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

/*************************************************************
 *                      Constructor                          *
 *************************************************************/
#if 0
skills::skills(string dir, string arq)
{
   dirs dirInfo;
   FILE* file;
   string arqDescricao;
   string arqImagem;
   char buffer[1024];
   char buf2[128];
   char buf3[128];
   char buf4[128];
   int num;
   if(!(file=fopen(arq.c_str(),"r")))
   {
       cerr << "Error while opening skills list: " << arq << endl;
       return;
   }

   fgets(buffer, sizeof(buffer), file);
   sscanf(buffer, "%d", &totalSkills);

   if(totalSkills > 0)
   {
      m_skills = new skill[totalSkills];
   }
   else
   {
      m_skills = NULL;
   }
   
   int aux;
   for(aux = 0; aux < totalSkills; aux++)
   {
      //fscanf(file,"%d %s",&num,&buffer[0]);
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer,"%d %s %s %s",&num, &buf2[0],&buf3[0], &buf4[0]);
      arqImagem = buf3;
      arqDescricao = buf2;
      arqDescricao = dir+arqDescricao;
      m_skills[aux].idString = buf4;

      /* Load the skill */
      FILE* desc;
      if(! (desc = fopen(arqDescricao.c_str(), "r")))
      {
         cerr << "Can't open skill file: " << arqDescricao << endl;
         return;
      }
      fgets(buffer, sizeof(buffer), desc);
      m_skills[aux].name = translateDataString(buffer);
      fgets(buffer, sizeof(buffer), desc);
      m_skills[aux].description = translateDataString(buffer);
      fscanf(desc,"%d",&m_skills[aux].baseAttribute);
      m_skills[aux].points = 0;
      m_skills[aux].mod = 2;
      m_skills[aux].prevPoints = 0;
      m_skills[aux].image = IMG_Load(dirInfo.getRealFile(arqImagem).c_str());
      if(!m_skills[aux].image)
      {
         cout << "Can't open skill image: " << dirInfo.getRealFile(arqImagem)
              << endl;
      }
      fclose(desc);
   }

   fclose(file);

   avaiblePoints = 0;
}
#endif

/*************************************************************
 *                      Constructor                          *
 *************************************************************/
/* Load from previous skill, but don't load images and descriptions */
skills::skills(skills* sk)
{
   int aux;

   totalSkills = sk->totalSkills;
   if(totalSkills > 0)
   {
      m_skills = new skill[totalSkills];
   }
   else
   {
      m_skills = NULL;
   }

   for(aux = 0; aux < totalSkills; aux++)
   {
      m_skills[aux].name = sk->m_skills[aux].name;
      m_skills[aux].description = "";//sk->m_skills[aux].descricao;
      m_skills[aux].points = 0;
      m_skills[aux].prevPoints = 0;
      m_skills[aux].mod = 2;
      m_skills[aux].baseAttribute = sk->m_skills[aux].baseAttribute;
      m_skills[aux].image = NULL;
      m_skills[aux].idString = sk->m_skills[aux].idString;
   }

   avaiblePoints = 0;
}

/*************************************************************
 *                       Destructor                          *
 *************************************************************/
skills::skills()
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
      m_skills = new skill[totalSkills];
   }
   else
   {
      m_skills = NULL;
   }

   /* Now define and load all skills */ 
   while(def.getNextTuple(key, value))
   {
      /* Break the value */
      sscanf(value.c_str(),"%d %s %s", &num, &buf[0], &buf2[0]);

      /* Define the skill */
      m_skills[num].idString = key;
      descFile = dir.getRealFile("skills/") + buf;
      imgFile = buf2;
   
      /* Read the Skill Description */
      if(! (desc = fopen(descFile.c_str(), "r")))
      {
         cerr << "Error: Can't open skill file: " << descFile << endl;
         return;
      }
      fgets(buffer, sizeof(buffer), desc);
      m_skills[num].name = translateDataString(buffer);
      fgets(buffer, sizeof(buffer), desc);
      m_skills[num].description = translateDataString(buffer);
      fscanf(desc,"%d",&m_skills[num].baseAttribute);
      m_skills[num].points = 0;
      m_skills[num].mod = 2;
      m_skills[num].prevPoints = 0;
      m_skills[num].image = IMG_Load(dir.getRealFile(imgFile).c_str());
      if(!m_skills[num].image)
      {
         cout << "Can't open skill image: " << imgFile << endl;
      }
      fclose(desc);
   }

   avaiblePoints = 0;
}

/*************************************************************
 *                       Destructor                          *
 *************************************************************/
skills::~skills()
{
  int aux;
  if(m_skills)
  {
     for(aux = 0; aux < totalSkills;aux++)
     {
        if( m_skills[aux].image)
        {
           SDL_FreeSurface( m_skills[aux].image );
        }
     }
     delete [] m_skills;
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
      if(idString == m_skills[i].idString)
      {
         return(&m_skills[i]);
      }
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
      if(idString == m_skills[i].idString)
      {
         return(i);
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
      m_skills[aux].points = 0;
      m_skills[aux].prevPoints = 0;
      m_skills[aux].mod = 2;
   }
}

