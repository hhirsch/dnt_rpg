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

/*! Define Objects of the DNT (an object is almost everything that 
 * isn't live (like chairs, doors, equipments, weapons, etc).*/
class object: public thing
{
   public:
      /*! Constructor 
       * \param path -> path to the object file
       * \param mdlList -> list of openned models */
      object(string path, modelList& mdlList);
      /*! Constructor
       * \param obj -> some object to be the source of actual */
      object(object* obj);
      /*! Constructor withou parameter. Do not load nothing */
      object();
      /*! Destructor */
      ~object();

      /*!
       * Draws the object
       * \param x -> X position 
       * \param z -> Z posotion
       * \param dist -> distance from observator (defines the LOD used)
       * \param orientation -> orientation angle of the object
       * \param inverted -> to invert the Y position */
      void draw(float x, float z,GLfloat dist, float orientation, bool inverted);

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
      
   protected:
      int inventSizeX,      /**< Size on inventory X axis */
          inventSizeY;      /**< Size on inventory Y axis */

      int deltaVar;         /**< Variation on static LOD */
      
      model3d* model3D;     /**< Pointer to used 3D Model Maximun Quality */
      SDL_Surface* model2d; /**< Pointer to used 2D Model */
      
      string name;          /**< Name of the Object */
      string fileName;      /**< FileName of the Object */
      string model2dName;   /**< FileName of the 2D Model */
};


#endif

