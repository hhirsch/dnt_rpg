
#ifndef _dnt_money_h
#define _dnt_money_h

#include "thing.h"

/*! money class is the money representation on DNT  */
class money: public thing
{
   public:
      /*! Constructor  */
      money();

      /*! Destructor */
      ~money();

      /*! Remove quantity from current money
       * \param qty -> quantity to remove 
       * \return -> true if can remove, false if not enought money */
      bool removeQuantity(int qty);

      /*! Add a money quantity to the current money
       * \param qty -> quantity to add to current */
      void addQuantity(int qty);

      /*! Virtual method to call dead animation */
      void callDeadAnimation(){};

      /*! Virtual method to call attack animation */
      void callAttackAnimation(){};

      /*! Virtual method to call attack animation */
      void callIdleAnimation(){};

   protected:
};


#endif

