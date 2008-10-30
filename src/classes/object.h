#ifndef _object_h
#define _object_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL.h>
#include <string>
using namespace std;

#include "../etc/modellist.h"
#include "thing.h"
#include "dices.h"

#define OBJECT_TYPE_GENERIC   0
#define OBJECT_TYPE_SCENERY   1
#define OBJECT_TYPE_WEAPON    2
#define OBJECT_TYPE_ARMOR     3
#define OBJECT_TYPE_HEAL      4
#define OBJECT_TYPE_EXPLOSIVE 5
#define OBJECT_TYPE_AMMO      6

/*! Define Objects of the DNT (an object is almost everything that 
 * isn't live (like chairs, doors, equipments, weapons, etc).
 * \note -> the object is automatically added to objectsList
 *          in its constructos, and removed in its desctructor. */
class object: public thing
{
   public:
      /*! Constructor 
       * \param path -> path to the object file
       * \param mdlList -> list of openned models */
      object(string path, modelList& mdlList, string curMap);
      /*! Constructor
       * \param obj -> some object to be the source of actual */
      object(object* obj);
      /*! Constructor withou parameter. Do not load anyhing */
      object(string path);
      /*! Destructor */
      ~object();

      /*! Verify if Map object can be got
       * \return true if the object is pickable */
      bool canGet();

      /*!
       * Draws the object
       * \param inverted -> to invert the Y position */
      void draw(bool inverted);

      /*! Draw the 2D Model to Surface
       * \param x -> x value on surface
       * \param y -> y value on surface
       * \param surface -> surface where will draw the 2D version. */
      void draw2D(int x, int y, SDL_Surface* surface);

      /*! Gets the actual Bounding box of the model 
       * \return -> the boundingBox of the model */
      boundingBox getBoundingBox();

      /*! Gets the name of the model
       * \return -> the name of the model*/
      string getName();

      /*! Gets the fileName of the model
       * \return -> the fileName of the model*/
      string getFileName();

      /*! Gets the Inventory size of the Model
       * \param x -> x inventory size
       * \param y -> y inventory size */
      void getInventorySize(int &x, int &y);

      /*! Call the dead animation of the model */
      void callDeadAnimation();

      /*! Call the attack animation of the model */
      void callAttackAnimation();

      /*! Call the idle animation of the model */
      void callIdleAnimation();

      /*! Update the health bar
       * \note not used */
      void updateHealthBar(){};

      /*! Get the type of the object */
      int getType();

      /*! Verify if the object is a Scenery one */
      bool isStaticScenery();

      /*! Verify if the item is usable or not
       * \return -> true if the item is usable */
      bool isUsable();

      /*! Add render position to the object.
       * \note -> this function ios only valid if the object is a 
       *          static scenery one. 
       * \param x -> x coordinate
       * \param y -> y coordinate
       * \param z -> z coordinate 
       * \param angle -> object angle */
      void addRenderPosition(float x, float y, float z, float angle);

      /*! Get the 2D image of the object
       * \return the pointer to the surface */
      SDL_Surface* get2dModel();

      /*! Get the 2D model name */
      string get2dModelName();

      /*! Verify, with all Meshes of the model, if the model colides
       *  with the bounding box defined as parameter. 
       *  \param angle -> model angle
       *  \param pX -> model X position
       *  \param pY -> model Y position
       *  \param pZ -> model Z position
       *  \param colMin -> colider min values of bounding box
       *  \param colMax -> colider max values of bounding box 
       *  \return -> true if one or more meshes colides, false otherwise */
      bool depthCollision(GLfloat angle, GLfloat pX, GLfloat pY, GLfloat pZ,
                          GLfloat colMin[3], GLfloat colMax[3]);

      /*! Get the related object dice information
       * \return -> diceThing of the object */
      diceThing getDiceInfo();

      /*! Get the next object on the list
       * \return -> pointer to the next */
      object* getNext();
      /*! Get the previous object on the list
       * \return -> pointer to the previous */
      object* getPrevious();

      /*! Set the next object on the list
       * \param o -> pointer to the next */
      void setNext(object* o);
      /*! Set the previous object on the list
       * \param o -> previous object */
      void setPrevious(object* o);

   protected:
      int usedFlag;         /**< The used flag of an object */
      int inventSizeX,      /**< Size on inventory X axis */
          inventSizeY;      /**< Size on inventory Y axis */

      diceThing diceInfo;   /**< The object's dice for action */

      bool staticScenery;   /**< True if is static scenery */

      int type;             /**< Type of the object */

      model3d* model3D;     /**< Pointer to used 3D Model Maximun Quality */
      SDL_Surface* model2d; /**< Pointer to used 2D Model */
      
      string fileName;      /**< FileName of the Object */
      string model2dName;   /**< FileName of the 2D Model */

      object* next;         /**< Next Object on List */
      object* previous;     /**< Previous Object on List */ 

      /*! Init the values (all with null or zero). Usually called
       * at begining of the constructors. */
      void cleanValues();

};

/*! The object list. All active objects on the game are here.
 * Objects are removed at map exit and added at map enter.
 * Also, objects are kept in the lis when in inventory. */
class objectsList
{
   public:
      /*! Remove an object from the list
       * \param o -> object to remove from the list
       * \note -> usually called at object's contructor */
      static void remove(object* o);

      /*! Insert an object to the list
       * \param o -> object to add to the list
       * \note -> usually called at object's destructor */
      static void insert(object* o);

      /*! Remove all static sceneries from the list,
       * deleting its pointers. */
      static void removeStaticSceneries();

      /*! Remove All objects from the list, deleting its pointers */
      static void removeAll();

      /*! Search for the object on the list
       * \return -> pointer to the object found or NULL
       * \note -> static sceneries objects always use the same object pointer */
      static object* search(string fileName, GLfloat posX, GLfloat posY,
                            GLfloat posZ);

   protected:
      static object* first;       /**< First object on the list */
      static int total;           /**< Total Objects on the list */
};


#endif

