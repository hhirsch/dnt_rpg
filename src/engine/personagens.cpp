/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "personagens.h"
#include <SDL/SDL_image.h>

/*********************************************************************
 *                    Construtor do Personagem                       *
 *********************************************************************/
personagem::personagem(featsList* ft)
{
   int i;

  lifeBar = new healthBar(10,103,117,123);

  for(i=0; i<MAX_DISTINCT_CLASSES; i++)
  {

      actualClass[i] = NULL;
      classLevels[i] = 1;
  }
  actualRace = NULL;
  actualAlign = NULL;
  
  for(i = 0; i < INVENTORY_PER_CHARACTER; i++)
  {
    inventories[i] = new inventory;
  }

  /* Feat Details */
  actualFeats.insertFeat(ft->featByNumber(FEAT_MELEE_ATTACK));
  actualFeats.insertFeat(ft->featByNumber(FEAT_RANGED_ATTACK));
}

/*********************************************************************
 *                    Destruidor do Personagem                       *
 *********************************************************************/
personagem::~personagem()
{
   int i;
   delete(lifeBar);
   for(i = 0; i < INVENTORY_PER_CHARACTER; i++)
   {
      if(inventories[i])
      {
         delete(inventories[i]);
      }
   }
} 


void personagem::DefineMaxLifePoints(int maxPoints)
{
  maxLifePoints = maxPoints;
  lifeBar->defineMaxHealth(maxPoints);
  DefineActualLifePoints(maxPoints);
}

void personagem::DefineActualLifePoints(int newLife)
{
   lifePoints = newLife;
   lifeBar->defineActualHealth(newLife);
   figura* fig = (figura*) portraits->primeiro->proximo;

   lifeBar->draw(fig->fig);

   glGenTextures(1, &portrait);
      glBindTexture(GL_TEXTURE_2D, portrait);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fig->fig->w, fig->fig->h, 
                   0, GL_RGBA, GL_UNSIGNED_BYTE, fig->fig->pixels);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void personagem::DrawMainPortrait(GLdouble x1, GLdouble y1, GLdouble z1,
                                  GLdouble x2, GLdouble y2, GLdouble z2,
                                  GLdouble x3, GLdouble y3, GLdouble z3,
                                  GLdouble x4, GLdouble y4, GLdouble z4)
{
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, portrait );
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
}


/*********************************************************************
 *                      Destruidor da Lista                          *
 *********************************************************************/
Lpersonagem::~Lpersonagem()
{
   personagem* per = (personagem*) primeiro->proximo;
   int aux;
   for(aux=0;aux<total;aux++)
   {
      RetirarPersonagem(per,0);
      per = (personagem*) per->proximo;
   }
} 
 
/*********************************************************************
 *                    Insere Personagem na Lista                     *
 *********************************************************************/
personagem* Lpersonagem::InserirPersonagem(string file, featsList* ft)

{
   FILE* arq;
   char buffer[128];
   //char buf2[128];
   string buf;
   string arqModelo;
   personagem* novo;
   novo = new personagem(ft);
   novo->portraits = new(Tlista);
   novo->objects = new(Tlista);
   novo->tipo = PERSONAGEM;
   novo->actualWeapon = NULL;
   novo->orientacao = 0.0;
   novo->posicaoFrente = 0.0;
   novo->posicaoLadoX = 0.0;
   novo->posicaoLadoZ = 0.0;
   novo->posicaoLadoY = 0.0;
   novo->dead = false;

   if(!(arq = fopen(file.c_str(),"r")))
   {
      printf("Error while opening character file: %s\n",file.c_str());
	return(0);
   }

   /* Character Name */
   fscanf(arq, "%s", buffer);
   novo->nome = buffer;
   fscanf(arq, "%s", buffer);
   arqModelo = buffer;
   fscanf(arq, "%s", buffer);
   novo->retratoConversa = buffer;

   while(fscanf(arq, "%s", buffer) != EOF)
   {
      buf = buffer;
      if(buf == "maxLifePoints")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer, "%d", &novo->lifePoints);
         novo->maxLifePoints = novo->lifePoints;
      }
      else if (buf == "baseModifier")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer,  "%d/%d/%d", &novo->fortitude, &novo->reflexes, 
                                     &novo->will); 
      }
      else if (buf == "attackModifier")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer, "%d", &novo->baseAttackModifier);
         //TODO others attack modifiers
      }
      else if (buf == "sizeModifier")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer, "%d", &novo->sizeModifier);
      }
      /*else if (buf == "lifeDice")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer, "d%d", &novo->lifeDice);
      }
      else if (buf == "race")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer, "%s", &buf2[0]);
         novo->race = numberConstant(buf2);
      }
      else if (buf == "class")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer, "%s", &buf2[0]);
         novo->cclass = numberConstant(buf2);
      }
      else if (buf == "tendency")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer, "%s", &buf2[0]);
         novo->tendency = numberConstant(buf2);
      }*/
      else if (buf == "psychoState")
      {
         fgets(buffer, sizeof(buffer),arq);
         sscanf(buffer, "%d", &novo->psychoState);
      }
      else
      {
         int cn;
         cn = numberConstant(buffer);
         if( (isAttribute(cn)) || (isSkill(cn)) )
         {
            fgets(buffer, sizeof(buffer),arq);
            sscanf(buffer, "%d", &novo->sk.m_skills[cn].pontos);
            //TODO COSTS per SKILL, based on classes...
         }
         else
         {
            fgets(buffer, sizeof(buffer),arq);
         }
         //TODO FEATS.
      }

      /*else if ( (buf == ATT_STR_LEVEL) || (buf == ATT_STR_STRENGHT) ||
                (buf == ATT_STR_DEXTERY) || (buf == ATT_STR_CONSTITUTION) ||
                (buf == ATT_STR_INTELIGENCY) || (buf == ATT_STR_WISDOW) ||
                (buf == ATT_STR_CHARISM) )*/

   }
  
   /* Define CA TODO others values to sum here*/ 
   novo->armatureClass = 10 + novo->sizeModifier + novo->attBonus(ATT_DEXTERY);
   
   /* Define os Retratos */
   novo->portraits->InserirFigura(POSRETX,POSRETY,0,0,
                                         novo->retratoConversa.c_str());
   novo->loadModel(arqModelo);

   
   InserirObj(novo);
   personagemAtivo = novo;

   return(novo);
} 

/*********************************************************************
 *                   Retira Personagem da Lista                      *
 *********************************************************************/
void Lpersonagem::RetirarPersonagem(personagem* persona, int tiraMemoria)
{
   glDeleteTextures(1,&persona->portrait);
   delete(persona->portraits);
   delete(persona->objects);
   if(tiraMemoria)
     Retirar(persona);
}

/*********************************************************************
 *                   Get Next Avaible Hostile Character              *
 *********************************************************************/
personagem* Lpersonagem::getEnemyCharacter(personagem* last)
{
   while(last != primeiro)
   {
      if(last->psychoState == PSYCHO_HOSTILE)
      {
         return(last);
      }
      last = (personagem*) last->proximo;
   }
   return(NULL);
}

