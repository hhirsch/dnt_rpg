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

#define DNT_PART_ELEMENT_RANDOM     1    /**< Randomically changes over time */
#define DNT_PART_ELEMENT_LINEAR     2    /**< Linear variation over time */

/*! Define a single element that will variate at a particle system
 *  (for example, position, color, scale factor, etc.) */
class dntPartElement
{
   public:
      /*! Constructor */
      dntPartElement();

      /*! Set the element as linear variation
       * \param i -> initial value 
       * \param f -> end value 
       * \param maxSteps -> number of steps */
      void setLinear(float i, float f, int maxSteps);

      /*! Set the element as random variation
       * \param i -> initial value 
       * \param m -> multiplier to the random
       * \param s -> sum to the random value */
      void setRandom(float i, float m, float s);

      /*! Set limits to the value
       * \param lower -> lower limit value
       * \param upper -> upper limit value */
      void setLimits(float lower, float upper);

      /*! Update current value, based on definitions
       * \return true on success */
      bool updateValue();

      /*! Get the current element value
       * \return -> current value */
      float getCurrentValue();

      /*! Get if the element is currently static
       * \return true if no variation on last iteration */
      bool getIsStatic();

   protected:
      float initialValue;     /**< Initial element value */
      float finalValue;       /**< Final Value (used at linear type) */
      float curValue;         /**< Current Value */
      float lastValue;        /**< Last Value */

      float lowerLimit;       /**< Lower value limit */
      float upperLimit;       /**< Upper value limit */
      bool limitsDefined;     /**< If limits are defined or not */

      float mult;             /**< Multiplier factor (used at random type) */
      float sum;              /**< Sum factor */

      int type;               /**< Element variation type */

      bool isStatic;          /**< If last value is equal to current value
                                   (no variations on last iteration) */
};


#endif

