#include "mapobjeto.h"
#include "../etc/glm.h"

/*********************************************************************
 *                   Desenha o objeto de Mapa                        *
 *********************************************************************/
void mapObjeto::Desenhar(int x, int z, GLfloat distancia)
{
   GLMmodel* modelo = (GLMmodel*) modelo3d; //modelo a ser desenhado
   /* Define qual modelo desenhar */
   if((distancia>deltaVariacao) && (distancia<2*deltaVariacao) &&
      (modeloMedio != NULL))
      modelo = (GLMmodel*) modeloMedio;
   else if((distancia>2*deltaVariacao) && (modeloMinimo!=NULL))
      modelo = (GLMmodel*) modeloMinimo;

   modelo->position[0] = x;
   modelo->position[1] = 0;
   modelo->position[2] = z;
   glmDraw(modelo);
}

/*********************************************************************
 *                        Destruidor da Lista                        *
 *********************************************************************/
LmapObjeto::~LmapObjeto()
{
   mapObjeto* obj = (mapObjeto*) primeiro->proximo;
   int aux;
   //int t = total;
   for(aux=0; aux<total; aux++)
   {
      //printf("total: %d\n",total);
      printf("Tirei: %d\n",aux);
      RetirarMapObjeto(obj,0);
      obj = (mapObjeto*) obj->proximo;
      //obj = (mapObjeto*) obj->proximo;
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
   novo->modelo3d = glmReadOBJ(arqModelo,dirTexturas,0);

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
   printf("%s\n",obj->nome);
   glmDelete((GLMmodel*)obj->modelo3d);
   printf("ADEUS modelo\n");
   if(obj->modeloMedio)
   {
     printf("TemMedio\n");
     glmDelete((GLMmodel*)obj->modeloMedio);
     printf("adeus medioa\n");
   }
   if(obj->modeloMinimo)
   {
     printf("Tem minimo\n");
     glmDelete((GLMmodel*)obj->modeloMinimo); 
     printf("Adeus Minimo\n");
   }
   if(tiraMemoria)
   {
     printf("vou tirar memoria\n");
     Retirar(obj);
     printf("tirei\n");
   }
   free(obj->nome);
   printf("Adeus nome\n");
   printf("Beleza\n");
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


