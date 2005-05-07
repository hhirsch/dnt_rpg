#include "dccnit.h"
#include <math.h>
#define LADOHEXA 16
#define DELAY 0

/* Conversor de graus para radianos */
inline double deg2Rad(double x){return 3.14159 * x/360.0;}

/*********************************************************************
 *                       Construtor da Engine                        *
 *********************************************************************/
engine::engine(/*Tmapa map*/)
{
   NPCs = new (Lpersonagem);
   PCs  = new (Lpersonagem);
   theta=0.5;
   phi=0;
   d=150;
   centroX = centroY = centroZ = 0;
   
   //mapa = map;
}

/*********************************************************************
 *                      Destruidor da Engine                         *
 *********************************************************************/
engine::~engine()
{
   delete(NPCs);
   delete(PCs);
}

/*********************************************************************
 *                 Redmensiona a Engine para a Tela                  *
 *********************************************************************/
void engine::Redmensiona(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   //glOrtho(-61.0,61.0,-61.0,61.0,0.01,100.0);
   gluPerspective(45.0, 1.0, 1.0, 1000.0);
   
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity();
   //gluLookAt (7.0,7.0, 7.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
   float x = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   float y = centroY + (float) 2*d * sin(deg2Rad(theta));
   float z = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(x,y,z, centroX,centroY,centroZ,0,1,0);
}

/*********************************************************************
 *                    Inicia e Engine para Uso                       *
 *********************************************************************/
void engine::Iniciar(SDL_Surface *screen)
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
   GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat high_shininess[] = { 100.0 };
   
   /* Carrega a Luz */
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
   
   /* Habilita a iluminacao */
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}


/*********************************************************************
 *                      Tratamento do Teclado                        *
 *********************************************************************/
int engine::TrataES(SDL_Surface *screen)
{
   int redesenha = 0;
   int redmensiona = 0;
   SDL_PumpEvents();
   Uint8 *keys;
   keys = SDL_GetKeyState(NULL);
   if ( keys[SDLK_q] )
      return(0);
   if(keys[SDLK_x])
   {
      RotacaoX+=5;
      if (RotacaoX >= 360) RotacaoX = 0;
      redesenha = 1;
   }
   if(keys[SDLK_y])
   {
      RotacaoY+=5;
      if (RotacaoY >= 360) RotacaoY = 0;
      redesenha = 1;
   }
   if(keys[SDLK_z])
   {
      RotacaoZ+=5;
      if (RotacaoZ >= 360) RotacaoZ = 0;
      redesenha = 1;
   }
   if(keys[SDLK_r])
   { 
      RotacaoX = 0;
      RotacaoY = 0;
      RotacaoZ = 0;
      redesenha = 1;
   }
   if(keys[SDLK_UP])
   {
       if (d>1)
       {
          d--;
          redmensiona = 1;
       }
   }
   if(keys[SDLK_DOWN])
   {
       d++;
       redmensiona = 1;
   }
   if(keys[SDLK_LEFT])
   {
       phi -=2;  // change
       redmensiona = 1;
   }
   if(keys[SDLK_RIGHT])
   {
      phi +=2;
      redmensiona = 1;
   }
   if(keys[SDLK_PAGEUP])
   {
      if (theta < 85)
      {
         theta +=2;
         redmensiona = 1;
      }
   }
   if(keys[SDLK_PAGEDOWN])
   {
      if (theta >-85)
      {
         theta -=2;
         redmensiona = 1;
      }
   }
   if(redmensiona)
   {
      Redmensiona(screen);
      redesenha = 1;
   }
   if(redesenha)
   {
      Desenhar();
      SDL_GL_SwapBuffers();
      SDL_Delay(DELAY);
   }
   return(1);
}


/*********************************************************************
 *                Funcao para desenhar a cena                        *
 *********************************************************************/
void engine::Desenhar()
{
   glClear ((GL_COLOR_BUFFER_BIT));
   glClear (GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   glRotatef(RotacaoX,1,0,0);
   glRotatef(RotacaoY,0,1,0);
   glRotatef(RotacaoZ,0,0,1);
      personagem* per = (personagem*) PCs->primeiro->proximo;
      int aux;
      for(aux=0;aux < PCs->total;aux++)
      {
         glmDraw(per->modelo3d, GLM_NONE | GLM_COLOR | GLM_SMOOTH | GLM_TEXTURE);
         per = (personagem*) per->proximo;
         glTranslatef(30,0,0);
      }
   glPopMatrix();
   glFlush();
}

/*********************************************************************
 *                          Roda a Engine                            *
 *********************************************************************/
int engine::Rodar(SDL_Surface *surface)
{
   /* Desenha a primeira Cena */
   Desenhar();
   SDL_GL_SwapBuffers();
  
   /* Roda realmente a engine */
   while(TrataES(surface));
   return(1);
}


