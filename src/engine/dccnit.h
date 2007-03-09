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
#include "../fight/fightSystem.h"
#include "../gui/farso.h"
#include "../lang/lang.h"
#include "../map/map.h"
#include "../particle/partSystem.h"
#include "../particle/wave.h"
#include "../sound/sound.h"

#include "alignwindow.h"
#include "attwindow.h"
#include "camera.h"
#include "classwindow.h"
#include "collision.h"
#include "cursor.h"
#include "initial.h"
#include "inventwindow.h"
#include "modstate.h"
#include "options.h"
#include "personagens.h"
#include "racewindow.h"
#include "skillwindow.h"
#include "sun.h"
#include "sky.h"
#include "util.h"

#ifdef REDE
  #include "../net/client.h"
#endif

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
 
      Lpersonagem *NPCs;               /**< NPC's List */
      Lpersonagem *PCs;                /**< PC's List */

      #ifdef REDE
         char *server;                 /**< Server to Connect \b NOTUSED */
         clientdata_t clientData;      /**< Client Data \b NOTUSED */
      #endif

      camera gameCamera;               /**< The Game Camera */
      interface* gui;                  /**< GUI used on Engine */
      GLdouble proj[16];               /**< Projection Matrix */
      GLdouble modl[16];               /**< ModelView Matrix  */
      GLint viewPort[4];               /**< ViewPort Matrix */

      /*!
       **************************************************************** 
       * Init Engine to utilization  
       * \param screen -> Pointer to the Screen's Surface
       ***************************************************************/
      void Init(SDL_Surface *screen);

      /*!
       **************************************************************** 
       * Runs the Engine  
       * \param surface -> Pointer to the Screen's Surface
       * \return 0 when stop running the main loop in engine.
       ***************************************************************/
      int  Run(SDL_Surface *surface);    

      /*!
       **************************************************************** 
       * Draws the Engine  
       * \note Need to run \b SDL_GL_SwapBuffers() after;
       ***************************************************************/
      void Draw();

      /*!
       **************************************************************** 
       * Load and activate Map to engine
       * \param arqMapa -> string with filename
       * \param RecarregaPCs -> 1 to reload PCs 0 to not.
       * \return 0 if can't load file .
       ***************************************************************/
      int LoadMap(string arqMapa, int RecarregaPCs);

      /*!
       ****************************************************************
       * Load Splash Screen
       ****************************************************************/
       void SplashScreen();

       /*! Fade in Some Texture on screen */
       void fadeInTexture(GLuint id, int x1, int y1, int x2, int y2);

       /*! Fade Out Some texture on screen */
       void fadeOutTexture(GLuint id, int x1, int y1, int x2, int y2);

      /*!
       **************************************************************** 
       * Load Initial Menu
       * \param Status -> ON_INIT or IN_GAME
       * \param idTextura -> loaded initial screen texture
       * \param reloadMusic -> true to reload the music
       * \return initialScreen return Values
       ***************************************************************/
      int InitialScreen(int Status, GLuint idTextura, bool reloadMusic);

      /*!
       **************************************************************** 
       * Load Character's Screens
       * \param idTextura -> loaded initial screen texture
       * \return CharacterScreens return values
       ***************************************************************/
      int CharacterScreen(GLuint idTextura);

      /*!
       *************************************************************** 
       * Load Options Screen
       * \param idTextura -> loaded initial screen texture
       * \return Options Screen Return Value.
       ***************************************************************/
      int OptionsScreen(GLuint idTextura);

      /*!
       *************************************************************** 
       * Load Informations Screen
       ***************************************************************/
      void InformationScreen();

      /*!
       *************************************************************** 
       * Load MiniMap Window
       ***************************************************************/
      void OpenMiniMapWindow();

      /*!
       *************************************************************** 
       * Load ShortCuts Window
       ***************************************************************/
      void OpenShortcutsWindow();

      /*!
       *************************************************************** 
       * Load/UnLoad Inventory Window
       ***************************************************************/
      void OpenCloseInventoryWindow();
      
      /*!
       *************************************************************** 
       * Load Function (and window)
       ***************************************************************/
      void load();
      
      /*!
       *************************************************************** 
       * Save Function (and window)
       ***************************************************************/
      void save();


   private:

      /*!
       *************************************************************** 
       * Define Screen to actual Screen Size  
       * \param screen -> Pointer to the Screen's Surface
       ***************************************************************/
      void redmensionateWindow(SDL_Surface *screen);

      /*!
       *************************************************************** 
       * Threat Input/Output Events
       * \param screen -> Pointer to the Screen's Surface
       * \param forcaAtualizacao -> to force the draw
       ***************************************************************/
      int threatIO(SDL_Surface *screen,int *forcaAtualizacao);

      /*!
       **************************************************************
       * Threat Gui Events
       * \param object -> last object modifief on GUI
       * \param eventInfo -> las event on GUI
       **************************************************************/
      void threatGuiEvents(Tobjeto* object, int eventInfo);

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
       ************************************************************** 
       * Draws the sky sphere to the list
       * \param lats -> How many latitudes
       * \param longs -> How many longitudes
       ***************************************************************/
      void drawSphereToList(int lats, int longs);

      /*!
       ************************************************************** 
       * actualize all PCs health bars to the current pcs life
       ***************************************************************/
      void actualizeAllHealthBars();

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
       * Put Engine in Turn Battle Mode
       ***************************************************************/
      void enterBattleMode();

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



      void draw2DMode();
      void draw3DMode();

      Map* actualMap;              /**< Actual Engine Map */
      cursor* cursors;             /**< Utilized mouse cursors */

      modState modifState;         /**< Current modification state */

      int walkStatus;              /**< Engine Walk Status */
      GLuint destinyImage;         /**< Move Destiny Image */
      GLfloat destinyVariation;    /**< Variation on Destiny Draw */

      collision colisionDetect;    /**< The collision detect Module */
 
      GLfloat visibleMatrix[6][4]; /**< Actual Frustum Matrix */
      int mouseX,                  /**< Actual mouse X coordinates on screen */
          mouseY;                  /**< Actual mouse Y coordinates on screen */
      double xReal,                /**< Actual mouse X coordinates on World */
             zReal,                /**< Actual mouse Z coordinates on World */
             yReal;                /**< Actual mouse Y coordinates on World */

      quadroTexto* FPS;            /**< Text that shows FPS */
      quadroTexto* ObjTxt;         /**< Text that shows actual pointed object */
      rolBar* briefTxt;            /**< Briefing Text */
      quadroTexto* hourTxt;        /**< Text for Hour */
      janela* shortCutsWindow;     /**< ShortCuts Window */
      janela* miniMapWindow;       /**< MiniMap Window */
      botao* botPerMiniMap;        /**< Character MiniMap Button */

      botao* buttonMenu;           /**< Button to call Menu */
      botao* buttonSave;           /**< Button to call Save Function */
      botao* buttonLoad;           /**< Button to call Load Function */
      oneTabButton* buttonAttackMode;/**< Button to Enter on Attack Mode */
      oneTabButton* buttonMap;     /**< Call MiniMap Button */
      oneTabButton* buttonInventory; /**< Call Inventory Button */
      oneTabButton* buttonEndTurn; /**< End Turn Button */

      inventWindow* inventoryWindow; /**< Inventory Window */
   
      Uint32 lastRead;             /**< Last Verification of I/O */
      Uint32 lastMouse;            /**< Last read from mouse */
      Uint32 lastMousePression;    /**< Last mouse pressure */
      Uint32 lastFPS;              /**< Last obtained FPS */
      Uint32 lastKeyb;             /**< Last read from keyboard */
      double actualFPS;            /**< Actual FPS */

      sound* snd;                  /**< Actual Sounds thing */
      sndfx* walkSound;            /**< Actual walking sound */
      options* option;             /**< Current Options */

      modelList* models;           /**< Current Models List */
      
      featsList* features;         /**< Feats descriptions */
      aligns* alignList;           /**< Alignments List */
      races* raceList;             /**< Races List */
      classes* classList;          /**< Classes List */

      lang language;               /**< Language struct: internationalization */

      partSystem* particleSystem;  /**< The Particle System */
      GLfloat seconds;             /**< Seconds from last actualization */
      
      int imgNumber;               /**< Used on exporting images to make video*/

      int exitEngine;              /**< Control if is to exit the engine */

      int engineMode;              /**< Current Mode of the Engine */

      bool showRange;              /**< If show range or not (CTRL to enable) */

      GLfloat moveCircleX,         /**< The Battle Move Circle X Coordinate */ 
              moveCircleY,         /**< The Battle Move Circle Y Coordinate */
              moveCircleZ;         /**< The Battle Move Circle Z Coordinate */


      /* Combat Things */
      GLuint  normalMoveCircle,    /**< One Movimentation Circle */
              fullMoveCircle,      /**< Full Round Movimentation Circle */
              rangeCircle;         /**< Range Circle */

      fightSystem fight;           /**< The Engine fight System */

      GLuint lastTurnTime;        /**< Time when last calls for turn */
      int fightStatus;            /**< Current Fight Status */
      bool fullMovePCAction;      /**< Taked Full Move Action on round? */
      bool canAttack;             /**< can attack in the round? */
      bool canMove;               /**< can move more in the round? */
      int attackFeat;             /**< attack feat to use. */

      conection* curConection;    /**< Current Mouse Conection */

      part2* effect;

      sky* gameSky;               /**< The internal sky reference */
      sun* gameSun;               /**< The internal sun reference */
      GLfloat hour;               /**< Hour on day time */

#ifdef VIDEO_MODE
      bool startVideo;            /**< Used to start video making */
#endif

};

#endif

