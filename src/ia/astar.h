#ifndef _astar_h
#define _astar_h

#include "../map/map.h"
#include "pattAgent.h"

class pointStar
{
   public:
      wayPoint wp;
      pointStar* up;
      pointStar* down;
      pointStar* left;
      pointStar* right;
};

/*! A* implementation */
class aStar
{
   public:
      aStar(Map* map);
      ~aStar();

   private:
      Map* actualMap;
};


#endif

