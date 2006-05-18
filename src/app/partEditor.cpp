#include "../gui/farso.h"
#include "../particle/particle.h"
#include "../particle/part1.h"
#include "../particle/part2.h"
#include "../particle/part3.h"
#include "../particle/part4.h"
#include "../particle/part5.h"
#include "../particle/part6.h"
#include "../engine/culling.h"
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

inline double deg2Rad(double x){return 3.1415927 * x/180.0;}

bool sair = false;
int tipo = 1;

barraTexto* bartSalvar;
GLfloat matrizVisivel[6][4]; /* Matriz do frustum atual */
GLdouble proj[16];
GLdouble modl[16];
GLint viewPort[4];

part1* p1;
part2* p2;
part3* p3;
part4* p4;
part5* p5;
part6* p6;

/************************************************************************
 *                     Redmensionamento da Janela SDL                   *
 ************************************************************************/
void Redmensiona(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei) screen->w / (GLsizei) screen->h, 1.0, 650.0);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/************************************************************************
 *                     Chamada Inicial SDL                              *
 ************************************************************************/
void Iniciar(SDL_Surface *screen)
{
  
   Redmensiona(screen);
   
   /* Limpa */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);

   /* Define detalhamento */
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Definicao da Luz */
   /*GLfloat light_ambient[] = { 0.8, 0.6, 0.6, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 30.0, 10.0, 20.0, 0.0 };
   
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/

   GLfloat light_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   
   /* Habilita a iluminacao */
   glEnable(GL_LIGHTING);
  
}

/************************************************************************
 *                     Chamada de Erro de Invocacao                     *
 ************************************************************************/
void erro()
{
   printf("/\n");
   printf("| Use:\n");
   printf("| To edit a new Particle: partEditor -n -t X\n");
   printf("| To edit an existent Particle: partEditor -e file.par -t X\n\n");
   exit(2);
}


/************************************************************************
 *                     Trata Botao Sair                                 *
 ************************************************************************/
int botaoSair(void *jan,void *ljan, SDL_Surface *screen)
{
   sair = true;
   return(1);
}


/************************************************************************
 *                     Escopo Principal                                 *
 ************************************************************************/
int main(int argc, char **argv)
{
   printf(" DccNitghtmare - Particle Editor\n");
   /* Inicia ou abre mapa Existente */
   char* entrada = ""; /*Arquivo de entrada*/
   int chamadaCorreta=0;
   char c;
   
   /* Cosntantes de CAMERA */
      double theta=37;
      double phi=0;
      double d=210;
      double centroX = 70;
      double centroZ = 208;
      double centroY = 30;
      double cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
      double cameraY = centroY + (float) d * sin(deg2Rad(theta));
      double cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
      

   int novo = 0;

   

   while((c=getopt(argc,argv,"e:a:l:n")) != -1){
      switch(c){
         case 'e':{
                     entrada = optarg;
                     chamadaCorreta = 1; 
                     novo = 0;
                     break;
                  }
	 case 'a':break;  
         case 'l':break;
         default: erro();
      } 
   } 

   if(!chamadaCorreta) erro();

   printf("Tipo do Sistema: ");
   scanf("%d",&tipo);

   /* Inicia o Sistema Bizarro */ 
   SDL_Surface* screen;
   Farso_Iniciar(&screen,"DccNiTghtmare's Particle Editor");
   Iniciar(screen);

   switch(tipo)
   {
      case 1:
             { 
                p1 = new part1(50,60,120,entrada);
                break;
             }
      case 2:
             {
                p2 = new part2(50,0,120,entrada);
                break;
             }
      case 3:
             {
                p3 = new part3(50,20,120);
                break;
             }
      case 4:
             {
                p4 = new part4(50,0,120,entrada);
                break;
             }
      case 5:
             {
               p5 = new part5(50,30,120,entrada);
               break;
             }
      case 6:
             {
               p6 = new part6(50,250,120,entrada);
               break;
             }
      default:
             {
                printf("Que merda é essa???");
                erro();
                break;
             }
   }


   Uint8 Mbotao;
   Uint8 *teclas;
   int mouseX,mouseY;

   interface* gui = new interface(NULL);
   janela* janSalvar;

   janSalvar = gui->ljan->InserirJanela(0,0,127,63,"Actions",1,1,NULL,NULL);
   janSalvar->objetos->InserirBotao(10,37,50,55,janSalvar->Cores.corBot.R,
                                                janSalvar->Cores.corBot.G,
                                                janSalvar->Cores.corBot.B,
                                                "Save",1,NULL);
   janSalvar->objetos->InserirBotao(77,37,117,55,janSalvar->Cores.corBot.R,
                                                janSalvar->Cores.corBot.G,
                                                janSalvar->Cores.corBot.B,
                                                "Exit",1,&botaoSair);
   bartSalvar = janSalvar->objetos->InserirBarraTexto(10,17,118,33,
                                         entrada,
                                         0,NULL);

   janSalvar->fechavel = 0;
   janSalvar->Abrir(gui->ljan);

   float wx,wy,wz;
   double xReal, zReal, yReal;

   double varTempo, segundos;
   double ultimaLeitura = 0;
   

   while(!sair)
   {

      varTempo = (SDL_GetTicks() - ultimaLeitura);
      if(varTempo >= 20)
      {
         ultimaLeitura = SDL_GetTicks();
         segundos = varTempo / 1000.0;
         SDL_PumpEvents();
         Mbotao = SDL_GetMouseState(&mouseX,&mouseY);
         teclas = SDL_GetKeyState(NULL);

         wx = mouseX; wy = 600-mouseY; 
              
        // glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
        // gluUnProject(wx,wy,wz,modl,proj,viewPort, &xReal, &yReal, &zReal);

         if(gui->ManipulaEventos(mouseX,mouseY,Mbotao,teclas)==NADA)
         {
         } 

         /* Draw */
  
         glClearColor(0,0,0,0);
         glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

         glLoadIdentity();
         /* Redefine a posicao dinamica da camera */
         cameraX = centroX+(float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
         cameraY = centroY+(float) d * sin(deg2Rad(theta));
         cameraZ = centroZ+(float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
         gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);
         AtualizaFrustum(matrizVisivel,proj,modl);
 
         
         /* Part Draw */
         glPushMatrix();
         switch(tipo)
         {
           case 1:
                  { 
                     p1->NextStep(segundos);
                     break;
                  }
           case 2:
                  {
                     p2->NextStep(segundos);
                     break;
                  }
           case  3:
                  {
                     p3->NextStep(segundos);
                     break;
                  }
           case 4:
                  {
                     p4->NextStep(segundos);
                     break;
                  }
           case 5:
                  {
                    p5->NextStep(segundos);
                    break;
                  }
           case 6:
                  {
                    p6->NextStep(segundos);
                    break;
                  }
         }
         glPopMatrix();

         glDisable(GL_LIGHTING);
/*         glDisable(GL_DEPTH_TEST);
         glDisable(GL_TEXTURE_2D);
         glDisable(GL_BLEND);
         glDisable(GL_COLOR_MATERIAL);*/
         //glPushMatrix();
           
            gui->Desenhar(proj,modl,viewPort);
            
         //glPopMatrix();

         glEnable(GL_LIGHTING);
         glEnable(GL_DEPTH_TEST);
         glFlush();
         SDL_GL_SwapBuffers();
      }

   }

   delete(gui);

}

