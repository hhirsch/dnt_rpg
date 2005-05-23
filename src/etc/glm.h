/*    
 *  GLM library.  Wavefront .obj file format reader/writer/manipulator.
 *
 *  Written by Nate Robins, 1997.
 *  email: ndr@pobox.com
 *  www: http://www.pobox.com/~ndr
 *  Texture, more colors in group face support, 
 *  blender .obj export support and animation on group
 *  support by DccNiTghtmare
 *  email: dccnightmare@dcc.ufmg.br
 *  www: http://dccnitghtmare.sourceforge.net/
 *
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


/* includes */
#include <GL/gl.h>
#include <SDL/SDL_image.h>


/* structs */

/* GLMmaterial: Structure that defines a material in a model. 
 */
typedef struct _GLMmaterial
{
  char* name;				/* name of material */
  GLfloat diffuse[4];			/* diffuse component */
  GLfloat ambient[4];			/* ambient component */
  GLfloat specular[4];			/* specular component */
  GLfloat emmissive[4];			/* emmissive component */
  GLfloat shininess;			/* specular exponent */
} GLMmaterial;

/* GLMtriangle: Structure that defines a triangle in a model.
 */
typedef struct {
  GLuint vindices[3];			/* array of triangle vertex indices */
  GLuint nindices[3];			/* array of triangle normal indices */
  GLuint tindices[3];			/* array of triangle texcoord indices*/
  GLuint findex;			/* index of triangle facet normal */
  int texture;                      /* ponteiro para o indice da textura */
  GLuint material;                      /* material utilizado pela face */
} GLMtriangle;

/* GLMgroup: Structure that defines a group in a model.
 */
typedef struct _GLMgroup {
  char*   name;		      /* name of this group */
  GLuint  numtriangles;	      /* number of triangles in this group */
  GLuint* triangles;          /* array of triangle indices */
  //GLuint  material;           /* index to material for group */
  GLfloat rotacao[3];
  GLfloat translacao[3];
  GLfloat escala[3];
  GLuint listaDesenhar;     /* Lista para desenhar o grupo */
  struct _GLMgroup* next;		/* pointer to next group in model */
} GLMgroup;

/* GLMtexture: Estrutura que define texturas no modelo
 */
typedef struct _GLMtexture {
   char* nome;                   /* Nome do arquivo ed Textura Correspondente */
   GLuint indice;                /* Indice da Textura internamente */
   GLuint w,h;
   struct _GLMtexture* proximo;  /* Proxima Textura na Lista */
}GLMtexture;

/* GLMmodel: Structure that defines a model.
 */
typedef struct {
  char*    pathname;			/* path to this model */
  char*    mtllibname;			/* name of the material library */
  char*    diretorioTexturas;           /* diretorio onde estao as texturas */

  GLuint   numvertices;			/* number of vertices in model */
  GLfloat* vertices;			/* array of vertices  */

  GLuint   numnormals;			/* number of normals in model */
  GLfloat* normals;			/* array of normals */

  GLuint   numtexcoords;		/* number of texcoords in model */
  GLfloat* texcoords;			/* array of texture coordinates */

  GLuint   numfacetnorms;		/* number of facetnorms in model */
  GLfloat* facetnorms;			/* array of facetnorms */

  GLuint       numtriangles;		/* number of triangles in model */
  GLMtriangle* triangles;		/* array of triangles */

  GLuint       nummaterials;		/* number of materials in model */
  GLMmaterial* materials;		/* array of materials */

  GLuint       numgroups;		/* number of groups in model */
  GLMgroup*    groups;			/* linked list of groups */

  GLuint       numtexturas;             /* numero de texturas no modelo */
  GLMtexture*   texturas;               /* lista de texturas */

  GLfloat position[3];			/* position of the model */

} GLMmodel;


/* public functions */


/* glmDelete: Deletes a GLMmodel structure.
 *
 * model - initialized GLMmodel structure
 */
GLvoid
glmDelete(GLMmodel* model);

/* glmReadOBJ: Reads a model description from a Wavefront .OBJ file.
 * Returns a pointer to the created object which should be free'd with
 * glmDelete().
 *
 * filename - name of the file containing the Wavefront .OBJ format data.  
 * diretorioTex - diretorio onde estao as texturas do modelo
 */
GLMmodel* 
glmReadOBJ(char* filename, char* diretorioTex);

/* glmDraw: Renders the model to the current OpenGL context using the
 * mode specified.
 *
 * model    - initialized GLMmodel structure
 * mode     - a bitwise OR of values describing what is to be rendered.
 *            GLM_NONE    -  render with only vertices
 *            GLM_FLAT    -  render with facet normals
 *            GLM_SMOOTH  -  render with vertex normals
 *            GLM_TEXTURE -  render with texture coords
 *            GLM_FLAT and GLM_SMOOTH should not both be specified.
 */
GLvoid
glmDraw(GLMmodel* model);

/* _glmFindGroup: Find a group in the model
 */
GLMgroup*
_glmFindGroup(GLMmodel* model, char* name);

void glmPrecomputaListas(GLMmodel* model, GLuint mode);


