/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/


#ifndef _dccnit_h
#define _dccnit_h

#include "../gui/farso.h"
#include "../map/map.h"
#include "cursor.h"
#include "personagens.h"
#include "sun.h"
#include "initial.h"
#include "options.h"
#include "skillwindow.h"
#include "alignwindow.h"
#include "attwindow.h"
#include "camera.h"
#include "util.h"
#include "collision.h"
#include "../classes/skills.h"
#include "../classes/feats.h"
#include "../classes/align.h"
#include "../sound/sound.h"
#include "../lang/lang.h"
#include "../particle/partSystem.h"
#include "../fight/fightSystem.h"

#include "../particle/wave.h"

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
       * Load Initial Menu
       * \param Status -> ON_INIT or IN_GAME
       * \param idTextura -> pointer to initial screen texture
       * \param reloadMusic -> true to reload the music
       * \return initialScreen return Values
       ***************************************************************/
      int InitialScreen(int Status, GLuint* idTextura, bool reloadMusic);

      /*!
       **************************************************************** 
       * Load Character's Screens
       * \param idTextura -> pointer to initial screen texture
       * \return CharacterScreens return values
       ***************************************************************/
      int CharacterScreen(GLuint* idTextura);

      /*!
       *************************************************************** 
       * Load Options Screen
       * \param idTextura -> pointer to initial screen texture
       * \return Options Screen Return Value.
       ***************************************************************/
      int OptionsScreen(GLuint* idTextura);

      /*!
       *************************************************************** 
       * Load Informations Screen
       ***************************************************************/
      void InformationScreen();

      /*!
       *************************************************************** 
       * Reason: Load MiniMap Window
       ***************************************************************/
      void OpenMiniMapWindow();

      /*!
       *************************************************************** 
       * Load ShortCuts Window
       ***************************************************************/
      void OpenShortcutsWindow();

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
       * Calls the game over screen
       ***************************************************************/
      void gameOver();

      /*!
       ************************************************************** 
       * Write actual Time on TXT box
       ***************************************************************/
      void hourToTxt();

      Map* actualMap;              /**< Actual Engine Map */
      cursor* cursors;             /**< Utilized mouse cursors */

      int walkStatus;              /**< Engine Walk Status */

      collision colisionDetect;    /**< The collision detect Module */
 
      GLfloat visibleMatrix[6][4]; /**< Actual Frustum Matrix */
      GLuint skyList;              /**< Sky GL List */
      GLuint sky;                  /**< Sky Texture */
      int mouseX,                  /**< Actual mouse X coordinates on screen */
          mouseY;                  /**< Actual mouse Y coordinates on screen */
      double xReal,                /**< Actual mouse X coordinates on World */
             zReal,                /**< Actual mouse Z coordinates on World */
             yReal;                /**< Actual mouse Y coordinates on World */

      quadroTexto* FPS;            /**< Text that shows FPS */
      quadroTexto* ObjTxt;         /**< Text that shows actual pointed object */
      quadroTexto* briefTxt;       /**< Briefing Text */
      quadroTexto* hourTxt;        /**< Text for Hour */
      janela* shortCutsWindow;     /**< ShortCuts Window */
      janela* miniMapWindow;       /**< MiniMap Window */
      botao* botPerMiniMap;        /**< Character MiniMap Button */

      botao* buttonMenu;           /**< Butto to call Menu */
      oneTabButton* buttonAttackMode;/**< Button to Enter on Attack Mode */
      oneTabButton* buttonMap;     /**< Call MiniMap Button */
      oneTabButton* buttonEndTurn; /**< End Turn Button */
   
      Uint32 lastRead;             /**< Last Verification of I/O */
      Uint32 lastMouse;            /**< Last read from mouse */
      Uint32 lastMousePression;    /**< Last mouse pressure */
      Uint32 lastFPS;              /**< Last obtained FPS */
      Uint32 lastKeyb;             /**< Last read from keyboard */
      double actualFPS;            /**< Actual FPS */

      sound* snd;                  /**< Actual Sounds thing */
      Mix_Music* music;            /**< Actual playing music */
      options* option;             /**< Current Options */

      featsList* features;         /**< Feats descriptions */
      aligns* alignList;           /**< Alignments List */

      lang language;               /**< Language struct: internationalization */

      partSystem* particleSystem;  /**< The Particle System */
      GLfloat seconds;             /**< Seconds from last actualization */
      
      int imgNumber;               /**< Used on exporting images to make video*/

      int exitEngine;              /**< Control if is to exit the engine */

      int engineMode;              /**< Current Mode of the Engine */

      GLfloat moveCircleX,         /**< The Battle Move Circle X Coordinate */ 
              moveCircleY,         /**< The Battle Move Circle Y Coordinate */
              moveCircleZ;         /**< The Battle Move Circle Z Coordinate */


      /* Combat Things */
      GLuint  normalMoveCircle,    /**< One Movimentation Circle */
              fullMoveCircle;      /**< Full Round Movimentation Circle */

      fightSystem fight;           /**< The Engine fight System */

      GLuint lastTurnTime;        /**< Time when last calls for turn */
      int fightStatus;            /**< Current Fight Status */
      bool fullMovePCAction;      /**< Taked Full Move Action on round? */
      bool canAttack;             /**< can attack in the round? */
      bool canMove;               /**< can move more in the round? */
      int attackFeat;             /**< attack feat to use. */

      waves* waveTest;

      sun* gameSun;               /**< The internal sun reference */
      GLfloat hour;               /**< Hour on day time */

#ifdef VIDEO_MODE
      bool startVideo;            /**< Used to start video making */
#endif

};

#endif

