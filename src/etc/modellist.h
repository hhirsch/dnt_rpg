#ifndef _modelslist_h
#define _modelslist_h

#include "animodel.h"
#include <string>
using namespace std;

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

/*! The Bounding Box Class */
class boundingBox
{
   public:
      GLfloat x1,
              z1,
              y1,
              x2,
              z2,
              y2;
};

/*! A render Positon of a Scenery Static Model Object */
class sceneryRenderPosition
{
   public:
      GLfloat x,        /**< x coordinate */
              y,        /**< y coordinate */
              z;        /**< z coordinate */
      GLfloat angle;    /**< scenery angle */

      sceneryRenderPosition* next;     /**< Next position on list */
      sceneryRenderPosition* previous; /**< Previous position on list */
};

/*! The 3D model of an object */
class model3d: public aniModel
{
   public:
      /*! Constructor
       * \param path -> string containing the path of model 
       * \param texturePath -> path to all textures of the model
       * \param staticScenery -> if the model is a static scenery */
      model3d(string path, string texturePath, bool staticScenery);

      /*! Destructor */
      ~model3d();

      /*! Draw Object on World
       * \note -> non static scenery ones */
      void draw();

      /*! Draw All Visible Static Scenery Objects Based on model
       * \param matrix -> visible matrix
       * \param inverted -> if inverted on Y axys. Usefull for reflexion. */
      void draw(GLfloat** matriz, bool inverted);

      /*! Increment the used flag */
      void incUsed();

      /*! Decrement the used flag */
      void decUsed();

      /*! Get used Flag Value
       * \return actual value of the used flag. */
      int getUsedFlag();

      /*! Gets the model filename
       * \return string with full filename of the model */
      string getFileName();

      /*! Gets the Bounding box of the model */
      boundingBox getBoundingBox();

      /*! Verify if the model is static scenery or not
       * \return -> true if the model is a static scenery */
      bool isStaticScenery();

      /*! Add a render position.
       * \note -> this only affects staticScenery objects 
       * \param x -> x coordinate
       * \param y -> y coordinate
       * \param z -> z coordinate 
       * \param angle -> orientation angle */
      void addPosition(float x, float y, float z, float angle);

      /*!
       **********************************************/
       void callDeadAnimation();

      /*!
       **********************************************/
       void callAttackAnimation();

      /*!
       **********************************************/
       void callIdleAnimation();
      
      model3d* next;     /**< next model on list */
      model3d* previous; /**< previous model on list */

   protected:
      string fileName;   /**< Nome of the Model's file */

      int usedFlag;      /**< The used flag. When used is 0, remove the 
                              object from game models list. Each thing that 
                              is with the objecy (like map is with object, 
                              inventory is with it, etc) will need to increment
                              the used flag in the moment they take the object,
                              and decrement the used flag when is no more 
                              using them. */

      bool staticFlag;   /**< The Static Flag: if this flag is used, the 
                              model is static, and can be utilized by more
                              than one entity.
                              \note -> this flag is only used for non-pickeable 
                              scenery objects and optimize a lot their render*/

      sceneryRenderPosition* positions;  /**< Positions List for the 
                                              statics objects */
      int totalPositions;                /**< Total Positions current on list */

};

/*! List of all 3D GLM models actually on game */
class modelList
{
   public:
      /*! Constructor */
      modelList();
      /*! Destructor */
      ~modelList();

      /*! Add Model to the List
       * \param path -> string containing the path of model
       * \param texturePath -> path to all textures of the model
       * \param staticScenery -> true if object is a static scenery
       * \return -> pointer to the added model (NULL if not added) */
      model3d* addModel(string path, string texturePath, bool staticScenery);

      /*! Find Model on the List
       * \param path -> string containing the path of model
       * \return -> pointer to the founded model (NULL if not found) */
      model3d* findModel(string path);

      /*! Remove Model from List
       * \param mdl -> model to remove */
      void removeModel(model3d* mdl);

      /*! Remove All Unused Models */
      void removeUnusedModels();

      /*! Get First MOdel on List (usually used on map editor application) */
      model3d* getFirst();

      /*! Get Total Models on list
       * \return -> number of models on the list */
      int getTotalModels(){return(totalModels);}

      /*! Print all Models with printf for debug! */
      void printAll();

   protected:
      model3d* first;      /**< First Model on List */
      int totalModels;     /**< Number of Models on List */
};

#endif

