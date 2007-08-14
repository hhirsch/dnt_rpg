#include "partlist.h"


/***************************************************************************
 *                             Constructor                                 *
 ***************************************************************************/
particleList::particleList()
{
   total = 0;
   first = NULL;
}

/***************************************************************************
 *                              Destructor                                 *
 ***************************************************************************/
particleList::~particleList()
{
   int i;
   particleSystem* part = first;
   /* Remove the remaining systens. Usually, no systems are at the
    * destructor time, cause the remove here can make a memory leack. */
   for(i = 0; i < total; i++)
   {
      first = part;
      part = part->next;
      delete(first);
   }
}


/***************************************************************************
 *                               getFirst                                  *
 ***************************************************************************/
particleSystem* particleList::getFirst()
{
   return(first);
}


/***************************************************************************
 *                              getTotal                                   *
 ***************************************************************************/
int particleList::getTotal()
{
   return(total);
}

/***************************************************************************
 *                              addSystem                                  *
 ***************************************************************************/
void particleList::addSystem(particleSystem* part)
{
   if(first == NULL)
   {
      part->next = part;
      part->previous = part;
   }
   else
   {
      part->next = first;
      part->previous = first->previous;
      part->next->previous = part;
      part->previous->next = part;
   }
   first = part;
   total++;
}


/***************************************************************************
 *                            removeSystem                                 *
 ***************************************************************************/
void particleList::removeSystem(particleSystem* part)
{
   if(part == first)
   {
      first = part->next;
   }
   part->previous->next = part->next;
   part->next->previous = part->previous;
   total--;
   if(total == 0)
   {
      first = NULL;
   }
}

