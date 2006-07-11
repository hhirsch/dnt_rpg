#ifndef _thing_h
#define _thing_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "skills.h"
#include "defs.h"

class thing
{
   public:
      thing();
      ~thing();

      int lifePoints;
      int fortitude;
      int reflexes;
      int will;
      int armatureClass;
      int displacement;
      int level;
      int baseAttackModifier;
      int sizeModifier;

      bool dead;   /* dead state or not.(if dead is only a corpse)*/

      skills sk; //skills without images and description

      int skillBonus(int skillNumber);
      int attBonus(int attNumber);
      int getBonusValue(int something);
};

#endif

