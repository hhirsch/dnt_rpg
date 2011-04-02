/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_feats_h
#define _dnt_feats_h

#include <SDL/SDL.h>
#include "defs.h"
#include <string>
using namespace std;

#include "thing.h"
#include "modifier.h"
#include "modeffect.h"
#include "weapon.h"

#include "../etc/message3d.h"
#include "../etc/list.h"
#include "../engine/briefing.h"
#include "../lang/translate.h"

#define MAX_FEATS          30 /**< Max number of Feats per Character */

enum
{
   FEAT_TYPE_PERMANENT=0,  /**< Permanent feat (infinite modEffect type) */
   FEAT_TYPE_ON_TARGET,    /**< Use-on-target feat */
   FEAT_TYPE_ON_AREA,      /**< Use-on-area feat */
   FEAT_TYPE_TOTAL
};


#define FEAT_WEAPON_ATTACK   0 /**< Weapon Attack */

/*! Dependence Feats are feats that its quantity is affected by the use of 
 * the owner feat. \par
 *
 * For Example: 
 * when use \e ameivasII you'll can do -2 \e ameivaI. (1/2 reason). */
class depFeat: public dntListElement
{
   public:
      float  reason;       /**< Dependence Reason (1/1, 1/2, 1/3, 2/1, etc) */
      string featId;       /**< ID String of the Feat */
};
/*! The dependence feat list (depFeat) */
class depFeatList: public dntList
{
   public:
       /*! Destructor */
       ~depFeatList();
   protected:
      /*! Free Element
       * \param obj -> pointer to the depFeat to delete */
      void freeElement(dntListElement* obj);
      
};
/*! Required factor */
class reqFactor: public dntListElement
{
   public:
      factor requiredFactor;  /**< Factor required */
      int requiredLevel;      /**< Factor level required */
};
/*! The required factor list (reqFactor) */
class reqFactorList: public dntList
{
   public:
       /*! Destructor */
       ~reqFactorList();
   protected:
      /*! Free Element
       * \param obj -> pointer to the depFeat to delete */
      void freeElement(dntListElement* obj);
      
};

/*! The feature general definitions */
class featDescription
{
   public:
      /*! Constructor */
      featDescription();
      /*! Destructor */
      ~featDescription();

      int type;                    /**< Feature type constant */
      int action;                  /**< Feat action (as <actions.h>) */

      string name;                 /**< Feat Name */
      string idString;             /**< Feat ID String */
      string description;          /**< Feat Description */

      string scriptFile;           /**< FileName of the script to run */

      int internalListNumber;      /**< Number on List */
      reqFactorList reqFactors;    /**< List of required factors */

      int powerLevel;              /**< Feat power level (to compare them) */
      
      int costToUse;               /**< Cost (in quantity) to use the feat */
      int quantityPerDay;          /**< Quantity avaible to use per day*/
      int aditionalQuantity;       /**< Quantity Added per AditionalLevel */
      int aditionalLevel;          /**< Number of Levels to AditionalQuantity */
      int aditionalDiv;            /**< The division factor */
      factor aditionalFactor;      /**< The factor that will allows aditional */

      int range;                   /**< Range action of the feat */

      depFeatList depFeats;        /**< Feat Dependency */
      
      SDL_Surface* image;          /**< Feat Image */
};

/*! The feat is the internal feat definition per characters,
 * with only the requerided fields to avoid loss memory with
 * unnecessary data. */
class feat
{
   public:
      featDescription* info;       /**< The feat info */
      float actualQuantity;        /**< Actual quantity to use */
      int featNumber;              /**< Number on feats */
};

class engine;

/*! the class feats defines the Character's Feats,
 * and its functions to up-to-date, use, etc. */
class feats               
{
   public:
      /*! feats Constuctor */
      feats(engine* usedEngine);
      /*! feats Destructior */
      ~feats();
      
      /*! Clear all feats */
      void clear();

      /*! Get total feats */
      int getTotal();

      /*! Return the feat with number featNumber  
       * \param featNumber -> internal number of feat to return
       * \return the feat struct with internal number. */
      feat* featByNumber(int featNumber);

      /*! Return the feat with name featName
       * \param featName -> name of feat to return
       * \return return the feat struct that has the name. */
      feat* featByString(string featName);

      /*! Insert a feat on Character's Feats.
       * \param featInsert -> featDescription of feat to insert
       * \return \c true if sucefully inserted. */
      bool insertFeat(featDescription* featInsert);

      /*! Refresh Quantities to use on a new day to all feats. */
      void newDay();

      /*! Get a random attack feat that can be used.
       * \param pers -> character that will use the feat
       * \param target -> thing that will receive the feat.
       * \return \c number of the feat got */
      int getRandomNPCAttackFeat(thing* pers, thing* target);

      /*! Get the powerfull attack feat that can be used.
       * \param pers -> character that will use the feat
       * \param target -> thing that will receive the feat.
       * \return \c number of the feat got */
      int getPowerfullAttackFeat(thing* pers, thing* target);

      /*! Get the first heal feat available
       * \param pers -> thing owner of the feat
       * \return feat Number or -1 if not found */
      int getFirstHealFeat(thing* pers);

      /*! Get a random heal feat available
       * \param pers -> thing owner of the feat
       * \return feat Number or -1 if not found */
      int getRandomHealFeat(thing* pers);

      /*! Get the powerfull heal feat available
       * \param pers -> thing owner of the feat
       * \return feat Number or -1 if not found */
      int getPowerfullHealFeat(thing* pers);

      /*! Use a feat at a target thing
       * \param actor -> thing owner of the feat
       * \param featNumber -> number of the feat to use
       * \param target -> target to use the feat 
       * \return true if used the feat, false if couldn't */
      bool useFeatAtTarget(thing* actor, int featNumber, thing* target);

      /*! Use a feat at a target area
       * \param actor -> thing owner of the feat
       * \param featNumber -> number of the feat to use
       * \param x -> X coordinate to use the feat
       * \param y -> Y coordinate to use the feat
       * \param z -> Z coordinate to use the feat
       * \return true if used the feat, false if couldn't */
      bool useFeatAtArea(thing* actor, int featNumber, 
             float x, float y, float z);

      /*! Apply a permanent feat to the owner
       * \param actor -> thing owner of the feat
       * \param featNumber -> number of the feat to use
       * \return true if used the feat, false if couldn't */
      bool applyPermanentFeat(thing* actor, int featNumber);

      /*! Verify if can use the feat
       * \param featNumber -> number of the feat
       * \return true if can use */
      bool canUse(int featNumber);
      /*! verify if can use the feat
       * \param f -> feat pointer to test
       * \return true if can use */
      bool canUse(feat* f);

   private:

      feat m_feats[MAX_FEATS];  /**< Internal Feats Struct */
      int  totalFeats;          /**< Actual Number of Feats */
      engine* uEngine;          /**< Pointer to the used engine */

      /*! Use a quantity of the feat and propagate to other dependent 
       * feats.
       * \param featNumber -> feat to use */
       void useFeat(int featNumber);
};

/*! List of All Feats on Game */
class featsList           
{
   public:
      /*!
       ***************************************************************
       * featsList Constructor
       * \param dir -> string with the directory where all feats are
       * \param arq -> file name with the list of avaible feats.
       ***************************************************************/
      void init(string dir, string arq);
      /*!
       ***************************************************************
       * featsList Destructor
       ***************************************************************/
      void finish();

      /*!
       ************************************************************** 
       * Return the featDescription with idString featName
       * \param featName -> name of feat to return
       * \return the featDescription struct.
       ***************************************************************/
      featDescription* featByString(string featName);

      /*!
       **************************************************************** 
       * Return the feat with number featNumber  
       * \param featNumber -> number of feat to return
       * \return the featDescription struct.
       ***************************************************************/
      featDescription* featByNumber(int featNumber);

      /*! Get the total number of feats */
      int getTotal();

   private:
      static featDescription* m_feats; /**< Internal Desc Struct */
      static int totalFeats;           /**< Total Declared Feats */
};

#endif

