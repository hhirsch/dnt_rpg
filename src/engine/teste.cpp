/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "../gui/farso.h"
#include "dccnit.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <stdlib.h>


/*********************************************************************
 *                           Corpo Principal                         *
 *********************************************************************/
int main()
{
  
   SDL_Surface *screen;
   
   #ifdef REDE
      printf("IP ou Nome do servidor: ");
      char* server = (char*) malloc(55*sizeof(char));
      scanf("%s",server);
      printf("Vou conectar entao a %s\n",server);
   #endif   
   Farso_Iniciar(&screen,"Teste Engine DccNiTghtmare");
   engine* Engine = new engine("../data/mapas/mapaTp4.map");
   Engine->Iniciar(screen);
   #ifdef REDE
      Engine->server = server;
   #endif
 //  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

   Engine->PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/portrait.jpg",0,0,
                 "Rainha","../data/models/personagens/logan_completo_final.obj","../data/pics/logan/");

   personagem* per;
   per = Engine->NPCs->InserirPersonagem(7,6,10,6,"../data/pics/logan/portrait.jpg",0,
                     0,"NPC","../data/models/personagens/logan_completo_final.obj", "../data/pics/logan/");
   per->posicaoLadoX = 30;
   per->posicaoLadoZ = 20;

   Engine->Rodar(screen);

   #ifdef REDE
      free(Engine->server); 
   #endif
   free(Engine);

   return(0); 
}


