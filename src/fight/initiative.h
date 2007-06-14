#ifndef _initiative_h
#define _initiative_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <stdlib.h>
#include <string>
using namespace std;
#include "../engine/personagens.h"
#include "../lang/lang.h"

/*! Implementation of the Struct of Initiative */
class initiativeStruct
{
   public:
      /*! Constructor */
      initiativeStruct(){};
      /*! Destructor */
      ~initiativeStruct(){};
 
      character* dude;         /**< Character Pointer */
      initiativeStruct* next;  /**< Next Character on Initiative Pointer */
      int initiativeValue;     /**< Initiative Value of the Character */
};

/*! Initiatives Implementation */
class initiative
{
   public:
     /*! Constructor */
     initiative();
     /*! Destructor */
     ~initiative();

     /*!
     *************************************************************** 
     * Insert Character on Initiative.  
     * \param pers -> Pointer to Character
     * \param brief -> briefing string
     ***************************************************************/
     void insertCharacter(character* pers, string& brief);
     /*!
     *************************************************************** 
     * Actualize the pointers to a new Round.  
     ***************************************************************/
     void newRound();
     /*!
     *************************************************************** 
     * Gets the next Character to Play.  
     * \return pointer to next character to play.
     ***************************************************************/
     character* nextCharacter();
     /*!
     *************************************************************** 
     * Gets the actual Character to Playing.  
     * \return pointer to actual character to playing.
     ***************************************************************/
     character* actualCharacter();
     /*!
     *************************************************************** 
     * Clears The initiative List.  
     ***************************************************************/
     void clear();

   private:
     initiativeStruct* first;  /**< First character on initiative list */
     initiativeStruct* next;   /**< Next character on initiative list */
     initiativeStruct* actual; /**< Actual character on initiative list */
     lang language; /**< Language Internationalization */
};

#endif

