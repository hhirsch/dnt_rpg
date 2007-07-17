/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "character.h"
#include "dialog.h"
#include "../gui/draw.h"
#include <SDL/SDL_image.h>

/*********************************************************************
 *                             constructor                           *
 *********************************************************************/
character::character(featsList* ft)
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

  /* Clear Models */
  headModel.modelName = "";
  headModel.meshID = -1;
  leftHandModel.modelName = "";
  leftHandModel.meshID = -1;
  rightHandModel.modelName = "";
  rightHandModel.meshID = -1;
  leftFingerModel.modelName = "";
  leftFingerModel.meshID = -1;
  rightFingerModel.modelName = "";
  rightFingerModel.meshID = -1;
  neckModel.modelName = "";
  neckModel.meshID = -1;
  footModel.modelName = "";
  footModel.meshID = -1;
  bodyModel.modelName = "";
  bodyModel.meshID = -1;

  /* Define the weapon */
  defineWeapon();
}

/*********************************************************************
 *                             destructor                            *
 *********************************************************************/
character::~character()
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
void character::newInventory()
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
void character::setConversationFile(string file)
{
   conversationFile = file;
}

/*********************************************************************
 *                        getConversationFile                        *
 *********************************************************************/
string character::getConversationFile()
{
   return(conversationFile);
}

/*********************************************************************
 *                        createConversation                         *
 *********************************************************************/
void character::createConversation(void* pEngine)
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
void character::openConversationDialog(interface* gui, character * PC)
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
bool character::treatConversation(guiObject* guiObj, int eventInfo, 
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
         return(cs->treat(guiObj, eventInfo, gui));
      }
   }
   return(false);
}

/*********************************************************************
 *                           definePortrait                          *
 *********************************************************************/
void character::definePortrait(string portraitFile)
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
string character::getPortraitFileName()
{
   return(retratoConversa);
}

/*********************************************************************
 *                       defineMaxLifePoints                         *
 *********************************************************************/
void character::defineMaxLifePoints(int maxPoints)
{
  maxLifePoints = maxPoints;
  lifeBar->defineMaxHealth(maxPoints);
  defineActualLifePoints(maxPoints);
}

/*********************************************************************
 *                      defineActualLifePoints                       *
 *********************************************************************/
void character::defineActualLifePoints(int newLife)
{
   lifePoints = newLife;
   lifeBar->defineActualHealth(newLife);
   lifeBar->draw(portraitImage);
}

/*********************************************************************
 *                         drawMainPortrait                          *
 *********************************************************************/
void character::drawMainPortrait()
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
int character::getPoints(points pt)
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
void character::clearSkills()
{
   sk.clear();
   applySkillCosts();
   sk.setAvaiblePoints(getFirstLevelSkillPoints(0));
}

/******************************************************************
 *                   getFirstLevelSkillPoints                     *
 ******************************************************************/
int character::getFirstLevelSkillPoints(int multiClassNumber)
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
int character::getOtherLevelSkillPoints(int multiClassNumber)
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
void character::defineInitialLifePoints()
{
   /* At First Level, the hit points is equal to the Max dice value 
    * plus  constitution bonus */
   defineMaxLifePoints(actualClass[0]->lifeDiceID + attBonus(ATT_CONSTITUTION));
}

/*********************************************************************
 *                           callDeadAnimation                       *
 *********************************************************************/
void character::callDeadAnimation()
{
   setState(STATE_DIE);
}

/*********************************************************************
 *                          callAttackAnimation                      *
 *********************************************************************/
void character::callAttackAnimation()
{
   setState(STATE_ATTACK_MEELE);
}

/*********************************************************************
 *                           callIdleAnimation                       *
 *********************************************************************/
void character::callIdleAnimation()
{
   setState(STATE_IDLE);
}

/*********************************************************************
 *                            setOrientation                         *
 *********************************************************************/
void character::setOrientation(GLfloat ori)
{
   orientacao = ori;
   pathFind.setOrientation(ori);
}

/*********************************************************************
 *                             defineWeapon                          *
 *********************************************************************/
void character::defineWeapon()
{
   object* obj = inventories->getFromPlace(INVENTORY_LEFT_HAND);
   weapon* wp = (weapon*) obj;
   diceThing dc;
   /* Load the Left Hand Weapon, if one is, and if is different than the
    * current one. */
   if(obj)
   {
      if( (obj->getName()) != (leftHandModel.modelName))
      {
         /* Detach the mesh */
         //TODO

         /* Attach the mesh */
         //TODO the big problem to attach the mesh now, is that we have
         //     a new cal3d model for each weapon, and the attach function
         //     of the cal3d only attachs meshes to the model (can't attach
         //     models to the model). So, the possibly solution is:
         //     1- Reload the mesh from the file, the materials, and its
         //        bones, than put them as a child of another bone, update
         //        all id values of the weapon and so it is on the model.
         //        It's a not so beautyful way to do it, and is a lot of 
         //        work to be done;
         //     2- Maybe, trying to use the 
         //           addCoreMaterial(CalCoreMaterial *pCoreMaterial);
         //           int addCoreMesh(CalCoreMesh *pCoreMesh);
         //        and so on. Will try that latter, when have more patience.

         /* Define the weapon */
         dc = wp->getDice();
         if(wp->getRangeType() == FEAT_MELEE_ATTACK)
         {
            actualFeats.defineMeleeWeapon(dc, wp->getRange());
         }
         else if(wp->getRangeType() == FEAT_RANGED_ATTACK)
         {
            actualFeats.defineRangedWeapon(dc, wp->getRange());
         }
      }
   }
   else
   {
      /* Detach the mesh */
      //TODO
      
      /* Define the weapon to bare hands */
      dc.baseDice.diceID = DICE_D2;
      dc.baseDice.numberOfDices = 1;
      dc.baseDice.sumNumber = 0;
      dc.initialLevel = 1;
      dc.baseDice.criticalMultiplier = 2;
      actualFeats.defineMeleeWeapon(dc, 
                                    (int)(WALK_PER_MOVE_ACTION * DNT_TO_METER));
   }   

   obj = inventories->getFromPlace(INVENTORY_RIGHT_HAND);
   wp = (weapon*) obj;
   /* Load the Right Hand Weapon, if one is, and if is different than the
    * current one. */
   if(obj)
   {
      if( (obj->getName()) != (leftHandModel.modelName))
      {
         /* Detach the mesh */
         //TODO

         /* Attach the mesh */
         //TODO the big problem to attach the mesh now, is that we have
         //     a new cal3d model for each weapon, and the attach function
         //     of the cal3d only attachs meshes to the model (can't attach
         //     models to the model). So, the possibly solution is:
         //     1- Reload the mesh from the file, the materials, and its
         //        bones, than put them as a child of another bone, update
         //        all id values of the weapon and so it is on the model.
         //        It's a not so beautyful way to do it, and is a lot of 
         //        work to be done;
         //     2- Maybe, trying to use the 
         //           addCoreMaterial(CalCoreMaterial *pCoreMaterial);
         //           int addCoreMesh(CalCoreMesh *pCoreMesh);
         //        and so on. Will try that latter, when have more patience.

         /* Define the weapon */
         dc = wp->getDice();
         if(wp->getRangeType() == FEAT_MELEE_ATTACK)
         {
            actualFeats.defineMeleeWeapon(dc, wp->getRange());
         }
         else if(wp->getRangeType() == FEAT_RANGED_ATTACK)
         {
            actualFeats.defineRangedWeapon(dc, wp->getRange());
         }
      }
   }
   else
   {
      /* Detach the mesh */
      //TODO
      
      /* Define the weapon to bare hands */

      if(!inventories->getFromPlace(INVENTORY_LEFT_HAND))
      {
         dc.baseDice.diceID = DICE_D2;
         dc.baseDice.numberOfDices = 1;
         dc.baseDice.sumNumber = 0;
         dc.initialLevel = 1;
         dc.baseDice.criticalMultiplier = 2;
         actualFeats.defineMeleeWeapon(dc, 
                                    (int)(WALK_PER_MOVE_ACTION * DNT_TO_METER));
      }
   }

}

/*********************************************************************
 *                           applySkillCosts                         *
 *********************************************************************/
void character::applySkillCosts()
{
   int i;

   /* Apply Race Costs */
   actualRace->applySkillCosts(&sk);

   //FIXME Classes Costs will be only per actual class?
   //      In the way bellow is always for all classes.
   for(i = 0; i<MAX_DISTINCT_CLASSES; i++)
   {
      if(actualClass[i] != NULL)
      {
         actualClass[i]->applySkillCosts(&sk);
      }
   }
}


/***************************************************************************
 ***************************************************************************
 *                             LPersonagem                                 *
 ***************************************************************************
 ***************************************************************************/


/*********************************************************************
 *                         listConstructor                           *
 *********************************************************************/
characterList::characterList()
{
   total = 0;
   first = new character(NULL);
   first->next = first;
   first->previous = first;
}

/*********************************************************************
 *                          listDestructor                           *
 *********************************************************************/
characterList::~characterList()
{
   character* per = first->next;
   character* ap;
   while(per != first)
   {
      ap = per;
      per = per->next;
      delete(ap);
   }
   delete(first);
} 
 
/*********************************************************************
 *                           insertCharacter                         *
 *********************************************************************/
character* characterList::insertCharacter(string file, featsList* ft,
                                           void* pEngine)

{
   FILE* arq;
   char buffer[128];
   char buf2[128];
   string buf; 
   string arqModelo;
   character* novo;
   novo = new character(ft);
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
            sscanf(buffer, "%d", &novo->sk.m_skills[cn].points);
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

   
   novo->next = first->next;
   novo->previous = first;
   first->next = novo;
   novo->next->previous = novo;
   total++;
   activeCharacter = novo;

   return(novo);
} 

/*********************************************************************
 *                          RetiraPersonagem                         *
 *********************************************************************/
void characterList::removeCharacter(character* persona)
{
   persona->previous->next = persona->next;
   persona->next->previous = persona->previous;
   total--;
   delete(persona);
}

/*********************************************************************
 *                            getEnemyCharacter                      *
 *********************************************************************/
character* characterList::getEnemyCharacter(character* last)
{
   while(last != first)
   {
      if(last->psychoState == PSYCHO_HOSTILE)
      {
         return(last);
      }
      last = (character*) last->next;
   }
   return(NULL);
}

/*********************************************************************
 *                           getActiveCharacter                      *
 *********************************************************************/
character* characterList::getActiveCharacter()
{
   return(activeCharacter);
}

/*********************************************************************
 *                           setActiveCharacter                      *
 *********************************************************************/
void characterList::setActiveCharacter(character* dude)
{
   if(dude != first)
   {
      activeCharacter = dude;
   }
   else
   {
      activeCharacter = dude->next;
   }
}



