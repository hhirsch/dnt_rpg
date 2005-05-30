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
	up = NULL;
	down = NULL;
	left = NULL;
	right = NULL;
	flags = 0;
        int aux;
        for(aux=0;aux<MAXOBJETOS;aux++)
        {
           objetos[aux] = NULL;
           objetosDesenha[aux] = 0;
           quadObjetos[aux] = NULL;
        }
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
int IDTextura(Map* mapa, char* textura)
{
   /* procura pela textura */
   int aux=0;
   texture* tex = mapa->Texturas;
   while(aux < mapa->numtexturas)
   {
      if(!strcmp(tex->nome, textura))
      {
         return(tex->indice); //a textura ja esta presente 
      }
      tex = tex->proximo;
      aux++;
   }
   return(-1);
}

/********************************************************************
 *         Insere a textura dentre as utilizadas pelo objeto        *
 ********************************************************************/
void InserirTextura(Map* mapa, char* arq, char* nome)
{
   texture* tex;

   SDL_Surface* img = IMG_Load(arq);
   if(!img)
   {
      printf("Erro ao abrir textura: %s\n",arq);
      return;
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
   strcpy(tex->nome,nome);

   SDL_Surface *imgPotencia = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   //SDL_Rect ret;
   //ret.w = 0; ret.h = 0; ret.x = img->w; ret.y = img->h;
   SDL_BlitSurface(img,NULL,imgPotencia,NULL);

   glGenTextures(1, &(tex->indice));
   glBindTexture(GL_TEXTURE_2D, tex->indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imgPotencia->w,imgPotencia->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, imgPotencia->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   mapa->numtexturas++;

   printf("Inseri Textura: %s :%d %d\n",nome,img->w,img->h);

   /* Libera a memoria utilizada */
   SDL_FreeSurface(img);
   SDL_FreeSurface(imgPotencia);
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
	Square * ref = first->down;
        Square * aux = first;
        int textura = -1;
        int i;
        if(aux) {
           textura = aux->textura;
           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, textura);
        }
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
	while( aux != NULL )
	{
             if((textura!= -1) && (aux->textura == -1))
             {
                 glDisable(GL_TEXTURE_2D); 
                 textura = -1;
             }
             else if(textura != aux->textura)
             {
                glEnd();
                textura = aux->textura;
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textura);
                glBegin(GL_QUADS);
             }
             if((aux->visivel) || (quadradoVisivel(aux->x1,0,aux->z1,aux->x2,ALTURAMAXIMA,aux->z2,matriz)))
             {
                for(i=0;i<MAXOBJETOS;i++)
                {
                   if(aux->quadObjetos[i] != NULL){ 
                       aux->quadObjetos[i]->visivel = 1;}
                }
                aux->visivel = 1;
                glTexCoord2f(0,0); 
                glVertex3f( aux->x1 , 0.0, aux->z1 );
                glTexCoord2f(0,1);
                glVertex3f( aux->x1 , 0.0, aux->z2);
                glTexCoord2f(1,1);
                glVertex3f( aux->x2, 0.0, aux->z2 );
                glTexCoord2f(1,0);
                glVertex3f( aux->x2, 0.0, aux->z1 );
             }
             else
               aux->visivel = 0;
                          
           //(*aux).draw(x,y);
           if(aux->right == NULL)   //chegou ao fim da linha
           {
              aux = ref;
              if(ref!= NULL) ref = ref->down;
           } 
           else      //senao continua na linha
           {
              aux = aux->right;
           } 
        }
        glEnd();

        /* Faz o desenho dos muros */
        muro* maux = muros;
        glBegin(GL_QUADS);
        while(maux!=NULL)
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
           if(quadradoVisivel(maux->x1,0,maux->z1,maux->x2,MUROALTURA,maux->z2,matriz))
           {
           /* Face de frente */
              glNormal3i(0,0,1);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,MUROALTURA,maux->z1);
              glTexCoord2f(1,0);
              glVertex3f(maux->x2,MUROALTURA,maux->z1);
              glTexCoord2f(1,1);
              glVertex3f(maux->x2,0,maux->z1);
              glTexCoord2f(0,1);
              glVertex3f(maux->x1,0,maux->z1);
           /* Face de tras */
              glNormal3i(0,0,-1);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,MUROALTURA,maux->z2);
              glTexCoord2f(1,0);
              glVertex3f(maux->x2,MUROALTURA,maux->z2);
              glTexCoord2f(1,1);
              glVertex3f(maux->x2,0,maux->z2);
              glTexCoord2f(0,1);
              glVertex3f(maux->x1,0,maux->z2);
           /* Face de esquerda */
              glNormal3i(-1,0,0);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,MUROALTURA,maux->z1);
              glTexCoord2f(1,0);
              glVertex3f(maux->x1,MUROALTURA,maux->z2);
              glTexCoord2f(1,1);
              glVertex3f(maux->x1,0,maux->z2);
              glTexCoord2f(0,1);
              glVertex3f(maux->x1,0,maux->z1);
           /* Face de direita */
              glNormal3i(1,0,0);
              glTexCoord2f(0,0);
              glVertex3f(maux->x2,MUROALTURA,maux->z1);
              glTexCoord2f(1,0);
              glVertex3f(maux->x2,MUROALTURA,maux->z2);
              glTexCoord2f(1,1);
              glVertex3f(maux->x2,0,maux->z2);
              glTexCoord2f(0,1);
              glVertex3f(maux->x2,0,maux->z1);
           /* Face de cima */
              glNormal3i(0,1,0);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,MUROALTURA,maux->z1);
              glTexCoord2f(1,0);
              glVertex3f(maux->x2,MUROALTURA,maux->z1);
              glTexCoord2f(1,1);
              glVertex3f(maux->x2,MUROALTURA,maux->z2);
              glTexCoord2f(0,1);
              glVertex3f(maux->x1,MUROALTURA,maux->z2);
           }
           maux = maux->proximo;
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);


        /* Faz o desenho dos objetos */
        ref = first->down;
        aux = first;
        int o;
        GLfloat distancia;
        GLfloat deltaX, deltaZ;
        GLfloat deltaY2 = cameraY*cameraY;

        while(aux!=NULL)
        {
           if(aux->visivel){
           deltaX = (cameraX - aux->x1+HALFSQUARESIZE);
           deltaZ = (cameraZ - aux->z1+HALFSQUARESIZE);
           distancia = sqrt(deltaX*deltaX+deltaY2+deltaZ*deltaZ) / SQUARESIZE;
           for(o=0;o<MAXOBJETOS;o++)
           {
              if((aux->objetos[o] != NULL) && (aux->objetosDesenha[o] == 1) )
              {
                  aux->objetos[o]->Desenhar(aux->x1+HALFSQUARESIZE,
                                            aux->z1+HALFSQUARESIZE,distancia);
              }
           }
           }
           aux->visivel = 0;
           if(aux->right == NULL)   //chegou ao fim da linha
           {
              aux = ref;
              if(ref!= NULL) ref = ref->down;
           } 
           else      //senao continua na linha
           {
              aux = aux->right;
           } 
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
	first = NULL;
	name = NULL;
        squareInic = NULL;
        muros = NULL; 
        x = z = xInic = zInic = 0;
}


Square* Map::quadradoRelativo(int x, int z)
{
   int ax,az;
   Square* result = first;
   for(ax=0;ax<(x-1);ax++) result = result->right;
   for(az=0;az<(z-1);az++) result = result->down;
   return(result);
}

/********************************************************************
 *                       Abre o Arquivo de Mapas                    *
 ********************************************************************/
int Map::open(char* arquivo)
{
   FILE* arq;        // arquivo utilizado para o mapa
   char buffer[128]; // buffer utilziado para ler
   char nomeArq[128], nome[128];
   Square* aux;
   Square* ant = NULL, *primLinha=NULL;
   int posX,posZ;    //posicao atual do quadrado anterior relativo
   int IDtexturaAtual = -1;
   char* nomeTexturaAtual = "nada\0";
   int IDmuroTexturaAtual = -1;
   char* nomeMuroTexturaAtual = "nada\0";
   int numObjetosAtual = 0;
   int pisavel=0;
   
   if(!(arq = fopen(arquivo,"r")))
   {
      printf("Erro ao se tentar abrir mapa: %s\n",arquivo);
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
      printf("Não foi definido o tamanho do mapa %s\n", arquivo); 
      printf(" Encontramos isso %s, ao invés de T iXi\n",buffer);
      fclose(arq);
      return(0);
   }

    /* Inicia Estruturas */
   Objetos = new(LmapObjeto);
   Texturas = NULL;

   muro* maux = NULL;

   posX = x;
   posZ = 0;

   /* Vamos ler todo o arquivo */
   while(fscanf(arq, "%s", buffer) != EOF)
   {
      switch(buffer[0])
      {
         case 'm': /* Define Muros (paredes) */
         {
            switch(buffer[1])
            {
               case 'u': /* Define o muro */
               {
                  maux = new(muro);
                  fgets(buffer, sizeof(buffer),arq);
                  sscanf(buffer,"%d,%d,%d,%d",&maux->x1,&maux->z1,
                                              &maux->x2,&maux->z2);
                  maux->x1 = (maux->x1)*SQUARESIZE;
                  maux->x2 = (maux->x2+1)*SQUARESIZE;
                  maux->z1 = (maux->z1)*SQUARESIZE;
                  maux->z2 = (maux->z2+1)*SQUARESIZE;
                  maux->proximo = muros;
                  maux->textura = -1;
                  muros = maux;
                  break;
               }
               case 't': /* Define a textura do muro */
               {
                  fgets(buffer, sizeof(buffer), arq);
                  sscanf(buffer,"%s",nome);
                  if(strcmp(nomeArq,nomeMuroTexturaAtual))
                  {
                     nomeMuroTexturaAtual = nome;
                     IDmuroTexturaAtual = IDTextura(this,nome);
                  }
                  maux->textura = IDmuroTexturaAtual;
                  break;
               }
            }
            break;
         }
         case 'i':/* Define a posição Inicial */
         {
            fgets(buffer, sizeof(buffer), arq); //até final da linha
            sscanf(buffer, "%d,%d",&xInic,&zInic);
            Square *au = first;
            int pos;
            for(pos = 0; pos <= xInic; pos++) 
                 au = au->right;
            for(pos = 0; pos <= zInic; pos++) 
                 au = au->down;
            squareInic = au;
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
            sscanf(buffer, "%s %s",nome,nomeArq);  
            InserirTextura(this,nomeArq, nome);
            break;
         }
         case 'p': /* Inserção de um novo quadrado */
         {
            ant = aux;
            aux = new(Square);
            numObjetosAtual = 0;
            if(first == NULL)
               first = aux;
            if(posX == x) //fim da linha de quadrados
            { 
                aux->up = primLinha;
                if(primLinha != NULL)
                  primLinha->down = aux;
                primLinha = aux;
                posX = 1;
                posZ++;
            }
            else  //continua na mesma linha
            {
               ant->right = aux;
               aux->left = ant;
               if(ant->up == NULL)
                  aux->up = NULL;
               else
               {
                  aux->up = ant->up->right;
                  aux->up->down = aux;
               }
               posX++;
            }
            fgets(buffer, sizeof(buffer), arq); //até final da linha
            sscanf(buffer, "%d",&pisavel);
            aux->x1 = (posX-1)*SQUARESIZE;
            aux->x2 = aux->x1+SQUARESIZE;
            aux->z1 = (posZ-1)*SQUARESIZE;
            aux->z2 = aux->z1+SQUARESIZE; 
            aux->posX = posX;
            aux->posZ = posZ;
            if(pisavel) aux->flags |= PISAVEL;
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
                  if(strcmp(nomeArq,nomeTexturaAtual))
                  /* Se sao Diferentes, tem de ser mudadas */
                  {
                     nomeTexturaAtual = nome;
                     IDtexturaAtual = IDTextura(this,nome);
                  }
                  aux->textura = IDtexturaAtual;
                  break;
               }
               case 'o': /* Insere Objeto no Quadrado */
               {
                  if(numObjetosAtual >= MAXOBJETOS)
                  {
                     printf("Estourado Limite de objetos por Quadrado\n");
                  }
                  else
                  {
                     fgets(buffer, sizeof(buffer), arq);
                     sscanf(buffer,"%s %d:%d,%d",nome,
                                      &aux->objetosDesenha[numObjetosAtual],
                                      &aux->Xobjetos[numObjetosAtual],
                                      &aux->Zobjetos[numObjetosAtual]);
                     aux->objetos[numObjetosAtual] = Objetos->EndMapObjeto(nome);
                     numObjetosAtual++;
                  }
                  break;
               }
               default:
                       printf("Que merda eh essa: %s em %s\n",buffer,arquivo);
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
                 printf("Que merda eh essa: %s em %s\n",buffer,arquivo);
                 break;
      }
   }
   fclose(arq);

   /* Agora varre todo o mapa, atualizando os ponteiros para os quadrados de
    * objeto. */
   printf("rodando paso2\n");
   aux = first;
   ant = first->down;
   int ax,az;
   int i;
   for(az=0;az<z;az++)
   {
       for(ax=0;ax<x;ax++)
       {
          for(i=0;i<MAXOBJETOS;i++)
            if(aux->objetos[i] != NULL)
            {
               aux->quadObjetos[i] = quadradoRelativo(aux->Xobjetos[i],aux->Zobjetos[i]);
               aux->Xobjetos[i] = HALFSQUARESIZE +
                    (aux->Xobjetos[i]-1)*SQUARESIZE;
               aux->Zobjetos[numObjetosAtual] = HALFSQUARESIZE +
                    (aux->Zobjetos[i]-1)*SQUARESIZE;
            }
          aux = aux->right;
       }
       aux = ant;
       if(ant!=NULL)
         ant = ant->down;
   }

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
   Square* baixo, *direita;
   baixo = first;
   while(baixo != NULL)
   {
      first = baixo;
      baixo = first->down;
      direita = first->right;
      while(direita != NULL)
      {
         delete(first);
         first = direita; 
         direita = direita->right;
      }
   }
   
}

