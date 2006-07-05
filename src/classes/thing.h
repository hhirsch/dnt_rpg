#ifndef _thing_h
#define _thing_h

#include "skills.h"

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

      //skills sk; //skills without images and description
};

#endif

