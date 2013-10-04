/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_potentAgent_h
#define _dnt_potentAgent_h

#include "agent.h"

class potentAgent: public agent
{
   public:
      potentAgent(bool oriented);
      ~potentAgent();

      /*! Defines Next Agent position.
       * \param run -> true if running, false if walking. NOT used here.
       * \return true if position was defined */
      bool defineNextPosition(bool run);

      /*! Defines the agent Constants
       * \param a -> Atraction constant
       * \param c -> Collision constant
       * \param r -> Repulsion constant */
      void defineConstants(GLfloat a, GLfloat c, GLfloat r);
      
   private:
      GLfloat ka, kc, kr;
};
                   


#endif

