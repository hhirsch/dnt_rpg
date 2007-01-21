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
       * \param objName -> name of the object
       * \param mdlList -> list of openned models */
      mapObject(string path, string objName, modelList& mdlList):
         object(path, objName, mdlList){};

      /*! Verify if Map object can be got
       * \return true if the object is pickable */
      bool canGet();

      mapObject* next;        /**< Next Object on List */
      mapObject* previous;    /**< Previous Object on List */
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
        * \param nome -> object name 
        * \param mdlList -> modelList
        * \return pointer to the map Object created */
       mapObject* insertMapObject(string arquivo, string nome, 
                                  modelList& mdlList);
       /*!
        * Search for object on map list
        * \param name -> object name 
        * \return pointer to the map Object founded. */
       mapObject* getMapObject(string name);

       /*!
        * Remove object from list
        * \param obj-> pointer to the object to remove. */
       void removeMapObject(mapObject* obj);

       mapObject* first;
       int total;
}; 

#endif
