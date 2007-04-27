#ifndef _mapobjeto_h
#define _mapobjeto_h

/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/objeto.h"
#include "../gui/lista.h"
#include "../etc/modellist.h"
#include "../classes/object.h"
#include <string>
using namespace std;


/*!
 ****************************************************
 *               Map's Objects                      *
 ****************************************************/
class mapObject: public object
{
   public:
      /*!Constructor 
       * \param path -> path to the object file
       * \param mdlList -> list of openned models */
      mapObject(string path, modelList& mdlList):
         object(path, mdlList){};

      /*! Verify if Map object can be got
       * \return true if the object is pickable */
      bool canGet();

      mapObject* next;        /**< Next Object on List */
      mapObject* previous;    /**< Previous Object on List */

   protected:
      void callDeadAnimation();
};

/*! Map Objects list */
class lMapObject
{
    public:
       /*! Constructor */
       lMapObject();
       /*! Destructor */
       ~lMapObject();

       /*!
        * Insert object on map list
        * \param arquivo -> filename to load
        * \param mdlList -> modelList
        * \return pointer to the map Object created */
       mapObject* insertMapObject(string arquivo, modelList& mdlList);
       /*!
        * Search for object on map list
        * \param fileName -> object file name 
        * \return pointer to the map Object founded. */
       mapObject* getMapObject(string fileName);

       /*!
        * Remove object from list
        * \param obj-> pointer to the object to remove. */
       void removeMapObject(mapObject* obj);

       mapObject* first;
       int total;

}; 

#endif
