/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _listtext_h
#define _listtext_h

#include "rolbar.h"
#include "tabbutton.h"

/*! A element of text on listText */
class textElement
{
   public:
      string text;            /**< The text of the element */
      textElement* previous;  /**< Pointer to previous on list */
      textElement* next;      /**< Pointer to next on list */
};

/*! The listText class. Is a list of texts elements to select one */
class listText: public guiObject
{
   public:
      /*! Constructor 
       * \param xa -> x1 position
       * \param ya -> y1 position
       * \param xb -> x2 position
       * \param yb -> y2 position
       * \param surface -> window surface to draw
       * \param list -> pointer to the GUI objects list */
      listText(int xa, int ya, int xb, int yb, SDL_Surface* surface,
               void* list);
      /*! Destructor */
      ~listText();

      /*! Clear the text list */
      void clear();

      /*! Insert Text Element on the list
       * \param text -> string to insert */
      void insertText(string text);

      /*! Remove text from list
       * \param text -> string to remove */
      void removeText(string text);

      /*! Draw the listText to the surface */
      void draw(SDL_Surface* screen);

      /*! Verify if some event is related to the listText
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the listText */
      bool eventGot(int type, guiObject* object);

      /*! Get the selected text
       * \return selected text */
      string getSelectedText();

      rolBar* roll;           /**< RolBar of the list */
      tabButton* table;       /**< TabButton of the list */

   private:
      SDL_Surface* windowSurface; /**< Pointer to the window surface */
      void* intList;              /**< Internal GUI Objects List */
      textElement* first;         /**< Pointer to the first element on list */
      int totalElements;          /**< Total Elements on list */
      oneTabButton** listButtons; /**< Buttons of the List*/
      string selectedText;        /**< Text selected */
      int maxButtons;             /**< Max number of buttons */
};

#endif

