#ifndef _iascript_h
#define _iascript_h

#include "iafuncs.h"
#include "iavariable.h"

#include "../engine/character.h"
#include "../classes/object.h"
#include "../classes/feats.h"
#include "../classes/skills.h"
#include "../map/map.h"

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

/*! FIXME, not here, temporaly */
class action
{
   public:
      bool done;
};

/*! The ia Script is the script that controls the behaviour
 * of characters, objects and events on DNT. */
class iaScript
{
   public:
      /*! Constructor
       * \param scriptFile -> fileName of the script */
      iaScript(string scriptFile);
      /*! Destructor */
      ~iaScript();
   
      /*! Define the owner of the script as a object
       * \param owner -> pointer to the object */
      void defineObjectOwner(object* owner);
      /*! Define the owner of the script as a Character
       * \param owner -> pointer to the character */
      void defineCharacterOwner(character* owner);

      /*! Define the actual openned Map
       * \param acMap -> pointer to the opened map */
      void defineMap(Map* acMap);

      /*! run the Script to the point of a more than one cicle action,
       * that will be marked as pending or at the end of the script.
       * if the scripts ends, a NULL is retorned, and the script can
       * be removed from the memory.
       * \return action pointer of a new pending action */
      action* run();

   protected:
      ifstream file;             /**< The script's file */
      string fileName;           /**< FileName of the script */
      object* objectOwner;       /**< Object Owner */
      character* characterOwner; /**< Character Owner */

      Map* actualMap;            /**< Actual Opened Map */

      action* pendingAction;     /**< Pending Action on the script. Only
                                      will advance on script when receive
                                      that this actions was done. */

      int actualLine;


};

#endif

