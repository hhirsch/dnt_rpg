/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "dccnit.h"
#include "culling.h"
#include <math.h>

#define DELAY 0
#define ANDAR 1.0        // O quanto o personagem anda a cada frame
#define GIRAR 2.5        // O quanto ele gira a cada frame
#define DELTACAMERA 2.0  // O quanto a camera meche a cada frame
#define ZOOMMAXIMO 80    // Valor máximo de zoom
#define ZOOMMINIMO 280   // Valor mínimo do zoom

/* Conversor de graus para radianos */
inline double deg2Rad(double x){return 6.2831853 * x/360.0;}

/*********************************************************************
 *                       Construtor da Engine                        *
 *********************************************************************/
engine::engine(char* arqMapa)
{
   /* Inicia as Listas Internas */
   NPCs = new (Lpersonagem);
   PCs  = new (Lpersonagem);

   /* Abre o Mapa Desejado */
   mapa = new(Map);
   mapa->open(arqMapa);

   /* Define a posicao da Camera Inicial */
   theta=35;
   phi=0;
   d=150;
   RotacaoX = RotacaoY = RotacaoZ = 0;
   centroX = centroZ = 0;
   centroY = 30;
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));

   /* Define a ultima vez em que desenhou (so por simplicidade) */
   ultimaLeitura = SDL_GetTicks();
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
   gluPerspective(45.0, (GLsizei) screen->w / (GLsizei) screen->h, 1.0, 1000.0);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);
}

/*********************************************************************
 *                   Inicia e Engine para Uso                        *
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
   
   /* Carrega a Luz */
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   
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
int engine::TrataES(SDL_Surface *screen,int *forcaAtualizacao)
{
   int redesenha = 0;
   int andou = 0;
   Uint32 tempo;
   double varX, varZ; // para evitar de ter de calcular 2 vezes

   tempo = SDL_GetTicks();
   if( ((tempo-ultimaLeitura)) >= 16)
   {
//      printf("FPS: %f\t",1000.0 / (float)(tempo-ultimaLeitura));
      SDL_PumpEvents();
      ultimaLeitura = tempo;

        
      redesenha = TrataIA();
       
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
             d-= DELTACAMERA+0.5;
             redesenha = 1;
          }
      }
      if(keys[SDLK_DOWN]) // Diminui o Zoom
      {
         if(d<ZOOMMINIMO)
         {
             d+= DELTACAMERA+0.5; 
             redesenha = 1;
         }
      }
      if(keys[SDLK_RIGHT]) // Roda Camera Antihorariamente
      {
          phi -= DELTACAMERA+0.5;  
          redesenha = 1;
      }
      if(keys[SDLK_LEFT]) // Roda Camera Horariamente
      {
         phi += DELTACAMERA+0.5;
         redesenha = 1;
      }
      if(keys[SDLK_PAGEUP]) // Sobe com a camera ate visao de cima
      {
         if (theta < 89)
         {
            theta += DELTACAMERA;
            redesenha = 1;
         }
      }
      if(keys[SDLK_PAGEDOWN]) // desce com a camera ate visao em 1ª pessoa
      {
         if (theta > 0)
         {
            theta -= DELTACAMERA;
            redesenha = 1;
         }
      }
      if (keys[SDLK_HOME]) // Zoom Maximo
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
            andou = 1;
         }
         // Anda com personagem para esquerda
         if((keys[SDLK_e])&& (podeAndar(varX,varZ,0))) 
         {
            centroX += varX;
            centroZ += varZ;
            PCs->personagemAtivo->posicaoLadoX += varX;
            PCs->personagemAtivo->posicaoLadoZ += varZ;
            redesenha = 1;
            andou = 1;
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
              andou = 1;
         }
           // Anda com personagem para tras
         if((keys[SDLK_s]) && (podeAndar(varX,varZ,0)) ) 
         {
             PCs->personagemAtivo->posicaoLadoX += varX;
             PCs->personagemAtivo->posicaoLadoZ += varZ;
             centroX += varX;
             centroZ += varZ;
             redesenha = 1;
             andou  = 1;
         }
      }

      if( (keys[SDLK_a]) || (keys[SDLK_d]))
      {
         // Gira personagem antihorariamente
         if((keys[SDLK_a]) && (podeAndar(0,0,2.0)) )  
         {
            PCs->personagemAtivo->orientacao += GIRAR;
            if(PCs->personagemAtivo->orientacao > 360.0)
               PCs->personagemAtivo->orientacao = 
                               PCs->personagemAtivo->orientacao  - 360.0;
            redesenha = 1;
            andou = 1;
         }
         // Gira o personagem horariamente
         if((keys[SDLK_d]) && (podeAndar(0,0,-2.0)) )
         {
            PCs->personagemAtivo->orientacao -= GIRAR;
            if(PCs->personagemAtivo->orientacao < 0.0)
               PCs->personagemAtivo->orientacao = 360.0 + 
                                        PCs->personagemAtivo->orientacao ;
         }
         redesenha = 1;
         andou = 1;
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
   }

   /*if((*forcaAtualizacao != 0))
      printf("Era para forcar %d %d\n",tempo, ultimaLeitura);
   if((tempo-ultimaLeitura)>=16)
      printf("Maior que 16\n");*/

   if( (redesenha) || ( (*forcaAtualizacao != 0)/* && ((tempo-ultimaLeitura)>=16)*/))
   {
      Desenhar();
      SDL_GL_SwapBuffers();
      *forcaAtualizacao = 0;
   }
 
   if(andou)
   {
      #ifdef REDE
        movchar(&clientData, PCs->personagemAtivo->ID, 
          PCs->personagemAtivo->posicaoLadoX,PCs->personagemAtivo->posicaoLadoZ,
          PCs->personagemAtivo->orientacao );
      #endif
   }
 
   return(1);
}


/*********************************************************************
 *                Funcao para desenhar a cena                        *
 *********************************************************************/
void engine::Desenhar()
{
   glLoadIdentity();

   /* Redefine a posicao dinamica da camera */
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);
 
   /* Autaliza para fazer o culling e o desenho da GUI */
   AtualizaFrustum(matrizVisivel,proj,modl);

   glClear ((GL_COLOR_BUFFER_BIT));
   glClear (GL_DEPTH_BUFFER_BIT);
   
   glPushMatrix();
   
   /* Rotacoes no Mundo de Teste apenas, sumirao com o tempo */
   glRotatef(RotacaoX,1,0,0);
   glRotatef(RotacaoY,0,1,0);
   glRotatef(RotacaoZ,0,0,1);

   /* Desenha o Mundo, fazendo culling do view frustum */
   mapa->draw(cameraX,cameraY,cameraZ,matrizVisivel);

   /* Desnha os Personagens do Jogador (PCs) */
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
      }
   glPopMatrix();

   /* Atualiza os Valores dos NPCS com o que já estiver no socket */
   //AtualizaNPCs
  
   /* Desenha Entao os NPCs */
      per = (personagem*) NPCs->primeiro->proximo;
      for(aux=0;aux < NPCs->total;aux++)
      {
         glPushMatrix();
           glTranslatef(per->posicaoLadoX, 0 ,per->posicaoLadoZ);
           glRotatef(per->orientacao,0,1,0);
           glmDrawLists(per->modelo3d);
         glPopMatrix();
         per = (personagem*) per->proximo;
      }

   /* Faz o Desenho da GUI */
   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;
   gluUnProject( 800, 600, 0.01, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject( 800, 600-80,0.01, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject( 800-60, 600-80, 0.01, modl, proj, viewPort, &x3, &y3, &z3);
   gluUnProject( 800-60, 600, 0.01, modl, proj, viewPort, &x4, &y4, &z4);

   /* Desenho do Portrait do jogador */
   per = (personagem*) PCs->personagemAtivo;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, per->portrait );
   glBegin(GL_QUADS);
      glColor3f(1,1,1);
      glTexCoord2f(1,0);
      glVertex3f(x1,y1,z1);
      glTexCoord2f(1,1);
      glVertex3f(x2,y2,z2);
      glTexCoord2f(0,1);
      glVertex3f(x3,y3,z3);
      glTexCoord2f(0,0);
      glVertex3f(x4,y4,z4);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   glFlush();
}


/******************************************/
/*    PARTE DA DETECCAO DE COLISAO!!!!!!! */
/******************************************/


/*********************************************************************
 *        Verifica se ha intersecao entre dois bounding boxes        *
 *********************************************************************/
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

/*********************************************************************
 *      Retorna o Quadrado do mapa Relativo à posicao posX,posZ      *
 *********************************************************************/
Square* quadradoRelativo(int posX,int posZ, Square* quad)
{
   Square* result = quad;
   int aux;
   for(aux=0;aux < (quad->posX-posX);aux++) result = result->left;
   for(aux=0;aux < (posX-quad->posX);aux++) result = result->right;
   for(aux=0;aux < (quad->posZ-posZ);aux++) result = result->up;
   for(aux=0;aux < (posZ-quad->posZ);aux++) result = result->down;
   return(result);
}

/*********************************************************************
 *          Faz o teste se o Quadrado quad é factivel de ser         *
 *                    ocupado pelo personagem                        *
 *********************************************************************/
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
 *                      Tratamento da IA dos NPCs                    *
 *********************************************************************/
/* Monta Conjunto de campos do Quadrado */
inline void engine::verificaQuad(Square* quad)
{
   if(quad)
   {
       if(quad->flags & PISAVEL)
       {
            if(quad->objetos[0])
            {
               ia->campoInfluencia(quad->posX,quad->posZ,
                                   TIPOOBSTACULO,30);
            }
        }
        else
           ia->campoInfluencia(quad->posX,quad->posZ,
                               TIPOOBSTACULO,30);
   }
}

/* Monta conjunto de Campos da linha */
inline void engine::verificaLinha(Square* centro)
{
   if(centro)
   {
       verificaQuad(centro);
       if(centro->left)
       {
          verificaQuad(centro->left);
          verificaQuad(centro->left->left);
       }
       if(centro->right)
       {
          verificaQuad(centro->right);
          verificaQuad(centro->right->right);
       }
   }   
}

/* Retorno !=0 se modificou posicao do personagem */
int engine::TrataIA()
{   

   int posX, posZ;     //Posicao Auxiliar
   double antX,antZ;
   personagem* per;    //Personagem Atual
   per = (personagem*) NPCs->primeiro->proximo;

   ia->iniciaListaCampos();

   /* Coloca o Unico PC existente */
   ia->campoInfluencia(PCs->personagemAtivo->posicaoLadoX,
                       PCs->personagemAtivo->posicaoLadoZ,
                       TIPOPC, 100);

    /* Em tese seria para todos os NPCs, mas como a perseguicao
       atual eh para so um NPC, entao roda so para ele.
    */

    /* Testa-se entao o campo de visao, montando os campos de influencia */
    if(per->ocupaQuad)
    {
       verificaLinha(per->ocupaQuad);
       if(per->ocupaQuad->up)
       {
          verificaLinha(per->ocupaQuad->up);
          verificaLinha(per->ocupaQuad->up);
       }
       verificaLinha(per->ocupaQuad->down);
    }
    else
      printf("O Personagem Saiu do Mapa!\n");
   
    antX = per->posicaoLadoX;
    antZ = per->posicaoLadoZ;
    ia->destinoNpc(per);

    /* Define-se A posicao do Personagem NPC */  
    posX =(int)floor((per->posicaoLadoX) / (SQUARESIZE))+1;
    posZ =(int)floor((per->posicaoLadoZ) / (SQUARESIZE))+1;
    per->ocupaQuad = quadradoRelativo(posX,posZ,per->ocupaQuad);

    return( (antX!=per->posicaoLadoX) || (antZ!=per->posicaoLadoZ));
}


/*********************************************************************
 *                          Roda a Engine                            *
 *********************************************************************/
int engine::Rodar(SDL_Surface *surface)
{

   int forcaAtualizacao = 1; //forca a atualizacao da tela, qdo o npc anda
   int posX, posZ;           //Posicao Auxiliar

   personagem* per;
	
   /* Coloca Personagem Ativo na posicao de Inicio do Mapa */
   PCs->personagemAtivo->posicaoLadoX = mapa->xInic;
   PCs->personagemAtivo->posicaoLadoZ = mapa->zInic;
   centroX = mapa->xInic;
   centroZ = mapa->zInic;
   PCs->personagemAtivo->ocupaQuad = mapa->squareInic;

   /* Define a Posicao do NPC em Squares */
   per = (personagem*) NPCs->primeiro->proximo;
   posX =(int)floor((per->posicaoLadoX) / (SQUARESIZE))+1;
   posZ =(int)floor((per->posicaoLadoZ) / (SQUARESIZE))+1;
   per->ocupaQuad = mapa->quadradoRelativo(posX,posZ);

   /* Inicia AI -- Vai mudar com o tempo e ser relativa a cada NPC */
   ia = new(AI); 
   
   #ifdef REDE
     netevent_p_t eventoRede;
     
     initclientdata( &clientData );
     if ( ( startconnection( &clientData, server, DEFAULTPORT )) == -1 )
     {
         printf("Não Consegui Iniciar Conexão!\nAbortando Uso.\n");
         return(1);
     }
     entergame( &clientData );
   #endif
  
   /* Roda realmente a engine */
   while(TrataES(surface,&forcaAtualizacao))
   {
      /* Trata a Rede. Por padrao, nao estamos usando a rede, uma vez 
       * que a mesma nao se encontra no escopo inicial do projeto, somente
       * sendo util quando se estiver em um estado estável do jogo.
       * No mais, MMORPG é um saco. O bom de rede eh jogar com conhecidos.
       * Mais uma vez farrer fazendo juízos no meio do código. Deve ser louco
       * mesmo. argh!
       */
    #ifdef REDE
      while( (eventoRede = pollnet( &clientData ) ) != NULL )
      {
         switch(eventoRede->type)
         {
             case MT_NEWCHAR: /* Insere Novo Caractere */
             {
                personagem* per;
                per = NPCs->InserirPersonagem(6,8,3,8,
                             "../data/pics/logan/cara.bmp",0,0,
               "LoganNPC","../data/models/personagens/logan_completo_final.obj",
                  "../data/pics/logan/");
                per->posicaoLadoX = eventoRede->x;
                per->posicaoLadoZ = eventoRede->y; 
                per->orientacao = eventoRede->teta;
                per->ID = eventoRede->obj;
		forcaAtualizacao = 1;
                break; 
             }
             case MT_MOV:
             {
                personagem* per = (personagem*)NPCs->primeiro->proximo;
                if(per != NPCs->primeiro) 
                {
                   while((per!=NPCs->primeiro) && (eventoRede->obj!=per->ID))
                   {
                      per = (personagem*) per->proximo;
                   }
                   if(per!=NPCs->primeiro)
                   {
                       per->posicaoLadoX = eventoRede->x;
                       per->posicaoLadoZ = eventoRede->y; 
                       per->orientacao = eventoRede->teta;
		       forcaAtualizacao = 1;
                   }
                }
                break; 
             }
             case MT_ERROR:
             {
                 printf("Recebi Erro do Servidor\n");
                 return(1);
             }
             case MT_ENDSYNC:
             {
                 PCs->personagemAtivo->ID = createchar( &clientData, 
                   PCs->personagemAtivo->posicaoLadoX, 
                   PCs->personagemAtivo->posicaoLadoZ, 
                   PCs->personagemAtivo->orientacao );
                 if( PCs->personagemAtivo->ID == -2 )
                 {
                     entergame( &clientData );
                 }
                 else if ( PCs->personagemAtivo->ID == -1 )
                 {
                     printf("Servidor Lotado\n");
                     return(1);
                 }
             }
             default:break; /* Por default nao faz nada! */
         }
      }
    #endif


   }

   delete(ia);
   return(1);
}


