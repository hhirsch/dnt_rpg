#ifndef _dnt_level_up_h
#define _dnt_level_up_h

#include "character.h"

/*! The levelUp class is used to level up characters when its
 * experience points reached enought level. */
class levelUp
{
   public:
      /*! Constructor
       * \param c -> pointer to character to level up */
      levelUp(character* c);
      /*! Destructor */
      ~levelUp();

      /*! Do the level Up process (calling the needed windows, etc) */
      void doLevelUp(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

   protected:
      character* current;
      int state;
};

#endif

