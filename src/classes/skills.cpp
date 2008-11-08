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

/*************************************************************
 *                      Constructor                          *
 *************************************************************/
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
   FILE* file;
   string arqDescricao;
   string arqImagem;
   char buffer[1024];
   char buf2[128];
   char buf3[128];
   char buf4[128];
   int num;
   string fName = "";
   if(!(file=fopen(dir.getRealFile("skills/skills.skl").c_str(),"r")))
   {
       cout << "Error while opening skills list: skills/skills.skl" << endl;
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
      /* get the skill definition */
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer,"%d %s %s %s",&num, &buf2[0],&buf3[0], &buf4[0]);
      arqImagem = buf3;
      arqDescricao = buf2;
      arqDescricao = dir.getRealFile("skills/") + arqDescricao;
      m_skills[aux].idString = buf4;

      /* Make sure idString not have \n or #13 */
      if( (m_skills[aux].idString[m_skills[aux].idString.length()-1] == '\n') ||
          (m_skills[aux].idString[m_skills[aux].idString.length()-1] == 13) )
      {
         m_skills[aux].idString.erase(m_skills[aux].idString.length()-1);
      }

      FILE* desc;
      if(! (desc = fopen(arqDescricao.c_str(), "r")))
      {
         cerr << "Can't open skill file: " << arqDescricao << endl;
         return;
      }
      fgets(buffer, sizeof(buffer), desc);
      m_skills[aux].name = translateDataString(buffer);
      fgets(buffer, sizeof(buffer), desc);
      m_skills[aux].description = "";
      fscanf(desc,"%d",&m_skills[aux].baseAttribute);
      m_skills[aux].points = 0;
      m_skills[aux].mod = 2;
      m_skills[aux].prevPoints = 0;
      m_skills[aux].image = NULL;
      fclose(desc);
   }

   fclose(file);

}

/*************************************************************
 *                       Destructor                          *
 *************************************************************/
skills::~skills()
{
  int aux;
  for(aux = 0; aux < totalSkills;aux++)
  {
     if( m_skills[aux].image)
     {
        SDL_FreeSurface( m_skills[aux].image );
     }
  }
  if(m_skills)
  {
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

