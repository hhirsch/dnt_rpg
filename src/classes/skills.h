#ifndef _skills_h
#define _skills_h

#define ATT_FORCA        1
#define ATT_DESTREZA     2
#define ATT_CONSTITUICAO 3
#define ATT_INTELIGENCIA 4
#define ATT_SABEDORIA    5
#define ATT_CARISMA      6

#define VAR_AUX1         40
#define VAR_AUX2         41
#define VAR_AUX3         42
#define VAR_AUX4         43
#define VAR_AUX5         44
#define VAR_AUX6         45

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
      ~skills();

     skill m_skills[MAX_SKILLS];
};

#endif
