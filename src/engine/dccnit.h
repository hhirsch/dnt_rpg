/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/


#ifndef _dccnit_h
#define _dccnit_h

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
#include "../particle/wave.h"
#include "../sound/sound.h"
#include "../ia/iascript.h"
#include "../etc/shader.h"


#include "alignwindow.h"
#include "aspectwindow.h"
#include "attwindow.h"
#include "barterwindow.h"
#include "briefing.h"
#include "camera.h"
#include "character.h"
#include "charwindow.h"
#include "classwindow.h"
#include "collision.h"
#include "cursor.h"
#include "dialog.h"
#include "initial.h"
#include "inventwindow.h"
#include "itemwindow.h"
#include "journalwindow.h"
#include "minimapwindow.h"
#include "modstate.h"
#include "options.h"
#include "racewindow.h"
#include "shadow.h"
#include "shortcutswindow.h"
#include "skillwindow.h"
#include "sun.h"
#include "sky.h"
#include "util.h"

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
      guiInterface* gui;               /**< GUI used on Engine */
      GLdouble proj[16];               /**< Projection Matrix */
      GLdouble modl[16];               /**< ModelView Matrix  */
      GLfloat camProj[16];             /**< Camera Projection */
      GLint viewPort[4];               /**< ViewPort Matrix */

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
       * \note Need to run \b SDL_GL_SwapBuffers() after;
       ***************************************************************/
      void drawWithoutShadows();

      /*!
       **************************************************************** 
       * Draws the Engine with all Shadows  
       * \note Need to run \b SDL_GL_SwapBuffers() after;
       ***************************************************************/
      void drawWithShadows();

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
       * Send all NPCs inventories to the modState
       ****************************************************************/
      void keepNPCInventoryStatus();

      /*!
       **************************************************************** 
       * Load and activate Map to engine
       * \param arqMapa -> string with filename
       * \param loadingGame -> true when loading map for a saveFile
       * \return 0 if can't load file .
       ***************************************************************/
      int loadMap(string arqMapa, bool loadingGame=false);

      /*!
       ****************************************************************
       * Load a new playable character to the engine
       * \param pcFile -> fileName of the character to load
       ****************************************************************/
      bool loadPC(string pcFile);

      /*!
       ****************************************************************
       * Load Splash Screen
       ****************************************************************/
       void splashScreen();

       /*! Fade in Some Texture on screen */
       void fadeInTexture(GLuint id, int x1, int y1, int x2, int y2,
                          int sizeX, int sizeY);

       /*! Fade Out Some texture on screen */
       void fadeOutTexture(GLuint id, int x1, int y1, int x2, int y2,
                           int sizeX, int sizeY);

      /*!
       **************************************************************** 
       * Load Initial Menu
       * \param Status -> ON_INIT or IN_GAME
       * \param idTextura -> loaded initial screen texture
       * \param reloadMusic -> true to reload the music
       * \return initialScreen return Values
       ***************************************************************/
      int menuScreen(int Status, GLuint idTextura, bool reloadMusic);

      /*!
       **************************************************************** 
       * Load Character's Screens
       * \param idTextura -> loaded initial screen texture
       * \return CharacterScreens return values
       ***************************************************************/
      int characterScreen(GLuint idTextura);

      /*!
       *************************************************************** 
       * Load Options Screen
       * \param idTextura -> loaded initial screen texture
       * \return Options Screen Return Value.
       ***************************************************************/
      int optionsScreen(GLuint idTextura);

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

   private:

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
      void treatGuiEvents(guiObject* object, int eventInfo);

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
      void showImage(string fileName);

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

      dirs dir;                    /**< Data directories finder */

      Map* actualMap;              /**< Actual Engine Map */
      cursor* cursors;             /**< Utilized mouse cursors */

      modState modifState;         /**< Current modification state */

      int walkStatus;              /**< Engine Walk Status */
      GLuint destinyImage;         /**< Move Destiny Image */
      GLfloat destinyVariation;    /**< Variation on Destiny Draw */
      GLfloat walkAngle;           /**< Current Walk Angle for continuous 
                                        mouse walk */

      collision colisionDetect;    /**< The collision detect Module */
 
      GLfloat** visibleMatrix;     /**< Actual Frustum Matrix */
      int mouseX,                  /**< Actual mouse X coordinates on screen */
          mouseY;                  /**< Actual mouse Y coordinates on screen */
      double xReal,                /**< Actual mouse X coordinates on World */
             zReal,                /**< Actual mouse Z coordinates on World */
             yReal;                /**< Actual mouse Y coordinates on World */

     
      character* activeCharacter;    /**< Pointer to the PC active */

      inventWindow* inventoryWindow; /**< Inventory Window */
      itemWindow* infoWindow;        /**< The info Window */
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

      sound* snd;                  /**< Actual Sounds thing */
      sndfx* walkSound;            /**< Actual walking sound */
      options* option;             /**< Current Options */

      modelList models;            /**< Current Models List */
      
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

      GLfloat moveCircleX,         /**< The Battle Move Circle X Coordinate */ 
              moveCircleY,         /**< The Battle Move Circle Y Coordinate */
              moveCircleZ;         /**< The Battle Move Circle Z Coordinate */
      
      thing* curTarget;            /**< The current target under mouse */


      /* Combat Things */
      GLuint  normalMoveCircle,    /**< One Movimentation Circle */
              fullMoveCircle,      /**< Full Round Movimentation Circle */
              featRangeCircle,     /**< Feat Range Circle */
              rangeCircle;         /**< Range Circle */

      fightSystem* fight;          /**< The Engine fight System */

      Uint32 lastTurnTime;        /**< Time when last calls for turn */
      int fightStatus;            /**< Current Fight Status */
      int attackFeat;             /**< attack feat to use. */

      conection* curConection;    /**< Current Mouse Conection */

      Uint32 walkPressTime;       /**< Ticks when pressed walk mouse button */
      GLfloat walkDistance;       /**< The current walk distance travelled */

      part2* effect;

      sky* gameSky;               /**< The internal sky reference */
      sun* gameSun;               /**< The internal sun reference */
      GLfloat hour;               /**< Hour on day time */

      shadow shadowMap;           /**< The shadowMap Module */
      GLfloat defaultColor[4];
      GLfloat blackColor[4];

      SDL_Surface* actualScreen;  /**< Pointer to the actual screen */

};

#endif

