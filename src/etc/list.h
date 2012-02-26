/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_list_h
#define _dnt_list_h

/*! The dntList types */
enum
{
   DNT_LIST_TYPE_ADD_AT_BEGIN=0,
   DNT_LIST_TYPE_ADD_AT_END
};

/*! The list element is the base class to define an element of a list */
class dntListElement
{
   public:
      /*! Constructor */
      dntListElement();
      /*! Destructor */
      ~dntListElement();

      /*! Get the next element
       * \return pointer to the next element */
      dntListElement* getNext();
      /*! Get the previous element
       * \return pointer to the previous element */
      dntListElement* getPrevious();

      /*! Set the next element
       * \param obj -> pointer to the next element on the list */
      void setNext(dntListElement* obj);
      /*! Set the previous element
       * \param obj -> pointer to the previous element on the list */
      void setPrevious(dntListElement* obj);

   protected:

      dntListElement* next;           /**< Next on list */
      dntListElement* previous;       /**< Previous on list */

};

/*! The basic chain list implementation */
class dntList
{
   public:
      /*! Constructor 
       * param t -> list type */
      dntList(int t=DNT_LIST_TYPE_ADD_AT_BEGIN);
      /*! Destructor
      * \note -> the children class must call clearList for 
       *          its root at its destructor  */
      virtual ~dntList();

      /*! Clear all list elements */
      void clearList();

      /*! Insert an element on the list, at the default position
       * \param obj -> object to insert
       * \return true if ok */
      bool insert(dntListElement* obj);

      /*! Insert element at the list's begin 
       * \param obj -> object to insert
       * \return true if ok */
      bool insertAtBegin(dntListElement* obj);

      /*! Insert element at list's end 
       * \param obj -> object to insert
       * \return true if ok */
      bool insertAtEnd(dntListElement* obj);

      /*! Remove an element from the list
       * \param obj -> element to remove
       * \note This function will delete the element
       *       calling freeElement when returning true.
       * \return true if ok */
      bool remove(dntListElement* obj);

      /*! Only remove the element from the list, without deleting it
       * \param obj -> obj to remove
       * \return true if ok */
      bool removeWithoutDelete(dntListElement* obj);

       /*! Get total elements on the list
       * \return total elements on the list */
      int getTotal();

      /*! Get the fisrt element
       * \return pointer to the first element */
      dntListElement* getFirst();

      /*! Get element from list */
      dntListElement* get(int i);

   protected:

      /*! Free an element (deleting it)
       * \param obj -> object to free */
      virtual void freeElement(dntListElement* obj)=0;

      dntListElement* first;          /**< Pointer to the first list element */
      int total;                      /**< Total elements on the list */

   private:
      int listType;                   /**< list type (FIFO, LIFO) */
};

#endif

