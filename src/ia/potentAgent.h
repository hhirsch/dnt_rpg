/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#ifndef _dnt_potentAgent_h
#define _dnt_potentAgent_h

#include "agent.h"

class potentAgent: public agent
{
   public:
      potentAgent(bool oriented);
      ~potentAgent();

      /*! Defines Next Agent position. Virtual. 
       * \return true if position was defined */
      bool defineNextPosition();

      /*! Defines the agent Constants
       * \param a -> Atraction constant
       * \param c -> Collision constant
       * \param r -> Repulsion constant */
      void defineConstants(GLfloat a, GLfloat c, GLfloat r);
      
   private:
      GLfloat ka, kc, kr;
};
                   


#endif

