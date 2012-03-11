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

#ifndef _dnt_object_h
#define _dnt_object_h

#include <SDL/SDL.h>
#include <string>

#include "../etc/scene.h"
#include "../etc/scenenode.h"
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
      object(std::string path, std::string curMap);
      /*! Constructor
       * \param obj -> some object to be the source of actual */
      object(object* obj);
      /*! Constructor. Only set path (Do not load anyhing) */
      object(std::string path);
      /*! Destructor */
      ~object();

      /*! Verify if Map object can be got
       * \return true if the object is pickable */
      bool canGet();

      /*! Draw the 2D Model to Surface
       * \param x -> x value on surface
       * \param y -> y value on surface
       * \param surface -> surface where will draw the 2D version. */
      void draw2D(int x, int y, SDL_Surface* surface);

      /*! Gets the name of the model
       * \return -> the name of the model*/
      std::string getName();

      /*! Gets the fileName of the model
       * \return -> the fileName of the model*/
      std::string getFileName();

      /*! Get the object's description
       * \return string with description */
      std::string getDescription();

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

      /*! Verify if the item need a depth collision check or not
       * \return -> true if depth collsion is needed, or false if only the
       *            boudingBox check is sufficient. */
      bool needDepthCollisionCheck();

      /*! Get the 2D image of the object
       * \return the pointer to the surface */
      SDL_Surface* get2dModel();

      /*! Get the 2D model name
       * \return -> model2D Name */
      std::string get2dModelName();

      /*! Get the related information string (comic book file name,
       * ammo type, etc.)
       * \return -> related info */
      std::string getRelatedInfo();

      /*! Verify, with all Meshes of the model, if the model colides
       *  with the bounding box defined as parameter. 
       *  \param angle -> model angle
       *  \param pX -> model X position
       *  \param pY -> model Y position
       *  \param pZ -> model Z position
       *  \param colBox -> collider bounding box
       *  \return -> true if one or more meshes colides, false otherwise */
      bool depthCollision(GLfloat angleX, GLfloat angleY, GLfloat angleZ, 
            GLfloat pX, GLfloat pY, GLfloat pZ, boundingBox colBox);
      bool depthCollision(GLfloat angleX, GLfloat angleY, GLfloat angleZ,
            GLfloat pX, GLfloat pY, GLfloat pZ, ray& colRay);

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

      /*! Remove the object's scene node (if defined)
       * \note Usually called when added object to inventory */
      void removeSceneNode();
      /*! Create new scene node to the object
       * \note Usually called when removed object from inventory */
      void createSceneNode(float pX, float pY, float pZ,
                           float aX, float aY, float aZ);

      /*! Set the object position when equiped
       * \param type -> 1 for first place, 2 for second place */
      void setEquipedPosition(int type, float pX, float pY, float pZ,
                              float aY);

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

      SDL_Surface* model2d; /**< Pointer to used 2D Model */

      int state;            /**< Object state. Its use is dependant of object. 
                                 For example, at computers, it keeps the 
                                 operational system installed. */

      std::string fileName;      /**< FileName of the Object */
      std::string model2dName;   /**< FileName of the 2D Model */
      std::string cal3dName;     /**< FileName of the cal3d model */
      std::string description;   /**< Description of the object */
      std::string relatedInfo;   /**< Some related fileName 
                                 (comic book file, for example) */

      bool depthCol;        /**< If do a depth collision check or not */

      object* next;         /**< Next Object on List */
      object* previous;     /**< Previous Object on List */ 

      /*! Init the values (all with null or zero). Usually called
       * at begining of the constructors. */
      void cleanValues();
   
      /*! Get the object type ID, based on string identifier */
      int getObjectTypeId(std::string t);

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
      static object* search(std::string fileName, GLfloat posX, GLfloat posY,
                            GLfloat posZ);

      /*! Search for the object on the list with scNode
       * \return object pointer or NULL */
      static object* search(sceneNode* scNode);

   protected:
      static object* first;       /**< First object on the list */
      static int total;           /**< Total Objects on the list */
};

/*! Create an object based on its type
 * \param arquivo -> filename to load
 * \return -> pointer to the Object created (object* as generic,
 *            but really create the child type, as weapon*, for example) */
object* createObject(std::string arquivo, std::string curMapFileName);

/*! Delete an object, based on its type
 * \param obj -> pointer to the object to delete */
void deleteObject(object* obj);


#endif

