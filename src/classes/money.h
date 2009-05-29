
#ifndef _dnt_money_h
#define _dnt_money_h

#include "thing.h"
#include "object.h"

/*! money class is the money representation on DNT  */
class money: public object
{
   public:
      /*! Constructor  */
      money();

      /*! Destructor */
      ~money();

      /*! Get the current money quantity (value)
       * \return -> current money value */
      int quantity();

      /*! Remove quantity from current money
       * \param qty -> quantity to remove 
       * \return -> true if can remove, false if not enought money */
      bool removeQuantity(int qty);

      /*! Add a money quantity to the current money
       * \param qty -> quantity to add to current */
      void addQuantity(int qty);

      /*! Set a money quantity to the current money
       * \param qty -> quantity to set as current */
      void setQuantity(int qty);

      /*! Virtual method to call dead animation */
      void callDeadAnimation(){};

      /*! Virtual method to call attack animation */
      void callAttackAnimation(){};

      /*! Virtual method to call attack animation */
      void callIdleAnimation(){};

   protected:

      /*! Calculate the weight */
      void calculateWeight();

};


#endif

