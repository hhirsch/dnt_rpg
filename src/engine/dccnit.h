/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/


#ifndef _dccnit_h
#define _dccnit_h

#include "../gui/farso.h"
#include "../map/map.h"
#include "cursor.h"
#include "personagens.h"
#include "luta.h"
#include "../ia/ai.h"
#include "initial.h"
#include "options.h"
#include "skillwindow.h"
#include "../classes/skills.h"
#include "../classes/feats.h"
#include "../sound/sound.h"
#include "../lang/lang.h"
#include "../particle/partSystem.h"
#include "../fight/fightSystem.h"

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

      double theta;                    /**< Rotation Up*/
      double ph                        /**< Rotation to Sides*/
      double d;                        /**< Zoom */
      double centroX,                  /**< Camera Central X Position */
      centroY,                         /**< Camera Central Y Position */
      centroZ;                         /**< Camera Central Z Position */
      double cameraX,                  /**< Camera X Position */
             cameraY,                  /**< Camera Y Position */
             cameraZ;                  /**< Camera Z Position */
      double deltaY;                   /**< Camera DeltaY */
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
      void Redmensiona(SDL_Surface *screen);

      /*!
       *************************************************************** 
       * Threat Input/Output Events
       * \param screen -> Pointer to the Screen's Surface
       * \param forcaAtualizacao -> to force the draw
       ***************************************************************/
      int  TrataES(SDL_Surface *screen,int *forcaAtualizacao);

      /*! Verify Square on IA TODO oxi, out here.
       * \param quad -> square to verify */
      inline void verificaQuad(Square* quad);
      /*! Verify Square Line on IA TODO oxi, out here.
       * \param centro -> center square of line */
      inline void verificaLinha(Square* centro);
      /*! Threat NPCs IA TODO not here!*/
      int TrataIA();

      /*!
       *************************************************************** 
       * Threat PC Colision (can walk?)
       * \param varX -> Variation on X position
       * \param varZ -> Variation on Z position
       * \param varAlpha -> Variation on angle
       * \return 1 if can walk to new position
       ***************************************************************/
      int podeAndar(GLfloat varX, GLfloat varZ, GLfloat varAlpha);

      /*!
       ************************************************************** 
       * Draws the sky sphere to the list
       * \param lats -> How many latitudes
       * \param longs -> How many longitudes
       ***************************************************************/
      void drawSphereToList(int lats, int longs);

      Map* mapa;                   /**< Actual Engine Map */
      cursor* cursors;             /**< Utilized mouse cursors */
 
      GLfloat matrizVisivel[6][4]; /**< Actual Frustum Matrix */
      GLuint listAtmosfera;        /**< Sky List */
      GLuint ceu;                  /**< Sky Texture */
      int mouseX,                  /**< Actual mouse X coordinates on screen */
          mouseY;                  /**< Actual mouse Y coordinates on screen */
      double xReal,                /**< Actual mouse X coordinates on World */
             zReal,                /**< Actual mouse Z coordinates on World */
             yReal;                /**< Actual mouse Y coordinates on World */

      quadroTexto* FPS;            /**< Text that shows FPS */
      quadroTexto* ObjTxt;         /**< Text that shows actual pointed object */
      janela* janAtalhos;          /**< ShortCuts Window */
      janela* janMiniMapa;         /**< MiniMap Window */
      botao* botPerMiniMap;        /**< MiniMap Button */
   
      Uint32 ultimaLeitura;        /**< Last Verification */
      Uint32 ultimoMouse;          /**< Last read from mouse */
      Uint32 ultimaPressaoMouse;   /**< Last mouse pressure */
      Uint32 ultimaFPS;            /**< Last obtained FPS */
      Uint32 ultimaKeyb;           /**< Last read from keyboard */
      double FPSatual;             /**< Actual FPS */

      sound* snd;                  /**< Actual Sounds thing */
      Mix_Music* musica;           /**< Actual playing music */
      options* option;             /**< Current Options */

      AI* ia;                      /**< IA Module. TODO Not Here. */
      featsList* features;         /**< Feats descriptions */

      lang language;               /**< Language struct: internationalization */

      partSystem* particleSystem;  /**< The Particle System */
      GLfloat segundos;            /**< Seconds from actualization \b NOTUSED */
      
      int imgNumber;               /**< Used on exporting images to make video*/
};

#endif

