/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _guilist_h
#define _guilist_h

#include <string>
using namespace std;

#include "guiobject.h"
#include "button.h"
#include "cxsel.h"
#include "picture.h"
#include "textbox.h"
#include "seltext.h"
#include "tabbutton.h"
#include "textbar.h"
#include "rolbar.h"
#include "listtext.h"
#include "filesel.h"

/*! The guiList Class: a list for guiObjects. */
class guiList
{
   public:
      /*! Constructor */
      guiList();
      /*! Destructor */
      ~guiList();

      /*! Remove Object From the List
       * \param obj -> pointer to the object to remove */
      void removeObject(guiObject* obj);

      /*! Insert a Button on List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate 
       * \param text -> button text
       * \param oval -> if the button is oval or rectangular
       * \return pointer to the button inserted */
      button* insertButton(int xa,int ya,int xb,int yb,
                           string text, bool oval);

      /*! Insert a new cxSel on the List
       * \param xa -> X coordinate
       * \param ya -> Y coordinate
       * \param selecionada -> if selected or not */
      cxSel* insertCxSel(int xa, int ya, bool selecionada);

      /*! Insert a new picture on the list
       * \param x -> x initial coordinate
       * \param y -> y initial coordinate
       * \param w -> width of the picture
       * \param h -> height of the picture
       * \param arquivo -> fileName to opwn or NULL to not open a file
       * \return pointer to the picture inserted */
      picture* insertPicture(int x,int y,int w,int h,const char* arquivo);

      /*! Insert a new tabButton on the list
       * \param x -> x initial coordinate
       * \param y -> y initial coordinate
       * \param w -> width of the picture
       * \param h -> height of the picture
       * \param arquivo -> fileName to opwn or NULL to not open a file 
       * \return tabbutton inserted */
      tabButton* insertTabButton(int x,int y,int w,int h,const char* arquivo);

      /*! Insert a textBar on the List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate 
       * \param yb -> y2 coordinate
       * \param text -> textBar text
       * \param cript -> if is crypt, or not
       * \return textBar inserted */
      textBar* insertTextBar(int xa,int ya,int xb,int yb, string text, 
                             int cript);

      /*! Insert a textBox on the List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param frameType -> type of the frame. 0 for no frames
       * \param text -> text of the textBox
       * \return textBox inserted */
      textBox* insertTextBox(int xa,int ya,int xb,int yb,
                             int frameType, string text);

      /*! Insert a selText on the list
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param text0 -> option 0 text
       * \param text1 -> option 1 text
       * \param text2 -> option 2 text
       * \param text3 -> option 3 text
       * \param text4 -> option 4 text
       * \return selText inserted */
      selText* insertSelText(int xa,int ya,int xb,int yb,string text0,
                             string text1,string text2,string text3, 
                             string text4);

      /*! Insert a rolBar on the List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param txt -> text on the rolbar
       * \return rolBar inserted */
      rolBar* insertRolBar(int xa,int ya,int xb,int yb,string txt);

      /*! Insert a listText on the List
       * \param xa -> x1 position
       * \param ya -> y1 position
       * \param xb -> x2 position
       * \param yb -> y2 position
       * \return listText inserted */
      listText* insertListText(int xa,int ya,int xb,int yb);

      /*! Insert a fileSel on the list
       * \param xa -> x coordinate
       * \param ya -> y coordinate
       * \param load -> true if the selector is a loading one, false if it
       *                is a saving one.
       * \param dir -> current dir to browse
       * \return -> pointer to the fileSel inserted */
      fileSel* insertFileSel(int xa, int ya, bool load, string dir);

      /*! Add a internal Menu to the list
       * \return menu Inserted */
      guiObject* addMenu();

      /*! Get the Internal Menu of The List
       * \return internal menu */
      guiObject* getMenu();

      /*! Remove Internal Menu, if exists one. */
      void removeMenu();

      /*! Set the window Surface to use */
      void setSurface(SDL_Surface* surface){wSurface=surface;};

      /*! Get the first object on the list
       * \return pointer to first object on list */
      guiObject* getFirst(){return(first);};

      /*! Get total objects on list
       * \return number of objects on the list */
      int getTotal(){return(total);};

   protected:
      /*! Insert the object on the list, actualizing the pointers
       * \param obj -> pointer to guiObject to insert */
      void insertObject(guiObject* obj);

      SDL_Surface* wSurface;  /**< window surface used */

      int total;         /**< Total Itens on List */
      guiObject *first;  /**< Head Node */
      guiObject* intMenu; /**< The internal Menu of the List */

};

#endif

