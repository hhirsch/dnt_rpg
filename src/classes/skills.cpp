/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "skills.h"
#include "defs.h"
#include "../lang/lang.h"

/*************************************************************
 *                      Constructor                          *
 *************************************************************/
skills::skills(string dir, string arq)
{
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
       printf("Error while opening skills list: %s\n",arq.c_str());
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
         printf("Can't open skill file: %s \n",arqDescricao.c_str() );
         return;
      }
      fgets(buffer, sizeof(buffer), desc);
      m_skills[aux].nome = buffer;
      fgets(buffer, sizeof(buffer), desc);
      m_skills[aux].descricao = buffer;
      fscanf(desc,"%d",&m_skills[aux].habilidadeBase);
      m_skills[aux].pontos = 0;
      m_skills[aux].mod = 2;
      m_skills[aux].antPontos = 0;
      m_skills[aux].imagem = IMG_Load(arqImagem.c_str());
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
      m_skills[aux].nome = sk->m_skills[aux].nome;
      m_skills[aux].descricao = "";//sk->m_skills[aux].descricao;
      m_skills[aux].pontos = 0;
      m_skills[aux].antPontos = 0;
      m_skills[aux].mod = 2;
      m_skills[aux].habilidadeBase = sk->m_skills[aux].habilidadeBase;
      m_skills[aux].imagem = NULL;
      m_skills[aux].idString = sk->m_skills[aux].idString;
   }

   avaiblePoints = 0;
}

/*************************************************************
 *                       Destructor                          *
 *************************************************************/
skills::skills()
{
   FILE* file;
   string arqDescricao;
   string arqImagem;
   char buffer[1024];
   char buf2[128];
   char buf3[128];
   char buf4[128];
   int num;
   lang language;
   string fName = "";
   if(!(file=fopen("../data/skills/skills.skl","r")))
   {
       printf("Error while opening skills list: ../data/skills/skills.skl\n");
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
      arqDescricao = language.SKILLS_DIR + arqDescricao;
      m_skills[aux].idString = buf4;

      FILE* desc;
      if(! (desc = fopen(arqDescricao.c_str(), "r")))
      {
         printf("Can't open skill file: %s \n",arqDescricao.c_str() );
         return;
      }
      fgets(buffer, sizeof(buffer), desc);
      m_skills[aux].nome = buffer;
      fgets(buffer, sizeof(buffer), desc);
      m_skills[aux].descricao = "";
      fscanf(desc,"%d",&m_skills[aux].habilidadeBase);
      m_skills[aux].pontos = 0;
      m_skills[aux].mod = 2;
      m_skills[aux].antPontos = 0;
      m_skills[aux].imagem = NULL;
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
     if( m_skills[aux].imagem)
     {
        SDL_FreeSurface( m_skills[aux].imagem );
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
      m_skills[aux].pontos = 0;
      m_skills[aux].antPontos = 0;
   }
}

