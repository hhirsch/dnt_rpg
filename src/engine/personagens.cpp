/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "personagens.h"
#include "conversa.h"
#include "../gui/desenho.h"
#include <SDL/SDL_image.h>

/*********************************************************************
 *                             constructor                           *
 *********************************************************************/
personagem::personagem(featsList* ft)
{
  int i;

  lifeBar = new healthBar(5,64,59,74);

  for(i=0; i<MAX_DISTINCT_CLASSES; i++)
  {

      actualClass[i] = NULL;
      classLevels[i] = 1;
  }
  actualRace = NULL;
  actualAlign = NULL;
  actualEnemy = NULL;
  portraitImage = NULL;
  conversationFile = "";
  conv = NULL;
  
  inventories = new inventory;

  /* Feat Details */
  if(ft != NULL)
  {
     actualFeats.insertFeat(ft->featByNumber(FEAT_MELEE_ATTACK));
     actualFeats.insertFeat(ft->featByNumber(FEAT_RANGED_ATTACK));
  }
}

/*********************************************************************
 *                             destructor                            *
 *********************************************************************/
personagem::~personagem()
{
   delete(lifeBar);
   if(conv != NULL)
   {
      conversation* cs = (conversation*)conv;
      delete(cs);
      conv = NULL;
   }
   if(portraitImage)
   {
     SDL_FreeSurface(portraitImage);
   }
   if(inventories)
   {
      delete(inventories);
   }
}

/*********************************************************************
 *                           newInventory                            *
 *********************************************************************/
void personagem::newInventory()
{
   if(inventories)
   {
      delete(inventories);
   }
   inventories = new inventory;
}

/*********************************************************************
 *                        setConversationFile                        *
 *********************************************************************/
void personagem::setConversationFile(string file)
{
   conversationFile = file;
}

/*********************************************************************
 *                        getConversationFile                        *
 *********************************************************************/
string personagem::getConversationFile()
{
   return(conversationFile);
}

/*********************************************************************
 *                        createConversation                         *
 *********************************************************************/
void personagem::createConversation(void* pEngine)
{
   if(conversationFile != "")
   {
      conv = (void*) new conversation(pEngine);
      conversation* cs = (conversation*)conv;
      cs->loadFile(conversationFile);
   }
}

/*********************************************************************
 *                      openConversationDialog                       *
 *********************************************************************/
void personagem::openConversationDialog(interface* gui, personagem * PC)
{
   conversation* cs = (conversation*) conv;
   if( (cs != NULL) && (!cs->windowOpened()) )
   {
      //TODO get number of dialog if it is != 0
      cs->openDialog(0, gui, this, PC);
      convPressed = true;
   }
}

/*********************************************************************
 *                         treatConversation                         *
 *********************************************************************/
bool personagem::treatConversation(Tobjeto* guiObject, int eventInfo, 
                                   interface* gui)
{
   if(convPressed)
   {
      int x, y;
      Uint8 Mbotao = SDL_GetMouseState(&x,&y);
      convPressed = (Mbotao & SDL_BUTTON(1));
   }
   else
   {
      conversation* cs = (conversation*) conv;
      if( (cs != NULL) && (cs->windowOpened()) )
      {
         return(cs->treat(guiObject, eventInfo, gui));
      }
   }
   return(false);
}

/*********************************************************************
 *                           definePortrait                          *
 *********************************************************************/
void personagem::definePortrait(string portraitFile)
{
   if(portraitImage != NULL)
   {
      SDL_FreeSurface(portraitImage);
   }

   SDL_Surface* img = IMG_Load(portraitFile.c_str());
   if(!img)
   {
      printf("Can't Load Portrait File: %s\n", portraitFile.c_str());
   }
   /* Convert to used style */
   portraitImage = SDL_CreateRGBSurface(SDL_HWSURFACE,img->w,img->h+10,32,
                                        0x000000FF,0x0000FF00,
                                        0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,portraitImage,NULL);
   SDL_FreeSurface(img);

   /* Define fileName */
   retratoConversa = portraitFile;
}

/*********************************************************************
 *                        getPortraitFileName                        *
 *********************************************************************/
string personagem::getPortraitFileName()
{
   return(retratoConversa);
}

/*********************************************************************
 *                       defineMaxLifePoints                         *
 *********************************************************************/
void personagem::defineMaxLifePoints(int maxPoints)
{
  maxLifePoints = maxPoints;
  lifeBar->defineMaxHealth(maxPoints);
  defineActualLifePoints(maxPoints);
}

/*********************************************************************
 *                      defineActualLifePoints                       *
 *********************************************************************/
void personagem::defineActualLifePoints(int newLife)
{
   lifePoints = newLife;
   lifeBar->defineActualHealth(newLife);
   lifeBar->draw(portraitImage);
}

/*********************************************************************
 *                         drawMainPortrait                          *
 *********************************************************************/
void personagem::drawMainPortrait()
{
   glRasterPos2f(800-portraitImage->w, 600);
   glPixelZoom(1.0, -1.0);
   glDrawPixels(portraitImage->w, portraitImage->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                portraitImage->pixels);
/*   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, portrait );
   glBegin(GL_QUADS);
      glColor3f(1,1,1);
      glTexCoord2f(portraitX,0);
      glVertex3f(x1,y1,z1);
      glTexCoord2f(portraitX,portraitY);
      glVertex3f(x2,y2,z2);
      glTexCoord2f(0,portraitY);
      glVertex3f(x3,y3,z3);
      glTexCoord2f(0,0);
      glVertex3f(x4,y4,z4);
   glEnd();
   glDisable(GL_TEXTURE_2D);*/
}

/******************************************************************
 *                             getPoints                          *
 ******************************************************************/
int personagem::getPoints(points pt)
{
   int total = 0;
   skill* att;
   att = sk.getSkillByString(pt.attID);
   int aMod = attBonus(att);
   if(pt.signal == SIGNAL_DEC)
   {
      total = (pt.sum - aMod) * pt.mult;
   }
   else
   {
      total = (pt.sum + aMod) * pt.mult;
   }
   if(total < 0)
   {
      total = 0;
   }
   return(total);
}

/******************************************************************
 *                            clearSkills                         *
 ******************************************************************/
void personagem::clearSkills()
{
   sk.clear();
   sk.setAvaiblePoints(getFirstLevelSkillPoints(0));
}

/******************************************************************
 *                   getFirstLevelSkillPoints                     *
 ******************************************************************/
int personagem::getFirstLevelSkillPoints(int multiClassNumber)
{
   /* Verify if the class exists */
   if( (multiClassNumber < 0) || (multiClassNumber >= MAX_DISTINCT_CLASSES) ||
       (!actualClass[multiClassNumber]) )
   {
      return(0);
   }
   return(getPoints(actualClass[multiClassNumber]->firstLevelSP));
}

/******************************************************************
 *                   getOtherLevelSkillPoints                     *
 ******************************************************************/
int personagem::getOtherLevelSkillPoints(int multiClassNumber)
{
   /* Verify if the class exists */
   if( (multiClassNumber < 0) || (multiClassNumber >= MAX_DISTINCT_CLASSES) ||
       (!actualClass[multiClassNumber]) )
   {
      return(0);
   }
   return(getPoints(actualClass[multiClassNumber]->otherLevelsSP));
}

/******************************************************************
 *                   defineInitialLifePoints                      *
 ******************************************************************/
void personagem::defineInitialLifePoints()
{
   /* At First Level, the hit points is equal to the Max dice value 
    * plus  constitution bonus */
   defineMaxLifePoints(actualClass[0]->lifeDiceID + attBonus(ATT_CONSTITUTION));
}

/*********************************************************************
 *                           callDeadAnimation                       *
 *********************************************************************/
void personagem::callDeadAnimation()
{
   setState(STATE_DIE);
}

/*********************************************************************
 *                          callAttackAnimation                      *
 *********************************************************************/
void personagem::callAttackAnimation()
{
   setState(STATE_ATTACK_MEELE);
}

/*********************************************************************
 *                            setOrientation                         *
 *********************************************************************/
void personagem::setOrientation(GLfloat ori)
{
   orientacao = ori;
   pathFind.setOrientation(ori);
}


/***************************************************************************
 ***************************************************************************
 *                             LPersonagem                                 *
 ***************************************************************************
 ***************************************************************************/


/*********************************************************************
 *                         listConstructor                           *
 *********************************************************************/
Lpersonagem::Lpersonagem()
{
   total = 0;
   primeiro = new personagem(NULL);
   primeiro->tipo = -1;
   primeiro->proximo = primeiro;
   primeiro->anterior = primeiro;
}

/*********************************************************************
 *                          listDestructor                           *
 *********************************************************************/
Lpersonagem::~Lpersonagem()
{
   personagem* per = (personagem*) primeiro->proximo;
   personagem* ap;
   while(per != primeiro)
   {
      ap = per;
      per = (personagem*) per->proximo;
      delete(ap);
   }
   delete(primeiro);
} 
 
/*********************************************************************
 *                           insertCharacter                         *
 *********************************************************************/
personagem* Lpersonagem::insertCharacter(string file, featsList* ft,
                                           void* pEngine)

{
   FILE* arq;
   char buffer[128];
   char buf2[128];
   string buf; 
   string arqModelo;
   personagem* novo;
   novo = new personagem(ft);
   novo->tipo = PERSONAGEM;
   novo->actualWeapon = NULL;
   novo->orientacao = 0.0;
   novo->posicaoLadoX = 0.0;
   novo->posicaoLadoZ = 0.0;
   novo->posicaoLadoY = 0.0;

   if(!(arq = fopen(file.c_str(),"r")))
   {
      printf("Error while opening character file: %s\n",file.c_str());
	return(0);
   }

   /* Character Name */
   fscanf(arq, "%s", buffer);
   novo->nome = buffer;

   /* Character FileName */
   fscanf(arq, "%s", buffer);
   novo->setCharacterFile(file);
   arqModelo = buffer;

   /* Define the Portrait */
   fscanf(arq, "%s", buffer);
   novo->definePortrait(buffer);

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
      else if (buf == "conversationFile")
      {
         fgets(buffer, sizeof(buffer), arq);
         sscanf(buffer, "%s", &buf2[0]);
         novo->setConversationFile(buf2);
         if(pEngine != NULL)
         {
            novo->createConversation(pEngine);
         }
      }

      //TODO
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
   
   /* Load The 3D Model */ 
   novo->loadModel(arqModelo);

   
   novo->proximo = primeiro->proximo;
   novo->anterior = primeiro;
   primeiro->proximo = novo;
   novo->proximo->anterior = novo;
   total++;
   activeCharacter = novo;

   return(novo);
} 

/*********************************************************************
 *                          RetiraPersonagem                         *
 *********************************************************************/
void Lpersonagem::removeCharacter(personagem* persona)
{
   persona->anterior->proximo = persona->proximo;
   persona->proximo->anterior = persona->anterior;
   total--;
   delete(persona);
}

/*********************************************************************
 *                            getEnemyCharacter                      *
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

/*********************************************************************
 *                           getActiveCharacter                      *
 *********************************************************************/
personagem* Lpersonagem::getActiveCharacter()
{
   return(activeCharacter);
}

/*********************************************************************
 *                           setActiveCharacter                      *
 *********************************************************************/
void Lpersonagem::setActiveCharacter(personagem* character)
{
   if(character != primeiro)
   {
      activeCharacter = character;
   }
   else
   {
      activeCharacter = (personagem*) character->proximo;
   }
}



