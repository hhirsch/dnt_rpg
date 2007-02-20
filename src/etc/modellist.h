#ifndef _modelslist_h
#define _modelslist_h

#include "glm.h"
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


/*! The 3D model of an object */
class model3d
{
   public:
      /*! Constructor
       * \param path -> string containing the path of model 
       * \param texturePath -> path to all textures of the model */
      model3d(string path, string texturePath);

      /*! Destructor */
      ~model3d();

      /*! Draw Object on World */
      void draw();

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

      
      model3d* next;     /**< next model on list */
      model3d* previous; /**< previous model on list */

   protected:
      GLMmodel* model;   /**< Pointer to opened GLM Object */
      string fileName;   /**< Nome of the Model's file */

      int usedFlag;      /**< The used flag. When used is 0, remove the 
                              object from game models list. Each thing that 
                              is with the objecy (like map is with object, 
                              inventory is with it, etc) will need to increment
                              the used flag in the moment they take the object,
                              and decrement the used flag when is no more 
                              using them. */

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
       * \return -> pointer to the added model (NULL if not added) */
      model3d* addModel(string path, string texturePath);

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

   protected:
      model3d* first;      /**< First Model on List */
      int totalModels;     /**< Number of Models on List */
};

#endif

