/* DCC Nightmare is Public Domain - Do whatever you want with this code.
 */

#include "map.h"
#include "../engine/culling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>



/********************************************************************
 *                      Construtor do Quadrado                      *
 ********************************************************************/
Square::Square()
{
	floor_texture_fname = NULL;
	flags = 0;
        h1 = h2 = h3 = h4 = 0;
        int aux;
        for(aux=0;aux<MAXOBJETOS;aux++)
        {
           objetos[aux] = NULL;
           objetosDesenha[aux] = 0;
           quadObjetos[aux] = NULL;
        }
        for(aux=0;aux<MAXMUROS;aux++)
           muros[aux] = NULL;
	return;
}

/********************************************************************
 *                       Destruidor do Quadrado                     *
 ********************************************************************/
Square::~Square()
{
	if( floor_texture_fname == NULL )
		free( floor_texture_fname );
	return;
}

/********************************************************************
 *                      Retorna o indice da Textura                 *
 ********************************************************************/
int IDTextura(Map* mapa, char* textura, GLuint* R, GLuint* G, GLuint* B)
{
   /* procura pela textura */
   int aux=0;
   texture* tex = mapa->Texturas;
   while(aux < mapa->numtexturas)
   {
      if(!strcmp(tex->nome, textura))
      {
         *R = tex->R; *G = tex->G; *B = tex->B;
         return(tex->indice); //a textura ja esta presente 
      }
      tex = tex->proximo;
      aux++;
   }
   return(-1);
}

/*********************************************************************
 *                   Retorna o nome da textura                       *
 *********************************************************************/
char* NomeTextura(Map* mapa, GLuint ID)
{
   int aux=0;
   texture* tex = mapa->Texturas;
   while(aux < mapa->numtexturas)
   {
      if(tex->indice == ID)
      {
         return(tex->nome);
      }
      tex = tex->proximo;
      aux++;
   }
   return(NULL);
}
/********************************************************************
 *         Insere a textura dentre as utilizadas pelo objeto        *
 ********************************************************************/
GLuint InserirTextura(Map* mapa, char* arq, char* nome, 
                    GLuint R, GLuint G, GLuint B)
{
   texture* tex;

   SDL_Surface* img = IMG_Load(arq);
   if(!img)
   {
      printf("Error Opening Texture: %s\n",arq);
      return(0);
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

/********************************************************************
 *                       Desenha o Quadrado                         *
 ********************************************************************/
int Square::draw( GLfloat x, GLfloat z )
{
#ifdef DEBUG_MAP
	printf("map.cpp - Square::draw(): Drawing square in (%f,%f)\n", x, z);
#endif
//	glEnable(GL_TEXTURE_2D);
	//glColor3f( 1.0, 1.0, 20.0 );
	//glBegin(GL_POLYGON);
	
//	glNormal3i( 0, 0, 1 );
	//glEnd();
	return(0);
}

/********************************************************************
 *                           Desenha o Mapa                         *
 ********************************************************************/
int Map::draw(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, GLfloat matriz[6][4])
{
        int textura = -1;
        int i, Xaux = 0, Zaux = 0;
        //if(aux) {
           textura = MapSquares[Xaux][Zaux]->textura;
           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, textura);
        //}
#ifdef DEBUG_MAP
	printf("map.cpp - Map::draw(): Drawing map %s.\n", name);
#endif
        GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 16.0784313725);
        
        glBegin(GL_QUADS);
        glNormal3i(0,1,0);
        //glColor3fv(mat_ambient);
        for(Xaux = 0; Xaux < x; Xaux++)
        {
           for(Zaux = 0; Zaux < z; Zaux++)
           {
             if((textura!= -1) && (MapSquares[Xaux][Zaux]->textura == -1))
             {
                 glDisable(GL_TEXTURE_2D); 
                 textura = -1;
             }
             else if(textura != MapSquares[Xaux][Zaux]->textura)
             {
                glEnd();
                textura = MapSquares[Xaux][Zaux]->textura;
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textura);
                glBegin(GL_QUADS);
             }
             if( (MapSquares[Xaux][Zaux]->visivel) || 
                 (quadradoVisivel(MapSquares[Xaux][Zaux]->x1,0,
                                  MapSquares[Xaux][Zaux]->z1,
                                  MapSquares[Xaux][Zaux]->x2,
                                  ALTURAMAXIMA,
                                  MapSquares[Xaux][Zaux]->z2, matriz)))
             {
                for(i=0;i<MAXOBJETOS;i++)
                {
                   if(MapSquares[Xaux][Zaux]->quadObjetos[i] != NULL)
                   { 
                     MapSquares[Xaux][Zaux]->quadObjetos[i]->visivel = 1;
                   }
                }
                MapSquares[Xaux][Zaux]->visivel = 1;
                glTexCoord2f(0,0); 
                glVertex3f( MapSquares[Xaux][Zaux]->x1 , 
                            MapSquares[Xaux][Zaux]->h1 , 
                            MapSquares[Xaux][Zaux]->z1 );
                glTexCoord2f(0,1);
                glVertex3f( MapSquares[Xaux][Zaux]->x1 , 
                            MapSquares[Xaux][Zaux]->h2 , 
                            MapSquares[Xaux][Zaux]->z2);
                glTexCoord2f(1,1);
                glVertex3f( MapSquares[Xaux][Zaux]->x2, 
                            MapSquares[Xaux][Zaux]->h3 , 
                            MapSquares[Xaux][Zaux]->z2 );
                glTexCoord2f(1,0);
                glVertex3f( MapSquares[Xaux][Zaux]->x2, 
                            MapSquares[Xaux][Zaux]->h4, 
                            MapSquares[Xaux][Zaux]->z1 );
             }
             else
             {
               MapSquares[Xaux][Zaux]->visivel = 0;
             }
           }
        }
        glEnd();

        /* Faz o desenho dos muros e meios Fio*/
        muro* maux = muros;
        int fezMeioFio = 0;
        GLfloat altura = MUROALTURA;
        glBegin(GL_QUADS);
        while( (maux != NULL) )
        {
           
           //if(x1 == x2) x2 += SQUARESIZE; 
           //if(z1 == z2) z2 += SQUARESIZE;
           if((textura!= -1) && (maux->textura == -1))
           {
               glDisable(GL_TEXTURE_2D); 
               textura = -1;
           }
           else if(textura != maux->textura)
           {
              glEnd();
              textura = maux->textura;
              glEnable(GL_TEXTURE_2D);
              glBindTexture(GL_TEXTURE_2D, textura);
              glBegin(GL_QUADS);
           }
           if(quadradoVisivel(maux->x1,0,maux->z1,maux->x2,
                              altura,maux->z2,matriz))
           {
           /* Face de frente */
              glNormal3i(0,0,1);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,altura,maux->z1);
              glTexCoord2f(1,0);
              glVertex3f(maux->x2,altura,maux->z1);
              glTexCoord2f(1,1);
              glVertex3f(maux->x2,0,maux->z1);
              glTexCoord2f(0,1);
              glVertex3f(maux->x1,0,maux->z1);
           /* Face de tras */
              glNormal3i(0,0,-1);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,altura,maux->z2);
              glTexCoord2f(1,0);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(1,1);
              glVertex3f(maux->x2,0,maux->z2);
              glTexCoord2f(0,1);
              glVertex3f(maux->x1,0,maux->z2);
           /* Face de esquerda */
              glNormal3i(-1,0,0);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,altura,maux->z1);
              glTexCoord2f(1,0);
              glVertex3f(maux->x1,altura,maux->z2);
              glTexCoord2f(1,1);
              glVertex3f(maux->x1,0,maux->z2);
              glTexCoord2f(0,1);
              glVertex3f(maux->x1,0,maux->z1);
           /* Face de direita */
              glNormal3i(1,0,0);
              glTexCoord2f(0,0);
              glVertex3f(maux->x2,altura,maux->z1);
              glTexCoord2f(1,0);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(1,1);
              glVertex3f(maux->x2,0,maux->z2);
              glTexCoord2f(0,1);
              glVertex3f(maux->x2,0,maux->z1);
           /* Face de cima */
              glNormal3i(0,1,0);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,altura,maux->z1);
              glTexCoord2f(1,0);
              glVertex3f(maux->x2,altura,maux->z1);
              glTexCoord2f(1,1);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(0,1);
              glVertex3f(maux->x1,altura,maux->z2);
           }
           maux = maux->proximo;
           if( (!maux) && (!fezMeioFio) )
           {
               maux = meiosFio;
               fezMeioFio = 1;
               altura = MEIOFIOALTURA;
           }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);


        /* Faz o desenho dos objetos */
        int o;
        GLfloat distancia;
        GLfloat deltaX, deltaZ;
        GLfloat deltaY2 = cameraY*cameraY;

        for(Xaux = 0; Xaux < x; Xaux++)
        for(Zaux = 0; Zaux < z; Zaux++)
        {
           if(MapSquares[Xaux][Zaux]->visivel){
           deltaX = (cameraX-MapSquares[Xaux][Zaux]->x1+HALFSQUARESIZE);
           deltaZ = (cameraZ-MapSquares[Xaux][Zaux]->z1+HALFSQUARESIZE);
           distancia = sqrt(deltaX*deltaX+deltaY2+deltaZ*deltaZ) 
                                                          / SQUARESIZE;
           for(o=0;o<MAXOBJETOS;o++)
           {
              if( (MapSquares[Xaux][Zaux]->objetos[o] != NULL) && 
                  (MapSquares[Xaux][Zaux]->objetosDesenha[o] == 1) )
              {
                  MapSquares[Xaux][Zaux]->objetos[o]->Desenhar(
                          MapSquares[Xaux][Zaux]->Xobjetos[o],
                          MapSquares[Xaux][Zaux]->Zobjetos[o],
                          distancia,
                          MapSquares[Xaux][Zaux]->orientacaoObjetos[o]);
              }
           }
           }
           MapSquares[Xaux][Zaux]->visivel = 0;
        }
        return(0);
}

/********************************************************************
 *                     Construtor dos Mapas                         *
 ********************************************************************/
Map::Map()
{
	numtexturas = 0;
        Texturas = NULL;
	//first = NULL;
	name = NULL;
        squareInic = NULL;
        muros = NULL;
        meiosFio = NULL; 
        MapSquares = NULL;
        /* Inicia Estruturas */
        Objetos = new(LmapObjeto);
        x = z = xInic = zInic = 0;
}


Square* Map::quadradoRelativo(int xa, int za)
{
   //printf("qrel: %d %d\n",x,z);
   if( (z <= za) || (x <= xa) || ( xa < 0) || (za < 0)) 
      return(NULL);
   /*int ax,az;
   Square* result = first;
   for(ax=0;ax<(xa-1);ax++) result = result->right;
   for(az=0;az<(za-1);az++) result = result->down;*/
   return(MapSquares[xa][za]);
}

/********************************************************************
 *                       Abre o Arquivo de Mapas                    *
 ********************************************************************/
int Map::open(char* arquivo)
{
   FILE* arq;        // arquivo utilizado para o mapa
   char buffer[128]; // buffer utilziado para ler
   char nomeArq[128], nome[128];
   int i;
   //Square* aux;
   //Square* ant = NULL, *primLinha=NULL;
   int posX,posZ;    //posicao atual do quadrado anterior relativo
   int IDtexturaAtual = -1;
   //char* nomeTexturaAtual = "nada\0";
   int IDmuroTexturaAtual = -1;
   char* nomeMuroTexturaAtual = "nada\0";
   int numObjetosAtual = 0;
   int pisavel=0;
   GLuint R,G,B;
   GLuint Ratual,Gatual,Batual;
   
   if(!(arq = fopen(arquivo,"r")))
   {
      printf("Error while opening map file: %s\n",arquivo);
	return(0);
   }

   /* Define o nome do mapa */
   name = arquivo;

   /* Faz a leitura do tamanho do mapa */   
   fscanf(arq, "%s", buffer);
   if(buffer[0] == 'T')
   {
      fgets(buffer, sizeof(buffer), arq); // lê até o final da linha 
      sscanf(buffer,"%dX%d",&x,&z); // lê as dimensoes
   }
   else
   {
      printf("Map Size not defined: %s\n", arquivo); 
      printf(" Found %s, where espected T iXi\n",buffer);
      fclose(arq);
      return(0);
   }

   /* Alloc MapSquares */
   MapSquares = (Square***) malloc(x*sizeof(Square*));
   for(i = 0; i < x; i++)
   {
      MapSquares[i] = (Square**) malloc(z*sizeof(Square));
   } 

   for(posX = 0; posX < x; posX++)
   {
      for(posZ = 0; posZ < z; posZ++)
      {
         MapSquares[posX][posZ] = new(Square);
      }
   }

   muro* maux = NULL;

   posX = -1;
   posZ = 0;

   /* Vamos ler todo o arquivo */
   while(fscanf(arq, "%s", buffer) != EOF)
   {
      switch(buffer[0])
      {
         case 'm': /* Define Muros (paredes) e Meios-Fio */
         {
            switch(buffer[1])
            {
               case 'u': /* Define o muro */
               {
                  maux = new(muro);
                  fgets(buffer, sizeof(buffer),arq);
                  sscanf(buffer,"%f,%f,%f,%f",&maux->x1,&maux->z1,
                                              &maux->x2,&maux->z2);
                  double tmp;
                  if(maux->x2 < maux->x1)
                  {
                     tmp = maux->x2;
                     maux->x2 = maux->x1;
                     maux->x1 = tmp;
                  }
                  if(maux->z2 < maux->z1)
                  {
                     tmp = maux->z2;
                     maux->z2 = maux->z1;
                     maux->z1 = tmp;
                  }  
                  maux->proximo = muros;
                  maux->textura = -1;
                  muros = maux;
                  break;
               }
               case 't': /* Define a textura do muro ou meio-Fio */
               {
                  fgets(buffer, sizeof(buffer), arq);
                  sscanf(buffer,"%s",nome);
                  if(strcmp(nomeArq,nomeMuroTexturaAtual))
                  {
                     nomeMuroTexturaAtual = nome;
                     IDmuroTexturaAtual = IDTextura(this,nome,&R,&G,&B);
                  }
                  maux->textura = IDmuroTexturaAtual;
                  break;
               }
               case 'e': /* Define MeioFio */
               {
                  maux = new(muro);
                  fgets(buffer, sizeof(buffer),arq);
                  sscanf(buffer,"%f,%f,%f,%f",&maux->x1,&maux->z1,
                                              &maux->x2,&maux->z2);

                  double tmp;
                  if(maux->x2 < maux->x1)
                  {
                     tmp = maux->x2;
                     maux->x2 = maux->x1;
                     maux->x1 = tmp;
                  }
                  if(maux->z2 < maux->z1)
                  {
                     tmp = maux->z2;
                     maux->z2 = maux->z1;
                     maux->z1 = tmp;
                  }  
                  maux->proximo = meiosFio;
                  maux->textura = -1;
                  meiosFio = maux;
                  break;
               }
            }
            break;
         }
         case 'i':/* Define a posição Inicial */
         {
            fgets(buffer, sizeof(buffer), arq); //até final da linha
            sscanf(buffer, "%d,%d",&xInic,&zInic);
            squareInic = MapSquares[xInic][zInic];
            xInic = (xInic)*SQUARESIZE + HALFSQUARESIZE;
            zInic = (zInic)*SQUARESIZE + HALFSQUARESIZE; 
            break;
         }
         case 'o': /* Insere Objetos no Mapa */
         {
             fgets(buffer, sizeof(buffer), arq); //até final da linha
             sscanf(buffer, "%s %s",nome,nomeArq);
             Objetos->InserirMapObjeto(nomeArq,nome);
             break;
         }
         case 't': /* Insere Texturas no Mapa */
         {
            fgets(buffer, sizeof(buffer), arq); //até final da linha
            sscanf(buffer, "%s %s %d %d %d",nome,nomeArq,&R,&G,&B);  
            InserirTextura(this,nomeArq, nome,R,G,B);
            break;
         }
         case 'p': /* Inserção de um novo quadrado */
         {
            numObjetosAtual = 0;
            if(posX == (x-1)) //fim da linha de quadrados
            { 
                posX = 0;
                posZ++;
            }
            else  //continua na mesma linha
            {
               posX++;
            }
            fgets(buffer, sizeof(buffer), arq); //até final da linha
            sscanf(buffer, "%d,%f,%f,%f,%f",&pisavel,
                                 &MapSquares[posX][posZ]->h1,
                                 &MapSquares[posX][posZ]->h2,
                                 &MapSquares[posX][posZ]->h3,
                                 &MapSquares[posX][posZ]->h4);
            MapSquares[posX][posZ]->x1 = (posX) * SQUARESIZE;
            MapSquares[posX][posZ]->x2 = MapSquares[posX][posZ]->x1 + SQUARESIZE;
            MapSquares[posX][posZ]->z1 = (posZ) * SQUARESIZE;
            MapSquares[posX][posZ]->z2 = MapSquares[posX][posZ]->z1 + SQUARESIZE; 
            MapSquares[posX][posZ]->posX = posX;
            MapSquares[posX][posZ]->posZ = posZ;
            if(pisavel) 
            {
               MapSquares[posX][posZ]->flags |= PISAVEL;
            }
            break;
         }
         case 'u':/* Utilização de Algo existente */
         {
            switch(buffer[1])
            {
               case 't': /* Define a Textura do Quadrado */
               {
                  fgets(buffer, sizeof(buffer), arq);
                  sscanf(buffer,"%s",nome);
                  IDtexturaAtual = IDTextura(this,nome,&Ratual,&Gatual,&Batual);
                  MapSquares[posX][posZ]->textura = IDtexturaAtual;
                  MapSquares[posX][posZ]->R = Ratual;
                  MapSquares[posX][posZ]->G = Gatual;
                  MapSquares[posX][posZ]->B = Batual;
                  break;
               }
               case 'o': /* Insere Objeto no Quadrado */
               {
                  if(numObjetosAtual >= MAXOBJETOS)
                  {
                     printf("Overflow of objects per Square\n");
                  }
                  else
                  {
                     fgets(buffer, sizeof(buffer), arq);
                     sscanf(buffer,"%s %d:%d,%d:%f,%f:%d",nome,
                       &MapSquares[posX][posZ]->objetosDesenha[numObjetosAtual],
                       &MapSquares[posX][posZ]->quadXobjetos[numObjetosAtual],
                       &MapSquares[posX][posZ]->quadZobjetos[numObjetosAtual],
                       &MapSquares[posX][posZ]->Xobjetos[numObjetosAtual],
                       &MapSquares[posX][posZ]->Zobjetos[numObjetosAtual],
                       &MapSquares[posX][posZ]->orientacaoObjetos[numObjetosAtual]);
                     MapSquares[posX][posZ]->objetos[numObjetosAtual] = 
                                                    Objetos->EndMapObjeto(nome);
                     MapSquares[posX][posZ]->quadObjetos[i] = quadradoRelativo(MapSquares[posX][posZ]->quadXobjetos[numObjetosAtual], MapSquares[posX][posZ]->quadZobjetos[numObjetosAtual] );
                     numObjetosAtual++;
                  }
                  break;
               }
               default:
                       printf("What the Hell: %s on %s\n",buffer,arquivo);
                       break;
            }
            break; 
         }
         case '#': //ignora o comentario
         {
             fgets(buffer, sizeof(buffer), arq);
             break;
         }
         default:
                 printf("What is: %s on %s ?\n",buffer,arquivo);
                 break;
      }
   }
   fclose(arq);

   int ax,az;
   /* Agora atualiza os ponteiros dos Muros */
   maux = muros;
   int inix,iniz,maxx,maxz;
   int indiceMuro;
   Square* aux;
   
   while(maux)
   {
      inix = (int)floor(maux->x1 / SQUARESIZE);
      iniz = (int)floor(maux->z1 / SQUARESIZE);
      maxx = (int)floor(maux->x2 / SQUARESIZE);
      maxz = (int)floor(maux->z2 / SQUARESIZE);
      for(ax = inix;ax<=maxx;ax++)
      {
          for(az = iniz;az<=maxz;az++)
          {
              indiceMuro = 0;
              aux = quadradoRelativo(ax,az);
              while((aux!=NULL) && (indiceMuro < MAXMUROS) && 
                    (aux->muros[indiceMuro] != NULL))
              {
                 indiceMuro++;
              }
              if((aux!=NULL) && (indiceMuro < MAXMUROS))
              {
                 aux->muros[indiceMuro] = maux;
              }
              else if(indiceMuro >= MAXMUROS)
                 printf("Quad: %d %d has more walls than permitted: %d\n",ax,az,
                          indiceMuro);
          }
      }
      maux = maux->proximo;
   }
   return(1);
}


/************************************************************************
 *                     Cria um Novo Mapa                                *
 ************************************************************************/
void Map::newMap(int X, int Z)
{
   int auxX, auxZ;
   int i;

   printf("Beginning a new Map: %d,%d\n",X,Z);

   /* Alloc MapSquares */
   MapSquares = (Square***) malloc(X*sizeof(Square*));
   for(i = 0; i < X; i++)
   {
      MapSquares[i] = (Square**) malloc(Z*sizeof(Square));
   } 

   for(auxX = 0; auxX < X; auxX++)
   {
      for(auxZ = 0; auxZ < Z; auxZ++)
      {
         MapSquares[auxX][auxZ] = new(Square);
      }
   }
   
   Square* saux;
   x = X;
   z = Z;
   int IDtextura = InserirTextura(this,
                                  "../data/texturas/chao_grama2.jpg", 
                                  "chao_grama2",20,20,100);

   for(auxZ = 0; auxZ < z; auxZ++)
   {
      for(auxX = 0; auxX < x; auxX++)
      {
          saux = quadradoRelativo(auxX,auxZ);
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
      }
   }

   xInic = 1*SQUARESIZE;
   zInic = 1*SQUARESIZE;
   squareInic = quadradoRelativo(0,0);
}


/*******************************************************************
 *   Otimiza o mapa (sobreposicao de muros, quadrados ocp por obj) *
 *******************************************************************/
void Map::optimize()
{
    /* Verifica Sobreposicao de Muros */
    muro* maux = muros;
    while(maux != NULL)
    {
        muro* maux2 = muros;
        while(maux2 !=NULL)
        {
            if(maux != maux2)
            {
                if( (maux->x1 == maux2->x1) && 
                    ( (maux->z1 >= maux2->z1) && (maux->z2 <= maux2->z2) ||
                      (maux->z1 <= maux2->z1) && (maux->z2 >= maux2->z2) ))
                {
                    if(maux->x2 != maux->x1+10)
                    {
                       maux->x1 += 10;
                    }
                    else if(maux2->x2 != maux2->x1+10)
                    {
                       maux2->x1+= 10;
                    }
                    else
                    {
                       //deleta maux(2)
                    }
                }
                else if( (maux->x2 == maux2->x2) && 
                    ( (maux->z1 >= maux2->z1) && (maux->z2 <= maux2->z2) ||
                      (maux->z1 <= maux2->z1) && (maux->z2 >= maux2->z2) ))
                {
                    if(maux->x2 != maux->x1+10)
                    {
                       maux->x2 -= 10;
                    }
                    else if(maux2->x2 != maux2->x1+10)
                    {
                       maux2->x2 -= 10;
                    }
                    else
                    {
                       //deleta maux(2)
                    }
                }
                else if( (maux->z1 == maux2->z1) && 
                    ( (maux->x1 >= maux2->x1) && (maux->x2 <= maux2->x2) ||
                      (maux->x1 <= maux2->x1) && (maux->x2 >= maux2->x2) ))
                {
                    if(maux->z2 != maux->z1+10)
                    {
                       maux->z1 += 10;
                    }
                    else if(maux2->z2 != maux2->z1+10)
                    {
                       maux2->z1+= 10;
                    }
                    else
                    {
                       //deleta maux(2)
                    }
                }
                else if( (maux->z2 == maux2->z2) && 
                    ( (maux->x1 >= maux2->x1) && (maux->x2 <= maux2->x2) ||
                      (maux->x1 <= maux2->x1) && (maux->x2 >= maux2->x2) ))
                {
                    if(maux->z2 != maux->z1+10)
                    {
                       maux->z2 -= 10;
                    }
                    else if(maux2->z2 != maux2->z1+10)
                    {
                       maux2->z2 -= 10;
                    }
                    else
                    {
                       //deleta maux(2)
                    }
                }
            }
            maux2 = maux2->proximo;
        }
        maux = maux->proximo;
    }

   /* Verifica quadrados ocupados pelos Objetos */
}


/********************************************************************
 *                      Salva o Arquivo de Mapas                    *
 ********************************************************************/
int Map::save(char* arquivo)
{
   optimize();
   FILE* arq;

   if(!(arq = fopen(arquivo,"w")))
   {
      printf("Error while creating: %s to save\n",arquivo);
	return(0);
   }
   
   /* Escreve Dimensões do Arquivo */
   fprintf(arq,"T %dX%d\n",x,z);
   fprintf(arq,"# Made by DccNiTghtmare's MapEditor, v0.0.2\n");

   /* Escreve os Objetos Utilizados */
   if(Objetos->total>0)
   {
      mapObjeto* objAux = (mapObjeto*)Objetos->primeiro->proximo;
      while(objAux != Objetos->primeiro)
      {
         fprintf(arq,"o %s %s\n",objAux->nome,objAux->nomeArq);
         objAux = (mapObjeto*)objAux->proximo;
      }
   }

   /* Escreve as Texturas Utilizadas */
   texture* tex = (texture*)Texturas;
   while(tex)
   {
      fprintf(arq,"t %s %s %d %d %d\n",tex->nome,tex->arqNome,tex->R,tex->G,tex->B);
      tex = (texture*)tex->proximo;
   }
   
   /* Escreve os Muros */
   muro* maux = (muro*)muros;
   int x1,z1,x2,z2;
   while(maux)
   {
      //x1 = maux->x1 / SQUARESIZE;
      //x2 = (maux->x2 / SQUARESIZE)-1;
      //z1 = maux->z1 / SQUARESIZE;
      //z2 = (maux->z2 / SQUARESIZE)-1;
      fprintf(arq,"muro %f,%f,%f,%f\n",maux->x1,maux->z1,maux->x2,maux->z2);
      fprintf(arq,"mt %s\n",NomeTextura(this, maux->textura));
      maux = (muro*)maux->proximo;
   }

   /* Escreve Meios Fio */
   maux = (muro*)meiosFio;
   while(maux)
   {
      fprintf(arq,"meioFio %f,%f,%f,%f\n",maux->x1,maux->z1,maux->x2,maux->z2);
      fprintf(arq,"mt %s\n",NomeTextura(this, maux->textura));
      maux = (muro*)maux->proximo;
   }

 
   /* Escreve Quadrados, linha a linha */
   //Square* saux = first;
   //Square* ultLinha;
   for(z1=0;z1<z;z1++)
   {
      //ultLinha = saux;
      for(x1=0;x1<x;x1++)
      {
          fprintf(arq,"p %d,%f,%f,%f,%f\n",
                  MapSquares[x1][z1]->flags & PISAVEL,
                  MapSquares[x1][z1]->h1,
                  MapSquares[x1][z1]->h2,
                  MapSquares[x1][z1]->h3,
                  MapSquares[x1][z1]->h4);
          fprintf(arq,"ut %s\n",NomeTextura(this, 
                                            MapSquares[x1][z1]->textura));
          int aux;
          for(aux=0;aux<MAXOBJETOS;aux++)
          {
            if(MapSquares[x1][z1]->objetos[aux])
            {
               x2 = (int)MapSquares[x1][z1]->Xobjetos[aux] / SQUARESIZE;
               z2 = (int)MapSquares[x1][z1]->Zobjetos[aux] / SQUARESIZE;
               fprintf(arq,"uo %s %d:%d,%d:%f,%f:%d\n",
                       MapSquares[x1][z1]->objetos[aux]->nome,
                       MapSquares[x1][z1]->objetosDesenha[aux],
                       x2+1,z2+1,
                       MapSquares[x1][z1]->Xobjetos[aux],
                       MapSquares[x1][z1]->Zobjetos[aux],
                       MapSquares[x1][z1]->orientacaoObjetos[aux]);
            }
          }
      }
   }

   /* Escreve Quadrado de Inicio */
   x1 = xInic / SQUARESIZE;
   z1 = zInic / SQUARESIZE;
   fprintf(arq,"i %d,%d\n",x1,z1);

   fclose(arq);
   return(1);
}

/********************************************************************
 *                       Destruidor do Mapa                         *
 ********************************************************************/
Map::~Map()
{
   /* Acabando com as texturas */
   texture* tex = Texturas;
   texture* au;
   int i;
   for(i=0;i<numtexturas;i++)
   {
      au = tex;
      tex = tex->proximo;
      free(au->nome);
      glDeleteTextures(1,&(au->indice));
      free(au);
   }
   
   /* Acabando com os muros */
   muro* m = muros;
   muro* am =NULL;
   while(m)
   {
      am = m;
      m = m->proximo;
      free(am);
   }


   /* Acabando com os objetos */
   delete(Objetos);
  
   /* Acabando com os Quadrados */
   int x1,z1;
   for(x1 = 0; x1<x; x1++)
   {
      for(z1 = 0; z1<z;z1++)
      {
         delete(MapSquares[x1][z1]);
      }
   }
   for(x1 = 0; x1<x;x1++)
   {
      free(MapSquares[x1]);
   }
   free(MapSquares);
}


/********************************************************************
 *               Desenha um  MiniMapa na Superfície                 *
 ********************************************************************/
void Map::drawMinimap(SDL_Surface* img)
{
   int x1,y1,x2,y2, X, Z;
   //printf("%d,%d %d,%d\n",img->w,img->h,x,z);
   //double razaoX = (float)img->w / (float)(x+1) ;
   //double razaoY = (float)img->h / (float)(z+1);

   x1 = 0; y1 = 0;
   for(X = 0; X < x; X++)
   {
      for(Z = 0; Z < z; Z++)
      {
          cor_Definir(MapSquares[X][Z]->R,
                      MapSquares[X][Z]->G,
                      MapSquares[X][Z]->B);
          retangulo_Colorir(img, x1, y1, x1+2, y1+2, 0);
          x1+=3;
      }
      x1 = 0;
      y1+=3;
   }

   cor_Definir(1, 1, 1);
   retangulo_2Cores(img,0,0,x*3-1,z*3-1,0,0,0,0);
   
   muro* maux = muros;
   while(maux!=NULL)
   {
       x1 = (int) ( ((float)maux->x1 / (float)SQUARESIZE /** razaoX*/ ))*3;
       x2 = (int) ( (((float)maux->x2 / (float)SQUARESIZE))-1 /** razaoX*/)*3;
       y1 = (int) ( ((float)maux->z1 / (float)SQUARESIZE /** razaoY*/ ))*3;
       y2 = (int) ( (((float)maux->z2 / (float)SQUARESIZE))-1 /** razaoY*/)*3;
       if( (x2-x1) < (y2-y1))
          x2 = x1;
       else 
          y2 = y1;
       cor_Definir(255,40,30);
       linha_Desenhar(img, x1,y1,x2,y2, 0);
       maux = maux->proximo;
   }

}

