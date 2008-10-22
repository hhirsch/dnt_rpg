#ifndef _mapobjeto_h
#define _mapobjeto_h

/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "../etc/modellist.h"
#include "../classes/object.h"
#include "../classes/weapon.h"
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

   protected:
      void callDeadAnimation();
      void updateHealthBar(){};
};

/*! Objects list */
class lObject
{
    public:
       /*! Constructor */
       lObject();
       /*! Destructor */
       ~lObject();

       /*!
        * Insert object on map list
        * \param arquivo -> filename to load
        * \param mdlList -> modelList
        * \param wTypes -> list of weapons types
        * \return pointer to the map Object created */
       object* insertObject(string arquivo, modelList& mdlList,
                            weaponTypes& wTypes);
       /*!
        * Search for object on map list
        * \param fileName -> object file name 
        * \return pointer to the map Object founded. */
       object* getObject(string fileName);

       /*!
        * Remove object from list
        * \param obj-> pointer to the object to remove. */
       void removeObject(object* obj);

       /*! Remove all unused objects */
       void removeUnusedObjects();

       object* first;  /**<  First object on the list */
       int total;      /**< Total objects on the list */

}; 

#endif
