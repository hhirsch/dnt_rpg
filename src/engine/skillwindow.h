#ifndef _skillwindow_h
#define _skillwindow_h

#include "../gui/farso.h"
#include "../classes/skills.h"
#include "../lang/lang.h"

#define CHAR_CANCEL  0
#define CHAR_CONFIRM 1
#define CHAR_OTHER   2

#define SKILLW_CANCEL  0
#define SKILLW_CONFIRM 1
#define SKILLW_OTHER   2

class skillWindow
{
   public:
      /* Skill Window constructor */
      skillWindow(skills* sk, int points, interface* inter);
      /* Treat Events on Window. Returns 0 on close, 1 otherwise */
      int treat(interface* inter);

      janela*      window;           /* Pointer to the internal window */
   private:
      skills*      externalSkill;    /* Pointer to the edited skillList */
      int          avaiblePoints;    /* Avaible points to distribute */
      int          curSkill;         /* Actual Skill */
      quadroTexto* desc;             /* Description Text */
      quadroTexto* txtAvaiblePoints; /* Avaible Points Text */
      quadroTexto* skillName;        /* Actual Skill Name */
      quadroTexto* txtPoints;        /* Actual Points Text */
      quadroTexto* txtCosts;         /* Actual Cost Text */
      figura*      skFig;            /* Actual skill Image */
      lang language;
 
};

#endif

