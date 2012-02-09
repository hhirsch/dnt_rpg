/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_scenenode_h
#define _dnt_scenenode_h

#include <SDL/SDL_opengl.h>
#include "animodel.h"

/*! The scene node is an aniModel with a render position on the scene.
 * It can or not own the aniModel (if not, it's only a render position of it) */
class sceneNode
{
    public:
       /*! Constructor of a scene node that not owns the model */
       sceneNode(aniModel* a, GLfloat x, GLfloat y, GLfloat z,
                 GLfloat aX, GLfloat aY, GLfloat aZ);
       /*! Constructor of a scene node that owns the model (will delete it
        * at the destructor) */
      sceneNode(std::string modelFileName, GLfloat x, GLfloat y, GLfloat z,
                 GLfloat aX, GLfloat aY, GLfloat aZ);
       /*! Destructor */
       ~sceneNode();

       /*! Set node position and angle */
       void set(GLfloat x, GLfloat y, GLfloat z, 
             GLfloat aX, GLfloat aY, GLfloat aZ);

       /*! Set node position */
       void setPosition(GLfloat x, GLfloat y, GLfloat z);

       /*! Set node angle */
       void setAngle(GLfloat aX, GLfloat aY, GLfloat aZ);

       /*! Update the bounding box position */
       void updateBoundingBox();

       /*! Enable the update of crude bounding box (related to the
        * aniModel, at the origin) */
       void enableCrudeBoundingBoxUpdate();

       /*! Disable the update of crude bounding box (related to the
        * aniModel, at the origin) */
       void disableCrudeBoundingBoxUpdate();
       
       /*! Get the related rotated & translated bounding box */
       boundingBox getBoundingBox(){return(bbox);};
       
       /*! Get x coordinate */
       float getPosX(){return(posX);};
       /*! Get y coordinate */
       float getPosY(){return(posY);};
       /*! Get z coordinate */
       float getPosZ(){return(posZ);};

       /*! Get rotation on x axis */
       float getAngleX(){return(angleX);};
       /*! Get rotation on y axis */
       float getAngleY(){return(angleY);};
       /*! Get rotation on z axis */
       float getAngleZ(){return(angleZ);};

       /*! Get the animodel related with the sceneNode */
       aniModel* getModel(){return(model);};

       /*! Verify if the model related is animated or not */
       bool isAnimated(){return(animated);};

       /*! Render the model. */
       void render(GLfloat** viewMatrix, bool update, bool reflexion, 
             bool shadow, GLfloat* shadowMatrix, float alpha, 
             bool& modelLoaded);

       /*! Set a rotation to do aftear all other rotations
        * \note -> this rotation won't affect the bouding box!
        * \note -> it is usually used for eqquiped objects */
       void setRotationLast(float angle, float x, float y, float z);

    protected:

       GLfloat posX,    /**< X position on scene */
               posY,    /**< Y position on scene */
               posZ;    /**< Z position on scene */

       GLfloat angleX,  /**< rotation angle on X axis */
               angleY,  /**< rotation angle on Y axis */
               angleZ;  /**< rotation angle on Z axis */

        GLfloat rotLast[4]; /**< Rotation to do after basic */
        bool enableRotLast; /**< If rotation after all is avaiable */

        aniModel* model; /**< pointer to the animodel related */
        bool animated;   /**< true if is animated (so, own the aniModel),
                              false if is only a render position */
        bool updateCrude;  /**< if will update crude bounding box */
        boundingBox bbox;  /**< translated and rotated bounding box 
                                relative to the scene node */

};

#endif


