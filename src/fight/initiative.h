#ifndef _initiative_h
#define _initiative_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <stdlib.h>
#include <string>
using namespace std;
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
     void insertCharacter(personagem* pers, string& brief);
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

