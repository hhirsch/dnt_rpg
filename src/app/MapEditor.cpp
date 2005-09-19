/* Editor de Mapas, em domínio público. Código mais zoneado impossível. */

#include "../gui/farso.h"
#include "../map/map.h"
#include "../engine/culling.h"
#include "../etc/glm.h"
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHAO           1
#define MUROX          2
#define MUROXINIC      3
#define MUROXINICQUAD  9
#define MUROZ          8
#define MUROZINIC      7
#define MUROZINICQUAD 10
#define OBJETO         4
#define DELMURO        5
#define DELOBJETO      6

double deg2Rad(double x){return 3.1415927 * x/180.0;}

int estado;
GLfloat matrizVisivel[6][4]; /* Matriz do frustum atual */
GLdouble proj[16];
GLdouble modl[16];
GLint viewPort[4];
GLuint texturaAtual;
mapObjeto* objAtual;
Map* mapa;
barraTexto* bartInserir;

/************************************************************************
 *              Trata Botao Objeto                                      *
 ************************************************************************/
int botaoObjeto(void *jan,void *ljan,SDL_Surface *screen)
{
   estado = OBJETO;

   return(1);
}

/************************************************************************
 *              Trata Botao Proximo Objeto                              *
 ************************************************************************/
int botaoProximoObjeto(void *jan,void *ljan,SDL_Surface *screen)
{
   if(objAtual)
      objAtual = (mapObjeto*)objAtual->proximo;
  if(objAtual == mapa->Objetos->primeiro)
      objAtual = (mapObjeto*)objAtual->proximo;
   return(1);
}

/************************************************************************
 *              Trata Botao Objeto Anterior                             *
 ************************************************************************/
int botaoObjetoAnterior(void *jan,void *ljan,SDL_Surface *screen)
{
   if(objAtual)
      objAtual = (mapObjeto*)objAtual->anterior;
   if(objAtual == mapa->Objetos->primeiro)
      objAtual = (mapObjeto*)objAtual->anterior;

   return(1);
}

/************************************************************************
 *                     Trata Botao Chao                                 *
 ************************************************************************/
int botaoChao(void *jan,void *ljan,SDL_Surface *screen)
{
    estado = CHAO; 
    return(1);
}

/************************************************************************
 *                     Trata Botao MuroX                                *
 ************************************************************************/
int botaoMuroX(void *jan,void *ljan,SDL_Surface *screen)
{
    estado = MUROX;
    return(1);
}

/************************************************************************
 *                     Trata Botao MuroY                                *
 ************************************************************************/
int botaoMuroZ(void *jan,void *ljan,SDL_Surface *screen)
{
    estado = MUROZ;
    return(1);
}

/************************************************************************
 *                   Ativa a Textura Anterior                           *
 ************************************************************************/
int TexturaAnterior(Map* mapa, GLuint ID)
{
   int aux=0;
   texture* tex = mapa->Texturas;
   while(aux < mapa->numtexturas-1)
   {
      if(tex->proximo->indice == ID)
      {
         return(tex->indice);
      }
      tex = tex->proximo;
      aux++;
   }
   return(mapa->Texturas->indice);
}

/************************************************************************
 *                   Ativa a Proxima Textura                            *
 ************************************************************************/
int ProximaTextura(Map* mapa, GLuint ID)
{
   int aux=0;
   texture* tex = mapa->Texturas;
   while(aux < mapa->numtexturas)
   {
      if(tex->indice == ID)
      {
         if(tex->proximo)
           return(tex->proximo->indice);
         else
           return(tex->indice);
      }
      tex = tex->proximo;
      aux++;
   }
   return(-1);
}

/************************************************************************
 *                     Trata Botao Proxima Textura                      *
 ************************************************************************/
int botaoProximaTextura(void *jan,void *ljan,SDL_Surface *screen)
{
   texturaAtual = ProximaTextura(mapa,texturaAtual);
   return(1);
}

/************************************************************************
 *                     Trata Botao Textura Anterior                     *
 ************************************************************************/
int botaoTexturaAnterior(void *jan,void *ljan,SDL_Surface *screen)
{
   texturaAtual = TexturaAnterior(mapa,texturaAtual);
   return(1);
}

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

/************************************************************************
 *                     Chamada de Erro de Invocacao                     *
 ************************************************************************/
void erro()
{
   printf("/\n");
   printf("| Use:\n");
   printf("| To edit a new Map: MapEditor -n\n");
   printf("| To edit an existent map: MapEditor -e file.map\n\n");
   exit(2);
}

/************************************************************************
 *                     Insere Textura no Quadrado                       *
 ************************************************************************/
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
       if(quad) 
         quad->textura = texturaID;
    }
}

/************************************************************************
 *             Insere Textura na Lista de Texturas                      *
 ************************************************************************/
int inserirTextura(Map* mapa, char* arq, char* nome)
{
   texture* tex;

   SDL_Surface* img = IMG_Load(arq);
   if(!img)
   {
      printf("Error on open texture : %s\n",arq);
      return(-1);
   }

   /* Insere realmente a textura */ 
   tex = (texture*) malloc(sizeof(texture));
   if(mapa->numtexturas == 0)
   {
      mapa->Texturas = tex;
      tex->proximo = NULL;
   }
   else
   {
      tex->proximo = mapa->Texturas;
      mapa->Texturas = tex;
   }

   tex->nome = (char*) malloc((strlen(nome)+1)*sizeof(char));
   tex->arqNome = (char*) malloc((strlen(arq)+1)*sizeof(char));

   strcpy(tex->arqNome,arq);
   strcpy(tex->nome,nome);

   SDL_Surface *imgPotencia = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   //SDL_Rect ret;
   //ret.w = 0; ret.h = 0; ret.x = img->w; ret.y = img->h;
   SDL_BlitSurface(img,NULL,imgPotencia,NULL);
 
   Uint8 R,G,B;
   
   Uint32 pixel = pixel_Pegar(imgPotencia,10,10);
   SDL_GetRGB(pixel,imgPotencia->format, &R, &G, &B);
   tex->R = R;
   tex->G = G;
   tex->B = B;

   glGenTextures(1, &(tex->indice));
   glBindTexture(GL_TEXTURE_2D, tex->indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imgPotencia->w,imgPotencia->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, imgPotencia->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   mapa->numtexturas++;

   //printf("Inseri Textura: %s :%d %d\n",nome,img->w,img->h);

   /* Libera a memoria utilizada */
   SDL_FreeSurface(img);
   SDL_FreeSurface(imgPotencia);
   return(tex->indice);
}

/************************************************************************
 *                     Trata Botao Inserir                              *
 ************************************************************************/
int botaoInserir(void *jan,void *ljan,SDL_Surface *screen)
{
   int tam = strlen(bartInserir->texto);
   if(bartInserir->texto[tam-1] == 'g') /* g, de jpg, textura*/
   {
       inserirTextura(mapa,bartInserir->texto,bartInserir->texto);
       printf("Inserted Texture: %s\n",bartInserir->texto);
   }
   else
   if(bartInserir->texto[tam-1] == 'c') /* c, de dcc, objeto*/
   {
       objAtual = mapa->Objetos->InserirMapObjeto(bartInserir->texto,
                                               bartInserir->texto);
       printf("Inserted Object: %s\n",bartInserir->texto);
   }
   return(1);
}

/************************************************************************
 *                     Cria um Novo Mapa                                *
 ************************************************************************/
void novoMapa(Map* mapa, int x, int z)
{
   int auxX, auxZ;
   Square* saux = new(Square);
   Square* primLinha = saux;
   mapa->first = saux;
   mapa->x = x;
   mapa->z = z;

   printf("Beginning a new Map: %d,%d\n",x,z);
   
   int IDtextura = inserirTextura(mapa,"../data/texturas/chao_grama2.jpg", 
                  "chao_grama2");

   /* Cria todos os quadrados necessários */
   for(auxZ = 0; auxZ < z; auxZ++)
   {
      for(auxX = 0; auxX < x; auxX++)
      {
          saux->x1 = (auxX)*SQUARESIZE;
          saux->x2 = saux->x1+SQUARESIZE;
          saux->z1 = (auxZ)*SQUARESIZE;
          saux->z2 = saux->z1+SQUARESIZE; 
          saux->posX = auxX;
          saux->posZ = auxZ;
          saux->flags |= PISAVEL;
          saux->textura = IDtextura;
          saux->R = 130;
          saux->G = 148;
          saux->B = 96;
          if(auxX != x-1)
          {
             saux->right = new(Square);
             saux->right->left = saux;
             if(saux->up == NULL)
                saux->right->up = NULL;
             else
             {
                saux->right->up = saux->up->right;
                saux->right->up->down = saux->right;
             }
             saux = saux->right;
          }
          else
          {
             primLinha->down = new(Square);
             primLinha->down->up = primLinha;
             primLinha = primLinha->down;
             saux = primLinha;
          }
      }
   }

   mapa->xInic = 1*SQUARESIZE;
   mapa->zInic = 1*SQUARESIZE;
   mapa->squareInic = mapa->first;
} 

/************************************************************************
 *                     Escopo Principal                                 *
 ************************************************************************/
int main(int argc, char **argv)
{
   estado = CHAO;
   printf(" DccNitghtmare - Map Editor\n");
   /* Inicia ou abre mapa Existente */
   char* entrada; /*Arquivo de entrada*/
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
   int x,z;

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
         case 'n':{
                    novo = 1;
                    printf("Number of Lines: ");
                    scanf("%d",&x);
                    printf("Number of Columns: ");
                    scanf("%d",&z);
                    chamadaCorreta = 1;
                    break; 
                  }
         default: erro();
      } 
   } 

   if(!chamadaCorreta) erro();

   /* Inicia o Sistema Bizarro */ 
   SDL_Surface* screen;
   Farso_Iniciar(&screen,"DccNiTghtmare's Map Editor");
   Iniciar(screen);  

   mapa = new(Map);
   if(novo)
   {
      if(x < 3) 
      {
          printf("Lines must be 3 or more. Setting to 3.\n");
          x = 3;
      }
      if(z<3)
      {
          printf("Columns must be 3 or more. Setting to 3.\n");
          z = 3;
      }
      /* Cria um mapa ja existente */
      novoMapa(mapa,x,z);
   }
   else
      /* Abre mapa ja existente */
      mapa->open(entrada);

   int sair = 0;
   Uint8 Mbotao;
   Uint8 *teclas;
   int mouseX,mouseY;
   double varX, varZ;

   interface* gui = new interface(NULL);
   janela* principal;
   principal = gui->ljan->InserirJanela(0,0,255,127,"Main",1,1,NULL,NULL);
   principal->objetos->InserirBotao(10,17,50,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Floor",1,&botaoChao);
   principal->objetos->InserirBotao(10,37,50,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "XWall",1,&botaoMuroX);
   principal->objetos->InserirBotao(10,57,50,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "ZWall",1,&botaoMuroZ);
   principal->objetos->InserirBotao(55,17,125,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Objects",1,&botaoObjeto);
   principal->objetos->InserirBotao(55,57,125,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Destroy",1,NULL);
   principal->objetos->InserirBotao(55,37,125,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Take Obj",1,NULL);
   principal->objetos->InserirBotao(130,17,200,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Texture >",1,
                                                &botaoProximaTextura);
   principal->objetos->InserirBotao(130,37,200,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Texture <",1,
                                                &botaoTexturaAnterior);
   principal->objetos->InserirBotao(130,57,200,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Object >",1,
                                                &botaoProximoObjeto); 
   principal->objetos->InserirBotao(130,77,200,95,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Object <",1,
                                                &botaoObjetoAnterior);
   bartInserir = principal->objetos->InserirBarraTexto(10,97,128,113,
                                         "../data/texturas/chao_grama.jpg",
                                         0,NULL);
   principal->objetos->InserirBotao(130,97,200,113,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Insert",1,&botaoInserir);
   principal->fechavel = 0;
   principal->Abrir(gui->ljan);
 
   muro* maux = NULL;
   int qx; int qz;
   double xReal, zReal, yReal;
   float wx,wy,wz;

   texturaAtual = mapa->Texturas->indice;
   objAtual = (mapObjeto*)mapa->Objetos->primeiro->proximo;

   while(!sair)
   {

      SDL_PumpEvents();
      Mbotao = SDL_GetMouseState(&mouseX,&mouseY);
      teclas = SDL_GetKeyState(NULL);

      wx = mouseX; wy = 600-mouseY; 
            
      glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
      gluUnProject( wx, wy, wz, modl, proj, viewPort, &xReal, &yReal, &zReal);
 
      qx = (int)xReal / SQUARESIZE; 
      qz = (int)zReal / SQUARESIZE;

      if(gui->ManipulaEventos(mouseX,mouseY,Mbotao,teclas)==NADA)
      {
         if(Mbotao & SDL_BUTTON(1))
         {

            if (qx > mapa->x) qx = mapa->x;
            else if (qx < 0) qx = 0;
            if (qz > mapa->z) qz = mapa->z;
            else if (qz < 0) qz = 0;
             
            if(estado == CHAO)
               colocaTextura(mapa, (int)xReal / SQUARESIZE, 
                             (int)zReal / SQUARESIZE, texturaAtual);
            else if(estado == OBJETO)
            {
               Square* saux = mapa->quadradoRelativo(qx+1,qz+1);
               int ob=0;
               if(saux)
               {
                  while( (ob < MAXOBJETOS ) && (saux->objetos[ob] != NULL))
                     ob++;
                  if(ob<MAXOBJETOS)
                  {
                     saux->objetos[ob] = objAtual;
                     //saux->quadXobjetos[ob] = qx;
                     //saux->quadZobjetos[ob] = qz;
                     //objAtual->x
                     saux->Xobjetos[ob] = xReal;
                     saux->Zobjetos[ob] = zReal;
                     saux->objetosDesenha[ob] = 1;
                     printf("%d° Object Inserted on %d %d\n",ob,qx+1,qz+1);
                     
                     GLMmodel* modelo = (GLMmodel*)objAtual->modelo3d; 
                     int minqx, minqz, maxqx, maxqz;
                     minqx = (int)(modelo->x1 + xReal) / SQUARESIZE;
                     minqz = (int)(modelo->z1 + zReal) / SQUARESIZE;
                     maxqx = (int)(modelo->x2 + xReal) / SQUARESIZE;
                     maxqz = (int)(modelo->z2 + zReal) / SQUARESIZE; 
                     int X, Z;
                     Square* qaux;
                     for(X = minqx; X<=maxqx; X++)
                     {
                         for(Z = minqz; Z <=maxqz; Z++) 
                         {
                             qaux = mapa->quadradoRelativo(X+1,Z+1);
                             if((qaux) && (qaux != saux))
                             {
                                 ob =0;
                                 while( (ob < MAXOBJETOS ) && 
                                        (qaux->objetos[ob] != NULL))
                                           ob++;
                                 if(ob < MAXOBJETOS)
                                 {
                                    qaux->objetos[ob] = objAtual;
                                    qaux->Xobjetos[ob] = xReal;
                                    qaux->Zobjetos[ob] = zReal;
                                    qaux->objetosDesenha[ob] = 0;
                                    printf("%d° Object Inserted on %d %d\n",
                                            ob,X+1,Z+1);
                                 }
                             }
                         }
                     }
                     
                     SDL_Delay(500);
                  }
                  else
                     printf("Objects Overflow on Square %d %d\n",qx+1,qz+1);
               }
               else
                 printf("Out of Map's Limits!\n");
            }
            else if( (estado == MUROX) || (estado == MUROZ))
            {
                /* Compara com Muros existentes, para "ligar"/"colar" */
                muro* m = mapa->muros;
                while( (m!=NULL))
                {
                    if(sqrt( (m->x1 - xReal)*(m->x1 - xReal) +
                             (m->z1 - zReal)*(m->z1 - zReal)) < 10)
                    {
                        xReal = m->x1;
                        zReal = m->z1;
                        m = NULL;
                    } 
                    else if(sqrt( (m->x2 - xReal)*(m->x2 - xReal) +
                             (m->z1 - zReal)*(m->z1 - zReal)) < 10)
                    {
                        xReal = m->x2;
                        zReal = m->z1;
                        m = NULL;
                    }
                    else if(sqrt( (m->x1 - xReal)*(m->x1 - xReal) +
                             (m->z2 - zReal)*(m->z2 - zReal)) < 10)
                    {
                        xReal = m->x1;
                        zReal = m->z2;
                        m = NULL;
                    }
                    else if(sqrt( (m->x2 - xReal)*(m->x2 - xReal) +
                             (m->z2 - zReal)*(m->z2 - zReal)) < 10)
                    {
                        xReal = m->x1;
                        zReal = m->z2;
                        m = NULL;
                    }
                    if(m!=NULL)
                      m = m->proximo;
                }
                maux = new(muro);
                maux->proximo = mapa->muros;
                mapa->muros = maux;
                maux->x1 = xReal/*qx*SQUARESIZE*/;
                //maux->x2 = xReal;//(qx)*SQUARESIZE;
                maux->z1 = zReal;
                //maux->z2 = zReal+10;//(qz)*SQUARESIZE;
                maux->textura = texturaAtual;
                if(estado == MUROX)
                {
                   estado = MUROXINIC;
                   maux->x2 = xReal;
                   maux->z2 = zReal+10;
                }
                else
                {
                   estado = MUROZINIC;
                   maux->z2 = zReal;
                   maux->x2 = xReal+10;
                }
            }
            else if ((estado == MUROXINIC) || (estado == MUROZINIC))
            {
                if(estado == MUROXINIC)
                   maux->x2 = xReal;//(qx)*SQUARESIZE;
                else
                   maux->z2 = zReal;//(qz)*SQUARESIZE;
            }
            else if(estado == MUROXINICQUAD)
            {
                maux->x2 = (qx+1)*SQUARESIZE;
            }
            else if(estado == MUROZINICQUAD)
            {
               maux->z2 = (qz+1)*SQUARESIZE;
            }
         }
         else
         {
             if((estado == MUROXINIC) || (estado == MUROZINIC) || 
                (estado == MUROXINICQUAD) || (estado == MUROZINICQUAD))
             {
                if(maux->x2 < maux->x1)
                {
                   float aux = maux->x1;
                   maux->x1 = maux->x2;
                   maux->x2 = aux;
                }
                if(maux->z2 < maux->z1)
                {
                    float aux = maux->z1;
                    maux->z1 = maux->z2;
                    maux->z2 = aux;
                }
                printf("Definido Muro: %f,%f,%f,%f\n",maux->x1,
                         maux->z1, maux->x2, maux->z2);
                
                if( (estado == MUROXINIC)||(estado == MUROXINICQUAD))
                   estado = MUROX;
                else
                   estado = MUROZ;
             }
         }
  
 
         sair = teclas[SDLK_ESCAPE];
         if(teclas[SDLK_b])
         {
             if(estado == MUROXINIC)
             {
                 float cmp = ((int)(maux->z1) / SQUARESIZE)*SQUARESIZE;
                 maux->x1 = ((int)round((maux->x1) / SQUARESIZE))*SQUARESIZE;
                 maux->x2 = ((int)round((maux->x2) / SQUARESIZE))*SQUARESIZE;
                 maux->z1 = ((int)round((maux->z1 / SQUARESIZE)))*SQUARESIZE;
                 if(cmp < maux->z1)
                       maux->z1 = maux->z1-10;
                 maux->z2 = maux->z1+10;
//TODO Minimize wall sobreposition
                 estado = MUROXINICQUAD;
             }
             else if(estado == MUROZINIC)
             {
                 float cmp = ((int)(maux->x1) / SQUARESIZE)*SQUARESIZE;
                 maux->z1 = ((int)round((maux->z1) / SQUARESIZE))*SQUARESIZE;
                 maux->z2 = ((int)round((maux->z2) / SQUARESIZE))*SQUARESIZE;
                 maux->x1 = ((int)round((maux->x1 / SQUARESIZE)))*SQUARESIZE;
                 if(cmp < maux->x1)
                       maux->x1 = maux->x1-10;
                 maux->x2 = maux->x1+10;
                 estado = MUROZINICQUAD;             }
         } 
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
         if(teclas[SDLK_i])
         {
             printf("x:%.3f z:%.3f phi:%.3f theta:%.3f \n",centroX, centroZ,
                                                           phi,theta);
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
      if( (estado == OBJETO) && (objAtual))
         objAtual->Desenhar(xReal, zReal, 0);
      //glPushMatrix();
      glFlush();
      SDL_GL_SwapBuffers();

   }
 
   delete(mapa);
   delete(gui);

}

