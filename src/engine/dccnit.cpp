/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "dccnit.h"
#include "culling.h"
#include <math.h>
#include <SDL/SDL_image.h>
#include "../etc/glm.h"


#define DELAY           0

#define ANDAR           0.75     /* O quanto o personagem anda a cada frame
                                  * A Velocidade do Caracter pode ser calculada
                                  * por ANDAR / 20, em unidades/milisegundo 
                                  */
 
#define GIRAR           2.5     // O quanto ele gira a cada frame
#define DELTACAMERA     2.5     // O quanto a camera meche a cada frame
#define ZOOMMAXIMO      80      // Valor máximo de zoom
#define ZOOMMINIMO      280     // Valor mínimo do zoom

#define SCREEN_X        800     // Largura da tela
#define SCREEN_Y        600     // Altura da tela

#define FARVIEW       (SQUARESIZE * 25)  // FARVIEW da camera 
#define HALFFARVIEW   (FARVIEW / 2)      // Metade do Farview


#define CORNEBLINA_R    1.0      // Componente vermelha da neblina
#define CORNEBLINA_G    1.0      // Componente verde da neblina
#define CORNEBLINA_B    1.0      // Componente azul da neblina

/* Conversor de graus para radianos */
inline double deg2Rad(double x){return 6.2831853 * x/360.0;}

/*********************************************************************
 *                       Construtor da Engine                        *
 *********************************************************************/
engine::engine()
{
   /* Inicia as Listas Internas */
   gui  = new interface(NULL);
   mapa = NULL;
   PCs = NULL;
   NPCs = NULL;
   janMiniMapa = NULL;
   janAtalhos = NULL;

   /* Define a posicao da Camera Inicial */
   theta=35;
   phi=0;
   d=150;
   centroX = centroZ = 0;
   centroY = 30;
   deltaY = 0;
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   mouseX = 0;
   mouseY = 0;

   /* Define a ultima vez em que desenhou (so por simplicidade) */
   ultimaLeitura = SDL_GetTicks();
   ultimoMouse = ultimaLeitura;
}

/*********************************************************************
 *                      Destruidor da Engine                         *
 *********************************************************************/
engine::~engine()
{
   //glDeleteLists(mapaDesenhar,1);
   gluDeleteQuadric(atmosfera);
   glDeleteTextures(1, &ceu);
   if(NPCs)
      delete(NPCs);
   if(PCs)
      delete(PCs);
   if(gui)
      delete(gui);
   if(mapa)
   {
      delete(mapa);
   }
}

/*********************************************************************
 *                     Aloca uma Textura no OPENGL                   *
 *********************************************************************/
void carregaTextura(SDL_Surface* img, GLuint* textID)
{
   glGenTextures(1,textID);
   glBindTexture(GL_TEXTURE_2D,*textID);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 
                               0, GL_RGBA, GL_UNSIGNED_BYTE, 
                               img->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/*********************************************************************
 *                       Carrega Mapa na Engine                      *
 *********************************************************************/
int engine::CarregaMapa(char* arqMapa, int RecarregaPCs)
{
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDisable(GL_LIGHTING);
   SDL_Surface* img = IMG_Load("../data/texturas/carregar.jpg");
   SDL_Surface* fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,fig,NULL);
   SDL_FreeSurface(img);
   img = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       256,32,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);


   cor_Definir(0,0,0);
   retangulo_Colorir(img,0,0,255,31,0);
   cor_Definir(200,20,20);
   selFonte(FFARSO,CENTRALIZADO,3);
   escxy(img,128,0,"Loading Map...");
   GLuint texturaTexto;
   carregaTextura(img,&texturaTexto);

   GLuint texturaCarga;
   carregaTextura(fig,&texturaCarga);
   SDL_FreeSurface(fig);


   AtualizaFrustum(matrizVisivel,proj,modl);
   AtualizaTela2D(texturaCarga,proj,modl,viewPort,272,236,527,363,0.01);
   AtualizaTela2D(texturaTexto,proj,modl,viewPort,272,365,527,396,0.01);
   glFlush();
   SDL_GL_SwapBuffers();

   /* Carregando o Mapa */

   if(mapa) 
   {
     delete(mapa);
   }
   mapa = new(Map);
   mapa->open(arqMapa);

   glDeleteTextures(1,&texturaTexto);
   cor_Definir(0,0,0);
   retangulo_Colorir(img,0,0,255,31,0);
   cor_Definir(200,20,20);
   selFonte(FFARSO,CENTRALIZADO,3);
   escxy(img,128,0,"Loading Characters...");
   carregaTextura(img,&texturaTexto);
   
   AtualizaTela2D(texturaCarga,proj,modl,viewPort,272,236,527,363,0.01);
   AtualizaTela2D(texturaTexto,proj,modl,viewPort,272,365,527,396,0.01);
   glFlush();
   SDL_GL_SwapBuffers();

   SDL_FreeSurface(img);
   glDeleteTextures(1,&texturaCarga);
   glDeleteTextures(1,&texturaTexto);

   /* Carregando Entao os NPCs */
   if(NPCs)
     delete(NPCs);
   NPCs = new (Lpersonagem);
   personagem* per;
   per = NPCs->InserirPersonagem(7,6,10,6,
                                 "../data/pics/logan/portrait.jpg","NPC",
                        "../data/models/personagens/Logan/modelo.cfg");
   per->posicaoLadoX = 30;
   per->posicaoLadoZ = 20;

   if(RecarregaPCs)
   {
       if(PCs)
          delete(PCs);
       PCs  = new (Lpersonagem);
       PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/portrait.jpg",
                              "Logan",
                       "../data/models/personagens/Logan/modelo.cfg");
       /*per = PCs->InserirPersonagem(7,6,9,7,"../data/pics/logan/portrait.jpg",
                              "Logan",
                       "/home/farrer/tp2/TPS/TP2/Logan/logan_completo.cfg");
       per->posicaoLadoX = 100;
       per->posicaoLadoZ = 100;*/

   }

   if(janMiniMapa)
     janMiniMapa->Fechar(gui->ljan);
   if(janAtalhos)
     janAtalhos->Fechar(gui->ljan);
   abreMiniMapa();
   abreAtalhos();

   glEnable(GL_LIGHTING);
   return(1);

}

/*********************************************************************
 *                       Menu Inicial do Jogo                        *
 *********************************************************************/
int engine::TelaInicial()
{
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDisable(GL_LIGHTING);
   SDL_Surface* img = IMG_Load("../data/texturas/menuEnglish.jpg");
   SDL_Surface* fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,fig,NULL);
   SDL_FreeSurface(img);
   
   GLuint textID;
   carregaTextura(fig,&textID);
   SDL_FreeSurface(fig);

   /*img = IMG_Load("../data/texturas/fundo.jpg");
   fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,fig,NULL);
   SDL_FreeSurface(img);
   GLuint tituloID;
   carregaTextura(fig,&tituloID);
   SDL_FreeSurface(fig);*/


   AtualizaFrustum(matrizVisivel,proj,modl);
   AtualizaTela2D(textID,proj,modl,viewPort,336,172,463,427,0.01);
//   AtualizaTela2D(tituloID,proj,modl,viewPort,144,44,655,555,0.012);
   glFlush();
   SDL_GL_SwapBuffers();

   int pronto = 0;
   int result;
   Uint8 Mbotao;
   int x,y;

   while(!pronto)   
   {
        SDL_PumpEvents();
        Mbotao = SDL_GetMouseState(&x,&y);
        if(Mbotao & SDL_BUTTON(1))
        {
            if(mouse_NaArea(360,208,440,242,x,y))
            {
                result = 1;
                pronto = 1;
            }
            else
            if(mouse_NaArea(360,358,440,392,x,y))
            {
                result = 0;
                pronto = 1;
            }
        }
   }

   glDeleteTextures(1,&textID);
  // glDeleteTextures(1,&tituloID);

   return(result);
}

/*********************************************************************
 *                 Redmensiona a Engine para a Tela                  *
 *********************************************************************/
void engine::Redmensiona(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei)screen->w / (GLsizei)screen->h, 1.0, FARVIEW);
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
  
   glEnable(GL_FOG);
   {
     GLfloat fogCor[4] = {CORNEBLINA_R,CORNEBLINA_G,CORNEBLINA_B,1.0}; 
     glFogi(GL_FOG_MODE,GL_LINEAR);
     glFogfv(GL_FOG_COLOR,fogCor);
     glFogf(GL_FOG_DENSITY,0.60);
     glHint(GL_FOG_HINT,GL_DONT_CARE);
     glFogf(GL_FOG_START,500.0);
     glFogf(GL_FOG_END,1600.0);
   }

   atmosfera = gluNewQuadric ();
   //gluQuadricTexture(atmosfera, GL_TRUE);
   gluQuadricTexture(atmosfera, GL_FALSE);

   SDL_Surface* img = IMG_Load("../data/texturas/ceu.jpg");
   SDL_Surface* fg = SDL_CreateRGBSurface(SDL_HWSURFACE,
                      img->w,img->h,32,
                      0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,fg,NULL);
   SDL_FreeSurface(img);

   glGenTextures(1, &ceu);
   glBindTexture(GL_TEXTURE_2D, ceu);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fg->w, fg->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, fg->pixels);
   /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, fg->w,
                     fg->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                     fg->pixels );

   /*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST);*/
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

   SDL_FreeSurface(fg);

}

/*********************************************************************
 *        Verifica se ha intersecao entre dois bounding boxes        *
 *********************************************************************/
int estaDentro(GLfloat min1[3], GLfloat max1[3],
               GLfloat min2[3], GLfloat max2[3],
               int inverso)
{
   //testa o mínimo X do 2
   if( (min1[0] < min2[0]) && (max1[0] > min2[0]  ) )
   {
      //testa minimo Y
      //if( (min1[1] < min2[1]) && (max1[1] > min2[1]) )
      //{
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      //}
      //testa maximo Y
      /*if( (min1[1] < max2[1]) && (max1[1] > max2[1]) )
      {
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      }*/
   }
   
   //testa com o máximo X do 2
   if( (min1[0] < max2[0]) && (max1[0] > max2[0]) )
   {
      //testa minimo Y
      //if( (min1[1] < min2[1]) && (max1[1] > min2[1]) )
      //{
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      //}
      //testa maximo Y
      /*if( (min1[1] < max2[1]) && (max1[1] > max2[1]) )
      {
         testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      }*/
   }

   //testa casos de cruz + 
   if( (min2[0] < min1[0]) && (max2[0] > max1[0]) &&
       (min2[2] > min1[2]) && (min2[2] < max1[2]))
   {
      return(1);
   }
   if( (min2[2] < min1[2]) && (max2[2] > max1[2]) &&
       (min2[0] > min1[0]) && (min2[0] < max1[0]))
   {
      return(1);
   }

   if(inverso)
   {
      return( estaDentro(min2, max2, min1, max1, 0));
   }
   else 
   {
      return(0);
   }
}


/*********************************************************************
 *                      Tratamento do Teclado                        *
 *********************************************************************/
int engine::TrataES(SDL_Surface *screen,int *forcaAtualizacao)
{
   bool redesenha = false;
   bool andou = false;
   bool passouTempo = false;
   //bool parado = false;
   Uint32 tempo;
   double varX, varZ; // para evitar de ter de calcular 2 vezes

   double passo;  // O quanto realmente anda, em decorrencia do tempo decorrido
   double rotacao; // O quanto realmente roda, em decorrencia do tempo (FPS)
   double varCamera;
   double varTempo;
   float wx,wy,wz;

   tempo = SDL_GetTicks();
   varTempo = (tempo-ultimaLeitura);
   if( ((varTempo)) >= 20)
   {
      redesenha = true;
      passouTempo = true;

      /* Atualiza Animacoes dos personagens */
      GLfloat segundos = varTempo / 1000.0;
      int aux;
      personagem *per = (personagem*) PCs->primeiro->proximo;
      for(aux=0;aux< PCs->total;aux++)
      {
         per->m_calModel->update(segundos); 
         per->CalculateBoundingBox();  
         per = (personagem*) per->proximo;
      }
      per = (personagem*) NPCs->primeiro->proximo;
      for(aux=0;aux < NPCs->total;aux++)
      {
         per->m_calModel->update(segundos);   
         per = (personagem*) per->proximo;
      }

      /* Calcula as Modificações Reais no Andar, rotacionar, girar, etc */
      varTempo /= 20.0;
      passo = (varTempo)*ANDAR;
      if(passo > 9)
        passo = 9; /* Para evitar que atravesse paredes ao ter LAG*/
      rotacao = (varTempo)*GIRAR;
      varCamera = varTempo*DELTACAMERA;
      
      FPSatual = (FPSatual + (1000.0 / varTempo)) / 2; 
     
      SDL_PumpEvents();
      if( (janAtalhos) && (tempo-ultimaFPS >= 300))
      {
         ultimaFPS = tempo;
         sprintf(FPS->texto,"FPS: %3.2f",1000.0 / (tempo-ultimaLeitura));
         janAtalhos->Desenhar();
      }
      ultimaLeitura = tempo;

        
      //redesenha = TrataIA();
       
      /* Tratamento das Teclas */
      Uint8 *keys;
      keys = SDL_GetKeyState(NULL);

      int x,y;
      Uint8 Mbotao = SDL_GetMouseState(&x,&y);

      /* Trata A GUI */
      if(gui->ManipulaEventos(x,y,Mbotao,keys)!=NADA)
      {
         redesenha = true;
      }
      else
      {
      if((tempo-ultimoMouse>=100 ))
      {
         ultimoMouse = tempo;
         mouseX = x;
         mouseY = y;
         wx = mouseX; wy = SCREEN_Y - mouseY; 
            
         glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
         gluUnProject(wx,wy,wz,modl,proj,viewPort,&xReal,&yReal,&zReal); 

       
         int qx, qz;
         qx = (int)xReal / SQUARESIZE;
         qz = (int)zReal / SQUARESIZE;
         Square* quaux = mapa->quadradoRelativo(qx,qz);
       if(quaux != NULL)
       {
         int pronto;
         int obj = 0;
         GLfloat minObj[3], maxObj[3];
         GLfloat minMouse[3], maxMouse[3];
         minMouse[0] = xReal-2;  maxMouse[0] = xReal+2;
         minMouse[1] = 0.0;      maxMouse[1] = 0.0;
         minMouse[2] = zReal-2;  maxMouse[2] = zReal+2;
         for(pronto = 0; ( (obj<MAXOBJETOS) && (!pronto) );obj++)
         {
            if(quaux->objetos[obj])
            {
               GLMmodel* modelo3d = (GLMmodel*) quaux->objetos[obj]->modelo3d;

               /* Rotaciona o bounding para a posicao correrta */
               if(quaux->orientacaoObjetos[obj] != 0)
               {
                  GLfloat cosseno=cos(deg2Rad(quaux->orientacaoObjetos[obj]));
                  GLfloat seno = sin(deg2Rad(quaux->orientacaoObjetos[obj]));
                  
                  minObj[0] = (modelo3d->z1*seno) + (modelo3d->x1*cosseno);
                  minObj[2] = (modelo3d->z1*cosseno) - (modelo3d->x1*seno);
                  maxObj[0] = (modelo3d->z2*seno) + (modelo3d->x2*cosseno);
                  maxObj[2] = (modelo3d->z2*cosseno) - (modelo3d->x2*seno);
               }
               minObj[0] += (quaux->Xobjetos[obj]);
               minObj[1] = 0.0;
               minObj[2] += (quaux->Zobjetos[obj]);
               maxObj[0] += (quaux->Xobjetos[obj]);
               maxObj[1] = 0.0;
               maxObj[2] += (quaux->Zobjetos[obj]);


               if(minObj[0] > maxObj[0])
               {
                  GLfloat swp = minObj[0];
                  minObj[0] = maxObj[0];
                  maxObj[0] = swp; 
               }
               if(minObj[2] > maxObj[2])
               {
                  GLfloat swp = minObj[2];
                  minObj[2] = maxObj[2];
                  maxObj[2] = swp; 
               }

               /*printf("OBJ: %.3f %.3f %.3f %.3f\n",minObj[0],minObj[2],maxObj[0],maxObj[2]);
               printf("MSE:  %.3f %.3f %.3f %.3f\n",minMouse[0],minMouse[2],maxMouse[0],maxMouse[2]);*/

               if(estaDentro( minObj, maxObj, minMouse, maxMouse, 1))
               /*if(pontoInterno((quaux->Xobjetos[obj]+modelo3d->x1),
                               (quaux->Zobjetos[obj]+modelo3d->z1),
                               (quaux->Xobjetos[obj]+modelo3d->x2),
                               (quaux->Zobjetos[obj]+modelo3d->z2),
                               xReal-2,zReal-2, xReal+2, zReal+2, true))*/
               {
                   sprintf(ObjTxt->texto,"%s",quaux->objetos[obj]->nome); 
                   janAtalhos->Desenhar();
                   pronto = 1;
               }
            }
         }
         if(!pronto)
         {
            sprintf(ObjTxt->texto,"Nothing"); 
            janAtalhos->Desenhar();
         }
        }
      }
      if(Mbotao & SDL_BUTTON(1)) 
      {
            
      }
      if ( keys[SDLK_ESCAPE] ) // Sai da Engine
         return(0);

      if(keys[SDLK_m])
      {
          if(!janMiniMapa)
          {
             abreMiniMapa();
             redesenha = true;
          }
      }

      if(keys[SDLK_n])
      {
          if(!janAtalhos)
          {
              abreAtalhos();
              redesenha = true;
          }
      }
      /* Tratamento das teclas para a Camera */
      if(keys[SDLK_UP])  // Aumenta o Zoom
      {
          if (d>ZOOMMAXIMO)
          {
             d-= varCamera;
             redesenha = true;
          }
      }
      if(keys[SDLK_DOWN]) // Diminui o Zoom
      {
         if(d<ZOOMMINIMO)
         {
             d+= varCamera; 
             redesenha = true;
         }
      }
      if(keys[SDLK_RIGHT]) // Roda Camera Antihorariamente
      {
          phi -= varCamera;  
          redesenha = true;
      }
      if(keys[SDLK_LEFT]) // Roda Camera Horariamente
      {
         phi += varCamera;
         redesenha = true;
      }
      if(keys[SDLK_PAGEUP]) // Sobe com a camera ate visao de cima
      {
            theta += varCamera;
            redesenha = true;
            if(theta > 89) 
               theta = 89;
      }
      if(keys[SDLK_PAGEDOWN]) // desce com a camera ate visao em 1ª pessoa
      {
         theta -= varCamera;
         redesenha = true;
         if(theta < 0)
            theta = 0;
      }
      if (keys[SDLK_HOME]) // Zoom Maximo
      {
         d = ZOOMMAXIMO;
         redesenha = true;
      }
      if(keys[SDLK_END]) // ZoomMinimo
      {
         d = ZOOMMINIMO;
         redesenha = true;
      }   
      if(keys[SDLK_INSERT]) //Maximo para cima
      {
         theta = 89;
         redesenha = true;
      }
      if(keys[SDLK_DELETE]) //Maximo para baixo
      {
         theta = 0;
         redesenha = true;
      }

      /* Tratamento da tecla para Movimentacao do Personagem */
      if(keys[SDLK_q] || keys[SDLK_e])
      {
          varX = passo * sin(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
          varZ = passo * cos(deg2Rad(PCs->personagemAtivo->orientacao+90.0));
         // Anda com personagem de lado para esquerda
         if(keys[SDLK_q]) 
         {
             varX *= -1;
             varZ *= -1;
         }

         if(podeAndar(varX,varZ,0)) 
         {
            centroX += varX;
            centroZ += varZ;
            PCs->personagemAtivo->posicaoLadoX += varX;
            PCs->personagemAtivo->posicaoLadoZ += varZ;
            redesenha = true;
            andou = true;
         }
         else if( (varX != 0) && (podeAndar(varX,0,0)))
         {
            centroX += varX;
            PCs->personagemAtivo->posicaoLadoX += varX;
            redesenha = true;
            andou = true;
         }
         else if( (varZ != 0) && podeAndar(0,varZ,0)) 
         {
            centroZ += varZ;
            PCs->personagemAtivo->posicaoLadoZ += varZ;
            redesenha = true;
            andou = true;
         }
        
      }
      if(keys[SDLK_w] || keys[SDLK_s])
      { 
         varX = passo * sin(deg2Rad(PCs->personagemAtivo->orientacao));
         varZ = passo * cos(deg2Rad(PCs->personagemAtivo->orientacao));
         if(keys[SDLK_w]) 
         {
              varX *= -1;
              varZ *= -1;
         }
         if((podeAndar(varX,varZ,0)) ) 
         {
             PCs->personagemAtivo->posicaoLadoX += varX;
             PCs->personagemAtivo->posicaoLadoZ += varZ;
             centroX += varX;
             centroZ += varZ;
             redesenha = true;
             andou  = true;
         }
         else if((varX != 0) && (podeAndar(varX,0,0)))
         {
              PCs->personagemAtivo->posicaoLadoX += varX;
              centroX += varX;
              redesenha = true;
              andou = true;
         }
         else if((varZ!=0) && (podeAndar(0,varZ,0)))
         {
              PCs->personagemAtivo->posicaoLadoZ += varZ;
              centroZ += varZ;
              redesenha = true;
              andou = true;
         }

      }

      if( (keys[SDLK_a]) || (keys[SDLK_d]))
      {
         // Gira personagem antihorariamente
         if((keys[SDLK_a]) && (podeAndar(0,0,rotacao)) )  
         {
            PCs->personagemAtivo->orientacao += rotacao;
            if(PCs->personagemAtivo->orientacao > 360.0)
               PCs->personagemAtivo->orientacao = 
                               PCs->personagemAtivo->orientacao  - 360.0;
            redesenha = true;
            andou = true;
         }
         // Gira o personagem horariamente
         if((keys[SDLK_d]) && (podeAndar(0,0,-rotacao)) )
         {
            PCs->personagemAtivo->orientacao -= rotacao;
            if(PCs->personagemAtivo->orientacao < 0.0)
               PCs->personagemAtivo->orientacao = 360.0 + 
                                        PCs->personagemAtivo->orientacao ;
         }
         redesenha = true;
         andou = true;
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
         centroX = PCs->personagemAtivo->posicaoLadoX;
         centroZ = PCs->personagemAtivo->posicaoLadoZ;
         redesenha = true;
         SDL_Delay(100);
      }
      
      /* Tratamento do Mouse */

      /* Tratamento do Mouse para Camera */
      if(x == 0)    // Gira a Camera horariamente
      {
         phi+=2; 
         redesenha = true;  
      }
      if(x == screen->w-1) // Gira a camera antihorariamente
      {
        phi-=2; 
        redesenha = true;
      }
   }
   }
   
   if( (redesenha) || ( (*forcaAtualizacao != 0)/* && ((tempo-ultimaLeitura)>=16)*/))
   {
      if(janMiniMapa)
      {
         GLint x = (int)(((PCs->personagemAtivo->posicaoLadoX) / (SQUARESIZE)));
         if(x > mapa->x-1)
         {
            x = mapa->x-1;
         }
         GLint z = (int)(((PCs->personagemAtivo->posicaoLadoZ) / (SQUARESIZE)));
         if( z > mapa->z-1)
         {
            z = mapa->z-1;
         }
         x = 8 + (x*3);
         z = 20 + (z*3);

         botPerMiniMap->x1 = x;
         botPerMiniMap->x2 = x+2;
         botPerMiniMap->y1 = z;
         botPerMiniMap->y2 = z+2;

         janMiniMapa->Desenhar();
      }
  
      Desenhar();
      SDL_GL_SwapBuffers();
      *forcaAtualizacao = 0;
   }
 
   if(andou)
   {
      PCs->personagemAtivo->SetState(STATE_WALK);
      #ifdef REDE
        movchar(&clientData, PCs->personagemAtivo->ID, 
          PCs->personagemAtivo->posicaoLadoX,PCs->personagemAtivo->posicaoLadoZ,
          PCs->personagemAtivo->orientacao );
      #endif
   }
   else if(passouTempo)
   {
      PCs->personagemAtivo->SetState(STATE_IDLE);
   }
 
   return(1);
}

/*********************************************************************
 *                Funcao para desenhar a cena                        *
 *********************************************************************/
void engine::Desenhar()
{
   glClear (GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   /* Redefine a posicao dinamica da camera */
   cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centroY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);
 
   /* Atualiza para fazer o culling e o desenho da GUI */
   AtualizaFrustum(matrizVisivel,proj,modl);

   /* CEU */
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);

   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, ceu);
      glTranslatef(mapa->x*HALFSQUARESIZE, 0 , mapa->z*HALFSQUARESIZE);
      gluSphere(atmosfera,HALFFARVIEW,5,5);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);

     /* DEBUG */
   glPushMatrix();
    glColor3f(0.5,0.8,0.1);
    glBegin(GL_POLYGON);
      glVertex3f(xReal-2, 0.2, zReal-2);
      glVertex3f(xReal-2, 0.2, zReal+2);
      glVertex3f(xReal+2, 0.2, zReal+2);
      glVertex3f(xReal+2, 0.2, zReal-2);
    glEnd();
   glPopMatrix();

   glPushMatrix();
   
   /* Desenha o Mundo, fazendo culling do view frustum */
   mapa->draw(cameraX,cameraY,cameraZ,matrizVisivel);

   /* Desenha os Personagens do Jogador (PCs) */
      personagem* per = (personagem*) PCs->primeiro->proximo;
      int aux;
      for(aux=0;aux < PCs->total;aux++)
      {
         glPushMatrix();
           glTranslatef(per->posicaoLadoX, per->posicaoLadoY,
                        per->posicaoLadoZ);
           glRotatef(per->orientacao,0,1,0);
           per->Render();
           glDisable(GL_LIGHTING);
           glColor3f(0.6,0.1,0.1);
           glBegin(GL_POLYGON);
              glVertex3f(per->min[0],per->min[1]+1,per->min[2]);
              glVertex3f(per->min[0],per->min[1]+1,per->max[2]);
              glVertex3f(per->max[0],per->min[1]+1,per->max[2]);
              glVertex3f(per->max[0],per->min[1]+1,per->min[2]);

           glEnd();
           glEnable(GL_LIGHTING);
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
           per->Render();
         glPopMatrix();
         per = (personagem*) per->proximo;
      }

   /* Faz o Desenho da GUI */
   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;
   gluUnProject(SCREEN_X,SCREEN_Y, 0.01, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(SCREEN_X,SCREEN_Y-80,0.01, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(SCREEN_X-60,SCREEN_Y-80,0.01,modl,proj,viewPort, &x3, &y3, &z3);
   gluUnProject(SCREEN_X-60,SCREEN_Y,0.01, modl, proj, viewPort, &x4, &y4, &z4);

   glDisable(GL_LIGHTING);

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
   
   gui->Desenhar(proj,modl,viewPort);
  
   glEnable(GL_LIGHTING);
 
   glFlush();
}


/******************************************/
/*    PARTE DA DETECCAO DE COLISAO!!!!!!! */
/******************************************/

//min[x,y,z] max[x,y,z]


/*********************************************************************
 *          Faz o teste se o Quadrado quad é factivel de ser         *
 *                    ocupado pelo personagem                        *
 *********************************************************************/
int testa(GLfloat min[3], GLfloat max[3],Square* quad)
{
   int result = 0;
   GLfloat min2[3];
   GLfloat max2[3];

   Square* proxima = quad;
   if(proxima->flags & PISAVEL)
   {
     result = 1;
   }
   if(result) // Se possivel entrar, testa com Muros
   {
      int mur = 0;
      while((proxima->muros[mur] != NULL))
      {
         min2[0] = proxima->muros[mur]->x1; 
         min2[1] = 0; 
         min2[2] = proxima->muros[mur]->z1;
         max2[0] = proxima->muros[mur]->x2; 
         max2[1] = MUROALTURA; 
         max2[2] = proxima->muros[mur]->z2;
         result &= !estaDentro(min,max,min2,max2,1);
         if(!result)
           return(0);
         mur++;
      }
   }
   if(result) // Se eh possivel entrar, testa com os objetos
   {
      int ob = 0;
      //GLfloat u1,u2,v1,v2;
      GLMmodel* modelo3d;
      while( (proxima->objetos[ob] != NULL)) 
      {

          modelo3d = (GLMmodel*)proxima->objetos[ob]->modelo3d;
          float X[2], Z[2];
          X[0] = modelo3d->x1;
          X[1] = modelo3d->x2;
          Z[0] = modelo3d->z1;
          Z[1] = modelo3d->z2;
          if(proxima->orientacaoObjetos[ob]!=0)
          {
              GLfloat xVelho, zVelho;
              GLfloat cosseno = cos(deg2Rad(proxima->orientacaoObjetos[ob]));
              GLfloat seno = sin(deg2Rad(proxima->orientacaoObjetos[ob]));
              int aux;
              for(aux = 0;aux<=1;aux++)
              {
                  xVelho = X[aux];
                  zVelho = Z[aux];
                  X[aux] = (zVelho*seno) + (xVelho*cosseno);
                  Z[aux] = (zVelho*cosseno) - (xVelho*seno);
              }
              if(X[0]>X[1])
              {
                 xVelho = X[0];
                 X[0] = X[1];
                 X[1] = xVelho;
              }
              if(Z[0]>Z[1])
              {
                 zVelho = Z[0];
                 Z[0] = Z[1];
                 Z[1] = zVelho;
              }
          }

          //printf("%f %f %f %f\n",x[0],z[0],x[1],z[1]);
          min2[0] = X[0]+proxima->Xobjetos[ob];
          max2[0] = X[1]+proxima->Xobjetos[ob];
          min2[1] = modelo3d->y1;//+Yobjetos. TODO
          max2[1] = modelo3d->y2;
          min2[2] = Z[0]+proxima->Zobjetos[ob];
          max2[2] = Z[1]+proxima->Zobjetos[ob];
          //printf("%f %f %f %f\n",u1,v1,u2,v2);
          //printf("%f %f %f %f %f %f %f %f\n",);
          result &= !estaDentro(min,max,min2,max2,1);
          if(!result) //se ja achou que nao pode, cai fora
             return(0);
          ob++;
      }
   }

   return(result);
}

/*********************************************************************
 *                   Verifica Colisao com MeioFio                    *
 *********************************************************************/
int ColisaoComMeioFio(GLfloat min[3],GLfloat max[3], muro* meiosFio)
{
    GLfloat min2[3];
    GLfloat max2[3];
    muro* maux = meiosFio;
    while(maux)
    {
       min2[0] = maux->x1;
       max2[0] = maux->x2;
       min2[1] = 0;
       max2[1] = MEIOFIOALTURA;
       min2[2] = maux->z1;
       max2[2] = maux->z2;
       if( (estaDentro(min, max, min2, max2, 1)) )
          return(1);
       //printf("Fora: %.3f %.3f %.3f %.3f,\n %.3f %.3f %.3f %.3f,\n %.3f %.3f %.3f %.3f\n ", maux->x1, maux->z1, maux->x2, maux->z2, x[0],x[1],x[2],x[3],z[0],z[1],z[2],z[3]);
       maux = maux->proximo;
    }
    return(0);
}

/*********************************************************************
 *                   Verifica se nao ha Colisao                      *
 *********************************************************************/
int engine::podeAndar(GLfloat varX, GLfloat varZ, GLfloat varAlpha)
{
   int result = 1;
   Square* saux;

   GLfloat min[3],min2[3];
   GLfloat max[3],max2[3];
   uint aux;

   GLfloat x[4],z[4];

   x[0] = PCs->personagemAtivo->min[0];
   z[0] = PCs->personagemAtivo->min[2];

   x[1] = PCs->personagemAtivo->min[0];
   z[1] = PCs->personagemAtivo->max[2]; 

   x[2] = PCs->personagemAtivo->max[0];
   z[2] = PCs->personagemAtivo->max[2];

   x[3] = PCs->personagemAtivo->max[0];
   z[3] = PCs->personagemAtivo->min[2];

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
   min[1] += PCs->personagemAtivo->min[1]+PCs->personagemAtivo->posicaoLadoY;
   max[1] += PCs->personagemAtivo->max[1]+PCs->personagemAtivo->posicaoLadoY;
   for(aux=0;aux<4;aux++)
   {
     x[aux] += PCs->personagemAtivo->posicaoLadoX+varX;
     z[aux] += PCs->personagemAtivo->posicaoLadoZ+varZ;
     if( (x[aux]<2) || (z[aux]<2) || 
         (x[aux]>mapa->x*SQUARESIZE-2) || (z[aux]>mapa->z*SQUARESIZE-2))
     {
         return(0);
     }
     if(aux == 0)
     {
        min[0] = x[0]; max[0] = x[0];
        min[2] = z[0]; max[2] = z[0];
     }
     else
     {
         if(x[aux] < min[0])
         {
            min[0] = x[aux];
         }
         if(x[aux] > max[0])
         {
            max[0] = x[aux];
         }
         if(z[aux] < min[2])
         {
            min[2] = z[aux];
         }
         if(z[aux] > max[2])
         {
            max[2] = z[aux];
         }

     }
   }
   



   /* Testa o Atual, ja que eh GRANDE! */
   min2[0] = PCs->personagemAtivo->ocupaQuad->x1;
   min2[1] = 0;
   min2[2] = PCs->personagemAtivo->ocupaQuad->z1;
   max2[0] = PCs->personagemAtivo->ocupaQuad->x2;
   max2[1] = 400;
   max2[2] = PCs->personagemAtivo->ocupaQuad->z2;
   if(estaDentro(min,max,min2,max2,1))
   {
      result &= testa(min,max,PCs->personagemAtivo->ocupaQuad);
      if(!result)
      {
         //printf("sai na atual\n"); 
         return(0);
      }
   }

 
   /* Testa quadrados a direita */
   saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
                             PCs->personagemAtivo->ocupaQuad->posZ);
   if(saux) 
   { 
      /* leste */
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      {
         result &= testa(min,max,saux);
         if(!result)
         {
            //printf("sai na direita\n"); 
            return(0);
         }
      }
      /* Nordeste */
      saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
                             PCs->personagemAtivo->ocupaQuad->posZ-1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1) )
         {
            result &= testa(min,max,saux);
            if(!result) 
            {
               //printf("sai na direita->cima\n"); 
               return(0);
            }
         }
      }
      /* Sudeste */
      saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX+1,
                             PCs->personagemAtivo->ocupaQuad->posZ+1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1))
         {
            result &= testa(min,max,saux);
            if(!result) 
            {
               //printf("sai na direita->baixo\n"); 
               return(0);
            }
         }
      }
   }

   /* Testa quadrados a esquerda */
   saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
                             PCs->personagemAtivo->ocupaQuad->posZ);
   if( saux ) 
   { 
      /* oeste */
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1))
      {
         result &= testa(min,max,saux);
         if(!result) 
         {
            //printf("sai na esquerda\n"); 
            return(0);
         }
      }

      /* Noroeste */
      saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
                             PCs->personagemAtivo->ocupaQuad->posZ-1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1) )
         {
            result &= testa(min,max,saux);
            if(!result) 
            {
               //printf("sai na esquerda->cima\n"); 
               return(0);
            }
         }
      }
      /* Sudoeste */
      saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX-1,
                             PCs->personagemAtivo->ocupaQuad->posZ+1);
      if( saux )
      { 
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1))
         {
            result &=testa(min,max,saux);
            if(!result) 
            {
                //printf("sai na esquerda->baixo\n"); 
               return(0);
            }
         }
      }
   }
  
   /* Testa quadrados abaixo */
   saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX,
                             PCs->personagemAtivo->ocupaQuad->posZ+1);
   if( saux ) 
   {   
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      { 
         /* sul */
         result &= testa(min,max,saux);
         if(!result) 
         {
            //printf("sai no de baixo\n"); 
            return(0);
         }
      }
   }

   /* Testa quadrados acima */
   saux = mapa->quadradoRelativo(PCs->personagemAtivo->ocupaQuad->posX,
                             PCs->personagemAtivo->ocupaQuad->posZ-1);
   if( saux )
   {  
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      { 
         /* norte */
         result &= testa(min,max,saux);
         if(!result) 
         {
            //printf("sai no de cima\n"); 
            return(0);
         }
      }
   }

   if( ColisaoComMeioFio( min, max, mapa->meiosFio) )
   {
      PCs->personagemAtivo->posicaoLadoY = MEIOFIOALTURA+0.1;
   }
   else
   {
      PCs->personagemAtivo->posicaoLadoY = 0.0;
   }

   GLfloat nx = ((min[0] + max[0]) / 2);//(PCs->personagemAtivo->posicaoLadoX+varX);
   GLfloat nz = ((min[2] + max[2]) / 2);//(PCs->personagemAtivo->posicaoLadoZ+varZ);

   int posX =(int)floor( nx / (SQUARESIZE));

   int posZ =(int)floor( nz / (SQUARESIZE));

   PCs->personagemAtivo->ocupaQuad = mapa->quadradoRelativo(posX,posZ);

   saux = mapa->quadradoRelativo( (int)(nx/SQUARESIZE),
                                  (int)(nz/SQUARESIZE));

   GLfloat dx1 = fabs(nx - saux->x1) / SQUARESIZE;
   GLfloat dz1 = fabs(nz - saux->z1) / SQUARESIZE;
   GLfloat dx2 = fabs(nx - saux->x2) / SQUARESIZE;
   GLfloat dz2 = fabs(nz - saux->z2) / SQUARESIZE;

   GLfloat ha = (dx2 * PCs->personagemAtivo->ocupaQuad->h1) + 
                (dx1 * PCs->personagemAtivo->ocupaQuad->h4);
   GLfloat hb = (dx2 * PCs->personagemAtivo->ocupaQuad->h2) + 
                (dx1 * PCs->personagemAtivo->ocupaQuad->h3);

   GLfloat res = (ha * dz2) + (hb * dz1);
 
   PCs->personagemAtivo->posicaoLadoY += res;
   deltaY = res;

   
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
   Square* saux;
   if(centro)
   {
       verificaQuad(centro);
       saux = mapa->quadradoRelativo(centro->posX-1,centro->posZ);
       if(saux)
       {
          verificaQuad(saux);
          saux = mapa->quadradoRelativo(centro->posX-2,centro->posZ);
          verificaQuad(saux);
       }
       saux = mapa->quadradoRelativo(centro->posX+1,centro->posZ);
       if(saux)
       {
          verificaQuad(saux);
          saux = mapa->quadradoRelativo(centro->posX+2,centro->posZ);
          verificaQuad(saux);
       }
   }   
}

/* Retorno !=0 se modificou posicao do personagem */
int engine::TrataIA()
{   

   int posX, posZ;     //Posicao Auxiliar
   double antX,antZ;
   Square* saux;
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
       saux = mapa->quadradoRelativo(per->ocupaQuad->posX,
                                     per->ocupaQuad->posZ-1);
       if(saux)
       {
          verificaLinha(saux);
          verificaLinha(saux);
       }
       saux = mapa->quadradoRelativo(per->ocupaQuad->posX,
                                     per->ocupaQuad->posZ+1);
       verificaLinha(saux);
    }
    else
      printf("What the HEll!! Square Map Out of bounds!\n");
   
    antX = per->posicaoLadoX;
    antZ = per->posicaoLadoZ;
    ia->destinoNpc(per);

    /* Define-se A posicao do Personagem NPC */  
    posX =(int)floor((per->posicaoLadoX) / (SQUARESIZE))+1;
    posZ =(int)floor((per->posicaoLadoZ) / (SQUARESIZE))+1;
    per->ocupaQuad = mapa->quadradoRelativo(posX,posZ);

    return( (antX!=per->posicaoLadoX) || (antZ!=per->posicaoLadoZ));
}

/*********************************************************************
 *                       Carrega Janela de MiniMapa                  *
 *********************************************************************/
void engine::abreMiniMapa()
{
   GLint x = (int)(((PCs->personagemAtivo->posicaoLadoX) / (SQUARESIZE)));
   if(x > mapa->x-1)
   {
      x = mapa->x-1;
   }
   GLint z = (int)(((PCs->personagemAtivo->posicaoLadoZ) / (SQUARESIZE)));
   if( z > mapa->z-1)
   {
      z = mapa->z-1;
   }
   x = 8 + (x*3);
   z = 20 + (z*3);
   janMiniMapa = gui->ljan->InserirJanela(0,344,255,471,"Map",1,1,NULL,NULL);

   botPerMiniMap = janMiniMapa->objetos->InserirBotao(x,z,x+2,z+2,255,255,128,
                                                      "",0,NULL);
   figura* fig = janMiniMapa->objetos->InserirFigura(8,20,NULL);
   mapa->drawMinimap(fig->fig);
   
                   
   janMiniMapa->ptrExterno = &janMiniMapa;
   janMiniMapa->Abrir(gui->ljan);
}

/*********************************************************************
 *                       Carrega Janela de Atalhos                   *
 *********************************************************************/
void engine::abreAtalhos()
{
   janAtalhos = gui->ljan->InserirJanela(0,472,511,599,"ShortCuts",1,1,NULL,NULL);
   FPS = janAtalhos->objetos->InserirQuadroTexto(8,20,100,45,0,"FPS:");
   ObjTxt = janAtalhos->objetos->InserirQuadroTexto(8,46,100,71,0,"Nothing");
   janAtalhos->ptrExterno = &janAtalhos;
   janAtalhos->Abrir(gui->ljan);
}

/*********************************************************************
 *                          Roda a Engine                            *
 *********************************************************************/
int engine::Rodar(SDL_Surface *surface)
{

   int forcaAtualizacao = 0; //forca a atualizacao da tela, qdo o npc anda
   int posX, posZ;           //Posicao Auxiliar
   FPSatual = 10.0;
   ultimaFPS = 0;

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
         printf("Can't connect!\nAborting...\n");
         return(1);
     }
     entergame( &clientData );
   #endif
  
   /* Roda realmente a engine */
   Desenhar();
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
                 printf("Arrived error from server\n");
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
                     printf("Server Full\n");
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


