/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include <stdlib.h>
#include <SDL/SDL.h>
#include "farso.h"


int botao1_Pressionado(void* jan,void *ljan,SDL_Surface *screen)
{
   printf("Bla\n");
   return(0);
}


int main(int argc, char *argv[])
{
    //cores_Iniciar();
   SDL_Surface *screen;
   Farso_Iniciar(&screen,"Teste da GUI");
   glViewport(0,0,800,600);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrtho(0.0f, 800, 600, 0.0f, -1.0f, 1.0f);
   //gluPerspective(45.0, (GLsizei) screen->w / (GLsizei) screen->h, 1.0, 1000.0);
	
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

    cores Cores;
    Cores.Iniciar();

   // cor_Definir(0,0,0);
   // retangulo_Colorir(screen,0,0,639,479,0);

    interface* interf = new interface("../data/pics/akira.bmp");

    janela *jan;
    jan=interf->ljan->InserirJanela(330,100,500,350,"Janela",1,1,NULL,NULL);
    jan->objetos->InserirFigura(8,20,"../data/pics/logan/cara.bmp");
    jan->objetos->InserirQuadroTexto(90,20,160,95,1,"Fale humano ridiculo.Tens alguma comida?");
    jan->objetos->InserirSelTexto(8,100,160,250,"1 - Ora, o que faz um mutante nesta janela?",
                      "2 - Aonde esta sua orelha esquerda,mutante?",
                      "3 - (entrega um grilo ao mutante) Fiquei sabendo que eh esta sua comida habitual.", "4 - EBA!","5 - Uai so, que que eu to fazendo aqui!",NULL);
    /* janela_Desenhar(*j.primeiro->proximo,1,screen, REDESENHA); */
    jan->Ativar(interf->ljan,interf->tela2D);
    jan=interf->ljan->InserirJanela(10,10,200,220,"Hehehe",1,1,NULL,NULL);
    jan->objetos->InserirBotao(143,25,183,43,Cores.corBot.R,Cores.corBot.G,
                   Cores.corBot.B,"Botao",1,
                   &botao1_Pressionado);
    jan->objetos->InserirBarraTexto(7,27,137,41,"Barra de Texto",0,NULL);
    jan->Ativar(interf->ljan,interf->tela2D);
    jan=interf->ljan->InserirJanela(20,351,265,479,"Mais Uma",1,1,NULL,NULL);
    jan->objetos->InserirCxSel(20,20,1);
    jan->objetos->InserirQuadroTexto(32,18,240,35,0,"cxSel + quadTexto");
    jan->Ativar(interf->ljan,interf->tela2D);

    


    while (interf->ManipulaEventos(screen));
    
    atexit(SDL_Quit);
     
    delete(interf);
   
    printf("Teste finalizado com exito\n");

    return(0);

}
