/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_object_h
#define _dnt_object_h

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
#define OBJECT_TYPE_NARCOTIC  7

#define OBJECT_TYPE_MONEY    19
#define OBJECT_TYPE_BOOK     20

/*! Define Objects of the DNT (an object is almost everything that 
 * isn't live (like chairs, doors, equipments, weapons, etc).
 * \note -> the object is automatically added to objectsList
 *          in its constructos, and removed in its desctructor. */
class object: public thing
{
   public:
      /*! Constructor 
       * \param path -> path to the object file
       * \param curmap -> current map filename */
      object(string path, string curMap);
      /*! Constructor
       * \param obj -> some object to be the source of actual */
      object(object* obj);
      /*! Constructor without parameter. Do not load anyhing */
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

      /*! Render the object as equipped at the character
       * \param type -> type of equiped [1,2]
       * \param pX -> x position
       * \param pY -> y position
       * \param pZ -> z position
       * \param angle -> object angle
       * \param angleXY -> object X,Y angle
       * \param angleYZ -> object Y,Z angle
       * \param reflexion -> true to render reflexion too
       * \param shadow -> true to render shadow too*/
      void renderEquipped(int type, float pX, float pY, float pZ, 
            float angle, float angleXY, float angleYZ,
            bool reflexion, bool shadow, GLfloat* shadowMatrix, 
            float shadowAlpha);

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

      /*! Get the 2D model name
       * \return -> model2D Name */
      string get2dModelName();

      /*! Get the 3D model relative to this object
       * \return -> pointer to the model3d */
      model3d* get3dModel();

      /*! Get the related information string (comic book file name,
       * ammo type, etc.)
       * \return -> related info */
      string getRelatedInfo();

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

      /*! Set the object state
       * \param st -> new state value */
      void setState(int st);

      /*! Get the object state
       * \return -> current object state value */
      int getState();

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
      int inventSizeX,      /**< Size on inventory X axis */
          inventSizeY;      /**< Size on inventory Y axis */

      float eqAngleX,       /**< X angle when equipped */
            eqAngleY,       /**< Y angle when equipped */
            eqAngleZ;       /**< Z angle when equipped */

      float eqTrans1[3],    /**< Translation when equipped at first place */
            eqTrans2[3];    /**< Translation when equipped at second place */

      diceThing diceInfo;   /**< The object's dice for action */

      bool staticScenery;   /**< True if is static scenery */

      int type;             /**< Type of the object */

      model3d* model3D;     /**< Pointer to used 3D Model Maximun Quality */
      SDL_Surface* model2d; /**< Pointer to used 2D Model */

      int state;            /**< Object state. Its use is dependant of object. 
                                 For example, at computers, it keeps the 
                                 operational system installed. */

      string fileName;      /**< FileName of the Object */
      string model2dName;   /**< FileName of the 2D Model */
      string relatedInfo;   /**< Some related fileName 
                                 (comic book file, for example) */

      object* next;         /**< Next Object on List */
      object* previous;     /**< Previous Object on List */ 

      /*! Init the values (all with null or zero). Usually called
       * at begining of the constructors. */
      void cleanValues();

    private:

      /*! Do the equipped object transforms */
      void equippedTransforms(int type);

};

/*! The object list. All active objects on the game are here.
 * Objects are removed at map exit and added at map enter.
 * Also, objects are kept in the lis when in inventory. */
class objectsList
{
   public:
      /*! Remove an object from the list
       * \param o -> object to remove from the list
       * \note -> usually called at object's destructor */
      static void remove(object* o);

      /*! Insert an object to the list
       * \param o -> object to add to the list
       * \note -> usually called at object's constructor */
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

/*! Create an object based on its type
 * \param arquivo -> filename to load
 * \return -> pointer to the Object created (object* as generic,
 *            but really create the child type, as weapon*, for example) */
object* createObject(string arquivo, string curMapFileName);

/*! Delete an object, based on its type
 * \param obj -> pointer to the object to delete */
void deleteObject(object* obj);


#endif

