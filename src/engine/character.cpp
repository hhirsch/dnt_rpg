/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "character.h"
#include "barterwindow.h"
#include "dialog.h"
#include "modstate.h"
#include "cursor.h"
#include "dccnit.h"

#include "../gui/draw.h"
#include "../ia/iascript.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include "../lang/translate.h"
#include "../classes/xp.h"

#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#include <iostream>
using namespace std;

#define BORDER_FILE "characters/portraits/borda.png"
#define BORDER_SIZE 5
#define BACK_FILE   "characters/portraits/back.png"

#define CHARACTER_TREAT_SCRIPTS  5

/*********************************************************************
 *                             constructor                           *
 *********************************************************************/
character::character(featsList* ft, engine* usedEngine)
{
   int i;

   /* Set the thing type */
   thingType = THING_TYPE_CHARACTER;

   /* Create the Health Bar */
   lifeBar = new Farso::HealthBar(5,64,59,74,NULL);

   /* Create the feats list */
   actualFeats = new feats(usedEngine);

   for(i=0; i<MAX_DISTINCT_CLASSES; i++)
   {
      actualClass[i] = NULL;
      classLevels[i] = 0;
   }
   inventoryFile = "";
   classLevels[0] = 1;
   actualRace = NULL;
   actualAlign = NULL;
   portraitImage = NULL;
   conversationFile = "";
   conv = NULL;
   cr = 1;
   curEngine = usedEngine;

   inventories = new inventory;

   /* Feat Details */
   insertDefaultNeededFeats(ft);
   activeFeat = FEAT_WEAPON_ATTACK;

   /* Bare hands damage dice */
   bareHandsDice.baseDice.setType(DICE_D2);
   bareHandsDice.baseDice.setNumberOfDices(1);
   bareHandsDice.baseDice.setSumNumber(0);
   bareHandsDice.baseDice.setCriticalMultiplier(1);
   bareHandsDice.initialLevel = 1;


   /* Effects */
   effects = new modEffectList();
   effects->setOwner(this);

   /* Scripts */
   generalScript = NULL;
   generalScriptFileName = "";
   killedScriptFileName = "";
   killedScript = NULL;
}

/*********************************************************************
 *                      insertDefaultNeededFeats                     *
 *********************************************************************/
void character::insertDefaultNeededFeats(featsList* ft)
{
   if(ft != NULL)
   {
      actualFeats->insertFeat(ft->featByNumber(FEAT_WEAPON_ATTACK));
   }
}

/*********************************************************************
 *                             destructor                            *
 *********************************************************************/
character::~character()
{
   if(lifeBar)
   {
      delete(lifeBar);
   }
   if(conv != NULL)
   {
      conversation* cs = (conversation*)conv;
      delete(cs);
      conv = NULL;
   }
   if(portraitImage)
   {
     glDeleteTextures(1, &portraitTexture);
     SDL_FreeSurface(portraitImage);
   }
   if(inventories)
   {
      delete(inventories);
   }
   if(generalScript)
   {
      delete(generalScript);
      generalScript = NULL;
   }
   if(killedScript)
   {
      delete(killedScript);
   }
   if(effects)
   {
      delete(effects);
   }
   if(actualFeats)
   {
      delete(actualFeats);
   }
}

/*********************************************************************
 *                            updateEffects                          *
 *********************************************************************/
void character::updateEffects()
{
   effects->doStep();
   /* Update Equipped objects positions too */
   
   /* Left hand */
   aniModel* mdl = scNode->getModel();
   object* obj = inventories->getFromPlace(INVENTORY_LEFT_HAND);
   if(obj)
   {
      obj->setEquipedPosition(2, mdl->leftHand.x, mdl->leftHand.y, 
            mdl->leftHand.z, scNode->getAngleY());
   }

   /* Right Hand */
   mdl = scNode->getModel();
   obj = inventories->getFromPlace(INVENTORY_RIGHT_HAND);
   if(obj)
   {
      obj->setEquipedPosition(1, mdl->rightHand.x, mdl->rightHand.y, 
            mdl->rightHand.z, scNode->getAngleY());
   }
}

/*********************************************************************
 *                         getAttModifiers                           *
 *********************************************************************/
void character::getAttModifiers(int mods[6])
{
   /* Get the race attributes modifiers */
   actualRace->getAttModifiers(mods, false, &sk);

   /* Get and add all the classes attributes modifiers */
   int i;
   for(i = 0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if(actualClass[i] != NULL)
      {
         actualClass[i]->getAttModifiers(mods, true, &sk);
      }
   }

   /* TODO -> get attribute modifiers from itens, spells, etc. */
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int character::getLevel()
{
   int lv = 0;
   int i;
   for(i = 0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if(actualClass[i] != NULL)
      {
         lv += classLevels[i];
      }
   }
   return(lv);
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int character::getLevel(classe* cl)
{
   int i;
   for(i = 0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if(actualClass[i] == cl)
      {
         return(classLevels[i]);
      }
   }
   return(0);
}

/*********************************************************************
 *                             getLevel                              *
 *********************************************************************/
int character::getLevel(string classId)
{
   int i;
   for(i = 0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if((actualClass[i] != NULL) && (actualClass[i]->strID == classId))
      {
         return(classLevels[i]);
      }
   }
   return(0);
}

/*********************************************************************
 *                            canHaveFeat                            *
 *********************************************************************/
bool character::canHaveFeat(featDescription* f)
{
   int i;
   reqFactor* req;

   if(f != NULL)
   {
      req = (reqFactor*)f->reqFactors.getFirst();
      for(i = 0; i < f->reqFactors.getTotal(); i++)
      {
         if(req->requiredFactor.type == MOD_TYPE_ATT)
         {
            /* Att, since if call getBonusValue will only receive 
             * the bonus, not the attribute*/
            skill* ski = sk.getSkillByString(req->requiredFactor.id);
            if(ski)
            {
               if(ski->points < req->requiredLevel)
               {
                  return(false);
               }
            }
         }
         else if(req->requiredFactor.type == MOD_TYPE_CLASS)
         {
            if(getLevel(req->requiredFactor.id) < req->requiredLevel)
            {
               return(false);
            }
         }
         else if(req->requiredFactor.type == MOD_TYPE_ALIGN)
         {
            if(actualAlign->strID != req->requiredFactor.id)
            {
               return(false);
            }
         }
         else if(req->requiredFactor.type == MOD_TYPE_RACE)
         {
            if(actualRace->strID != req->requiredFactor.id)
            {
               return(false);
            }
         }
         else
         {
            if(getBonusValue(req->requiredFactor) < req->requiredLevel)
            {
               return(false);
            }
         }
         req = (reqFactor*)req->getNext();
      }
      return(true);
   }

   return(false);
}

/*********************************************************************
 *                              haveFeat                             *
 *********************************************************************/
bool character::haveFeat(string featId)
{
   return(actualFeats->featByString(featId) != NULL);
}
/*********************************************************************
 *                           getActiveFeat                           *
 *********************************************************************/
int character::getActiveFeat()
{
   return(activeFeat);
}

/*********************************************************************
 *                         getActiveFeatPtr                          *
 *********************************************************************/
feat* character::getActiveFeatPtr()
{
   return(actualFeats->featByNumber(activeFeat));
}

/*********************************************************************
 *                           setActiveFeat                           *
 *********************************************************************/
void character::setActiveFeat(int f)
{
   if((f >= 0) && (f < actualFeats->getTotal()))
   {
      activeFeat = f;
   }

   activeFeat = FEAT_WEAPON_ATTACK;
}

/*********************************************************************
 *                           setActiveFeat                           *
 *********************************************************************/
void character::setActiveFeat(feat* f)
{
   int i;
   feat* cf;

   for(i=0; i<actualFeats->getTotal(); i++)
   {
      cf = (feat*)actualFeats->featByNumber(i);
      if(cf == f)
      {
         /* here it is */
         activeFeat = i;
         return;
      }
   }

   /* None found */
   cerr << "Warning: character has no feat '" 
        << f->info->name << "' !" << endl;
   activeFeat = FEAT_WEAPON_ATTACK;
}

/*********************************************************************
 *                         getBaredHandsDice                         *
 *********************************************************************/
diceThing character::getBaredHandsDice()
{
   return(bareHandsDice);
}

/*********************************************************************
 *                             isAlignOf                             *
 *********************************************************************/
bool character::isAlignOf(string al)
{
   if(actualAlign)
   {
      return(actualAlign->strID.find(al) != string::npos);
   }
   return(false);
}

/*********************************************************************
 *                         getGeneralScript                          *
 *********************************************************************/
iaScript* character::getGeneralScript()
{
   return(generalScript);
}

/*********************************************************************
 *                     getKilledScriptFileName                       *
 *********************************************************************/
void character::callKilledScript()
{
   if( (curEngine) && (!killedScriptFileName.empty()) )
   {
      killedScript = new iaScript(killedScriptFileName, curEngine);
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
 *                           definePortrait                          *
 *********************************************************************/
void character::definePortrait(string portraitFile)
{
   /* Define Machine Bit Order */
   Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif
   
   if(portraitImage != NULL)
   {
      SDL_FreeSurface(portraitImage);
   }
   else
   {
      glGenTextures(1, &portraitTexture);
   }

   dirs dir;

   /* Open Portrait Image */
   SDL_Surface* img = IMG_Load(dir.getRealFile(portraitFile).c_str());
   if(!img)
   {
      cerr << "Can't Load Portrait File: " << portraitFile << endl;
      return;
   }

   /* Load Back */
   SDL_Surface* back = IMG_Load(dir.getRealFile(BACK_FILE).c_str());
   if(!back)
   {
      SDL_FreeSurface(img);
      cerr << "Couldn't load back image file: " << BACK_FILE << endl;
      return;
   }

   /* Load Border */
   SDL_Surface* border = IMG_Load(dir.getRealFile(BORDER_FILE).c_str());
   if(!border)
   {
      SDL_FreeSurface(img);
      SDL_FreeSurface(back);
      cerr << "Can't Load Border File: " << BORDER_FILE << endl;
      return;
   }

   /* Blit Back Image and Image With Border */
   SDL_Rect rect;
   rect.x = BORDER_SIZE;
   rect.y = BORDER_SIZE;
   SDL_SetAlpha(back, 0, 0);
   SDL_BlitSurface(back, NULL, border, &rect);

   rect.x = BORDER_SIZE;
   rect.y = BORDER_SIZE;
   SDL_BlitSurface(img, NULL, border, &rect);

   /* Put With space for health bar */
#ifdef __APPLE__
   /* In OSX, the images are bgra, not rgba. */
   portraitImage = SDL_CreateRGBSurface(SDL_SWSURFACE,border->w,border->h+10,32,
                                        bmask, gmask, rmask, amask);
#else
   portraitImage = SDL_CreateRGBSurface(SDL_SWSURFACE,border->w,border->h+10,32,
                                        rmask, gmask, bmask, amask);
#endif
   /* Fix to the blit get the alpha from source! */
   SDL_SetAlpha(border, 0, 0);
   SDL_BlitSurface(border,NULL,portraitImage,NULL);

   /* Free Things */
   SDL_FreeSurface(img);
   SDL_FreeSurface(border);
   SDL_FreeSurface(back);

   /* Load Texture */
   Farso::setTextureRGBA(portraitImage, portraitTexture, false, GL_RGBA);
   portraitPropX = portraitImage->w /
                   (float)Farso::smallestPowerOfTwo(portraitImage->w);
   portraitPropY = portraitImage->h /
                   (float)Farso::smallestPowerOfTwo(portraitImage->h);

   /* Define fileName */
   talkPortrait = portraitFile;

   /* Define surface to the health bar */
   lifeBar->setSurface(portraitImage);
}

/*********************************************************************
 *                        getPortraitFileName                        *
 *********************************************************************/
string character::getPortraitFileName()
{
   return(talkPortrait);
}

/*********************************************************************
 *                         updateHealthBar                           *
 *********************************************************************/
void character::updateHealthBar()
{
  lifeBar->defineMaxHealth(maxLifePoints);
  lifeBar->defineActualHealth(lifePoints);
  Farso::setTextureRGBA(portraitImage, portraitTexture);
}

/*********************************************************************
 *                         drawMainPortrait                          *
 *********************************************************************/
void character::drawMainPortrait()
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, portraitTexture);

   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(Farso::SCREEN_X-portraitImage->w-1, Farso::SCREEN_Y-1);
      glTexCoord2f(0.0f, portraitPropY);
      glVertex2f(Farso::SCREEN_X-portraitImage->w-1, 
            Farso::SCREEN_Y-portraitImage->h-1);
      glTexCoord2f(portraitPropX, portraitPropY);
      glVertex2f(Farso::SCREEN_X-1, Farso::SCREEN_Y-portraitImage->h-1);
      glTexCoord2f(portraitPropX, 0.0f);
      glVertex2f(Farso::SCREEN_X-1, Farso::SCREEN_Y-1);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

/*********************************************************************
 *                        mouseUnderPortrait                         *
 *********************************************************************/
bool character::mouseUnderPortrait(int mouseX, int mouseY)
{
   cursor cur;
   char buf[256];

   if(Farso::isMouseAt(Farso::SCREEN_X-portraitImage->w-1, 1,
                Farso::SCREEN_X-1, 64, mouseX, mouseY) )
   {
      sprintf(buf, "%s (%d/%d)", name.c_str(), xp, nextLevelXP(xp)); 
      cur.set(CURSOR_WALK);
      cur.setTextOver(buf);
      return(true);
   }
   return(false);
}

/*********************************************************************
 *                        mouseUnderHealthBar                        *
 *********************************************************************/
bool character::mouseUnderHealthBar(int mouseX, int mouseY)
{
   cursor cur;
   char buf[128];
   
   if(Farso::isMouseAt(Farso::SCREEN_X-portraitImage->w+4, 65,
            Farso::SCREEN_X-6, 75, mouseX, mouseY) )
   {
      sprintf(buf, "%s: %d/%d", gettext("HP"), lifePoints, maxLifePoints); 
      cur.setTextOver(buf);
      cur.set(CURSOR_WALK);
      return(true);
   }
   return(false);
}

/******************************************************************
 *                         getInventoryFile                       *
 ******************************************************************/
string character::getInventoryFile()
{
   return(inventoryFile);
}

/******************************************************************
 *                             getPoints                          *
 ******************************************************************/
int character::getPoints(points pt)
{
   int total = 0;
   skill* att;
   att = sk.getSkillByString(pt.attID);
   int aMod = 0;

   if(att)
   {
      aMod = attBonus(att);
   }
   else
   {
      cerr << "Unknow Base Attribute for Skill Points: " << pt.attID << endl;
   }

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
   sk.setAvaiblePoints(getFirstLevelSkillPoints(actualClass[0]));
}

/******************************************************************
 *                   getFirstLevelSkillPoints                     *
 ******************************************************************/
int character::getFirstLevelSkillPoints(classe* cl)
{
   /* Verify if the class exists */
   if(!cl)
   {
      return(0);
   }

   return(getPoints(cl->firstLevelSP));
}

/******************************************************************
 *                   getOtherLevelSkillPoints                     *
 ******************************************************************/
int character::getOtherLevelSkillPoints(classe* cl)
{
   /* Verify if the class exists */
   if(cl == NULL)
   {
      return(0);
   }
   return(getPoints(cl->otherLevelsSP));
}

/******************************************************************
 *                   defineInitialLifePoints                      *
 ******************************************************************/
void character::defineInitialLifePoints()
{
   /* At First Level, the hit points is equal to the Max dice value 
    * plus  constitution bonus */
   setMaxLifePoints(2*actualClass[0]->lifeDiceID + attBonus(ATT_CONSTITUTION));
}

/*********************************************************************
 *                           callDeadAnimation                       *
 *********************************************************************/
void character::callDeadAnimation()
{
   scNode->getModel()->setState(STATE_DIE);
}

/*********************************************************************
 *                              instantKill                          *
 *********************************************************************/
void character::instantKill()
{
   dead = true;
   scNode->getModel()->setState(STATE_DEAD);
}

/*********************************************************************
 *                          callAttackAnimation                      *
 *********************************************************************/
void character::callAttackAnimation()
{
   scNode->getModel()->setState(STATE_ATTACK_MEELE);
}

/*********************************************************************
 *                           callIdleAnimation                       *
 *********************************************************************/
void character::callIdleAnimation()
{
   scNode->getModel()->setState(STATE_IDLE);
}

/*********************************************************************
 *                         callActionAnimation                       *
 *********************************************************************/
void character::callActionAnimation(int id)
{
   scNode->getModel()->callActionAnimation(id);
}

/*********************************************************************
 *                              seAnimation                          *
 *********************************************************************/
void character::callAnimation(int id)
{
   scNode->getModel()->setState(id);
}

/*********************************************************************
 *                            setOrientation                         *
 *********************************************************************/
void character::setOrientation(GLfloat ori)
{
   scNode->setAngle(0.0f, ori, 0.0f);
   pathFind.setOrientation(ori);
}

/*********************************************************************
 *                         getEquipedWeapon                          *
 *********************************************************************/
weapon* character::getEquipedWeapon()
{
   return((weapon*)inventories->getFromPlace(INVENTORY_RIGHT_HAND));
}

/*********************************************************************
 *                         getActiveFeatRange                        *
 *********************************************************************/
int character::getActiveFeatRange()
{
   if(activeFeat == FEAT_WEAPON_ATTACK)
   {
       feat* f = actualFeats->featByNumber(FEAT_WEAPON_ATTACK);
       /* Get the weapons range */
       weapon* wp = getEquipedWeapon();
       if(wp)
       {
          f->info->range = wp->getRange();
          return(wp->getRange());
       }

       /* Bare Hands */
       f->info->range = 45;
       return(45);
   }
   else
   {
      feat* ft = actualFeats->featByNumber(activeFeat);
      if(ft)
      {
         return(ft->info->range);
      }
   }

   return(0);
}


/*********************************************************************
 *                           applySkillCosts                         *
 *********************************************************************/
void character::applySkillCosts()
{
   int i;

   /* Apply Race Costs */
   if(actualRace)
   {
      actualRace->applySkillCosts(&sk);
   }

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

/*********************************************************************
 *                         applyBonusAndSaves                        *
 *********************************************************************/
void character::applyBonusAndSaves()
{
   int i;

   /* Clear the current */
   curBonusAndSaves.clear();

   /* Add from all classes */
   for(i = 0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if(actualClass[i] != NULL)
      {
         curBonusAndSaves = curBonusAndSaves + 
                            actualClass[i]->bonus[classLevels[i]-1];
      }
   }
}

/*********************************************************************
 *                             canClass                              *
 *********************************************************************/
bool character::canClass(classe* cl)
{
   int i;
   for(i=0; i < MAX_DISTINCT_CLASSES; i++)
   {
      /* If multiclass spaces empty or already have a level at this class,
       * the character can get a level at it. */
      if( (actualClass[i] == NULL) || (actualClass[i] == cl))
      {
         return(true);
      }
   }

   return(false);
}

/*********************************************************************
 *                          getNewClassLevel                         *
 *********************************************************************/
void character::getNewClassLevel(classe* cl)
{
   int i;

   if( (canClass(cl)) && (upLevels > 0))
   {
      /* Got the level */
      upLevels--;

      /* Find the class space */
      i = 0;
      while( (i < MAX_DISTINCT_CLASSES) && 
             (actualClass[i] != NULL) &&
             (actualClass[i] != cl) ) 
      {
         i++;
      }
      if(i == MAX_DISTINCT_CLASSES)
      {
         cerr << "Ops, classes overflow! Something really bad occurred!";
         return;
      }

      /* Define the class new level */
      actualClass[i] = cl;
      classLevels[i]++;

      /* Roll a life dice */
      dice d;
      d.setType(cl->lifeDiceID);
      setMaxLifePoints(maxLifePoints + d.roll()+attBonus(ATT_CONSTITUTION));

      /* Apply skill costs! */
      applySkillCosts();

      /* Apply the Saves and Bonus */
      applyBonusAndSaves();

      /* TODO, something related to the feats, when fully implemented */
   }
}

/*********************************************************************
 *                          addModEffect                             *
 *********************************************************************/
void character::addModEffect(int mod, int time, int periodicTime,
      string factorId, string factorType)
{
   modEffect* m = new modEffect(mod, time, periodicTime, factorId, factorType);
   effects->insert(m, true);
}

/*********************************************************************
 *                        removeAllModEffects                        *
 *********************************************************************/
void character::removeAllModEffects()
{
   effects->clearList();
}

/*********************************************************************
 *                            getEffects                             *
 *********************************************************************/
modEffectList* character::getEffects()
{
   return(effects);
}

/*********************************************************************
 *                          defineOcSquare                           *
 *********************************************************************/
void character::defineOcSquare(Map* curMap)
{
   int posX =(int)floor(scNode->getPosX() / curMap->squareSize());
   int posZ =(int)floor(scNode->getPosZ() / curMap->squareSize());
   
   ocSquare = curMap->relativeSquare(posX, posZ);
}

/*********************************************************************
 *                                save                               *
 *********************************************************************/
bool character::save(string saveFile)
{
   ofstream file;
   int i;

   /* Open the file to save */
   file.open(saveFile.c_str(), ios::out | ios::binary);
   if(!file)
   {
      cerr << "Error Opening File: " << saveFile << endl;
      return(false);
   }

   /* Name */
   file << "name = " << name << endl;
   /* Model */
   file << "model = " << scNode->getModel()->getFileName() << endl;
   /* Portrait */
   file << "portrait = " << talkPortrait << endl;
   /* Race */
   if(actualRace != NULL)
   {
      file << "race = " << actualRace->strID << endl;
   }
   /* Alignment */
   if(actualAlign)
   {
      file << "align = " << actualAlign->strID << endl;
   }
   /* Classes */
   for(i = 0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if(actualClass[i] != NULL)
      {
         file << "class = " << actualClass[i]->strID << " " 
                            << classLevels[i] << endl;
      }
   }
   /* Scripts (if any) */
   if(!battleScriptFileName.empty())
   {
      file << "battleScript = " << battleScriptFileName << endl;
   }
   if(!generalScriptFileName.empty())
   {
      file << "generalScript = " << generalScriptFileName << endl; 
   }
   if(!killedScriptFileName.empty())
   {
      file << "killedScript = " << killedScriptFileName << endl;
   }
   /* Blood Filename and position */
   file << "bloodFileName = " << bloodFileName << endl;
   file << "bloodPosition = " << bloodPosition << endl;
   /* Conversation File (if one) */
   if(!conversationFile.empty())
   {
      file << "conversationFile = " << conversationFile << endl;
   }
   /* Challenge Rating */
   if(cr != 0)
   {
      file << "challengeRating = " << cr << endl;
   }
   /* Life Points */
   file << "maxLifePoints = " << maxLifePoints << endl;
   file << "curLifePoints = " << lifePoints << endl;
   /* Size Modifier */
   file << "sizeModifier = " << sizeModifier << endl;
   /* PsychoState */
   file << "psychoState = " << psychoState << endl;
   /* Walk Interval */
   file << "walk_interval = " << (walk_interval / WALK_UPDATE) << endl;
   /* Sex Type */
   file << "sex = " << sexType << endl;
   /* Age */
   file << "age = " << age << endl;
   /* Experience Points */
   file << "xp = " << xp << endl;
   /* Displacement */
   file << "displacement = " << displacement << endl;
   /* Initiative bonus */
   file << "initiativeBonus = " << initiativeBonus << endl;
   /* Armature class */
   file << "armatureClass = " << armatureClass << endl;
   /* Fortitude */
   file << "fortitude = " << curBonusAndSaves.fortitude << endl;
   /* Reflexes */
   file << "reflexes = " << curBonusAndSaves.reflexes << endl;
   /* iAmNotAFool */
   file << "iAmNotAFool = " << curBonusAndSaves.iAmNotAFool << endl;
   /* attack bonus */
   file << "attackBonus = " << curBonusAndSaves.baseAttackBonus.toInt() << endl;
   /* InstantKilled */
   file << "instantKilled = " <<  instantKilled << endl;
   /* Up Levels */
   file << "upLevels = " << upLevels << endl;
   /* Total Levels */
   file << "totalLevels = " <<  curBonusAndSaves.level << endl;

   /* Inventory File */
   if(!inventoryFile.empty())
   {
      file << "inventory = " << inventoryFile << endl;
   }


   /* Now, put all not 0 skills */
   skill* ski;
   for(i = 0; i < sk.getTotalSkills(); i++)
   {
      ski = sk.getSkillByInt(i);
      if( (ski != NULL) && (ski->definition != NULL) && (ski->points != 0))
      {
         file << ski->definition->idString << " = " 
              << ski->points << endl;
      }
   }

   /* And all active modifiers */
   modEffect* meff = (modEffect*)effects->getFirst();
   for(i = 0; i < effects->getTotal(); i++)
   {
      file << "modEffect = " << meff->toSaveText() << endl;
      meff = (modEffect*)meff->getNext();
   }

   /* Save all feats! */
   feat* ft;
   for(i = 0; i < actualFeats->getTotal(); i++)
   {
      ft = actualFeats->featByNumber(i);
      file << "feat = " << ft->info->idString << endl;
      file << "featQuantity = " << ft->actualQuantity << endl;
   }

   /* Close the file and return */
   file.close();
   return(true);
}

/***************************************************************************
 ***************************************************************************
 *                            characterList                                *
 ***************************************************************************
 ***************************************************************************/


/*********************************************************************
 *                         listConstructor                           *
 *********************************************************************/
characterList::characterList()
{
   curTreat = NULL;
}

/*********************************************************************
 *                          listDestructor                           *
 *********************************************************************/
characterList::~characterList()
{
   clearList();
}

/*********************************************************************
 *                           insertCharacter                         *
 *********************************************************************/
character* characterList::insertCharacter(string file, featsList* ft,
                                          engine* pEngine, string curMap)

{
   defParser def;
   races racesDesc;
   aligns alignsDesc;
   classes classesDesc;
   string key, value;
   char buf[256];
   int lvl;
   int curClass = 0;
   int tmp=0;
   feat* cFeat = NULL;
   string cal3dName = "";
   scene curScene;

   bool definedBonusAndSave = false;
  
   /* Create the Character */ 
   character* novo;
   novo = new character(ft, pEngine);

   /* Try to Load the file */
   if(!def.load(file))
   {
      cerr << "Error while opening character file: " << file << endl;
      delete(novo);
      return(0);
   }

   /* Parse it! */
   while(def.getNextTuple(key, value))
   {
      /* Character Name */
      if(key == "name")
      {
         novo->name = translateDataString(value);
      }
      /* Character FileName */
      else if(key == "model")
      {
         novo->setCharacterFile(file);
         cal3dName = value;
      }
      /* Define the Portrait */
      else if(key == "portrait")
      {
         novo->definePortrait(value);
      }
      /* LifePoints */
      else if(key == "maxLifePoints")
      {
         sscanf(value.c_str(), "%d", &novo->lifePoints);
         novo->maxLifePoints = novo->lifePoints;
      }
      else if(key == "curLifePoints")
      {
         sscanf(value.c_str(),"%d",&novo->lifePoints);
      }
      /* Size Mofifier */
      else if(key == "sizeModifier")
      {
         sscanf(value.c_str(), "%d", &novo->sizeModifier);
      }
      /* Walk Interval */
      else if(key == "walk_interval")
      {
         sscanf(value.c_str(), "%f", &novo->walk_interval);
         novo->walk_interval *= WALK_UPDATE;
      }
      /* Conversation File */
      else if(key == "conversationFile")
      {
         novo->setConversationFile(value);
         novo->createConversation(curMap);
      }
      /* General Script */
      else if(key == "generalScript")
      {
         /* Create the script */
         novo->generalScript = new iaScript(value, pEngine);
         novo->generalScriptFileName = value;
         /* Set the owner */
         novo->generalScript->defineCharacterOwner(novo);
      }
      /* Battle Script */
      else if(key == "battleScript")
      {
         /* Create the Script */
         iaScript* isc = new iaScript(value, pEngine);
         novo->setBattleScript(isc, value);
         /* Set the owner */
         isc->defineCharacterOwner(novo);
      }
      /* killed Script */
      else if(key == "killedScript")
      {
         novo->killedScriptFileName = value;
      }
      /* Blood Position */
      else if(key == "bloodPosition")
      {
         sscanf(value.c_str(), "%f", &novo->bloodPosition); 
      }
      /* Blood File Name */
      else if(key == "bloodFileName")
      {
         novo->bloodFileName = value;
      }
      /* Challenge Rating */
      else if(key == "challengeRating")
      {
         sscanf(value.c_str(), "%f", &novo->cr);
      }
      /* Race */
      else if(key == "race")
      {
         novo->actualRace = racesDesc.getRaceByString(value);
      }
      /* Alignment */
      else if(key == "align")
      {
         novo->actualAlign = alignsDesc.getAlignByString(value);
      }
      /* Class */
      else if(key == "class")
      {
         sscanf(value.c_str(), "%s %d", &buf[0], &lvl);
         if(curClass < MAX_DISTINCT_CLASSES)
         {
            novo->actualClass[curClass] = classesDesc.getClassByString(buf);
            novo->classLevels[curClass] = lvl;
            curClass++;
         }
         else
         {
            cerr << "Error: class overflow for: " << file << endl;
         }
      }
      /* Experience Points */
      else if(key == "xp")
      {
         sscanf(value.c_str(),"%d", &novo->xp);
      }
      /* Up Levels */
      else if(key == "upLevels")
      {
         sscanf(value.c_str(),"%d", &novo->upLevels);
      }
      /* Instant Killed */
      else if(key == "instantKilled")
      {
         sscanf(value.c_str(), "%d", &novo->instantKilled);
      }
      /* Inventory Population */
      else if(key == "inventory")
      {
         novo->inventoryFile = value;
      }
      /* Displacement */
      else if(key == "displacement")
      {
         sscanf(value.c_str(),"%d", &novo->displacement);
      }
      /* Initiative bonus */
      else if(key == "initiativeBonus")
      {
         sscanf(value.c_str(),"%d", &novo->initiativeBonus);
      }
      /* Armature class */
      else if(key == "armatureClass")
      {
         sscanf(value.c_str(),"%d", &novo->armatureClass);
      }
      /* Psycho State */
      else if(key == "psychoState")
      {
         sscanf(value.c_str(), "%d", &novo->psychoState);
      }
      /* Sex Type */
      else if(key == "sex")
      {
         sscanf(value.c_str(), "%d", &novo->sexType);
      }
      /* Psycho State */
      else if(key == "age")
      {
         sscanf(value.c_str(), "%d", &novo->age);
      }
      /* Fortitude */
      else if(key == "fortitude")
      {
         definedBonusAndSave = true;
         sscanf(value.c_str(), "%d", &tmp);
         novo->curBonusAndSaves.setFortitude(tmp);
      }
      /* Reflexes */
      else if(key == "reflexes")
      {
         definedBonusAndSave = true;
         sscanf(value.c_str(), "%d", &tmp);
         novo->curBonusAndSaves.setReflexes(tmp);
      }
      /* iAmNotAFool */
      else if(key == "iAmNotAFool")
      {
         definedBonusAndSave = true;
         sscanf(value.c_str(), "%d", &tmp);
         novo->curBonusAndSaves.setIAmNotAFool(tmp);
      }
      /* attack bonus */
      else if( (key == "attackBonus") || (key == "baseAttack"))
      {
         definedBonusAndSave = true;
         sscanf(value.c_str(), "%d", &tmp);
         novo->curBonusAndSaves.setBaseAttack(tmp);
      }
      /* total level */
      else if(key == "totalLevels")
      {
         definedBonusAndSave = true;
         sscanf(value.c_str(), "%d", &tmp);
         novo->curBonusAndSaves.level = tmp;
      }
      else if(key == "baseDamage")
      {
         int dices=0, diceId=1, sum=0, crit=1;
         sscanf(value.c_str(), "%dd%d+%dx%d", &dices, &diceId, &sum, &crit);
         novo->bareHandsDice.baseDice.setType(diceId);
         novo->bareHandsDice.baseDice.setNumberOfDices(dices);
         novo->bareHandsDice.baseDice.setSumNumber(sum);
         novo->bareHandsDice.baseDice.setCriticalMultiplier(crit);
         novo->bareHandsDice.initialLevel = 1;
      }
      /* ModifierEffects */
      else if(key == "modEffect")
      {
         modEffect* m = new modEffect(value);
         /* Only insert, don't apply, since it's already at applied the
          * loaded values from file. */
         novo->effects->insert(m, false);
      }
      /* Feat (and feat quantity) */
      else if(key == "feat")
      {
         featDescription* fDesc = ft->featByString(value);
         if(fDesc)
         {
            novo->actualFeats->insertFeat(fDesc);
            cFeat = novo->actualFeats->featByString(value);
         }
         else
         {
            cerr << "Error: Couldn't found feat '" << value << "' "
                 << "while loading character: " << file << endl;
         }
      }
      else if(key == "featQuantity")
      {
         if(cFeat != NULL)
         {
            sscanf(value.c_str(), "%f", &cFeat->actualQuantity);
         }
         else
         {
            cerr << "Error: Got featQuantity without feats for character: "
                 << file << endl;
         }
      }

      /* Some Skill or Attribute definition */
      else
      {
         skill* skl =  novo->sk.getSkillByString(key);

         if(skl != NULL)
         {
            sscanf(value.c_str(), "%d", &skl->points);
         }
         else
         {
            /* No skill?? So unknow Token! */
            cout << "Unknow token '" << key << "' at file: " << file << endl;
         }
      }

   }

   /* Define the AC if not yet defined at the file */
   if(novo->armatureClass == 0)
   {
      /* Define AC TODO others values to sum here*/ 
      novo->armatureClass = 10+novo->sizeModifier+novo->attBonus(ATT_DEXTERITY);
   }

   /* Apply Saves and Bonus, if not yet defined (some npcs without classes
    * defines its bonus on .npc file) */
   if(!definedBonusAndSave)
   {
      novo->applyBonusAndSaves();
   }

   /* Apply cost to skills */
   novo->applySkillCosts();
   
   /* Load The 3D Model */
   novo->scNode = curScene.createSceneNode(false, cal3dName, 
         0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   /* Now insert it on the list */
   insert(novo);
   
   /* And set it as the active character */
   activeCharacter = novo;

   return(novo);
} 

/*********************************************************************
 *                          removeCharacter                          *
 *********************************************************************/
void characterList::removeCharacter(character* dude)
{
   /* Make shure curTreat still exits */
   if(dude == curTreat)
   {
      curTreat = NULL;
   }

   /* Remove the character from the list */
   remove(dude);
}

/*********************************************************************
 *                            freeElement                            *
 *********************************************************************/
void characterList::freeElement(dntListElement* dude)
{
   scene curScene;
   character* c = (character*)dude;
   curScene.deleteSceneNode(c->scNode);
   c->scNode = NULL;
   delete(c);
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
 *                              getCharacter                         *
 *********************************************************************/
character* characterList::getCharacter(string fileName)
{
   character* ch = (character*)first;
   int i;

   /* Search the list for it */
   for(i = 0; i < total; i++)
   {
      if(ch->getCharacterFile() == fileName)
      {
         return(ch);
      }
      ch = (character*)ch->getNext();
   }

   return(NULL);
}

/*********************************************************************
 *                              getCharacter                         *
 *********************************************************************/
character* characterList::getCharacter(sceneNode* scNode)
{
   character* ch = (character*)first;
   int i;

   /* Search the list for it */
   for(i = 0; i < total; i++)
   {
      if(ch->scNode == scNode)
      {
         return(ch);
      }
      ch = (character*)ch->getNext();
   }

   return(NULL);
}

/*********************************************************************
 *                         getNextSameCharacter                      *
 *********************************************************************/
character* characterList::getNextSameCharacter(character* ch)
{
   character* cur = (character*)ch->getNext();

   /* Search the list for it */
   while( (cur != ch) && (cur != first) )
   {
      if(cur->getCharacterFile() == ch->getCharacterFile())
      {
         return(cur);
      }
      cur = (character*)cur->getNext();
   }

   return(NULL);
}

/*********************************************************************
 *                            isCharacterIn                          *
 *********************************************************************/
bool characterList::isCharacterIn(character* ch)
{
   character* c = (character*)first;
   int i;

   /* Search the list for it */
   for(i = 0; i < total; i++)
   {
      if(c == ch)
      {
         return(true);
      }
      c = (character*)c->getNext();
   }

   return(false);
}

/*********************************************************************
 *                           setActiveCharacter                      *
 *********************************************************************/
void characterList::setActiveCharacter(character* dude)
{
   activeCharacter = dude;
}

/*********************************************************************
 *                                  update                           *
 *********************************************************************/
void characterList::update()
{
   int i;
   character* ch = (character*)first;
   for(i=0; i < total; i++)
   {
      ch->updateEffects();
      ch = (character*)ch->getNext();
   }
}

/*********************************************************************
 *                           treatGeneralScripts                     *
 *********************************************************************/
void characterList::treatGeneralScripts(Map* actualMap, characterList* NPCs)
{
   int i;
   iaScript* script;
   dialogWindow dlg;
   character* ch;

   for(i=0; i < CHARACTER_TREAT_SCRIPTS; i++)
   {
      if(total <= 0)
      {
         /* No characters -> Nothing to treat */
         return;
      }
      
      /* Set, if needed the current character to treat */
      if(!curTreat)
      {
         curTreat = (character*)first;
      }
            
      /* Get and treat script */
      script = (iaScript*) curTreat->getGeneralScript();
      if( (script) && (curTreat->isAlive()))
      {
         /* Only treat a character script, if no dialog with it */
         // TODO barter window too!
         ch = script->getCharacterOwner();
         if( (ch == NULL) || 
             (!dlg.isOpened((conversation*)ch->getConversation())))
         {
            script->defineMap(actualMap, NPCs);
            script->run(MAX_SCRIPT_LINES);
         }
      }

      /* Treat killed scripts (defined when character is killed, 
       * and removed from list when ends) */
      script = curTreat->killedScript;
      if((script) && (!curTreat->isAlive()))
      {
         script->defineMap(actualMap, NPCs);
         script->run(MAX_SCRIPT_LINES);
         if(script->finished())
         {
            /* Done with killed script, must clear it! */
            delete(curTreat->killedScript);
            curTreat->killedScript = NULL;
         }
      }

      /* forward on the list */
      curTreat = (character*)curTreat->getNext();
   }
}

