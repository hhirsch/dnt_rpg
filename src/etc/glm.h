/*! /file glm.h
 *    
 *  GLM library.  Wavefront .obj file format reader/writer/manipulator. \par
 *  Written by Nate Robins, 1997. \par
 *  email: ndr@pobox.com \par
 *  www: http://www.pobox.com/~ndr \par
 *  Texture, more colors in group face support, 
 *  blender .obj export support and animation on group
 *  support by DccNiTghtmare\par
 *  email: dccnightmare@listas.dcc.ufmg.br \par
 *  www: http://dccnitghtmare.sourceforge.net/ \par
 *
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _glm_h
#define _glm_h

/* includes */
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <string>
using namespace std;


/* structs */

/*! GLMmaterial: Structure that defines a material in a model. 
 */
class GLMmaterial
{
   public:
     string name;				/**< name of material */
     GLfloat diffuse[4];			/**< diffuse component */
     GLfloat ambient[4];			/**< ambient component */
     GLfloat specular[4];			/**< specular component */
     GLfloat emmissive[4];			/**< emmissive component */
     GLfloat shininess;			/**< specular exponent */
};

/*! GLMtriangle: Structure that defines a triangle in a model.
 */
class GLMtriangle {
   public:
     GLuint vindices[3];		/**< array of triangle vertex indices */
     GLuint nindices[3];		/**< array of triangle normal indices */
     GLuint tindices[3];		/**< array of triangle texcoord indices*/
     GLuint findex;                 /**< index of triangle facet normal */
     int texture;                   /**< pointer to texture index */
     GLuint material;               /**< face used material */
};

/*! GLMgroup: Structure that defines a group in a model.
 */
class GLMgroup {
   public:
     string   name;		      /**< name of this group */
     GLuint  numtriangles;	      /**< number of triangles in this group */
     GLuint* triangles;          /**< array of triangle indices */
     //GLuint  material;         /* index to material for group */
     GLfloat rotacao[3];         /**< rotation vector */
     GLfloat translacao[3];      /**< translation vector */
     GLfloat escala[3];          /**< scale vector */
     GLuint listaDesenhar;       /**< list to draw th group */
     struct GLMgroup* next;     /**< pointer to next group in model */
};

/*! GLMtexture: Struct that defines model's textures 
 */
class GLMtexture {
   public:
      GLMtexture()
      {
         nome = "";
         indice = -1;
         proximo = NULL;
      }
      string nome;                 /**< Texture file name */
      GLuint indice;              /**< Indice da Textura internamente */
      GLuint w,                   /**< Texture width */
      h;                          /**< Texture height */
      struct GLMtexture* proximo;/**< Next Texture on list */
};

/*! GLMmodel: Structure that defines a model.
 */
class GLMmodel {
   public:
  string    pathname;			/**< path to this model */
  string    mtllibname;			/**< name of the material library */
  string    diretorioTexturas;           /**< texture directory */

  GLuint   numvertices;			/**< number of vertices in model */
  GLfloat* vertices;			/**< array of vertices  */

  GLuint   numnormals;			/**< number of normals in model */
  GLfloat* normals;			/**< array of normals */

  GLuint   numtexcoords;		/**< number of texcoords in model */
  GLfloat* texcoords;			/**< array of texture coordinates */

  GLuint   numfacetnorms;		/**< number of facetnorms in model */
  GLfloat* facetnorms;			/**< array of facetnorms */

  GLuint       numtriangles;		/**< number of triangles in model */
  GLMtriangle* triangles;		/**< array of triangles */

  GLuint       nummaterials;		/**< number of materials in model */
  GLMmaterial* materials;		/**< array of materials */

  GLuint       numgroups;		/**< number of groups in model */
  GLMgroup*    groups;			/**< linked list of groups */

  GLuint       numtexturas;             /**< numero de texturas no modelo */
  GLMtexture*   texturas;               /**< lista de texturas */

  GLfloat position[3];			/**< position of the model */

  GLfloat x1,z1,y1,x2,y2,z2;            /**< bounding box */

  GLuint boundingDefinido;              /**< 1 if defined bounding box */

};


/* public functions */


/*!
 * Deletes a GLMmodel structure.
 * \param model -> initialized GLMmodel structure
 */
GLvoid
glmDelete(GLMmodel* model);

/*!
 * Reads a model description from a Wavefront .OBJ file.
 * \return a pointer to the created object which should be free'd with
 * glmDelete().
 *
 * \param filename -> name of the file containing the Wavefront .OBJ data.  
 * \param diretorioTex -> directory where are the models textures files
 * \param fazListas -> 1 if wanna computate the draw lists.
 */
GLMmodel* 
glmReadOBJ(const string filename,const string diretorioTex,int fazListas);

/*!
 * Renders the model to the current OpenGL context using the
 * mode specified.
 *
 * \param model -> initialized GLMmodel structure
 */
GLvoid
glmDraw(GLMmodel* model);

/*!
 * Find a group in the model
 * \param model -> model to find
 * \param name -> model name
 */
GLMgroup*
_glmFindGroup(GLMmodel* model, string name);

/*!
 * Prepocess the drawning lists.
 * \param model -> model to draw
 * \param mode -> draw mode 
 */
void glmPrecomputaListas(GLMmodel* model, GLuint mode);
/*!
 * Draw the drawning lists.
 * \param model -> model to draw
 */
GLvoid glmDrawLists(GLMmodel* model);

#endif

