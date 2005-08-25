#include "../gui/farso.h"
#include "../map/map.h"
#include "../engine/culling.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHAO      1
#define MURO      2
#define MUROINIC  3
#define OBJETO    4
#define DELMURO   5
#define DELOBJETO 6

double deg2Rad(double x){return 6.2831853 * x/360.0;}

int estado;
GLfloat matrizVisivel[6][4]; /* MAtriz do frustum atual */
GLdouble proj[16];
GLdouble modl[16];
GLint viewPort[4];

int botaoChao(void *jan,void *ljan,SDL_Surface *screen)
{
    estado = CHAO; 
    return(1);
}

int botaoMuro(void *jan,void *ljan,SDL_Surface *screen)
{
    estado = MURO;
    return(1);
}

void Redmensiona(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei) screen->w / (GLsizei) screen->h, 1.0, 650.0);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);
}

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
   GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 300.0, 10.0, 200.0, 1.0 };
   
   /* Carrega a Luz */
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   
   /* Habilita a iluminacao */
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
  
}

void erro()
{
   printf("/\n");
   printf("| Utiliza-se:\n");
   printf("| Para um novo Mapa: MapEditor -e arquivo.map -a ALTURA -l LARGURA\n");
   printf("| Para um Mapa Existente: MapEditor -e arquivo.map\n\n");
   exit(2);
}

void colocaTextura(Map* mapa, int x, int z, GLuint texturaID)
{
    if( (x <= mapa->x) && (z <= mapa->z) )
    {
       Square* quad = mapa->first;
       int aux;
       for(aux = 0; aux<x; aux++) 
          quad = quad->right;
       for(aux = 0; aux<z; aux++)
          quad = quad->down;
       quad->textura = texturaID;
    }
}


int main(int argc, char **argv)
{
   estado = CHAO;
   printf(" DccNitghtmare - Editor de Mapas\n");
   /* Inicia ou abre mapa Existente */
   char* entrada; /*Arquivo de entrada*/
   int w,h;
   int chamadaCorreta=0;
   char c;
   Map* mapa;

   /* Cosntantes de CAMERA */
      double theta=0;
      double phi=0;
      double d=1;
      double centroX = 0;
      double centroZ = 0;
      double centroY = 30;
      double cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
      double cameraY = centroY + (float) d * sin(deg2Rad(theta));
      double cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
      

   while((c=getopt(argc,argv,"e:a:l")) != -1){
      switch(c){
         case 'e':entrada = optarg;
                  chamadaCorreta = 1; 
                  break;
	 case 'a':break;  
         case 'l':break;
         default: erro();
      } 
   } 

   if(!chamadaCorreta) erro();

   /* Inicia o Sistema Bizarro */ 
   SDL_Surface* screen;
   Farso_Iniciar(&screen,"Editor de Mapas");
   Iniciar(screen);  

   mapa = new(Map);
   /* Abre mapa ja existente */
   mapa->open(entrada);

   int sair = 0;
   Uint8 Mbotao;
   Uint8 *teclas;
   int mouseX,mouseY;
   double varX, varZ;
   double orientacao = 0;

   interface* gui = new interface(NULL);
   janela* principal;
   principal = gui->ljan->InserirJanela(0,0,255,127,"Editor de Mapas",1,1,NULL,NULL);
   principal->objetos->InserirBotao(10,17,50,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Chao",1,&botaoChao);
   principal->objetos->InserirBotao(10,37,50,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Muro",1,&botaoMuro);
   principal->objetos->InserirBotao(55,17,125,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Objetos",1,NULL);
   principal->objetos->InserirBotao(55,57,125,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Destruir",1,NULL);
   principal->objetos->InserirBotao(55,37,125,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Pegar Obj",1,NULL);
   principal->objetos->InserirBotao(130,17,200,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Textura >",1,NULL);
   principal->objetos->InserirBotao(130,37,200,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Textura <",1,NULL);
   principal->objetos->InserirBotao(130,57,200,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Objeto >",1,NULL); 
   principal->objetos->InserirBotao(130,77,200,95,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Objeto <",1,NULL);
   principal->Abrir(gui->ljan);
 
   muro* maux = NULL;
   int qx; int qz;

   while(!sair)
   {

      SDL_PumpEvents();
      Mbotao = SDL_GetMouseState(&mouseX,&mouseY);
      teclas = SDL_GetKeyState(NULL);
      if(gui->ManipulaEventos(mouseX,mouseY,Mbotao,teclas)==NADA)
      {
         if(Mbotao & SDL_BUTTON(1))
         {
            double xReal, zReal, yReal;
            float wx,wy,wz;
            wx = mouseX; wy = 600-mouseY; 
            glReadPixels(wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
            //wz = 0;
            gluUnProject( wx, wy, wz, modl, proj, viewPort, &xReal, &yReal, &zReal);
 
            qx = (int)xReal / SQUARESIZE; 
            qz = (int)zReal / SQUARESIZE;

            if (qx > mapa->x) qx = mapa->x;
            else if (qx < 0) qx = 0;
            if (qz > mapa->z) qz = mapa->z;
            else if (qz < 0) qz = 0;
             
            if(estado == CHAO)
               colocaTextura(mapa, (int)xReal / SQUARESIZE, (int)zReal / SQUARESIZE, mapa->Texturas->indice);
            else if (estado == MURO)
            {
                maux = new(muro);
                maux->proximo = mapa->muros;
                mapa->muros = maux;
                maux->x1 = qx*SQUARESIZE;
                maux->x2 = (qx)*SQUARESIZE;
                maux->z1 = qz*SQUARESIZE;
                maux->z2 = (qz)*SQUARESIZE;
                maux->textura = mapa->Texturas->indice;
                estado = MUROINIC;
                
            }
            else if (estado == MUROINIC)
            {
                maux->x2 = (qx)*SQUARESIZE;
                maux->z2 = (qz)*SQUARESIZE;
            }
            //centroX = xReal; centroZ = zReal;
            //printf("%.3f , %.3f , %.3f %d,%d\n",xReal,yReal,zReal,mouseX,mouseY);
         }
         else
         {
             if(estado == MUROINIC)
             {
                /*int max = maux->x2 / SQUARESIZE;
                int x = maux->x1 / SQUARESIZE;
                int z = maux->z1 / SQUARESIZE;
                Square* saux;
                for(aux = maux->x1 / SQUARESIZE;aux<=max;aux++) 
                {
                    saux = quadradoRelativo(x,z);
                }*/

//TODO Colocar Pisavel = 0 para todos quadrados que o muro ocupa
                 estado = MURO;
             }
         }
  
 
         sair = teclas[SDLK_ESCAPE];
         if(teclas[SDLK_UP] || teclas[SDLK_DOWN])
         {
            varX = 2.0 * sin(deg2Rad(phi));
            varZ = 2.0 * cos(deg2Rad(phi));
            if(teclas[SDLK_UP]) 
            {
                 varX *= -1;
                 varZ *= -1;
            }
            centroX += varX;
            centroZ += varZ;
         }
         if(teclas[SDLK_RIGHT] || teclas[SDLK_LEFT])
         {
            varX = 2.0 * sin(deg2Rad(phi)+deg2Rad(90));
            varZ = 2.0 * cos(deg2Rad(phi)+deg2Rad(90));
            if(teclas[SDLK_LEFT])
            {
                varX *= -1;
                varZ *= -1;
            }
            centroX += varX;
            centroZ += varZ;
         }
         if(teclas[SDLK_PAGEUP]) // Sobe com a camera ate visao de cima
         {
            theta += 1;
            if(theta > 89) 
               theta = 89;
         }
         if(teclas[SDLK_PAGEDOWN]) // desce com a camera ate visao em 1ª pessoa
         {
            theta -= 1;
            if(theta < 0)
               theta = 0;
         }
         if(teclas[SDLK_HOME])
         {
            d -= 1;
            if (d<1) d = 1;
         }
         if(teclas[SDLK_END])
         {
            d += 1;
            if (d>300) d = 300;
         }
         if(teclas[SDLK_PERIOD])
         {
            phi -= 1;
            //phi = phi % 360;
         }
         if(teclas[SDLK_COMMA])
         {
            phi += 1;
            //phi = phi % 360;
         }
         if(teclas[SDLK_s])
         {
            mapa->save("../data/mapas/teste.map");
            SDL_Delay(200);
         }
      }
      glClearColor(0,0,0,0);
      glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

      glLoadIdentity();
      /* Redefine a posicao dinamica da camera */
      cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
      cameraY = centroY + (float) d * sin(deg2Rad(theta));
      cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
      gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);
      AtualizaFrustum(matrizVisivel,proj,modl);
      
      //glPopMatrix();
      mapa->draw(cameraX,cameraY,cameraZ,matrizVisivel);
      glDisable(GL_LIGHTING);
      gui->Desenhar(proj,modl,viewPort);
      glEnable(GL_LIGHTING);
      //glPushMatrix();
      glFlush();
      SDL_GL_SwapBuffers();

   }
 
   delete(mapa);
   delete(gui);

}

