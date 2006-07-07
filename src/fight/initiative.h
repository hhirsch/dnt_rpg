#ifndef _initiative_h
#define _initiative_h

#include <stdlib.h>
#include "../engine/personagens.h"


class initiativeStruct
{
   public:

      initiativeStruct(){};
      ~initiativeStruct(){};
 
      personagem* character;   /* Character Pointer */
      initiativeStruct* next;  /* Next Character on Initiative Pointer */
      int initiativeValue;     /* Initiative Value of the Character */
};

class initiative
{
   public:
     initiative();
     ~initiative();

     /*************************************************************** 
     * Reason: Insert Character on Initiative.  
     * Param:
     *        pers -> Pointer to Character
     ***************************************************************/
     void includeCharacter(personagem* pers);
     /*************************************************************** 
     * Reason: Actualize the pointers to a new Round.  
     * Param:
     ***************************************************************/
     void newRound();
     /*************************************************************** 
     * Reason: Return the next Character to Play.  
     * Param:
     ***************************************************************/
     personagem* nextCharacter();

   private:
     initiativeStruct* first;  /* First character on initiative list */
     initiativeStruct* actual; /* Actual character on initiative list */
};

#endif

