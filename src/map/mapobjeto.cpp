#include "mapobjeto.h"
#include "../etc/glm.h"

/*********************************************************************
 *                   Desenha o objeto de Mapa                        *
 *********************************************************************/
void mapObjeto::Desenhar(float x, float z, GLfloat distancia, float orientacao)
{
   GLMmodel* modelo = (GLMmodel*) modelo3d; //modelo a ser desenhado
   /* Define qual modelo desenhar */
   if((distancia>deltaVariacao) && (distancia<2*deltaVariacao) &&
      (modeloMedio != NULL))
      modelo = (GLMmodel*) modeloMedio;
   else if((distancia>2*deltaVariacao) && (modeloMinimo!=NULL))
      modelo = (GLMmodel*) modeloMinimo;

   /*modelo->position[0] = x;
   modelo->position[1] = 0;
   modelo->position[2] = z;*/
   //modelo->orientacao = orientacao;
   glPushMatrix();
      glTranslatef(x, 0 ,z);
      glRotatef(orientacao,0,1,0);
      //glmDraw(modelo);
      glmDrawLists(modelo);
  glPopMatrix();
}

/*********************************************************************
 *                        Destruidor da Lista                        *
 *********************************************************************/
LmapObjeto::~LmapObjeto()
{
   mapObjeto* obj = (mapObjeto*) primeiro->proximo;
   int aux;
   for(aux=0; aux<total; aux++)
   {
      RetirarMapObjeto(obj,0);
      obj = (mapObjeto*) obj->proximo;
   }
}

/*********************************************************************
 *                 Insere objeto de mapa na Lista                    *
 *********************************************************************/
mapObjeto* LmapObjeto::InserirMapObjeto(char* arquivo, char* nome)
{
   FILE* arq;
   mapObjeto* novo;
   char arqModelo[128], dirTexturas[128];

   if(!(arq=fopen(arquivo,"r")))
   {
       printf("Error on open object %s\n",arquivo);
       return(NULL);
   }
 
   novo = new(mapObjeto);
   novo->tipo = MAPOBJETO;
   novo->nome = (char*)malloc((strlen(nome)+1)*sizeof(char));
   novo->nomeArq = (char*)malloc((strlen(arquivo)+1)*sizeof(char));
   strcpy(novo->nome,nome);
   strcpy(novo->nomeArq,arquivo);

   /* Le a variacao entre Modelos */
   fscanf(arq,"%d",&novo->deltaVariacao);
   fscanf(arq,"%s",arqModelo);
   fscanf(arq,"%s",dirTexturas); 
   novo->modelo3d = glmReadOBJ(arqModelo,dirTexturas,1);

   if(novo->deltaVariacao !=0) //S'existe variacao, le os outros modelos
   {
      fscanf(arq,"%s",arqModelo);
      fscanf(arq,"%s",dirTexturas);
      novo->modeloMedio = glmReadOBJ(arqModelo,dirTexturas,0);
      fscanf(arq,"%s",arqModelo);
      fscanf(arq,"%s",dirTexturas);
      novo->modeloMinimo = glmReadOBJ(arqModelo,dirTexturas,0);
   }
   else //Senao, os ignora.
   {
      novo->modeloMedio = NULL;
      novo->modeloMinimo = NULL;
   }

   fclose(arq);
   InserirObj(novo);
   return(novo);
}

/*********************************************************************
 *                 Retira Objeto de Mapa da Lista                    *
 *********************************************************************/
void LmapObjeto::RetirarMapObjeto(mapObjeto* obj, int tiraMemoria)
{
   glmDelete((GLMmodel*)obj->modelo3d);
   if(obj->modeloMedio)
   {
     glmDelete((GLMmodel*)obj->modeloMedio);
   }
   if(obj->modeloMinimo)
   {
     glmDelete((GLMmodel*)obj->modeloMinimo); 
   }
   if(tiraMemoria)
   {
     Retirar(obj);
   }
   free(obj->nome);
}

/*********************************************************************
 *                 Retorna Objeto de nome nome                       *
 *********************************************************************/
mapObjeto* LmapObjeto::EndMapObjeto(char* nome)
{
   int aux;
   mapObjeto* obj = (mapObjeto*) primeiro->proximo;
   for(aux=0;aux<total;aux++)
   {
      if(!strcmp(obj->nome,nome))
      {
         return(obj);
      }
      obj = (mapObjeto*) obj->proximo;
   }
   return(NULL);
}


