/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "../gui/draw.h"
#include "../ia/iascript.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include "../lang/translate.h"

#include <SDL/SDL_image.h>
#include <iostream>
using namespace std;

#define BORDER_FILE "characters/portraits/borda.png"
#define BORDER_SIZE 5

#define CHARACTER_TREAT_SCRIPTS  5

/*********************************************************************
 *                             constructor                           *
 *********************************************************************/
character::character(featsList* ft)
{
   int i;

   /* Set the thing type */
   thingType = THING_TYPE_CHARACTER;

   /* Create the Health Bar */
   lifeBar = new healthBar(5,64,59,74);

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
   cr = 0;

   inventories = new inventory;

   /* Feat Details */
   if(ft != NULL)
   {
      actualFeats.insertFeat(ft->featByNumber(FEAT_MELEE_ATTACK));
      actualFeats.insertFeat(ft->featByNumber(FEAT_RANGED_ATTACK));
   }

   /* Effects */
   effects = new modEffectList();

   /* Scripts */
   generalScript = NULL;
   generalScriptFileName = "";

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
      iaScript* isc = (iaScript*)generalScript;
      delete(isc);
      generalScript = NULL;
   }
   if(effects)
   {
      delete(effects);
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
void* character::getGeneralScript()
{
   return(generalScript);
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
      printf("Can't Load Portrait File: %s\n", portraitFile.c_str());
      return;
   }

   /* Load Border */
   SDL_Surface* border = IMG_Load(dir.getRealFile(BORDER_FILE).c_str());
   if(!border)
   {
      SDL_FreeSurface(img);
      printf("Can't Load Border File: %s\n", BORDER_FILE);
      return;
   }

   /* Blit Image With Border */
   SDL_Rect rect;
   rect.x = BORDER_SIZE;
   rect.y = BORDER_SIZE;
   SDL_BlitSurface(img, NULL, border, &rect);

   /* Put With space for health bar */
   portraitImage = SDL_CreateRGBSurface(SDL_SWSURFACE,border->w,border->h+10,32,
                                        0x000000FF,0x0000FF00,
                                        0x00FF0000,0xFF000000);
   /* Fix to the blit get the alpha from source! */
   SDL_SetAlpha(border, 0, 0);
   SDL_BlitSurface(border,NULL,portraitImage,NULL);

   /* Free Things */
   SDL_FreeSurface(img);
   SDL_FreeSurface(border);

   /* Load Texture */
   setTextureRGBA(portraitImage, portraitTexture);
   portraitPropX = portraitImage->w /
                   (float)smallestPowerOfTwo(portraitImage->w);
   portraitPropY = portraitImage->h /
                   (float)smallestPowerOfTwo(portraitImage->h);

   /* Define fileName */
   talkPortrait = portraitFile;
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
  lifeBar->draw(portraitImage);
  setTextureRGBA(portraitImage, portraitTexture);
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
      glVertex2f(SCREEN_X-portraitImage->w-1, SCREEN_Y-1);
      glTexCoord2f(0.0f, portraitPropY);
      glVertex2f(SCREEN_X-portraitImage->w-1, SCREEN_Y-portraitImage->h-1);
      glTexCoord2f(portraitPropX, portraitPropY);
      glVertex2f(SCREEN_X-1, SCREEN_Y-portraitImage->h-1);
      glTexCoord2f(portraitPropX, 0.0f);
      glVertex2f(SCREEN_X-1, SCREEN_Y-1);
   glEnd();

   glDisable(GL_TEXTURE_2D);
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
   setState(STATE_DIE);
}

/******************************************************
 *                    instantKill                     *
 ******************************************************/
void character::instantKill()
{
   dead = true;
   setState(STATE_DEAD);
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
   orientation = ori;
   pathFind.setOrientation(ori);
}

/*********************************************************************
 *                             defineWeapon                          *
 *********************************************************************/
void character::defineWeapon()
{
   object* obj = inventories->getFromPlace(INVENTORY_LEFT_HAND);
   weapon* wp = (weapon*) obj;
   
   /* Load the Left Hand Weapon, if one is, and if is different than the
    * current one. */
   if(obj)
   {
      if( (obj->getName()) != (leftHandModel.modelName))
      {
         leftHandModel.modelName = obj->getName();
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
         addModel(obj->get3dModel());

         /* Define the weapon */
         actualFeats.defineWeapon(wp);
      }
   }
   else
   {
      if(leftHandModel.modelName != "Bare Hands")
      {
         leftHandModel.modelName = "Bare Hands";
         if(!inventories->getFromPlace(INVENTORY_RIGHT_HAND))
         {
            /* Detach the mesh */
            //TODO

            /* Define as without weapons */
            actualFeats.defineWeapon(NULL);
         }
      }
   }   

   obj = inventories->getFromPlace(INVENTORY_RIGHT_HAND);
   wp = (weapon*) obj;
   /* Load the Right Hand Weapon, if one is, and if is different than the
    * current one. */
   if(obj)
   {
      if( (obj->getName()) != (rightHandModel.modelName))
      {
         rightHandModel.modelName = obj->getName();
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
         addModel(obj->get3dModel());

         /* Define the weapon */
         actualFeats.defineWeapon(wp);
      }
   }
   else
   {
      /* Detach the mesh */
      //TODO
      
      /* Define the weapon to bare hands */
      if(rightHandModel.modelName != "Bare Hands")
      {
         rightHandModel.modelName = "Bare Hands";
         if(!inventories->getFromPlace(INVENTORY_LEFT_HAND))
         {
            actualFeats.defineWeapon(NULL);
         }
      }
   }

}

/*********************************************************************
 *                       getActiveFeatRangeType                      *
 *********************************************************************/
int character::getActiveFeatRangeType()
{
   return(actualFeats.getAttackFeatRangeType());
}

/*********************************************************************
 *                         getActiveFeatRange                        *
 *********************************************************************/
int character::getActiveFeatRange()
{
   return(actualFeats.getAttackFeatRange());
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
   file << "model = " << modelFileName << endl;
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
   /* Battle Script (if one) */
   if(!battleScriptFileName.empty())
   {
      file << "battleScript = " << battleScriptFileName << endl;
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
   /* Up Levels */
   file << "upLevels = " << upLevels << endl;
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

   /* TODO, save all feats! */

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
                                          void* pEngine, string curMap)

{
   defParser def;
   races racesDesc;
   aligns alignsDesc;
   classes classesDesc;
   string arqModelo;
   string key, value;
   char buf[256];
   int lvl;
   int curClass = 0;
  
   /* Create the Character */ 
   character* novo;
   novo = new character(ft);
   novo->orientation = 0.0f;
   novo->xPosition = 0.0f;
   novo->zPosition = 0.0f;
   novo->yPosition = 0.0f;

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
         arqModelo = value;
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
         iaScript* isc = new iaScript(value, pEngine);
         novo->generalScript = isc;
         novo->generalScriptFileName = value;
         /* Set the owner */
         isc->defineCharacterOwner(novo);
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
      else if(key == "upLevels")
      {
         sscanf(value.c_str(),"%d", &novo->upLevels);
      }
      /* Inventory Population */
      else if(key == "inventory")
      {
         novo->inventoryFile = value;
      }

      //TODO
      /*else if(key == "lifeDice")
      {
         sscanf(value.c_str(), "d%d", &novo->lifeDice);
      }
      else if(key == "race")
      {
         novo->race = numberConstant(value);
      }
      else if(key == "class")
      {
         novo->cclass = numberConstant(value);
      }
      else if(key == "tendency")
      {
         novo->tendency = numberConstant(value);
      }*/

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
         //TODO FEATS.
      }

   }
  
   /* Define AC TODO others values to sum here*/ 
   novo->armatureClass = 10+novo->sizeModifier+novo->attBonus(ATT_DEXTERITY);

   /* Apply Saves and Bonus and Costs */
   novo->applyBonusAndSaves();
   novo->applySkillCosts();
   
   /* Load The 3D Model */ 
   novo->loadModel(arqModelo);

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
   character* c = (character*)dude;
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
 *                           setActiveCharacter                      *
 *********************************************************************/
void characterList::setActiveCharacter(character* dude)
{
   activeCharacter = dude;
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

      /* forward on the list */
      curTreat = (character*)curTreat->getNext();
   }
}

