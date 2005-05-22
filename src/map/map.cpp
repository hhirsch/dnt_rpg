/* DCC Nightmare is Public Domain - Do whatever you want with this code.
 */

#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glu.h>
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

   glGenTextures(1, &(tex->indice));
   glBindTexture(GL_TEXTURE_2D, tex->indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   mapa->numtexturas++;

   printf("Inseri Textura: %s :%d %d\n",nome,img->w,img->h);

   /* Libera a memoria utilizada */
   SDL_FreeSurface(img);
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
int Map::draw()
{
	Square * ref = first->down;
        Square * aux = first;
	GLfloat x = 0.0, z = 0.0;
        int textura = -1;
        if(aux) {
           textura = aux->textura;
           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, textura);
        }
#ifdef DEBUG_MAP
	printf("map.cpp - Map::draw(): Drawing map %s.\n", name);
#endif
        glBegin(GL_QUADS);
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
             glTexCoord2f(1,1);
             glVertex3f( x + (SQUARESIZE/2), 0.0, z + (SQUARESIZE/2) );
             glTexCoord2f(0,1);
             glVertex3f( x - (SQUARESIZE/2), 0.0, z + (SQUARESIZE/2) );
             glTexCoord2f(0,0);
             glVertex3f( x - (SQUARESIZE/2), 0.0, z - (SQUARESIZE/2) );
             glTexCoord2f(1,0);
             glVertex3f( x + (SQUARESIZE/2), 0.0, z - (SQUARESIZE/2) );
           //(*aux).draw(x,y);
           if(aux->right == NULL)   //chegou ao fim da linha
           {
              aux = ref;
              if(ref!= NULL) ref = ref->down;
              z += SQUARESIZE;
              x = 0.0;
           } 
           else      //senao continua na linha
           {
              aux = aux->right;
              x += SQUARESIZE;
           } 
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
	return(0);

}

/********************************************************************
 *                     Construtor dos Mapas                         *
 ********************************************************************/
Map::Map()
{
	numtexturas = 0;
	first = NULL;
	name = NULL;
        x = z = xInic = zInic = 0;
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
   int numObjetosAtual;
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

   posX = x;
   posZ = 0;

   /* Vamos ler todo o arquivo */
   while(fscanf(arq, "%s", buffer) != EOF)
   {
      switch(buffer[0])
      {
         case 'i':/* Define a posição Inicial */
         {
            fgets(buffer, sizeof(buffer), arq); //até final da linha
            sscanf(buffer, "%d,%d",&xInic,&zInic);
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
                     sscanf(buffer,"%s %d",nome,
                                      &aux->objetosDesenha[numObjetosAtual]);
                     aux->objetos[numObjetosAtual] = Objetos->EndMapObjeto(nome);
                  }
                  break;
               }
               default:
                       printf("Que merda eh essa: %s em %s\n",buffer,arquivo);
                       break;
            }
            break; 
         }
         default:
                 printf("Que merda eh essa: %s em %s\n",buffer,arquivo);
                 break;
      }
   }
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

