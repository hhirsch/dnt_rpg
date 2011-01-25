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

#ifndef _dnt_part_element_h
#define _dnt_part_element_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;

#define DNT_PART_ELEMENT_CONSTANT   0    /**< Constant (only the initial) */
#define DNT_PART_ELEMENT_RANDOM     1    /**< Randomically changes over time */
#define DNT_PART_ELEMENT_LINEAR     2    /**< Linear variation over time */
#define DNT_PART_ELEMENT_RANDOM_INITIAL 3 /*< Random at init and constant */

/*! Define a single element that will variate at a particle system
 *  (for example, position, color, scale factor, etc.) */
class dntPartElement
{
   public:
      /*! Constructor */
      dntPartElement();

      /*! Set the element constant over time
       * \param i -> constant value of the element */
      void setConstant(float i);

      /*! Set the element as linear variation
       * \param i -> initial value 
       * \param f -> end value 
       * \param maxSteps -> number of steps */
      void setLinear(float i, float f, int maxSteps);

      /*! Set the element to be random at its init and constant over time
       * \param i -> base value 
       * \param m -> multiplier to the random
       * \param s -> sum to the random value */
      void setRandomInitial(float i, float m, float s);

      /*! Set the element as random variation
       * \param i -> base value 
       * \param m -> multiplier to the random
       * \param s -> sum to the random value */
      void setRandom(float i, float m, float s);

      /*! Set limits to the value
       * \param lower -> lower limit value
       * \param upper -> upper limit value */
      void setLimits(float lower, float upper);

      /*! Populate the dntPartElement from an string
       * \param s -> string with dntPartElement info
       * \return true -> if could populate */
      bool fromString(string s);

      /*! Convert the dntPartElement to an string (used to save it on files)
       * \return string containing the dntPartElement info or empty, 
       *         if not used */
      string toString();

      /*! Update current value, based on definitions
       * \param curValue -> current value (will receive the new one)
       * \return true on success */
      bool updateValue(float& curValue);

      /*! Get if the element is currently static
       * \return true if no variation on last iteration */
      bool getIsStatic();

      /*! Get if the dntPartElement is used or not
       * \return true if used */
      bool isUsed();

      /*! Get initial value of the element, based on type rule
       * \return -> initial value */
      float getInitialValue();

      /*! As friend to avoid useless functions */
      friend class partElementWindow;

   protected:
      float initialValue;     /**< Initial element value */
      float finalValue;       /**< Final Value (used at linear type) */

      float lowerLimit;       /**< Lower value limit */
      float upperLimit;       /**< Upper value limit */
      bool limitsDefined;     /**< If limits are defined or not */

      float mult;             /**< Multiplier factor (used at random type) */
      float sum;              /**< Sum factor */

      bool used;              /**< If the element is used or not */

      int type;               /**< Element variation type */

      bool isStatic;          /**< If last value is equal to current value
                                   (no variations on last iteration) */
};


#endif

