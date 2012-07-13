/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_dccnit_h
#define _dnt_dccnit_h

#include "../classes/skills.h"
#include "../classes/feats.h"
#include "../classes/align.h"
#include "../classes/race.h"
#include "../classes/classes.h"
#include "../classes/weapon.h"
#include "../classes/actions.h"
#include "../classes/pendingaction.h"
#include "../classes/mission.h"
#include "../classes/money.h"
#include "../etc/message3d.h"
#include "../etc/dirs.h"
#include "../etc/npcfile.h"
#include "../fight/fightSystem.h"
#include "../gui/farso.h"
#include "../map/map.h"
#include "../particle/partcontroller.h"
#include "../sound/sound.h"
#include "../ia/iascript.h"
#include "../etc/shader.h"


#include "alignwindow.h"
#include "aspectwindow.h"
#include "attwindow.h"
#include "barterwindow.h"
#include "briefing.h"
#include "camera.h"
#include "campaign.h"
#include "character.h"
#include "charwindow.h"
#include "classwindow.h"
#include "collision.h"
#include "cursor.h"
#include "dialog.h"
#include "featsel.h"
#include "initial.h"
#include "inventwindow.h"
#include "itemwindow.h"
#include "journalwindow.h"
#include "minimapwindow.h"
#include "modstate.h"
#include "moneywindow.h"
#include "options.h"
#include "racewindow.h"
#include "shadow.h"
#include "shortcutswindow.h"
#include "skillwindow.h"
#include "sun.h"
#include "sky.h"
#include "util.h"

#define MAX_MOUSE_BUTTONS  3

/*!
    The Engine Class is where all things are "merged" on game.
 */
class engine
{
   public:
      /*! Engine Constructor */
      engine();
      /*! Engine Desctructor */                        
      ~engine();                       
 
      characterList *NPCs;             /**< NPC's List */
      characterList *PCs;              /**< PC's List */

      pendingActionController* actionControl; /**< The action controller */
      
      #ifdef REDE
         char *server;                 /**< Server to Connect \b NOTUSED */
         clientdata_t clientData;      /**< Client Data \b NOTUSED */
      #endif

      camera gameCamera;               /**< The Game Camera */
      Farso::GuiInterface* gui;        /**< GUI used on Engine */
      GLdouble proj[16];               /**< Projection Matrix */
      GLdouble modl[16];               /**< ModelView Matrix  */
      GLfloat camProj[16];             /**< Camera Projection */
      GLint viewPort[4];               /**< ViewPort Matrix */
      GLuint idTextura;            /**< Main screen back texture */

      /*!
       **************************************************************** 
       * Init Engine to utilization  
       * \param screen -> Pointer to the Screen's Surface
       ***************************************************************/
      void init(SDL_Surface *screen);

      /*!
       **************************************************************** 
       * Runs the Engine  
       * \param surface -> Pointer to the Screen's Surface
       * \param commingBack -> True if comming back to game, false if
       *                       just loaded the map
       * \return 0 when stop running the main loop in engine.
       ***************************************************************/
      int  run(SDL_Surface *surface, bool commingBack);

      /*!
       **************************************************************** 
       * Draws the Engine without Shadow
       * \param flush -> if will call glFlush or not
       * \note Need to run \b SDL_GL_SwapBuffers() after;
       ***************************************************************/
      void drawWithoutShadows(bool flush=true);

      /*!
       **************************************************************** 
       * Draws the Engine with all Shadows  
       * \param flush -> if will call glFlush or not
       * \note Need to run \b SDL_GL_SwapBuffers() after;
       ***************************************************************/
      void drawWithShadows(bool flush=true);

      /*!
       **************************************************************** 
       * Render all the "Shadownable" things 
       * \param lightPass -> true if at the lighting map generating
       * \param updateAnimations -> true to update animations
       ****************************************************************/
      void renderScene(bool lightPass, bool updateAnimations);

      /*!
       ****************************************************************
       * Render no "shadownable" things, like walk circles, 
       * particles, etc. 
       *****************************************************************/
      void renderNoShadowThings();

      /*!
       *****************************************************************
       * Render th GUI related Things
       *****************************************************************/
      void renderGUI();

      /*!
       ****************************************************************
       * Set current game hour
       ****************************************************************/
      void setHour(float h);

      /*!
       ****************************************************************
       * Get current game hour 
       ****************************************************************/
      float getHour();

      /*!
       ****************************************************************
       * Send all NPCs info to the modState
       ****************************************************************/
      void keepNPCStatus();

      /*!
       **************************************************************** 
       * Load a campaign
       * \param campaignFile -> campaign file name
       * \param loadingGame -> true when loading from a saveFile
       ****************************************************************/ 
      void loadCampaign(std::string campaignFile, bool loadingGame=false);

      /*!
       **************************************************************** 
       * Load and activate Map to engine
       * \param arqMapa -> string with filename
       * \param loadingGame -> true when loading map for a saveFile
       * \return 0 if can't load file .
       ***************************************************************/
      int loadMap(std::string arqMapa, bool loadingGame=false);

      /*!
       ****************************************************************
       * Load a new playable character to the engine
       * \param pcFile -> fileName of the character to load
       ****************************************************************/
      bool loadPC(std::string pcFile);

      /*!
       ****************************************************************
       * Load Splash Screen
       ****************************************************************/
       void splashScreen();

       /*! Fade in effect to current map */
       void fadeIn();

       /*! Fade out effect from current map */
       void fadeOut();

       /*! Fade in Some Texture on screen */
       void fadeInTexture(GLuint id, int x1, int y1, int x2, int y2,
                          int sizeX, int sizeY);

       /*! Fade Out Some texture on screen */
       void fadeOutTexture(GLuint id, int x1, int y1, int x2, int y2,
                           int sizeX, int sizeY);

       /*! Do an effect to the main screen */
      void mainScreenEffect();

      /*!
       **************************************************************** 
       * Load Initial Menu
       * \param Status -> ON_INIT or IN_GAME
       * \param reloadMusic -> true to reload the music
       * \return initialScreen return Values
       ***************************************************************/
      int menuScreen(int Status, bool reloadMusic);

      /*!
       **************************************************************** 
       * Load Character's Screens
       * \return CharacterScreens return values
       ***************************************************************/
      int characterScreen();

      /*!
       *************************************************************** 
       * Load Options Screen
       * \return Options Screen Return Value.
       ***************************************************************/
      int optionsScreen();

      /*!
       *************************************************************** 
       * Load/UnLoad Inventory Window
       ***************************************************************/
      void openCloseInventoryWindow();
      
      /*!
       *************************************************************** 
       * Load Game Function (and window)
       ***************************************************************/
      bool loadGame();
      
      /*!
       *************************************************************** 
       * Save Game Function (and window)
       ***************************************************************/
      void saveGame();

      /*!
       **************************************************************
       * Put Engine in Turn Battle Mode
       * \param surprisePC -> true if surprise atack for PC
       ***************************************************************/
      void enterBattleMode(bool surprisePC);

      /*!
       **************************************************************
       * Exit the battle (puting all enemys as neutrals)
       **************************************************************/
      void exitBattleMode();

      /*!
       ***************************************************************
       * Get the engine fightSystem
       * \return pointer to the fightSystem
       ***************************************************************/
      fightSystem* getFightSystem();

      /*!
       ***************************************************************
       * Get the current opened map at the engine
       * \return pointer to the current opened map
       ***************************************************************/
      Map* getCurrentMap();

      /*!
       ***************************************************************
       * Get the current opened campaign at the engine
       * \return pointer to the current opened campaign
       ***************************************************************/
      DNT::Campaign* getCurrentCampaign();

   private:

      /*! do the opengl clear with needd flags */
      void clearOpenGL();

      /*!
       *************************************************************** 
       * Close all related windows  
       ***************************************************************/
      void closeWindows();

      /*!
       *************************************************************** 
       * Put all pc characters to rest (if no enemies)
       ***************************************************************/
      void rest();

      /*!
       *************************************************************** 
       * Verify if there are alive enemies at the current map
       * \return true if one or more enemies
       ***************************************************************/
      bool hasEnemies();

      /*!
       *************************************************************** 
       * Define Screen to actual Screen Size  
       * \param screen -> Pointer to the Screen's Surface
       * \param actualFarView -> FarView Value
       ***************************************************************/
      void redefineWindow(SDL_Surface *screen, float actualFarView);

      /*!
       *************************************************************** 
       * Treat Input/Output Events
       * \param screen -> Pointer to the Screen's Surface
       ***************************************************************/
      int treatIO(SDL_Surface *screen);

      /*!
       **************************************************************
       * Treat Gui Events
       * \param object -> last object modifief on GUI
       * \param eventInfo -> las event on GUI
       **************************************************************/
      void treatGuiEvents(Farso::GuiObject* object, int eventInfo);

      /*!
       **************************************************************
       * Treat the pending actions
       **************************************************************/
      void treatPendingActions();

      /*!
       **************************************************************
       * Treat all characters scripts
       **************************************************************/
      void treatScripts();

      /*!
       **************************************************************
       * Do all characters A* cycles
       **************************************************************/
      void doAStar();

      /*
       ****************************************************************
       *  Update the Mouse Coordinates on World
       ****************************************************************/
      void updateMouseWorldPos(); 
      void updateMouseFloorPos();

      /*!
       ***********************************************************************
       * Verify and treat the actions made by mouse on the game (not on gui!)
       * \param Mbutton -> mouse Button State
       * \return 1 if the map was changed, 0 otherwise
       ***********************************************************************/
      int verifyMouseActions(Uint8 Mbutton);

      /*!
       *************************************************************** 
       * Threat PC Colision (can walk?)
       * \param varX -> Variation on X position
       * \param varZ -> Variation on Z position
       * \param varAlpha -> Variation on angle
       * \return true if can walk to new position
       ***************************************************************/
      bool canWalk(GLfloat varX, GLfloat varZ, GLfloat varAlpha);


      /*! Verify if the active playable character have 
       * available walk actions  */
      bool pcHaveWalkAction();

      /*!
       *************************************************************** 
       * Try walk the character with the move variation
       * \param varX -> Variation on X position
       * \param varZ -> variation on Z position
       ***************************************************************/
      bool tryWalk(GLfloat varX, GLfloat varZ);

      /*!
       ***************************************************************
       * Define New Character Height, based on character position
       * \param c -> pointer to the character
       * \param nx -> character new X position
       * \param nz -> character new Z position
       * \return -> true if can define new heigh, false if can't move.
       ***************************************************************/
      bool defineCharacterHeight(character* c, GLfloat nx, GLfloat nz);

      /*!
       ************************************************************** 
       * Draws the sky sphere to the list
       * \param lats -> How many latitudes
       * \param longs -> How many longitudes
       ***************************************************************/
      void drawSphereToList(int lats, int longs);

      /*!
       ************************************************************** 
       * update all PCs health bars to the current pcs life
       ***************************************************************/
      void updateAllHealthBars();

      /*!
       ************************************************************** 
       * Show an Image to Screen and wait mouse Button.
       ***************************************************************/
      void showImage(std::string fileName);

      /*!
       ************************************************************** 
       * Write actual Time on TXT box
       ***************************************************************/
      void hourToTxt();

      /*!
       **************************************************************
       * End Character Turn in Battle Mode
       ***************************************************************/
      void endTurn();

      /*! 
       **************************************************************
       * Verify if the action is in range 
       ***************************************************************/
      bool rangeAction(GLfloat posX, GLfloat posZ, 
                       GLfloat targX, GLfloat targZ,
                       GLfloat range);
      /*! 
       **************************************************************
       * Define the front surface, copying it from the front buffer
       * \note -> this function is usually called before potential saves
       ***************************************************************/
      void defineFrontSurface();

      /*! Update all things before render (listener position,camera,sun,etc) */
      void updateBeforeRender();

      /*! Clear and free all current game related things */
      void quitCurrentGame();
      /*! Clear the SDL events queue, ignoring them all */
      void clearEventsQueue();

      dirs dir;                    /**< Data directories finder */

      DNT::Campaign* curCampaign;  /**< Current Campaign */
      bool callCampaignScript;     /**< Call the campaign script or not */
      Map* actualMap;              /**< Actual Engine Map */
      cursor* cursors;             /**< Utilized mouse cursors */

      modState modifState;         /**< Current modification state */

      int walkStatus;              /**< Engine Walk Status */
      GLuint destinyImage;         /**< Move Destiny Image */
      GLfloat destinyVariation;    /**< Variation on Destiny Draw */
      GLfloat walkAngle;           /**< Current Walk Angle for continuous 
                                        mouse walk */

      collision colisionDetect;    /**< The collision detect Module */
      shaders glslShaders;         /**< The GLSL shaders used */
 
      GLfloat** visibleMatrix;     /**< Actual Frustum Matrix */
      int mouseX,                  /**< Actual mouse X coordinates on screen */
          mouseY;                  /**< Actual mouse Y coordinates on screen */
      int mouseWheel;              /**< Mouse Wheel relative move */
      bool mouseButtonReleased[MAX_MOUSE_BUTTONS]; /**< Release state */
      double xReal,                /**< Actual mouse X coordinates on World */
             zReal,                /**< Actual mouse Z coordinates on World */
             yReal;                /**< Actual mouse Y coordinates on World */
      double xFloor,               /**< Mouse floor X intersection */
             zFloor;               /**< Mouse floor Z intersection */
     
      character* activeCharacter;    /**< Pointer to the PC active */

      inventWindow* inventoryWindow; /**< Inventory Window */
      itemWindow* infoWindow;        /**< The info Window */
      moneyWindow moneySplitWindow;  /**< Money split window */
      charWindow* charInfoWindow;    /**< The character info Window */
      miniMapWindow* mapWindow;      /**< The miniMap window */
      shortcutsWindow* shortcuts;    /**< The shortcuts window */
      journalWindow* journal;        /**< The journal window */

      briefing* brief;               /**< The briefing controller */

      SDL_Surface* frontSurface;   /**< Front Buffer Screenshot */

      Uint32 lastRead;             /**< Last Verification of I/O */
      Uint32 lastMouse;            /**< Last read from mouse */
      Uint32 lastMousePression;    /**< Last mouse pressure */
      Uint32 lastFPS;              /**< Last obtained FPS */
      Uint32 waitDieTimer;         /**< Timer for wait die animation */
      double actualFPS;            /**< Actual FPS */

      Uint32 lastKeyb;             /**< Last time read from keyboard */
      Uint32  lastKey;             /**< Last key with refresh readed */
      Uint8 *keys;                 /**< current keyboard state */

      sound* snd;                  /**< Actual Sounds thing */
      sndfx* walkSound;            /**< Actual walking sound */
      options* option;             /**< Current Options */

      scene curScene;              /**< Current Models List */
      
      featsList* features;         /**< Feats descriptions */
      aligns* alignList;           /**< Alignments List */
      races* raceList;             /**< Races List */
      classes* classList;          /**< Classes List */
      skillsDefinitions skillsList;/**< List of Skills */
      weaponTypes weaponsTypes;    /**< List of Categories of Weapons */

      messageController* msgController;   /**< The message controller */
      missionsController* missions;       /**< The missions controller */
      partController particleController;  /**< The paticle controller */

      GLfloat seconds;             /**< Seconds from last actualization */
      
      int imgNumber;               /**< Used on exporting images to make video*/

      int exitEngine;              /**< Control if is to exit the engine */

      int engineMode;              /**< Current Mode of the Engine */

      bool showRange;              /**< If show range or not (CTRL to enable) */
      bool occPosition;            /**< If can occupy position or not */
      GLuint canOccImage;          /**< "Can Occupy Position" Image */

      GLfloat moveCircleX,         /**< The Battle Move Circle X Coordinate */ 
              moveCircleY,         /**< The Battle Move Circle Y Coordinate */
              moveCircleZ;         /**< The Battle Move Circle Z Coordinate */
      
      thing* curTarget;            /**< The current target under mouse */

      Farso::GuiInterface* fpsGui; /**< A gui jsut for fps */
      Farso::Window* fpsWindow;    /**< an invisible window for fps display */
      Farso::TextBox* fpsText;     /**< The fps text */

      /* Combat Things */
      GLuint  normalMoveCircle,    /**< One Movimentation Circle */
              fullMoveCircle,      /**< Full Round Movimentation Circle */
              featAreaCircle,      /**< Feat radius area circle */
              featRangeCircle,     /**< Feat Range Circle */
              rangeCircle;         /**< Range Circle */

      fightSystem* fight;          /**< The Engine fight System */

      Uint32 lastTurnTime;        /**< Time when last calls for turn */
      int fightStatus;            /**< Current Fight Status */

      conection* curConection;    /**< Current Mouse Conection */

      Uint32 walkPressTime;       /**< Ticks when pressed walk mouse button */
      GLfloat walkDistance;       /**< The current walk distance travelled */

      sky* gameSky;               /**< The internal sky reference */
      sun* gameSun;               /**< The internal sun reference */
      GLfloat hour;               /**< Hour on day time */

      shadow shadowMap;           /**< The shadowMap Module */
      GLfloat defaultColor[4];
      GLfloat blackColor[4];

      SDL_Surface* actualScreen;  /**< Pointer to the actual screen */

};

#endif

