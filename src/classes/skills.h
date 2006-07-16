#ifndef _skills_h
#define _skills_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#define MAX_SKILLS       46  /**< Max Number of Skills Avaible.*/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <string>
using namespace std;

/*! The individual skill definition.
 */
typedef struct _skill
{
   int pontos;             /**< Total skill points */
   int antPontos;          /**< Previous skill points, when adding and cancel */
   int mod;                /**< Needed points to add +1 on pontos */
   string nome;            /**< Skill Name */
   string descricao;       /**< Skill Description */
   int habilidadeBase;     /**< Base Attibute */
   SDL_Surface* imagem;    /**< Skill Image */
}skill;

/*! The group skill definitions. All game skills are here.
  */
class skills
{
   public:
      /*! skills contructor. Read all skills from file.
       *  \param dir -> directory of the skills files
       *  \param arq -> filename of the skills list. */
      skills(string dir, string arq); 
      /*! skills contructor. Read all skills from a skills. Do not load 
       *  descriptions. 
       *  \param sk -> other skills pointer.
       */
      skills(skills* sk);
      /*! skills contructor. Read all skills from a skills. Do not load 
       *  descriptions and images.
       */
      skills();
      /*! skills destructor. */
      ~skills();

     skill m_skills[MAX_SKILLS]; /**< the internal skills */
};

#endif
