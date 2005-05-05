#include "../GUI/farso.h"
#include "dccnit.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <stdlib.h>


/*********************************************************************
 *                           Corpo Principal                         *
 *********************************************************************/
int main()
{
   engine* Engine = new(engine);
   SDL_Surface *screen;
   Farso_Iniciar(&screen,"Teste Engine DccNiTghtmare");
   Engine->Iniciar(screen);
// int aux;
// for(aux = 0;aux<10;aux++)
   Engine->PCs->InserirPersonagem(7,6,9,7,"pic/logan.bmp",0,0,"Logan",
                                 "pic/BMWZ4J.obj","../PICS/Logan/");

   Engine->Rodar(screen);
 
   free(Engine);

   return(0);
}


