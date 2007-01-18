#include "collision.h"
#include "util.h"
#include "personagens.h"
#include "../etc/glm.h"


/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
collision::collision()
{
   actualMap = NULL;
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
collision::~collision()
{
   actualMap = NULL;
}


/*********************************************************************
 *                             defineMap                             *
 *********************************************************************/
void collision::defineMap(Map* map)
{
   actualMap = map;
}

/*********************************************************************
 *                           verifySquare                            *
 *********************************************************************/
bool collision::verifySquare(GLfloat min[3], GLfloat max[3], Square* quad)
{
   bool result = false;
   GLfloat min2[3];
   GLfloat max2[3];

   Square* proxima = quad;
   if(proxima->flags & PISAVEL)
   {
     result = true;
   }
   
   if(result) // Se possivel entrar, testa com Muros
   {
      int mur = 0;
      while((mur < MAXMUROS ) && (proxima->muros[mur] != NULL))
      {
         min2[0] = proxima->muros[mur]->x1; 
         min2[1] = 0; 
         min2[2] = proxima->muros[mur]->z1;
         max2[0] = proxima->muros[mur]->x2; 
         max2[1] = MUROALTURA; 
         max2[2] = proxima->muros[mur]->z2;
         result &= !estaDentro(min,max,min2,max2,1);
         if(!result)
           return(false);
         mur++;
      }
   }
   if(result) // Se eh possivel entrar, testa com os objects
   {
      int ob = 0;
      //GLfloat u1,u2,v1,v2;
      boundingBox bounding;
      GLfloat X[4], Z[4];
      while( (proxima->objects[ob] != NULL)) 
      {
        if(!proxima->pisavelObj[ob])
        {
          bounding = proxima->objects[ob]->getBoundingBox();
          X[0] = bounding.x1;
          Z[0] = bounding.z1;
          X[1] = bounding.x1;
          Z[1] = bounding.z2;
          X[2] = bounding.x2;
          Z[2] = bounding.z2;
          X[3] = bounding.x2;
          Z[3] = bounding.z1;
/* TODO +Yobjects */
          rotTransBoundingBox(proxima->objectsOrientation[ob], X, Z,
                              proxima->Xobjects[ob], bounding.y1, 
                              bounding.y2, proxima->Zobjects[ob], 
                              min2, max2);

          result &= !estaDentro(min,max,min2,max2,1);
          if(!result) //se ja achou que nao pode, cai fora
             return(false);
        }
        ob++;
      }
   }

   return(result);
}


/*********************************************************************
 *                           verifyMeioFio                           *
 *********************************************************************/
bool collision::verifyMeioFio(GLfloat min[3],GLfloat max[3], muro* meiosFio)
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
       {
          return(true);
       }
       maux = maux->proximo;
    }
    return(false);
}

/*********************************************************************
 *                              canWalk                              *
 *********************************************************************/
bool collision::canWalk(GLfloat perX, GLfloat perY, GLfloat perZ, 
                        GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                        GLfloat perX2, GLfloat perY2, GLfloat perZ2, 
                        GLfloat perOrientation, Square* perQuad,
                        Lpersonagem* NPCs, GLfloat& varHeight,
                        GLfloat& nx, GLfloat& nz)
{
   bool result = true;
   Square* saux;

   GLfloat min[3],min2[3];
   GLfloat max[3],max2[3];

   GLfloat x[4],z[4];

   if(!actualMap)
   {
      return(false);
   }


   x[0] = perX1;
   z[0] = perZ1;

   x[1] = perX1;
   z[1] = perZ2; 

   x[2] = perX2;
   z[2] = perZ2;

   x[3] = perX2;
   z[3] = perZ1;

   /* Rotaciona e translada o Bounding Box */
   rotTransBoundingBox(perOrientation, x, z, perX, perY1 + perY, 
                       perY2 + perY, perZ, min, max );

   /* Testa limites do Mapa */
   if( (min[0]<2) || (min[2]<2) || 
       (max[0]>actualMap->x*SQUARESIZE-2) || (max[2]>actualMap->z*SQUARESIZE-2))
   {
      return(false);
   }

   /* Testa Portas */
   door* porta = actualMap->portas;
   while( porta != NULL )
   {
      GLfloat minObj[3], maxObj[3];
      boundingBox boundPorta = porta->object->getBoundingBox();
      GLfloat XA[4]; GLfloat ZA[4];
      XA[0] = boundPorta.x1;
      ZA[0] = boundPorta.z1;

      XA[1] = boundPorta.x1;
      ZA[1] = boundPorta.z2; 

      XA[2] = boundPorta.x2;
      ZA[2] = boundPorta.z2;

      XA[3] = boundPorta.x2;
      ZA[3] = boundPorta.z1;
      rotTransBoundingBox(porta->orientacao, XA, ZA,
                          porta->x, 0.0,0.0,porta->z, 
                          minObj, maxObj);
      if(estaDentro( min, max, minObj, maxObj, 1))
      {
         return(false);
      }
      porta = porta->proximo;
   }

   /* Testa o Atual, ja que eh GRANDE! */
   min2[0] = perQuad->x1;
   min2[1] = 0;
   min2[2] = perQuad->z1;
   max2[0] = perQuad->x2;
   max2[1] = 400;
   max2[2] = perQuad->z2;
   if(estaDentro(min,max,min2,max2,1))
   {
      result &= verifySquare(min,max,perQuad);
      if(!result)
      {
         //printf("sai na atual\n"); 
         return(false);
      }
   }

 
   /* Testa quadrados a direita */
   saux = actualMap->quadradoRelativo(perQuad->posX+1,
                             perQuad->posZ);
   if(saux) 
   { 
      /* leste */
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      {
         result &= verifySquare(min,max,saux);
         if(!result)
         {
            //printf("sai na direita\n"); 
            return(false);
         }
      }
      /* Nordeste */
      saux = actualMap->quadradoRelativo(perQuad->posX+1,
                             perQuad->posZ-1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1) )
         {
            result &= verifySquare(min,max,saux);
            if(!result) 
            {
               //printf("sai na direita->cima\n"); 
               return(false);
            }
         }
      }
      /* Sudeste */
      saux = actualMap->quadradoRelativo(perQuad->posX+1,
                             perQuad->posZ+1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1))
         {
            result &= verifySquare(min,max,saux);
            if(!result) 
            {
               //printf("sai na direita->baixo\n"); 
               return(false);
            }
         }
      }
   }

   /* Testa quadrados a esquerda */
   saux = actualMap->quadradoRelativo(perQuad->posX-1,
                             perQuad->posZ);
   if( saux ) 
   { 
      /* oeste */
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1))
      {
         result &= verifySquare(min,max,saux);
         if(!result) 
         {
            //printf("sai na esquerda\n"); 
            return(false);
         }
      }

      /* Noroeste */
      saux = actualMap->quadradoRelativo(perQuad->posX-1,
                             perQuad->posZ-1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1) )
         {
            result &= verifySquare(min,max,saux);
            if(!result) 
            {
               //printf("sai na esquerda->cima\n"); 
               return(false);
            }
         }
      }
      /* Sudoeste */
      saux = actualMap->quadradoRelativo(perQuad->posX-1,
                             perQuad->posZ+1);
      if( saux )
      { 
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(estaDentro(min,max,min2,max2,1))
         {
            result &=verifySquare(min,max,saux);
            if(!result) 
            {
                //printf("sai na esquerda->baixo\n"); 
               return(false);
            }
         }
      }
   }
  
   /* Testa quadrados abaixo */
   saux = actualMap->quadradoRelativo(perQuad->posX,
                             perQuad->posZ+1);
   if( saux ) 
   {   
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      { 
         /* sul */
         result &= verifySquare(min,max,saux);
         if(!result) 
         {
            //printf("sai no de baixo\n"); 
            return(false);
         }
      }
   }

   /* Testa quadrados acima */
   saux = actualMap->quadradoRelativo(perQuad->posX,
                             perQuad->posZ-1);
   if( saux )
   {  
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(estaDentro(min,max,min2,max2,1) )
      { 
         /* norte */
         result &= verifySquare(min,max,saux);
         if(!result) 
         {
            //printf("sai no de cima\n"); 
            return(false);
         }
      }
   }

   /* Testa colisao com npcs */
   if(NPCs)
   {
      personagem* pers = (personagem*) NPCs->primeiro->proximo;
      while( (pers != NPCs->primeiro) )
      {
         x[0] = pers->min[0];
         z[0] = pers->min[2];

         x[1] = pers->min[0];
         z[1] = pers->max[2]; 

         x[2] = pers->max[0];
         z[2] = pers->max[2];
 
         x[3] = pers->max[0];
         z[3] = pers->min[2];

         /* Rotaciona e translada o Bounding Box */
         rotTransBoundingBox(pers->orientacao, x, z,
                          pers->posicaoLadoX, 
                          0.0, 0.0, 
                          pers->posicaoLadoZ, min2, max2 );

         if(estaDentro( min, max, min2, max2, 1))
         {
            return(false);
         }
    
         pers = (personagem*) pers->proximo;
      }
   }

   /* Testa Meio-fio */
   if( verifyMeioFio( min, max, actualMap->meiosFio) )
   {
      varHeight = MEIOFIOALTURA+0.1;
   }
   else
   {
      varHeight = 0.0;
   }
   
   nx = ((min[0] + max[0]) / 2);
   nz = ((min[2] + max[2]) / 2);
      
   return(result);
}


