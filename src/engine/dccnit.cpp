/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "dccnit.h"
#include <math.h>

#define LADOHEXA 16
#define DELAY 0
#define ANDAR 1.0
#define DELTACAMERA 30.0

/* Conversor de graus para radianos */
inline double deg2Rad(double x){return 6.2831853 * x/360.0;}

/*********************************************************************
 *                       Construtor da Engine                        *
 *********************************************************************/
engine::engine(char* arqMapa)
{
   NPCs = new (Lpersonagem);
   PCs  = new (Lpersonagem);
   mapa = new(Map);
   mapa->open(arqMapa);
   mapaDesenhar = glGenLists(1);
   glNewList(mapaDesenhar,GL_COMPILE);
     mapa->draw();
   glEndList();  
   theta=0;
   phi=0;
   d=150;
   centroX = centroZ = 0;
   centroY = 30;
   //deltaCameraX = 0;
   //deltaCameraZ = 0;   
   //mapa = map;
}

/*********************************************************************
 *                      Destruidor da Engine                         *
 *********************************************************************/
engine::~engine()
{
   glDeleteLists(mapaDesenhar,1);
   delete(NPCs);
   delete(PCs);
   delete(mapa);
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
   gluPerspective(45.0, (GLsizei) screen->w / (GLsizei) screen->h, 1.0, 1000.0);
   
   glMatrixMode (GL_MODELVIEW);
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
   GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat high_shininess[] = { 100.0 };
   
   /* Carrega a Luz */
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   /*glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);*/
   
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
   double varX, varZ; // para evitar de ter de calcular 2 vezes
   SDL_PumpEvents();

   /* Tratamento das Teclas */
   Uint8 *keys;
   keys = SDL_GetKeyState(NULL);
   if ( keys[SDLK_ESCAPE] ) // Sai da Engine
      return(0);

   /* Tratamento das teclas para a Camera */
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
   if(keys[SDLK_UP])  // Aumenta o Zoom
   {
       if (d>80)
       {
          d--;
          redesenha = 1;
       }
   }
   if(keys[SDLK_DOWN]) // Diminui o Zoom
   {
     if(d<280)
     {
       d++; 
       redesenha = 1;
     }
   }
   if(keys[SDLK_LEFT]) // Roda Camera Antihorariamente
   {
       phi -=2;  
       redesenha = 1;
   }
   if(keys[SDLK_RIGHT]) // Roda Camera Horariamente
   {
      phi +=2;
      redesenha = 1;
   }
   if(keys[SDLK_PAGEUP]) // Sobe com a camera ate visao de cima
   {
      if (theta < 89)
      {
         theta +=1;
         redesenha = 1;
      }
   }
   if(keys[SDLK_PAGEDOWN]) // desce com a camera ate visao em 1� pessoa
   {
      if (theta > 0)
      {
         theta -=1;
         redesenha = 1;
      }
   }
   /* Tratamento da tecla para Movimentacao do Personagem */

   if(keys[SDLK_q] || keys[SDLK_e])
   {
      varX = ANDAR * sin(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
      varZ = ANDAR * cos(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
      if(keys[SDLK_q]) // Anda com personagem de lado para esquerda
      {
         PCs->personagemAtivo->posicaoLadoX -= varX;
         PCs->personagemAtivo->posicaoLadoZ -= varZ;
         redesenha = 1;
      }
      if(keys[SDLK_e]) // Anda com personagem para esquerda
      {
         PCs->personagemAtivo->posicaoLadoX += varX;
         PCs->personagemAtivo->posicaoLadoZ += varZ;
         redesenha = 1;
      }
   }
   if(keys[SDLK_w] || keys[SDLK_s])
   {
      varX = ANDAR * sin(deg2Rad(PCs->personagemAtivo->orientacao));
      varZ = ANDAR * cos(deg2Rad(PCs->personagemAtivo->orientacao));
      if(keys[SDLK_w]) // Anda com personagem para frente
      {
           PCs->personagemAtivo->posicaoLadoX -= varX;
           PCs->personagemAtivo->posicaoLadoZ -= varZ;
           centroZ -= varZ;
           centroX -= varX;
           redesenha = 1;
      }
      if(keys[SDLK_s]) // Anda com personagem para tras
      {
          PCs->personagemAtivo->posicaoLadoX += varX;
          PCs->personagemAtivo->posicaoLadoZ += varZ;
          centroX += varX;
          centroZ += varZ;
          redesenha = 1;
      }
   }

   if(keys[SDLK_a]) // Gira personagem antihorariamente 
   {
      PCs->personagemAtivo->orientacao += 1.0;
      if(PCs->personagemAtivo->orientacao > 360.0)
         PCs->personagemAtivo->orientacao = 1.0;
      redesenha = 1;
   }
   if(keys[SDLK_d]) // Gira o personagem horariamente
   {
      PCs->personagemAtivo->orientacao -= 1.0;
      if(PCs->personagemAtivo->orientacao < 0.0)
         PCs->personagemAtivo->orientacao = 360.0;

      redesenha = 1;
   }
   if(keys[SDLK_TAB]) //troca de personagem ativo
   {
      if(keys[SDLK_LCTRL]) //para tras
      {
         PCs->personagemAtivo = (personagem*)PCs->personagemAtivo->anterior;
         if(PCs->personagemAtivo == PCs->primeiro)
            PCs->personagemAtivo = (personagem*)PCs->primeiro->anterior;
      }
      else //para frente
      {
         PCs->personagemAtivo = (personagem*)PCs->personagemAtivo->proximo;
         if(PCs->personagemAtivo == PCs->primeiro)
            PCs->personagemAtivo = (personagem*)PCs->primeiro->proximo;
      }
      SDL_Delay(100);
   }
   if(keys[SDLK_b])
   {
      if(keys[SDLK_LSHIFT])
      {
         PCs->personagemAtivo->braco_d->rotacao[1]+=1;
         PCs->personagemAtivo->mao_d->rotacao[1]+=1;
         PCs->personagemAtivo->ante_d->rotacao[1]+=1;
      }
      else
      {
         PCs->personagemAtivo->braco_d->rotacao[1]-=1;
         PCs->personagemAtivo->mao_d->rotacao[1]-=1;
         PCs->personagemAtivo->ante_d->rotacao[1]-=1;
      }
      redesenha = 1;
   }
   if(keys[SDLK_i])
   {
      printf("Orientacao %f�\t sen %f\tcos %f\trad %f\n",
              PCs->personagemAtivo->orientacao,
              sin(deg2Rad(PCs->personagemAtivo->orientacao)),
              cos(deg2Rad(PCs->personagemAtivo->orientacao)),
              deg2Rad(PCs->personagemAtivo->orientacao));
      printf("PosicaoLadoX %f\n",PCs->personagemAtivo->posicaoLadoX);
      printf("PosicaoLadoZ %f\n",PCs->personagemAtivo->posicaoLadoZ);
      SDL_Delay(100);
   }
   if(keys[SDLK_t])
   {
      GLuint aux = 0;
      GLMtexture* tex = PCs->personagemAtivo->modelo3d->texturas;
      while(aux<PCs->personagemAtivo->modelo3d->numtexturas)
      {
         printf("Textura: %s\n",tex->nome);
         tex = tex->proximo;
         aux++;
      }
      SDL_Delay(100);
   }
      

   /* Tratamento do Mouse */
   int x,y;
   SDL_GetMouseState(&x,&y);

   /* Tratamento do Mouse para Camera */
   if(x == 0)    // Gira a Camera Antihorariamente
   {
      phi-=2; 
      redesenha = 1;  
   }
   if(x == screen->w-1) // Gira a camera horariamente
   {
     phi+=2; 
     redesenha = 1;
   }
  
   if(redesenha)
   {
      Uint32 antes = SDL_GetTicks();
      Desenhar();
      SDL_GL_SwapBuffers();
      antes = SDL_GetTicks() - antes;
      printf("FPS: %f\t",1000.0 / (float)antes);
      SDL_Delay(DELAY);
   }
   return(1);
}


/*********************************************************************
 *                Funcao para desenhar a cena                        *
 *********************************************************************/
void engine::Desenhar()
{
    glLoadIdentity();
   //gluLookAt (7.0,7.0, 7.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
   float x = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   float y = centroY + (float) 2*d * sin(deg2Rad(theta));
   float z = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(x,y,z, centroX,centroY,centroZ,0,1,0);

   glClear ((GL_COLOR_BUFFER_BIT));
   glClear (GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   glRotatef(RotacaoX,1,0,0);
   glRotatef(RotacaoY,0,1,0);
   glRotatef(RotacaoZ,0,0,1);
   //mapa->draw();
   glCallList(mapaDesenhar);
      personagem* per = (personagem*) PCs->primeiro->proximo;
      int aux;
      for(aux=0;aux < PCs->total;aux++)
      {
         glPushMatrix();
         /* O personagem nao movimenta no eixo Y, mas sim no Z (^) e X (>) */
         glTranslatef(per->posicaoLadoX, 0 ,per->posicaoLadoZ);
         glRotatef(per->orientacao,0,1,0);
         //glCallList(per->personagemDesenhar);
         glmDraw(per->modelo3d);
         glPopMatrix();
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


