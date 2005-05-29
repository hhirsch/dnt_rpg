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
   Farso_Iniciar(&screen,"Teste Engine DccNiTghtmare");
   engine* Engine = new engine("../data/mapas/arquivo.map");
   Engine->Iniciar(screen);
   //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
// int aux;
// for(aux = 0;aux<10;aux++)
   Engine->PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/cara.bmp",0,0,
                 "Rainha","../data/models/personagens/logan_completo_final.obj","../data/pics/logan/");
   /*Engine->PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/cara.bmp",0,0,
                 "Homer","../data/models/logan_completo_final.obj","../../PICS/Logan/");
   Engine->PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/cara.bmp",0,0,
                 "Castelo","../data/models/castelo.obj","../../PICS/Logan");
   Engine->PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/cara.bmp",0,0,
                 "Logan","../data/models/BMWZ4J.obj","../../PICS/Logan/");

   Engine->PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/cara.bmp",0,0,
                 "Logan","../data/models/computador.obj","../../PICS/Logan/");

   Engine->PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/cara.bmp",0,0,
                 "Logan","../data/models/mesa_redonda.obj","../../PICS/Logan/");*/

   Engine->Rodar(screen);
 
   free(Engine);

   return(0); 
}


