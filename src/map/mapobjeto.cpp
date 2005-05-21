#include "mapobjeto.h"
#include "../etc/glm.h"

/*********************************************************************
 *                   Desenha o objeto de Mapa                        *
 *********************************************************************/
void mapObjeto::Desenhar(int x, int z, int distancia)
{
   GLMmodel* modelo = (GLMmodel*) modelo3d; //modelo a ser desenhado
 
   /* Define qual modelo desenhar */
   if(distancia<2*deltaVariacao)
      modelo = (GLMmodel*) modeloMedio;
   else 
      modelo = (GLMmodel*) modeloMinimo;

   glmDraw(modelo, GLM_NONE | GLM_COLOR | GLM_SMOOTH | GLM_TEXTURE);
}

/*********************************************************************
 *                        Destruidor da Lista                        *
 *********************************************************************/
LmapObjeto::~LmapObjeto()
{
   mapObjeto* obj = (mapObjeto*) primeiro->proximo;
   int aux;
   for(aux=0;aux<total;aux++)
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
       printf("Erro ao abrir objeto %s\n",arquivo);
       return(NULL);
   }
 
   novo = new(mapObjeto);
   novo->tipo = MAPOBJETO;
   novo->nome = (char*)malloc((strlen(nome)+1)*sizeof(char));
   strcpy(novo->nome,nome);

   /* Le a variacao entre Modelos */
   fscanf(arq,"%d",&novo->deltaVariacao);
   fscanf(arq,"%s",arqModelo);
   fscanf(arq,"%s",dirTexturas);
   novo->modelo3d = glmReadOBJ(arqModelo,dirTexturas);

   if(novo->deltaVariacao !=0) //S'existe variacao, le os outros modelos
   {
      fscanf(arq,"%s",arqModelo);
      fscanf(arq,"%s",dirTexturas);
      novo->modeloMedio = glmReadOBJ(arqModelo,dirTexturas);
      fscanf(arq,"%s",arqModelo);
      fscanf(arq,"%s",dirTexturas);
      novo->modeloMinimo = glmReadOBJ(arqModelo,dirTexturas);
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
   free(obj->nome);
   glmDelete((GLMmodel*)obj->modelo3d);
   if(obj->modeloMedio)
     glmDelete((GLMmodel*)obj->modeloMedio);
   if(obj->modeloMinimo)
     glmDelete((GLMmodel*)obj->modeloMinimo);
   if(tiraMemoria)
     Retirar(obj);
}

/*********************************************************************
 *                 Retorna Objeto de nome nome                       *
 *********************************************************************/
mapObjeto* LmapObjeto::EndMapObjeto(char* nome)
{
   int aux;
   mapObjeto* obj = (mapObjeto*) primeiro;
   for(aux=0;aux<total;aux++)
   {
      if(!strcmp(obj->nome,nome))
         return(obj);
      obj = (mapObjeto*) obj->proximo;
   }
   return(NULL);
}


