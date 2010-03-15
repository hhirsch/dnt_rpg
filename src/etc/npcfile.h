/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_npc_file_h
#define _dnt_npc_file_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

#include "list.h"

/*! A temporaly parse struct for npcs */
class npcParseStruct: public dntListElement
{
   public:
      string name;       /**< The NPC name */
      string fileName;   /**< The NPC fileName */
      GLfloat posX,      /**< The NPC Initial X Position */
              posZ,      /**< The NPC Initial Z Position */
              angle;     /**< The NPC Initial Orientation Angle */
      int psycho;        /**< The NPC Initial Psycho state */
};

/*! The NPC file is used to load lists of npcs, with its positions,
 * to later really load its models or just verify its state (allDead, allAlive,
 * etc.) at the modState controller. */
class npcFile: public dntList
{
   public:
      /*! Constructor */
      npcFile();

      /*! Destructor */
      ~npcFile();

      /* Load the npc file
       * \param fileName -> relative fileName of the npc's file
       * \return -> true if can save, false otherwise */
      bool load(string fileName);

      /*! Save the npc file
       * \param fileName -> relative fileName of the npc's file
       * \return -> true if can save, false otherwise */
      bool save(string fileName);

      /*! Get the next character readed from the file
       * \param name -> name of the npc
       * \param fileName -> fileName of the character got
       * \param posX -> X position of the character got 
       * \param posZ -> Z position of the character got
       * \param ange -> Orientation angle of the character got
       * \param psycho -> Psycho state of the character got
       * \return -> true if can get the next character, false otherwise */
      bool getNextCharacter(string& name, string& fileName, GLfloat& posX, 
                            GLfloat& posZ, GLfloat& angle, int& psycho);

      /* Insert a character at the list
       * \param name -> name of the npc
       * \param fileName -> fileName of the character got
       * \param posX -> X position of the character got 
       * \param posZ -> Z position of the character got
       * \param angle -> Orientation angle of the character got
       * \param psycho -> Character psycho state */
      void insertCharacter(string name, string fileName, GLfloat posX, 
                           GLfloat posZ, GLfloat angle, int psycho);

      /*! Kill All NPCs defined by the NPC file */
      void killAll();

   protected:

      /*! Delete a npcParseStruct
       * \param obj -> pointer to delete */
      void freeElement(dntListElement* obj);

      string npcFileName;        /**< Name of the NPC list file opened */
      npcParseStruct* current;   /**< Current NPC */

};

#endif

