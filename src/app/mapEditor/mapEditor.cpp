/* Editor de Mapas, em dom�nio p�blico. C�digo mais zoneado imposs�vel. */


#include "../../gui/farso.h"
#include "../../map/map.h"
#include "../../engine/culling.h"
#include "../../etc/glm.h"
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHAO               1
#define MUROX              2
#define MUROXINIC          3
#define MUROXINICQUAD      9
#define MUROZ              8
#define MUROZINIC          7
#define MUROZINICQUAD     10
#define OBJETO             4
#define DELMURO            5
#define DELOBJETO          6
#define PEGAOBJETO        11
#define MODALTURA         12
#define MEIOX             13
#define MEIOZ             14
#define MEIOXINIC         15
#define MEIOZINIC         16
#define MEIOXINICQUAD     17
#define MEIOZINICQUAD     18
#define PORTAL            19
#define PORTALINIC        20
#define PORTA             21


double deg2Rad(double x){return 3.1415927 * x/180.0;}


Map* mapa;


void inserirObjetoMapa(GLfloat xReal, GLfloat zReal, int orObj,
                       mapObjeto* objAtual, int qx, int qz)
{
   Square* saux = mapa->quadradoRelativo(qx,qz);
   int ob=0;
   if(saux)
   {
     while( (ob < MAXOBJETOS ) && (saux->objects[ob] != NULL))
     {
        ob++;
     }
     if(ob<MAXOBJETOS)
     {
        saux->objects[ob] = objAtual;
        //saux->quadXobjects[ob] = qx;
        //saux->quadZobjects[ob] = qz;
        //objAtual->x
        saux->Xobjects[ob] = xReal;
        saux->Zobjects[ob] = zReal;
        saux->objectsOrientation[ob] = orObj;
        saux->objectsDesenha[ob] = 1;
        printf("%d� Object Inserted on %d %d\n",ob,qx+1,qz+1);
                  
        GLMmodel* modelo = (GLMmodel*)objAtual->modelo3d; 

        float X[2], Z[2];
        X[0] = modelo->x1;
        X[1] = modelo->x2;
        Z[0] = modelo->z1;
        Z[1] = modelo->z2;
        if(orObj!=0)
        {
           GLfloat xVelho, zVelho;
           GLfloat cosseno = cos(deg2Rad(orObj));
           GLfloat seno = sin(deg2Rad(orObj));
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

       int minqx, minqz, maxqx, maxqz;
       minqx = (int)(X[0] + xReal) / SQUARESIZE;
       minqz = (int)(Z[0] + zReal) / SQUARESIZE;
       maxqx = (int)(X[1] + xReal) / SQUARESIZE;
       maxqz = (int)(Z[1] + zReal) / SQUARESIZE; 
       int X1, Z1;
       Square* qaux;
       for(X1 = minqx; X1<=maxqx; X1++)
       {
          for(Z1 = minqz; Z1 <=maxqz; Z1++) 
          {
             qaux = mapa->quadradoRelativo(X1,Z1);
             if((qaux) && (qaux != saux))
             {
                ob =0;
                while( (ob < MAXOBJETOS ) && 
                       (qaux->objects[ob] != NULL))
                {
                   ob++;
                }
                if(ob < MAXOBJETOS)
                {
                   qaux->objects[ob] = objAtual;
                   qaux->Xobjects[ob] = xReal;
                   qaux->Zobjects[ob] = zReal;
                   qaux->objectsDesenha[ob] = 0;
                   printf("%d� Object Inserted on %d %d\n",ob,X1+1,Z1+1);
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

void inserirPortal(float portalX[2], float portalZ[2], int qx, int qz)
{
   Square* s = mapa->quadradoRelativo(qx,qz);
   if(s)
   {
       if(portalX[0] > portalX[1])
       {
           GLfloat tmp = portalX[0];
           portalX[0] = portalX[1];
           portalX[1] = tmp;
       }
       if(portalZ[0] > portalZ[1])
       {
           GLfloat tmp = portalZ[0];
           portalZ[0] = portalZ[1];
           portalZ[1] = tmp;
       } 
       s->mapConection.x1 = portalX[0];
       s->mapConection.x2 = portalX[1];
       s->mapConection.z1 = portalZ[0];
       s->mapConection.z2 = portalZ[1];
       s->mapConection.mapName = "TODO"; //TODO mapname prompt
       s->mapConection.active = true;
       printf("MapPortal on %d %d\n",qx,qz);

      
       int minqx, minqz, maxqx, maxqz;
       minqx = (int)(portalX[0]) / SQUARESIZE;
       minqz = (int)(portalZ[0]) / SQUARESIZE;
       maxqx = (int)(portalX[1]) / SQUARESIZE;
       maxqz = (int)(portalZ[1]) / SQUARESIZE; 
       int X1, Z1;
       Square* q;
       for(X1 = minqx; X1<=maxqx; X1++)
       {
          for(Z1 = minqz; Z1 <=maxqz; Z1++) 
          {
             q = mapa->quadradoRelativo(X1,Z1);
             if((q) && (q != s))
             {
                 q->mapConection.x1 = portalX[0];
                 q->mapConection.x2 = portalX[1];
                 q->mapConection.z1 = portalZ[0];
                 q->mapConection.z2 = portalZ[1];
                 q->mapConection.mapName = "TODO"; //TODO mapname prompt
                 q->mapConection.active = true;
                 printf("MapPortal on %d %d\n",X1,Z1);
             }
          }
       }
   }
}


int estado;
GLfloat matrizVisivel[6][4]; /* Matriz do frustum atual */
GLdouble proj[16];
GLdouble modl[16];
GLint viewPort[4];
GLuint texturaAtual;
mapObjeto* objAtual;
mapObjeto* porta;
muro* muroPorta;
GLdouble xPorta, zPorta;
int orPorta;
int modoPorta;
barraTexto* bartInserir;
barraTexto* bartSalvar;
int sair;



int estaDentro(GLfloat ax, GLfloat  az, 
               GLfloat bx1, GLfloat  bz1, GLfloat bx2, GLfloat bz2)
{
   return( (ax >= bx1 ) && (ax <= bx2) && (az >= bz1) && (az <=bz2) );
}



/************************************************************************
 *              Trata Botao Portal                                      *
 ************************************************************************/
int botaoPortal(void *jan,void *ljan,SDL_Surface *screen)
{
   estado = PORTAL;
   return(1);
}

/************************************************************************
 *              Trata Botao Porta                                       *
 ************************************************************************/
int botaoPorta(void *jan,void *ljan,SDL_Surface *screen)
{
   if(!mapa->muros)
   {
      return(0);
   }
   porta = (mapObjeto*) mapa->Objetos->primeiro->proximo;
   while( (porta != mapa->Objetos->primeiro) && 
          (strcmp(porta->nome,"Door") != 0) ) 
   {
      porta = (mapObjeto*) porta->proximo;
   }
   if(porta != mapa->Objetos->primeiro) 
   {
      estado = PORTA;
      modoPorta = 1;
   }
   return(1);
}



/************************************************************************
 *              Trata Botao Objeto                                      *
 ************************************************************************/
int botaoObjeto(void *jan,void *ljan,SDL_Surface *screen)
{
   if(objAtual != mapa->Objetos->primeiro)
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
 *                     Trata Botao MuroX                                *
 ************************************************************************/
int botaoMeioX(void *jan,void *ljan,SDL_Surface *screen)
{
    estado = MEIOX;
    return(1);
}

/************************************************************************
 *                     Trata Botao MuroY                                *
 ************************************************************************/
int botaoMeioZ(void *jan,void *ljan,SDL_Surface *screen)
{
    estado = MEIOZ;
    return(1);
}


/************************************************************************
 *                     Trata Botao Salvar                               *
 ************************************************************************/
int botaoSalvar(void *jan,void *ljan, SDL_Surface *screen)
{
   mapa->save(bartSalvar->texto.c_str());
   printf("Saved Map as `%s`\n",bartSalvar->texto.c_str());
   SDL_Delay(200);
   return(1);
}

/************************************************************************
 *                     Trata Botao Sair                                 *
 ************************************************************************/
int botaoSair(void *jan,void *ljan, SDL_Surface *screen)
{
   sair = 1;
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
 *                     Trata Botao PEGA OBJETO                          *
 ************************************************************************/
int botaoPegaObjeto(void *jan,void *ljan,SDL_Surface *screen)
{
    estado = PEGAOBJETO; 
    return(1);
}

/************************************************************************
 *                     Trata Botao ALTURA                               *
 ************************************************************************/
int botaoAltura(void *jan,void *ljan,SDL_Surface *screen)
{
   estado = MODALTURA;
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
   GLfloat light_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
#if 0 
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 30.0, 10.0, 20.0, 0.0 };
   
   /* Carrega a Luz */
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

#endif
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
   
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
    Square* quad = mapa->quadradoRelativo(x,z);
    if(quad)
    {
       quad->textura = texturaID;
    }
}

/************************************************************************
 *             Insere Textura na Lista de Texturas                      *
 ************************************************************************/
int inserirTextura(Map* mapa, const char* arq, const char* nome)
{
   texture* tex;

   SDL_Surface* img = IMG_Load(arq);
   if(!img)
   {
      printf("Error on open texture : %s\n",arq);
      return(-1);
   }

   /* Insere realmente a textura */ 
   tex = (texture*) new(texture);
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

   /*tex->nome = (char*) malloc((strlen(nome)+1)*sizeof(char));
   tex->arqNome = (char*) malloc((strlen(arq)+1)*sizeof(char));*/

   tex->arqNome = arq;
   tex->nome = nome;

   SDL_Surface *imgPotencia = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   //SDL_Rect ret;
   //ret.w = 0; ret.h = 0; ret.x = img->w; ret.y = img->h;
   SDL_BlitSurface(img,NULL,imgPotencia,NULL);
 
   Uint8 R,G,B;
   
   Uint32 pixel = pixel_Pegar(imgPotencia,15,15);
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
   int tam = strlen(bartInserir->texto.c_str());
   if(bartInserir->texto[tam-1] == 'g') /* g, de jpg, textura*/
   {
       inserirTextura(mapa,bartInserir->texto.c_str(),
                      bartInserir->texto.c_str());
       printf("Inserted Texture: %s\n",bartInserir->texto.c_str());
   }
   else
   if(bartInserir->texto[tam-1] == 'c') /* c, de dcc, object*/
   {
       objAtual = mapa->Objetos->InserirMapObjeto(bartInserir->texto.c_str(),
                                               bartInserir->texto.c_str());
       printf("Inserted Object: %s\n",bartInserir->texto.c_str());
   }
   return(1);
}

/************************************************************************
 *                     Escopo Principal                                 *
 ************************************************************************/
int main(int argc, char **argv)
{
   estado = CHAO;
   printf(" DccNitghtmare - Map Editor\n");
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
      mapa->newMap(x,z);
   }
   else
      /* Abre mapa ja existente */
      mapa->open(entrada);

   sair = 0;
   Uint8 Mbotao;
   Uint8 *teclas;
   int mouseX,mouseY;
   double varX, varZ;
   int orObj = 0;

   interface* gui = new interface(NULL);
   janela* principal;
   janela* janSalvar;
   principal = gui->ljan->InserirJanela(0,64,255,191,"Main",1,1,NULL,NULL);
   principal->objects->InserirBotao(10,17,50,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Floor",1,&botaoChao);
   principal->objects->InserirBotao(10,37,50,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "XWall",1,&botaoMuroX);
   principal->objects->InserirBotao(10,57,50,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "ZWall",1,&botaoMuroZ);
   principal->objects->InserirBotao(10,77,50,95,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Door",1,&botaoPorta);
   principal->objects->InserirBotao(55,17,125,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Objects",1,&botaoObjeto);
   principal->objects->InserirBotao(55,37,125,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Take Obj",1,&botaoPegaObjeto);
   principal->objects->InserirBotao(55,57,125,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Destroy",1,NULL);
   principal->objects->InserirBotao(55,77,125,95,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Portal",1,&botaoPortal);
   principal->objects->InserirBotao(130,17,200,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Texture >",1,
                                                &botaoProximaTextura);
   principal->objects->InserirBotao(130,37,200,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Texture <",1,
                                                &botaoTexturaAnterior);
   principal->objects->InserirBotao(130,57,200,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Object >",1,
                                                &botaoProximoObjeto); 
   principal->objects->InserirBotao(130,77,200,95,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Object <",1,
                                                &botaoObjetoAnterior);
   bartInserir = principal->objects->InserirBarraTexto(10,97,128,113,
                                         "../data/texturas/chao_grama.jpg",
                                         0,NULL);
   principal->objects->InserirBotao(130,97,200,113,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Insert",1,&botaoInserir);
   principal->objects->InserirBotao(205,17,250,35,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Height",1,&botaoAltura);
   principal->objects->InserirBotao(205,37,250,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "1/2 X",1,&botaoMeioX);
   principal->objects->InserirBotao(205,57,250,75,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "1/2 Z",1,&botaoMeioZ);

   principal->fechavel = 0;
   principal->Abrir(gui->ljan);

   janSalvar = gui->ljan->InserirJanela(0,0,127,63,"Actions",1,1,NULL,NULL);
   janSalvar->objects->InserirBotao(10,37,50,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Save",1,&botaoSalvar);
   janSalvar->objects->InserirBotao(77,37,117,55,principal->Cores.corBot.R,
                                                principal->Cores.corBot.G,
                                                principal->Cores.corBot.B,
                                                "Exit",1,&botaoSair);
   bartSalvar = janSalvar->objects->InserirBarraTexto(10,17,118,33,
                                         entrada,
                                         0,NULL);

   janSalvar->fechavel = 0;
   janSalvar->Abrir(gui->ljan);

 
   muro* maux = NULL;
   int qx; int qz;
   double xReal, zReal, yReal;
   float wx,wy,wz;
   GLfloat portalX[2], portalZ[2];
   //Tobjeto* object = NULL;
   int eventInfo = NADA;
   int tempo = 0;

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

      if(xReal < 0)
        xReal = 0;
      if(zReal < 0)
        zReal = 0;

      if(xReal > mapa->x*SQUARESIZE)
        xReal = mapa->x*SQUARESIZE;

      if(zReal > mapa->z*SQUARESIZE)
        zReal = mapa->z*SQUARESIZE;
 
      qx = (int)xReal / SQUARESIZE; 
      qz = (int)zReal / SQUARESIZE;

      if(qx > mapa->x)
        qx = mapa->x;

      if(qz > mapa->z) 
        qz = mapa->z;

      gui->manipulateEvents(mouseX,mouseY,Mbotao,teclas,&eventInfo);
      if( (eventInfo == NADA) && (SDL_GetTicks() - tempo >= 20) )
      {
         tempo = SDL_GetTicks();
         if (qx > mapa->x) qx = mapa->x;
         else if (qx < 0) qx = 0;
         if (qz > mapa->z) qz = mapa->z;
         else if (qz < 0) qz = 0;

         if(Mbotao & SDL_BUTTON(1))
         {

            if( estado == PORTA )
            {
               GLfloat mx1,mx2,mz1,mz2;
               mx1 = muroPorta->x1;
               mx2 = muroPorta->x2;
               mz1 = muroPorta->z1;
               mz2 = muroPorta->z2;
               muro* novoMuro;
               novoMuro = new(muro);
               novoMuro->dX = muroPorta->dX;
               novoMuro->dY = muroPorta->dY;
               novoMuro->dZ = muroPorta->dZ;
               muro* maux;
               GLMmodel* modelo = (GLMmodel*)porta->modelo3d;
               if( orPorta == 0 )
               {
                  muroPorta->x2 = xPorta;
                  novoMuro->x1 = xPorta + (modelo->x2 - modelo->x1);
                  novoMuro->x2 = mx2;
                  novoMuro->z1 = mz1;
                  novoMuro->z2 = mz2;
               }
               else
               {
                  muroPorta->z2 = zPorta - (modelo->x2 - modelo->x1);
                  novoMuro->z1 = zPorta;
                  novoMuro->x2 = mx2;
                  novoMuro->x1 = mx1;
                  novoMuro->z2 = mz2;
               }
               novoMuro->textura = muroPorta->textura;
               maux = mapa->muros;
               mapa->muros = novoMuro;
               novoMuro->proximo = maux;

               //Coloca a Porta no Mapa
               /*inserirObjetoMapa(xPorta, zPorta, orPorta, porta, 
                                 (int)(xPorta / SQUARESIZE), 
                                 (int)(zPorta / SQUARESIZE) );*/
               door* novaPorta = new(door);
               novaPorta->x = xPorta;
               novaPorta->z = zPorta;
               novaPorta->orientacao = orPorta;
               novaPorta->object = porta;
               door* paux = mapa->portas;
               paux = mapa->portas;
               mapa->portas = novaPorta;
               novaPorta->proximo = paux;
               printf("Added Door: %.3f %.3f\n",xPorta,zPorta);
               SDL_Delay(500);
            }
            else if( estado == PORTAL )
            {
                portalX[0] = xReal;
                portalX[1] = xReal;
                portalZ[0] = zReal;
                portalZ[1] = zReal;
                estado = PORTALINIC;
            }
            else if(estado == PORTALINIC)
            {
                portalX[1] = xReal;
                portalZ[1] = zReal;
            }
            else if(estado == MODALTURA)
            {
               Square* saux = mapa->quadradoRelativo(qx,qz);
 
               if(saux)
               {
 
                  GLfloat d1,d2,d3,d4; //distancia ao vertice do quadrado

                  GLfloat dx1 = xReal - saux->x1;
                  dx1 *= dx1;
                  GLfloat dz1 = zReal - saux->z1;
                  dz1 *= dz1;
                  GLfloat dx2 = xReal - saux->x2;
                  dx2 *= dx2;
                  GLfloat dz2 = zReal - saux->z2;
                  dz2 *= dz2;

                  d1 = (sqrt( dx1 + dz1 ));

                  d2 = (sqrt( dx1 + dz2 ));

                  d3 = (sqrt( dx2 + dz2 ));

                  d4 = (sqrt( dx2 + dz1 ));
               


                  if( (d1<=d2) && (d1<=d3) && (d1<=d4) )
                  {
                     saux->h1 += 1;
                     saux = mapa->quadradoRelativo(qx-1,qz);
                     if(saux)
                        saux->h4 += 1;
                     saux = mapa->quadradoRelativo(qx,qz-1);
                     if(saux)
                     {
                        saux->h2 += 1;
                        saux = mapa->quadradoRelativo(qx-1,qz-1); 
                        if(saux)
                          saux->h3 += 1;
                     }
                  }
                  else if( (d2<=d1) && (d2<=d3) && (d2<=d4) )
                  {
                     saux->h2 += 1;
                     saux = mapa->quadradoRelativo(qx-1,qz);
                     if(saux)
                       saux->h3 += 1;
                     saux = mapa->quadradoRelativo(qx,qz+1);
                     if(saux)
                     { 
                        saux->h1 += 1;
                        saux = mapa->quadradoRelativo(qx-1,qz+1);
                        if(saux)
                          saux->h4 += 1;
                     } 
                  } 
                  else if( (d3<=d1) && (d3<=d2) && (d1<=d4) )
                  {
                     saux->h3 += 1;
                     saux = mapa->quadradoRelativo(qx+1,qz);
                     if(saux)
                        saux->h2 += 1;
                     saux = mapa->quadradoRelativo(qx,qz+1);
                     if(saux)
                     {
                        saux->h4 += 1;
                        saux = mapa->quadradoRelativo(qx+1,qz+1);
                        if(saux)
                            saux->h1 += 1;
                     }
                  }
                  else if( (d4<=d1) && (d4<=d2) && (d4<=d3) )
                  {
                     saux->h4 += 1;
                     saux = mapa->quadradoRelativo(qx+1,qz);
                     if(saux)
                       saux->h1 += 1;
                     saux = mapa->quadradoRelativo(qx,qz-1);
                     if(saux)
                     {
                        saux->h3 += 1;
                        saux = mapa->quadradoRelativo(qx+1,qz-1);
                        if(saux)
                            saux->h2 += 1;
                     }
                  }
               }
            } 
            else if(estado == CHAO)
               colocaTextura(mapa, (int)xReal / SQUARESIZE, 
                             (int)zReal / SQUARESIZE, texturaAtual);
            else if(estado == OBJETO)
            {
               inserirObjetoMapa(xReal, zReal, orObj, objAtual, qx, qz);
            }
            else if( (estado == MUROX) || (estado == MUROZ) || 
                     (estado == MEIOX) || (estado == MEIOZ) )
            {
                /* Compara com Muros existentes, para "ligar"/"colar" */
                muro* m;
                if( (estado == MUROX) || (estado == MUROZ) )
                {
                   m  = mapa->muros;
                }
                else
                {
                   m = mapa->meiosFio;
                }   
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
                maux->dX = 16; maux->dY = 16; maux->dZ = 16;
                if ( (estado == MUROX) || (estado == MUROZ))
                {
                    maux->proximo = mapa->muros;
                    mapa->muros = maux;
                }
                else
                {
                    maux->proximo = mapa->meiosFio;
                    mapa->meiosFio = maux;
                }
                maux->x1 = xReal/*qx*SQUARESIZE*/;
                //maux->x2 = xReal;//(qx)*SQUARESIZE;
                maux->z1 = zReal;
                //maux->z2 = zReal+10;//(qz)*SQUARESIZE;
                maux->textura = texturaAtual;
                if( (estado == MUROX) || (estado == MEIOX))
                {
                   maux->x2 = xReal;
                   if(estado == MEIOX)
                   {
                     maux->z2 = zReal+2.5;  
                     estado = MEIOXINIC;
                   }
                   else
                   {
                     maux->z2 = zReal+10;
                     estado = MUROXINIC;
                   }
                }
                else
                {
                   maux->z2 = zReal;
                   if (estado == MEIOZ)
                   {
                      maux->x2 = xReal+2.5;
                      estado = MEIOZINIC;
                   }
                   else
                   {
                      maux->x2 = xReal+10; 
                      estado = MUROZINIC;
                   }
                }
            }
            else if ( (estado == MUROXINIC) || (estado == MUROZINIC) ||
                      (estado == MEIOXINIC) || (estado == MEIOZINIC) )
            {
                if( (estado == MUROXINIC) || (estado == MEIOXINIC))
                   maux->x2 = xReal;//(qx)*SQUARESIZE;
                else
                   maux->z2 = zReal;//(qz)*SQUARESIZE;
            }
            else if( (estado == MUROXINICQUAD) || (estado == MEIOXINICQUAD))
            {
                maux->x2 = (qx)*SQUARESIZE;
            }
            else if( (estado == MUROZINICQUAD) || (estado == MEIOZINICQUAD) )
            {
               maux->z2 = (qz)*SQUARESIZE;
            }
         }
         else
         {
             if(estado == PORTA)
             {
                //Pega Muro Mais Proximo
                muro* m = mapa->muros;
                muroPorta = m;

                while( m != NULL )
                {
                   if( estaDentro(xReal,zReal,m->x1,m->z1,m->x2,m->z2) ||
                       estaDentro(xReal-2,zReal,m->x1,m->z1,m->x2,m->z2) ||
                       estaDentro(xReal+2,zReal,m->x1,m->z1,m->x2,m->z2) ||
                       estaDentro(xReal,zReal-2,m->x1,m->z1,m->x2,m->z2) ||
                       estaDentro(xReal-2,zReal-2,m->x1,m->z1,m->x2,m->z2) ||
                       estaDentro(xReal+2,zReal-2,m->x1,m->z1,m->x2,m->z2) ||
                      estaDentro(xReal,zReal+2,m->x1,m->z1,m->x2,m->z2) ||
                       estaDentro(xReal-2,zReal+2,m->x1,m->z1,m->x2,m->z2) ||
                       estaDentro(xReal+2,zReal+2,m->x1,m->z1,m->x2,m->z2))
                   {
                      muroPorta = m;
                   }

                   m = m->proximo;
                }
                //Coloca X ou Z da porta fixo nele
                if( (muroPorta->x2 - muroPorta->x1) == 10)
                {
                   if(modoPorta)
                   {
                      xPorta = muroPorta->x2;
                   }
                   else
                   {
                      xPorta = muroPorta->x1;
                   }
                   orPorta = 90;  
                   if(zReal > muroPorta->z2)
                   {
                      zPorta = muroPorta->z2; 
                   }
                   else if(zReal < muroPorta->z1)
                   {
                      zPorta = muroPorta->z1;
                   } 
                   else
                   {
                      zPorta = zReal; 
                   }
                }
                else
                {
                   if(modoPorta)
                   {
                      zPorta = muroPorta->z2;
                   }
                   else
                   {
                      zPorta = muroPorta->z1;
                   }
                   orPorta = 0;
                   if(xReal > muroPorta->x2)
                   {
                      xPorta = muroPorta->x2; 
                   }
                   else if(xReal < muroPorta->x1)
                   {
                      xPorta = muroPorta->x1;
                   } 
                   else
                   {
                      xPorta = xReal; 
                   }
                }
             }
             else if(estado == PORTALINIC)
             {
                 portalX[1] = xReal;
                 portalZ[1] = zReal;
                 inserirPortal(portalX, portalZ, qx, qz); 
                 estado = PORTAL;
             }
             else
             if((estado == MUROXINIC) || (estado == MUROZINIC) || 
                (estado == MUROXINICQUAD) || (estado == MUROZINICQUAD) ||
                (estado == MEIOXINIC) || (estado == MEIOZINIC) || 
                (estado == MEIOXINICQUAD) || (estado == MEIOZINICQUAD))
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
                printf("Added Wall: %f,%f,%f,%f\n",maux->x1,
                         maux->z1, maux->x2, maux->z2);
                
                if( (estado == MUROXINIC)||(estado == MUROXINICQUAD))
                   estado = MUROX;
                else if ( (estado == MUROZINIC)||(estado == MUROZINICQUAD))
                   estado = MUROZ;
                else if( (estado == MEIOXINIC)||(estado == MEIOXINICQUAD))
                   estado = MEIOX;
                else if ( (estado == MEIOZINIC)||(estado == MEIOZINICQUAD))
                   estado = MEIOZ;

             }
         }
  
         if(Mbotao & SDL_BUTTON(3))
         {
            if(estado == MODALTURA)
            {
               Square* saux = mapa->quadradoRelativo(qx,qz);

               if(saux)
               {
 
               GLfloat d1,d2,d3,d4; //distancia ao vertice do quadrado

               GLfloat dx1 = xReal - saux->x1;
               dx1 *= dx1;
               GLfloat dz1 = zReal - saux->z1;
               dz1 *= dz1;
               GLfloat dx2 = xReal - saux->x2;
               dx2 *= dx2;
               GLfloat dz2 = zReal - saux->z2;
               dz2 *= dz2;

               d1 = (sqrt( dx1 + dz1 ));

               d2 = (sqrt( dx1 + dz2 ));

               d3 = (sqrt( dx2 + dz2 ));

               d4 = (sqrt( dx2 + dz1 ));
               


               if( (d1<=d2) && (d1<=d3) && (d1<=d4) )
               {
                  if(saux->h1 >= 1) saux->h1 -= 1;
                  saux = mapa->quadradoRelativo(qx-1,qz);
                  if( (saux) && (saux->h4 >= 1))
                  {
                     saux->h4 -= 1;
                  }
                  saux = mapa->quadradoRelativo(qx,qz-1);
                  if(saux)
                  {
                     if(saux->h2>=1) saux->h2 -= 1;
                     saux = mapa->quadradoRelativo(qx-1,qz-1);
                     if( (saux) && (saux->h3>=1))
                       saux->h3 -= 1;
                  }
               }
               else if( (d2<=d1) && (d2<=d3) && (d2<=d4) )
               {
                  if(saux->h2>=1)saux->h2 -= 1;
                  saux = mapa->quadradoRelativo(qx-1,qz);
                  if((saux) &&(saux->h3 >=1))
                    saux->h3 -= 1;
                  saux = mapa->quadradoRelativo(qx,qz+1);
                  if(saux)
                  { 
                     if(saux->h1>=1) saux->h1 -= 1;
                     saux = mapa->quadradoRelativo(qx-1,qz+1);
                     if( (saux) && (saux->h4>=1))
                       saux->h4 -= 1;
                  } 
               } 
               else if( (d3<=d1) && (d3<=d2) && (d1<=d4) )
               {
                  if(saux->h3>=1) saux->h3 -= 1;
                  saux = mapa->quadradoRelativo(qx+1,qz);
                  if( (saux) && (saux->h2>=1))
                    saux->h2 -= 1;
                  saux = mapa->quadradoRelativo(qx,qz+1);
                  if(saux)
                  {
                     if(saux->h4>=1) saux->h4 -= 1;
                     saux = mapa->quadradoRelativo(qx+1,qz+1);
                     if( (saux) && (saux->h1>=1))
                         saux->h1 -= 1;
                  }
               }
               else if( (d4<=d1) && (d4<=d2) && (d4<=d3) )
               {
                  if(saux->h4>=1)saux->h4 -= 1;
                  saux = mapa->quadradoRelativo(qx+1,qz);
                  if((saux) && (saux->h1 >= 1))
                    saux->h1 -= 1;
                  saux = mapa->quadradoRelativo(qx,qz-1);
                  if(saux)
                  {
                     if(saux->h3>=1) saux->h3 -= 1;
                     saux = mapa->quadradoRelativo(qx+1,qz-1);
                     if((saux) && (saux->h2>=1))
                         saux->h2 -= 1;
                  }
               }
            }
            }
         }
 
         sair = teclas[SDLK_ESCAPE];
         if(teclas[SDLK_b])
         {
             if(estado == PORTA)
             {
                modoPorta = !modoPorta;
                SDL_Delay(150);
             }
             else if( (estado == MUROXINIC) || (estado == MEIOXINIC) )
             {
                 float cmp = ((int)(maux->z1) / SQUARESIZE)*SQUARESIZE;
                 maux->x1 = ((int)round((maux->x1) / SQUARESIZE))*SQUARESIZE;
                 maux->x2 = ((int)round((maux->x2) / SQUARESIZE))*SQUARESIZE;
                 maux->z1 = ((int)round((maux->z1 / SQUARESIZE)))*SQUARESIZE;
                 if(cmp < maux->z1)
                 {
                       if(estado == MUROXINIC)
                          maux->z1 = maux->z1-10;
                       else
                          maux->z1 = maux->z1-2.5;
                 }
                 if(estado == MUROXINIC)
                 {
                    maux->z2 = maux->z1+10;
                    estado = MUROXINICQUAD;
                 }
                 else
                 {
                    maux->z2 = maux->z1+2.5;
                    estado = MEIOXINICQUAD;
                 }
             }
             else if( (estado == MUROZINIC) || (estado == MEIOZINIC) )
             {
                 float cmp = ((int)(maux->x1) / SQUARESIZE)*SQUARESIZE;
                 maux->z1 = ((int)round((maux->z1) / SQUARESIZE))*SQUARESIZE;
                 maux->z2 = ((int)round((maux->z2) / SQUARESIZE))*SQUARESIZE;
                 maux->x1 = ((int)round((maux->x1 / SQUARESIZE)))*SQUARESIZE;
                 if(cmp < maux->x1)
                 {
                       if(estado == MUROZINIC)
                          maux->x1 = maux->x1-10;
                       else
                          maux->x1 = maux->x1-2.5;
                 }
                 if(estado == MUROZINIC)
                 {
                    maux->x2 = maux->x1+10;
                    estado = MUROZINICQUAD;             
                 }
                 else
                 {
                    maux->x2 = maux->x1+2.5;
                    estado = MEIOZINICQUAD;
                 }
             }
         } 
         if(teclas[SDLK_UP] || teclas[SDLK_DOWN])
         {
            varX = 4.0 * sin(deg2Rad(phi));
            varZ = 4.0 * cos(deg2Rad(phi));
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
            varX = 4.0 * sin(deg2Rad(phi)+deg2Rad(90));
            varZ = 4.0 * cos(deg2Rad(phi)+deg2Rad(90));
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
         if(teclas[SDLK_PAGEDOWN]) // desce com a camera ate visao em 1� pessoa
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
         }
         if(teclas[SDLK_COMMA])
         {
            phi += 1;
         }
         if(teclas[SDLK_i])
         {
             printf("x:%.3f z:%.3f phi:%.3f theta:%.3f \n",centroX, centroZ,
                                                           phi,theta);
         }
         if( (teclas[SDLK_k]) & (estado == OBJETO))
         {
             orObj += 1;
             orObj %= 360;
         }
         if((teclas[SDLK_l]) && (estado == OBJETO) )
         {
            orObj -= 1;
            orObj %= 360;
         }

         if( (teclas[SDLK_x]) && (maux) )
         {
            if(teclas[SDLK_LSHIFT])
              maux->dX--;
            else
              maux->dX++;
         }
         if( (teclas[SDLK_y]) && (maux) )
         {
            if(teclas[SDLK_LSHIFT])
              maux->dY--;
            else
              maux->dY++;
         }
         if( (teclas[SDLK_z]) && (maux) )
         {
            if(teclas[SDLK_LSHIFT])
              maux->dZ--;
            else
              maux->dZ++;
         }

         if(centroX > ((mapa->x * SQUARESIZE)+20))
         {
            centroX = ((mapa->x * SQUARESIZE)+20);
         }
         else if(centroX < -20)
         {
            centroX = -20;
         }

         if(centroZ > ((mapa->z * SQUARESIZE)+20))
         {
            centroZ = ((mapa->z * SQUARESIZE)+20);
         }
         else if(centroZ < -20)
         {
            centroZ = -20;
         }



      }
      glClearColor(0,0,0,1);
      glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

      glLoadIdentity();
      /* Redefine a posicao dinamica da camera */
      cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
      cameraY = centroY + (float) d * sin(deg2Rad(theta));
      cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
      gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);
      AtualizaFrustum(matrizVisivel,proj,modl);

      glPushMatrix();
      if( (estado == PORTALINIC) )
      {
         glBegin(GL_QUADS);
         glVertex3f(portalX[0],1.0,portalZ[0]);
         glVertex3f(portalX[0],1.0,portalZ[1]);
         glVertex3f(portalX[1],1.0,portalZ[1]);
         glVertex3f(portalX[1],1.0,portalZ[0]);
         glEnd();
      }
      glPopMatrix();
      
      glPushMatrix();
         mapa->draw(cameraX,cameraY,cameraZ,matrizVisivel);
      glPopMatrix();
      
      glPushMatrix();
      glDisable(GL_LIGHTING);
      gui->draw(proj,modl,viewPort);
      glEnable(GL_LIGHTING);
      glPopMatrix();

      glPushMatrix();
      if( (estado == OBJETO) && (objAtual))
         objAtual->Desenhar(xReal, zReal, 0, orObj);
      glPopMatrix();
 
      glPushMatrix();
      int delta = -2;
      if( (estado == PORTA) && (porta))
      {
         if(modoPorta) delta = 2;
         if(orPorta)
         {
           porta->Desenhar(xPorta+delta, zPorta, 0, orPorta);
         }
         else
         {
           porta->Desenhar(xPorta, zPorta+delta, 0, orPorta);
         }
         glBegin(GL_QUADS);
            glVertex3f(xReal-2,1,zReal-2);
            glVertex3f(xReal-2,1,zReal+2);
            glVertex3f(xReal+2,1,zReal+2);
            glVertex3f(xReal+2,1,zReal-2);
         glEnd();
      }
      glPopMatrix();


      glFlush();
      SDL_GL_SwapBuffers();

   }
 
   delete(mapa);
   delete(gui);

}

