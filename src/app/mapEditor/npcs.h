#ifndef _npcs_h
#define _npcs_h

#include "../../map/map.h"
#include "../../etc/modellist.h"
#include "../../engine/util.h"
#include "../../engine/personagens.h"
#include "../../classes/feats.h"
#include "message.h"


/*! The Npcs MapEditor Controller Class */
class npcs
{
   public:
      /*! Constructor
       * \param map -> pointer to opened map
       * \param NPCsList -> pointer to the list of NPCs 
       * \param lFeats -> pointer to features descriptions list*/
      npcs(Map* map, Lpersonagem* NPCsList, featsList* lFeats);
      /*! Destructor */
      ~npcs();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param tool -> current Tool*/
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int mouseXw, int mouseYw, int tool,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

      /*! Define the actual npc */
      void defineActualNpc(string fileName);

      /*! Get the actual fileName of the actual NPC */
      string getNpcFileName();

      /*! Save The NPCs list to a file
       * \param fileName -> name of the file to save */
      bool saveFile(string fileName);
                        
   private:
      Lpersonagem* NPCs;          /**< Actual NPCs List */
      Lpersonagem* intList;       /**< Internal NPCs List */
      featsList* features;        /**< Actual Features */
      Map* actualMap;             /**< Actual Internal Map */
      personagem* actualNpc;      /**< Actual Npc */
      string npcFile;             /**< Actual Npc Filename */
      int actualTool;             /**< Actual Tool */
      int state;                  /**< Actual Internal State */

      GLfloat mX, mY, mZ;         /**< Mouse Coordinates */

      Uint8 mB;                   /**< Mouse Buttons */

      void insertNpc(GLfloat xReal, GLfloat zReal,
                     personagem* npc, int qx, int qz);
};

#endif

