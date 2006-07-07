#ifndef _skills_h
#define _skills_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#define MAX_SKILLS       46

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <string>
using namespace std;

typedef struct _skill
{
   int pontos;             /* Total skill points */
   int antPontos;          /* Previous skill points, when adding and cancel */
   int mod;                /* Needed points to add +1 on pontos */
   string nome;            /* Skill Name */
   string descricao;       /* Skill Description */
   int habilidadeBase;     /* Base Attibute */
   SDL_Surface* imagem;    /* Skill Image */
}skill;


class skills
{
   public:
      skills(string dir, string arq);
      skills(skills* sk);
      skills();
      ~skills();

     skill m_skills[MAX_SKILLS];
};

#endif
