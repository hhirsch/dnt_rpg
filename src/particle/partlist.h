#ifndef _partlist_h
#define _partlist_h

#include "particle.h"

/*! List of particles Systems  */
class particleList
{
   public:
      /*! Constructor  */
      particleList();
      /*! Destructor */
      ~particleList();

      /*! Get the first element on the list
       * \return -> pointer to the first element on the list */
      particleSystem* getFirst();

      /*! Get the total systems on the list
       * \return -> total elements on the list */
      int getTotal();

      /*! Add a particle System to the list
       * \param part -> system to add */
      void addSystem(particleSystem* part);

      /*! Remove a particle System from the list. Note: it don't free 
       * the system's structs, so, delete them after remove!
       * \param part -> system to remove */
      void removeSystem(particleSystem* part);

   protected:
      particleSystem* first;     /**< First Element on the list */
      int total;                 /**< Total Elements on the list */
};

#endif

