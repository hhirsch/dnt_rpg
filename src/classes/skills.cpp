/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "skills.h"

skills::skills(string dir, string arq)
{
   FILE* file;
   string arqDescricao;
   string arqImagem;
   char buffer[1024];
   char buf2[128];
   char buf3[128];
   int num;
   if(!(file=fopen(arq.c_str(),"r")))
   {
       printf("Error while opening skills list: %s\n",arq.c_str());
       return;
   }
   
   int aux;
   for(aux = 0; aux<MAX_SKILLS; aux++)
   {
      //fscanf(file,"%d %s",&num,&buffer[0]);
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer,"%d %s %s",&num,&buf2[0],&buf3[0]);
      arqImagem = buf3;
      arqDescricao = buf2;
      arqDescricao = dir+arqDescricao;

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

}

/* Load from previous skill, but don't load images and descriptions */
skills::skills(skills* sk)
{
   int aux;
   for(aux = 0; aux<MAX_SKILLS; aux++)
   {
      m_skills[aux].nome = sk->m_skills[aux].nome;
      m_skills[aux].descricao = "";//sk->m_skills[aux].descricao;
      m_skills[aux].pontos = 0;
      m_skills[aux].antPontos = 0;
      m_skills[aux].mod = 2;
      m_skills[aux].habilidadeBase = sk->m_skills[aux].habilidadeBase;
      m_skills[aux].imagem = NULL;
   }
}

skills::skills()
{
   FILE* file;
   string arqDescricao;
   string arqImagem;
   char buffer[1024];
   char buf2[128];
   char buf3[128];
   int num;
   if(!(file=fopen("../data/skills/skills.skl","r")))
   {
       printf("Error while opening skills list: ../data/skills/skills.skl\n");
       return;
   }
   
   int aux;
   for(aux = 0; aux<MAX_SKILLS; aux++)
   {
      //fscanf(file,"%d %s",&num,&buffer[0]);
      fgets(buffer, sizeof(buffer), file);
      sscanf(buffer,"%d %s %s",&num,&buf2[0],&buf3[0]);
      arqImagem = buf3;
      arqDescricao = buf2;
      arqDescricao = "../data/skills/Portugues/"+arqDescricao;

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

skills::~skills()
{
  int aux;
  for(aux = 0; aux<MAX_SKILLS;aux++)
  {
     if( m_skills[aux].imagem)
     {
        SDL_FreeSurface( m_skills[aux].imagem );
     }
  }
}

