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

#ifdef REDE
  #include "../net/client.h"
#endif

class engine
{
   public:
 
      engine();                        /* Engine Constructor */
      ~engine();                       /* Engine Desctructor */
 
      Lpersonagem *NPCs;               /* NPC's List */
      Lpersonagem *PCs;                /* PC's List */

      #ifdef REDE
         char *server;                 /* Server to Connect */
         clientdata_t clientData;      /* Client Data */
      #endif

      double theta, phi, d;            /* Rotation Up, Sides and Zoom */
      double centroX,centroY,centroZ;  /* Camera Central Position */
      double cameraX,cameraY,cameraZ;  /* Camera Position */
      double deltaY;                   /* Camera DeltaY */
      interface* gui;                  /* GUI use on Engine */
      GLdouble proj[16];               /* Projection Matrix */
      GLdouble modl[16];               /* ModelView Matrix  */
      GLint viewPort[4];               /* ViewPort Matrix */

      /*************************************************************** 
       * Reason: Init Engine to utilization  
       * Param:
       *        screen -> Pointer to the Screen's Surface
       ***************************************************************/
      void Init(SDL_Surface *screen);

      /*************************************************************** 
       * Reason: Runs the Engine  
       * Param:
       *        screen -> Pointer to the Screen's Surface
       ***************************************************************/
      int  Run(SDL_Surface *surface);    

      /*************************************************************** 
       * Reason: Draws the Engine  
       * Param:
       * Obs:    Need to run SDL_GL_SwapBuffers() after;
       ***************************************************************/
      void Draw();

      /*************************************************************** 
       * Reason: Load and activate Map to engine
       * Param:
       *          arqMapa -> string with filename
       *          RecarregaPCs -> 1 to reload PCs 0 to not.
       ***************************************************************/
      int LoadMap(string arqMapa, int RecarregaPCs);

      /*************************************************************** 
       * Reason: Load Initial Menu
       * Param:
       *          Status -> ON_INIT or IN_GAME
       *          idTextura -> pointer to initial screen texture
       *          reloadMusic -> true to reload the music
       ***************************************************************/
      int InitialScreen(int Status, GLuint* idTextura, bool reloadMusic);

      /*************************************************************** 
       * Reason: Load Character's Screens
       * Param:
       *          idTextura -> pointer to initial screen texture
       ***************************************************************/
      int CharacterScreen(GLuint* idTextura);

      /*************************************************************** 
       * Reason: Load Options Screen
       * Param:
       *          idTextura -> pointer to initial screen texture
       ***************************************************************/
      int OptionsScreen(GLuint* idTextura);

      /*************************************************************** 
       * Reason: Load Informations Screen
       * Param:
       ***************************************************************/
      void InformationScreen();

      /*************************************************************** 
       * Reason: Load MiniMap Window
       * Param:
       ***************************************************************/
      void OpenMiniMapWindow();

      /*************************************************************** 
       * Reason: Load ShortCuts
       * Param:
       ***************************************************************/
      void OpenShortcutsWindow();

   private:

      /*************************************************************** 
       * Reason: Define Screen to actual Screen Size  
       * Param:
       *        screen -> Pointer to the Screen's Surface
       ***************************************************************/
      void Redmensiona(SDL_Surface *screen);

      /*************************************************************** 
       * Reason: Threat Input/Output Events
       * Param:
       *        screen -> Pointer to the Screen's Surface
       *        forcaAtualizacao -> to force the draw
       ***************************************************************/
      int  TrataES(SDL_Surface *screen,int *forcaAtualizacao);

      /* Threat NPCs IA TODO not here!*/
      inline void verificaQuad(Square* quad);
      inline void verificaLinha(Square* centro);
      int TrataIA();

      /*************************************************************** 
       * Reason: Threat PC Colision
       * Param:
       *        varX, varZ -> Variation Position
       *        varAlpha -> Variation angle
       ***************************************************************/
      int podeAndar(GLfloat varX, GLfloat varZ, GLfloat varAlpha);

      /*************************************************************** 
       * Reason: Draws the sky sphere to the list
       * Param:
       *        lats -> How many latitudes
       *        longs -> How many longitudes
       ***************************************************************/
      void drawSphereToList(int lats, int longs);

      Map* mapa;                   /* Actual Engine Map */
      cursor* cursors;             /* Utilized mouse cursors */
 
      GLfloat matrizVisivel[6][4]; /* Actual Frustum Matrix */
      GLuint listAtmosfera;        /* Sky List */
      GLuint ceu;                  /* Sky Texture */
      int mouseX,mouseY;           /* Actual mouse coordinates on screen */
      double xReal, zReal, yReal;  /* Actual mouse coordinates on World */

      quadroTexto* FPS;            /* Text that shows FPS */
      quadroTexto* ObjTxt;         /* Text that shows actual pointed object */
      janela* janAtalhos;          /* ShortCuts Window */
      janela* janMiniMapa;         /* MiniMap Window */
      botao* botPerMiniMap;        /* MiniMap Button */
   
      Uint32 ultimaLeitura;        /* Last Verification */
      Uint32 ultimoMouse;          /* Last read from mouse */
      Uint32 ultimaPressaoMouse;   /* Last mouse pressure */
      Uint32 ultimaFPS;            /* Last obtained FPS */
      Uint32 ultimaKeyb;           /* Last read from keyboard */
      double FPSatual;             /* Actual FPS */

      sound* snd;                  /* Actual Sounds thing */
      Mix_Music* musica;           /* Actual playing music */
      options* option;             /* Current Options */

      AI* ia;                      /* IA Module. TODO Not Here. */
      featsList* features;         /* Feats descriptions */

      lang language;               /* Language struct: internationalization */

      partSystem* particleSystem;  /* The Particle System */
      GLfloat segundos; 
      
      int imgNumber;
};

#endif

