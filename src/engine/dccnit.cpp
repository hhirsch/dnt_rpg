/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "dccnit.h"
#include "culling.h"
#include <math.h>

#define LADOHEXA 16
#define DELAY 0
#define ANDAR 1.0
#define DELTACAMERA 30.0
#define ZOOMMAXIMO 80
#define ZOOMMINIMO 280

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
   //mapaDesenhar = glGenLists(1);
   //glNewList(mapaDesenhar,GL_COMPILE);
    // mapa->draw();
   //glEndList();  
   theta=0;
   phi=0;
   d=150;
   RotacaoX = RotacaoY = RotacaoZ = 0;
   centroX = centroZ = 0;
   centroY = 30;
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   //deltaCameraX = 0;
   //deltaCameraZ = 0;   
   //mapa = map;
}

/*********************************************************************
 *                      Destruidor da Engine                         *
 *********************************************************************/
engine::~engine()
{
   //glDeleteLists(mapaDesenhar,1);
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
   GLfloat light_ambient[] = { 0.75, 0.75, 0.75, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
   /*GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat high_shininess[] = { 100.0 };*/
   
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
  
   /*glEnable(GL_FOG);
   {
     GLfloat fogCor[4] = {1.0,1.0,1.0,1.0}; 
     glFogi(GL_FOG_MODE,GL_LINEAR);
     glFogfv(GL_FOG_COLOR,fogCor);
     glFogf(GL_FOG_DENSITY,0.35);
     glHint(GL_FOG_HINT,GL_DONT_CARE);
     glFogf(GL_FOG_START,300.0);
     glFogf(GL_FOG_END,1000.0);
   }*/

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
       if (d>ZOOMMAXIMO)
       {
          d--;
          redesenha = 1;
       }
   }
   if(keys[SDLK_DOWN]) // Diminui o Zoom
   {
     if(d<ZOOMMINIMO)
     {
       d++; 
       redesenha = 1;
     }
   }
   if(keys[SDLK_RIGHT]) // Roda Camera Antihorariamente
   {
       phi -=2;  
       redesenha = 1;
   }
   if(keys[SDLK_LEFT]) // Roda Camera Horariamente
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
   if(keys[SDLK_PAGEDOWN]) // desce com a camera ate visao em 1ª pessoa
   {
      if (theta > 0)
      {
         theta -=1;
         redesenha = 1;
      }
   }
   if(keys[SDLK_HOME]) // Zoom Maximo
   {
      d = ZOOMMAXIMO;
      redesenha = 1;
   }
   if(keys[SDLK_END]) // ZoomMinimo
   {
      d = ZOOMMINIMO;
      redesenha = 1;
   }
   if(keys[SDLK_INSERT]) //Maximo para cima
   {
      theta = 89;
      redesenha = 1;
   }
   if(keys[SDLK_DELETE]) //Maximo para baixo
   {
      theta = 0;
      redesenha = 1;
   }
   /* Tratamento da tecla para Movimentacao do Personagem */

   if(keys[SDLK_q] || keys[SDLK_e])
   {
      varX = ANDAR * sin(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
      varZ = ANDAR * cos(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
      // Anda com personagem de lado para esquerda
      if((keys[SDLK_q])&& (podeAndar(-varX,-varZ,0))) 
      {
         PCs->personagemAtivo->posicaoLadoX -= varX;
         PCs->personagemAtivo->posicaoLadoZ -= varZ;
         centroX -= varX;
         centroZ -= varZ;
         redesenha = 1;
      }
       // Anda com personagem para esquerda
      if((keys[SDLK_e])&& (podeAndar(varX,varZ,0))) 
      {
         centroX += varX;
         centroZ += varZ;
         PCs->personagemAtivo->posicaoLadoX += varX;
         PCs->personagemAtivo->posicaoLadoZ += varZ;
         redesenha = 1;
      }
   }
   if(keys[SDLK_w] || keys[SDLK_s])
   {
      varX = ANDAR * sin(deg2Rad(PCs->personagemAtivo->orientacao));
      varZ = ANDAR * cos(deg2Rad(PCs->personagemAtivo->orientacao));
        // Anda com personagem para frente
      if((keys[SDLK_w]) && (podeAndar(-varX,-varZ,0)) ) 
      {
           PCs->personagemAtivo->posicaoLadoX -= varX;
           PCs->personagemAtivo->posicaoLadoZ -= varZ;
           centroZ -= varZ;
           centroX -= varX;
           redesenha = 1;
      }
        // Anda com personagem para tras
      if((keys[SDLK_s]) && (podeAndar(varX,varZ,0)) ) 
      {
          PCs->personagemAtivo->posicaoLadoX += varX;
          PCs->personagemAtivo->posicaoLadoZ += varZ;
          centroX += varX;
          centroZ += varZ;
          redesenha = 1;
      }
   }

   if( (keys[SDLK_a]) || (keys[SDLK_d]))
   {
      // Gira personagem antihorariamente
      if((keys[SDLK_a]) && (podeAndar(0,0,2.0)) )  
      {
         PCs->personagemAtivo->orientacao += 2.0;
         if(PCs->personagemAtivo->orientacao > 360.0)
            PCs->personagemAtivo->orientacao = 2.0;
         redesenha = 1;
      }
      // Gira o personagem horariamente
      if((keys[SDLK_d]) && (podeAndar(0,0,-2.0)) )
      {
         PCs->personagemAtivo->orientacao -= 2.0;
         if(PCs->personagemAtivo->orientacao < 0.0)
            PCs->personagemAtivo->orientacao = 360.0;
      }
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
      printf("Orientacao %f°\t sen %f\tcos %f\trad %f\n",
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
   if(x == 0)    // Gira a Camera horariamente
   {
      phi+=2; 
      redesenha = 1;  
   }
   if(x == screen->w-1) // Gira a camera antihorariamente
   {
     phi-=2; 
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
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);

   AtualizaFrustum(matrizVisivel);

   glClear ((GL_COLOR_BUFFER_BIT));
   glClear (GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   glRotatef(RotacaoX,1,0,0);
   glRotatef(RotacaoY,0,1,0);
   glRotatef(RotacaoZ,0,0,1);
   mapa->draw(cameraX,cameraY,cameraZ,matrizVisivel);
      personagem* per = (personagem*) PCs->primeiro->proximo;
      int aux;
      for(aux=0;aux < PCs->total;aux++)
      {
         glPushMatrix();
           // O personagem nao movimenta no eixo Y, mas sim no Z (^) e X (>) 
           glTranslatef(per->posicaoLadoX, 0 ,per->posicaoLadoZ);
           glRotatef(per->orientacao,0,1,0);
           //glCallList(per->personagemDesenhar);
           glmDrawLists(per->modelo3d);
         glPopMatrix();
         per = (personagem*) per->proximo;
         glTranslatef(30,0,0);
      }
   glPopMatrix();
   glFlush();
}


int estaDentro(GLfloat x[4], GLfloat z[4],
               GLfloat u1, GLfloat v1, GLfloat u2, GLfloat v2)
{
   return( ( ( ((x[0] >= u1) && (x[0] <= u2)) || 
               ((x[1] >= u1) && (x[1] <= u2)) ||
               ((x[2] >= u1) && (x[2] <= u2)) || 
               ((x[3] >= u1) && (x[3] <= u2)) ) && 
             ( ((z[0] >= v1) && (z[0] <= v2)) || 
               ((z[1] >= v1) && (z[1] <= v2)) ||
               ((z[2] >= v1) && (z[2] <= v2)) || 
               ((z[3] >= v1) && (z[3] <= v2)) ) ) || 
           ( ( ((u1<=x[0]) && ((u1 >= x[1]) || (u1 >= x[2]) || (u1 >= x[3]))) ||
               ((u1<=x[1]) && ((u1 >= x[0]) || (u1 >= x[2]) || (u1 >= x[3]))) ||
               ((u1<=x[2]) && ((u1 >= x[1]) || (u1 >= x[0]) || (u1 >= x[3]))) ||
               ((u1<=x[3]) && ((u1 >= x[1]) || (u1 >= x[2]) || (u1 >= x[0]))) ||
               ((u2<=x[0]) && ((u2 >= x[1]) || (u2 >= x[2]) || (u2 >= x[3]))) ||
               ((u2<=x[1]) && ((u2 >= x[0]) || (u2 >= x[2]) || (u2 >= x[3]))) ||
               ((u2<=x[2]) && ((u2 >= x[1]) || (u2 >= x[0]) || (u2 >= x[3]))) ||
               ((u2<=x[3]) && ((u2 >= x[1]) || (u2 >= x[2]) || (u2 >= x[0]))))&&
             ( ((v1<=z[0]) && ((v1 >= z[1]) || (v1 >= z[2]) || (v1 >= z[3]))) ||
               ((v1<=z[1]) && ((v1 >= z[0]) || (v1 >= z[2]) || (v1 >= z[3]))) ||
               ((v1<=z[2]) && ((v1 >= z[1]) || (v1 >= z[0]) || (v1 >= z[3]))) ||
               ((v1<=z[3]) && ((v1 >= z[1]) || (v1 >= z[2]) || (v1 >= z[0]))) ||
               ((v2<=z[0]) && ((v2 >= z[1]) || (v2 >= z[2]) || (v2 >= z[3]))) ||
               ((v2<=z[1]) && ((v2 >= z[0]) || (v2 >= z[2]) || (v2 >= z[3]))) ||
               ((v2<=z[2]) && ((v2 >= z[1]) || (v2 >= z[0]) || (v2 >= z[3]))) ||
               ((v2<=z[3]) && ((v2 >= z[1]) || (v2 >= z[2]) || (v2 >= z[0]))) ))
          );
}

Square* quadradoRelativo(int posX,int posZ, Square* quad)
{
   Square* result = quad;
  // printf("%d %d\n",quad->posX,quad->posZ);
   int aux;
   for(aux=0;aux < (quad->posX-posX);aux++) result = result->left;
   for(aux=0;aux < (posX-quad->posX);aux++) result = result->right;
   for(aux=0;aux < (quad->posZ-posZ);aux++) result = result->up;
   for(aux=0;aux < (posZ-quad->posZ);aux++) result = result->down;

   return(result);
}

int testa(GLfloat x[4],GLfloat z[4],Square* quad)
{
   int result = 0;
   Square* proxima = quad;
   if(proxima->flags & PISAVEL)
   {
     result = 1;
   }
   if(result) // Se eh possivel entrar, testa com os objetos
   {
      int ob = 0;
      GLfloat u1,u2,v1,v2;
      GLMmodel* modelo3d;
      while(proxima->objetos[ob] != NULL)
      {
          modelo3d = (GLMmodel*)proxima->objetos[ob]->modelo3d;
          u1 = modelo3d->x1+proxima->Xobjetos[ob];
          u2 = modelo3d->x2+proxima->Xobjetos[ob];
          v1 = modelo3d->z1+proxima->Zobjetos[ob];
          v2 = modelo3d->z2+proxima->Zobjetos[ob];
          //printf("obj: %s %.3f,%.3f,%.3f,%.3f %.3f,%.3f,%.3f,%.3f\n",proxima->objetos[ob]->nome,u1,v1,u2,v2,x1,z1,x2,z2);
          result &= !estaDentro(x,z,u1,v1,u2,v2);
          if(!result) //se ja achou que nao pode, cai fora
             return(0);
          ob++;
      }
   }

   return(result);
}

/*********************************************************************
 *                   Verifica se nao ha Colisao                      *
 *********************************************************************/
int engine::podeAndar(GLfloat varX, GLfloat varZ, GLfloat varAlpha)
{
   int result = 1;

   GLfloat x[4],z[4];
   int aux;

   x[0] = PCs->personagemAtivo->modelo3d->x1;
   z[0] = PCs->personagemAtivo->modelo3d->z1;

   x[1] = PCs->personagemAtivo->modelo3d->x1;
   z[1] = PCs->personagemAtivo->modelo3d->z2; 

   x[2] = PCs->personagemAtivo->modelo3d->x2;
   z[2] = PCs->personagemAtivo->modelo3d->z2;

   x[3] = PCs->personagemAtivo->modelo3d->x2;
   z[3] = PCs->personagemAtivo->modelo3d->z1;

   /* Rotaciona o bounding para a posicao correrta */
   if(PCs->personagemAtivo->orientacao+varAlpha != 0)
   {
      GLfloat xVelho, zVelho;
      GLfloat cosseno = cos(deg2Rad(PCs->personagemAtivo->orientacao +
                            varAlpha));
      GLfloat seno = sin(deg2Rad(PCs->personagemAtivo->orientacao +
                            varAlpha));
      for(aux = 0;aux<4;aux++)
      {
         xVelho = x[aux];
         zVelho = z[aux];
         x[aux] = (zVelho*seno) + (xVelho*cosseno);
         z[aux] = (zVelho*cosseno) - (xVelho*seno);
      }
   }

   /* translada o bounding box para o local correto*/
   for(aux=0;aux<4;aux++)
   {
     x[aux] += PCs->personagemAtivo->posicaoLadoX+varX;
     z[aux] += PCs->personagemAtivo->posicaoLadoZ+varZ;
     if( (x[aux]<0) || (z[aux]<0) || 
         (x[aux]>mapa->x*SQUARESIZE) || (z[aux]>mapa->z*SQUARESIZE))
         return(0);
   }
 
   /* Testa quadrados a direita */
   if((PCs->personagemAtivo->ocupaQuad->right)) 
   { 
      /* leste */
      if(estaDentro(x,z,PCs->personagemAtivo->ocupaQuad->right->x1,
                 PCs->personagemAtivo->ocupaQuad->right->z1,
                 PCs->personagemAtivo->ocupaQuad->right->x2,
                 PCs->personagemAtivo->ocupaQuad->right->z2) )
      {
         result &= testa(x,z,PCs->personagemAtivo->ocupaQuad->right);
         if(!result)
         {
            //printf("sai na direita\n"); 
            return(0);
         }
      }
      /* Nordeste */
      if( (PCs->personagemAtivo->ocupaQuad->right->up) &&
          estaDentro(x,z,PCs->personagemAtivo->ocupaQuad->right->up->x1,
                 PCs->personagemAtivo->ocupaQuad->right->up->z1,
                 PCs->personagemAtivo->ocupaQuad->right->up->x2,
                 PCs->personagemAtivo->ocupaQuad->right->up->z2) )
      {
         result &= testa(x,z,PCs->personagemAtivo->ocupaQuad->right->up);
         if(!result) 
         {
            //printf("sai na direita->cima\n"); 
            return(0);
         }

      }
      /* Sudeste */
      if( (PCs->personagemAtivo->ocupaQuad->right->down) &&
           estaDentro(x,z,PCs->personagemAtivo->ocupaQuad->right->down->x1,
                 PCs->personagemAtivo->ocupaQuad->right->down->z1,
                 PCs->personagemAtivo->ocupaQuad->right->down->x2,
                 PCs->personagemAtivo->ocupaQuad->right->down->z2))
      {
         result &= testa(x,z,PCs->personagemAtivo->ocupaQuad->right->down);
         if(!result) 
         {
            //printf("sai na direita->baixo\n"); 
            return(0);
         }
      }
   }

   /* Testa quadrados a esquerda */
   if((PCs->personagemAtivo->ocupaQuad->left)) 
   { 
      /* oeste */
      if(estaDentro(x,z,PCs->personagemAtivo->ocupaQuad->left->x1,
                 PCs->personagemAtivo->ocupaQuad->left->z1,
                 PCs->personagemAtivo->ocupaQuad->left->x2,
                 PCs->personagemAtivo->ocupaQuad->left->z2))
      {
         result &= testa(x,z,PCs->personagemAtivo->ocupaQuad->left);
         if(!result) 
         {
            //printf("sai na esquerda\n"); 
            return(0);
         }
      }

      /* Noroeste */
      if( (PCs->personagemAtivo->ocupaQuad->left->up) &&
          estaDentro(x,z,PCs->personagemAtivo->ocupaQuad->left->up->x1,
                 PCs->personagemAtivo->ocupaQuad->left->up->z1,
                 PCs->personagemAtivo->ocupaQuad->left->up->x2,
                 PCs->personagemAtivo->ocupaQuad->left->up->z2) )
      {
         result &= testa(x,z,PCs->personagemAtivo->ocupaQuad->left->up);
         if(!result) 
         {
            //printf("sai na esquerda->cima\n"); 
            return(0);
         }
      }
      /* Sudoeste */
      if( (PCs->personagemAtivo->ocupaQuad->left->down) &&
           estaDentro(x,z,PCs->personagemAtivo->ocupaQuad->left->down->x1,
                 PCs->personagemAtivo->ocupaQuad->left->down->z1,
                 PCs->personagemAtivo->ocupaQuad->left->down->x2,
                 PCs->personagemAtivo->ocupaQuad->left->down->z2))
      {
         result &= testa(x,z,PCs->personagemAtivo->ocupaQuad->left->down);
         if(!result) 
         {
            //printf("sai na esquerda->baixo\n"); 
            return(0);
         }
      }
   }
  
   /* Testa quadrados abaixo */
   if((PCs->personagemAtivo->ocupaQuad->down) && 
      estaDentro(x,z,PCs->personagemAtivo->ocupaQuad->down->x1,
                 PCs->personagemAtivo->ocupaQuad->down->z1,
                 PCs->personagemAtivo->ocupaQuad->down->x2,
                 PCs->personagemAtivo->ocupaQuad->down->z2) )
   { 
      /* sul */
      result &= testa(x,z,PCs->personagemAtivo->ocupaQuad->down);
      if(!result) 
      {
         //printf("sai no de baixo\n"); 
         return(0);
      }
   }

   /* Testa quadrados acima */
   if((PCs->personagemAtivo->ocupaQuad->up) && 
      estaDentro(x,z,PCs->personagemAtivo->ocupaQuad->up->x1,
                 PCs->personagemAtivo->ocupaQuad->up->z1,
                 PCs->personagemAtivo->ocupaQuad->up->x2,
                 PCs->personagemAtivo->ocupaQuad->up->z2) )
   { 
      /* norte */
      result &= testa(x,z,PCs->personagemAtivo->ocupaQuad->up);
      if(!result) 
      {
         //printf("sai no de cima\n"); 
         return(0);
      }
   }

   int posX =(int)floor((PCs->personagemAtivo->posicaoLadoX+varX) 
                                                  / (SQUARESIZE))+1;
   int posZ =(int)floor((PCs->personagemAtivo->posicaoLadoZ+varZ) 
                                                  / (SQUARESIZE))+1;

   PCs->personagemAtivo->ocupaQuad = quadradoRelativo(posX,posZ,
                                       PCs->personagemAtivo->ocupaQuad);
   return(result);
}

/*********************************************************************
 *                          Roda a Engine                            *
 *********************************************************************/
int engine::Rodar(SDL_Surface *surface)
{
   PCs->personagemAtivo->posicaoLadoX = mapa->xInic;
   PCs->personagemAtivo->posicaoLadoZ = mapa->zInic;
   centroX = mapa->xInic;
   centroZ = mapa->zInic;
   PCs->personagemAtivo->ocupaQuad = mapa->squareInic;
   
   /* Desenha a primeira Cena */
   Desenhar();
   SDL_GL_SwapBuffers();
  
   /* Roda realmente a engine */
   while(TrataES(surface));
   return(1);
}


