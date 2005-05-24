/*    
 *  GLM library.  Wavefront .obj file format reader/writer/manipulator.
 *
 *  Written by Nate Robins, 1997.
 *  email: ndr@pobox.com
 *  www: http://www.pobox.com/~ndr
 *  Texture and Color support by DccNiTghtmare
 *  email: dccnightmare@dcc.ufmg.br
 *  www: http://dccnitghtmare.sourceforge.net/
 *
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 *
 */

/* includes */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "glm.h"
#include "../gui/desenho.h"

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.1415926
#endif

/* defines */
#define GLM_NONE     (0)		/* render with only vertices */
#define GLM_FLAT     (1 << 0)		/* render with facet normals */
#define GLM_SMOOTH   (1 << 1)		/* render with vertex normals */
#define GLM_TEXTURE  (1 << 2)		/* render with texture coords */
#define GLM_COLOR    (1 << 3)		/* render with colors */
#define GLM_MATERIAL (1 << 4)		/* render with materials */

/* defines */
#define T(x) model->triangles[(x)]


/* enums */
enum { X, Y, Z, W };			/* elements of a vertex */


/* typedefs */

/* _GLMnode: general purpose node
 */
typedef struct _GLMnode {
  GLuint           index;
  GLboolean        averaged;
  struct _GLMnode* next;
} GLMnode;

/* strdup is actually not a standard ANSI C or POSIX routine
   so implement a private one.  OpenVMS does not have a strdup; Linux's
   standard libc doesn't declare strdup by default (unless BSD or SVID
   interfaces are requested). */
static char *
stralloc(const char *string)
{
  char *copy;

  copy = (char*) malloc(strlen(string) + 1);
  if (copy == NULL)
    return NULL;
  strcpy(copy, string);
  return copy;
}



/* _glmFindGroup: Find a group in the model
 */
GLMgroup*
_glmFindGroup(GLMmodel* model, char* name)
{
  GLMgroup* group;

  group = model->groups;
  while(group) {
    if (!strcmp(name, group->name))
      break;
    group = group->next;
  }

  return group;
}

/* _glmAddGroup: Add a group to the model
 */
GLMgroup*
_glmAddGroup(GLMmodel* model, char* name)
{
  GLMgroup* group;

  group = _glmFindGroup(model, name);
  if (!group) {
    group = (GLMgroup*)malloc(sizeof(GLMgroup));
    group->name = stralloc(name);
    group->numtriangles = 0;
    int aux;
    for(aux=0;aux<3;aux++)
    {
       group-> rotacao[aux] = 0;
       group-> escala[aux] = 1;
       group-> translacao[aux] = 0;
    }
    group->triangles = NULL;
    group->next = model->groups;
    model->groups = group;
    model->numgroups++;
  }

  return group;
}

/* _glmFindGroup: Find a material in the model
 */
GLuint
_glmFindMaterial(GLMmodel* model, char* name)
{
  GLuint i;

  for (i = 0; i < model->nummaterials; i++) {
    if (!strcmp(model->materials[i].name, name))
      goto found;
  }

  /* didn't find the name, so set it as the default material */
  printf("_glmFindMaterial(): Nao achei material  \"%s\".\n", name);
  i = 0;

found:
  return i;
}


/* _glmDirName: return the directory given a path
 *
 * path - filesystem path
 *
 * The return value should be free'd.
 */
char*
_glmDirName(char* path)
{
  char* dir;
  char* s;

  dir = stralloc(path);

  s = strrchr(dir, '/');
  if (s)
    s[1] = '\0';
  else
    dir[0] = '\0';

  return dir;
}


/* _glmReadMTL: read a wavefront material library file
 *
 * model - properly initialized GLMmodel structure
 * name  - name of the material library
 */
GLvoid
_glmReadMTL(GLMmodel* model, char* name)
{
  FILE* file;
  char* dir;
  char* filename;
  char  buf[128];
  GLuint nummaterials, i;

  dir = _glmDirName(model->pathname);
  filename = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(name) + 1));
  strcpy(filename, dir);
  strcat(filename, name);
  free(dir);

  /* open the file */
  file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "_glmReadMTL() falhou: nao abri arquivo de material\"%s\".\n",
	    filename);
    exit(1);
  }
  free(filename);

  /* count the number of materials in the file */
  nummaterials = 1;
  while(fscanf(file, "%s", buf) != EOF) {
    switch(buf[0]) {
    case '#':				/* comment */
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      break;
    case 'n':				/* newmtl */
      fgets(buf, sizeof(buf), file);
      nummaterials++;
      sscanf(buf, "%s %s", buf, buf);
      break;
    default:
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      break;
    }
  }

  rewind(file);

  /* allocate memory for the materials */
  model->materials = (GLMmaterial*)malloc(sizeof(GLMmaterial) * nummaterials);
  model->nummaterials = nummaterials;

  /* set the default material */
  for (i = 0; i < nummaterials; i++) {
    model->materials[i].name = NULL;
    model->materials[i].shininess = 0;
    model->materials[i].diffuse[0] = 0.8;
    model->materials[i].diffuse[1] = 0.8;
    model->materials[i].diffuse[2] = 0.8;
    model->materials[i].diffuse[3] = 1.0;
    model->materials[i].ambient[0] = 0.2;
    model->materials[i].ambient[1] = 0.2;
    model->materials[i].ambient[2] = 0.2;
    model->materials[i].ambient[3] = 1.0;
    model->materials[i].specular[0] = 0.0;
    model->materials[i].specular[1] = 0.0;
    model->materials[i].specular[2] = 0.0;
    model->materials[i].specular[3] = 1.0;
  }
  model->materials[0].name = stralloc("default");

  /* now, read in the data */
  nummaterials = 0;
  while(fscanf(file, "%s", buf) != EOF) {
    switch(buf[0]) {
    case '#':				/* comment */
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      break;
    case 'n':				/* newmtl */
      fgets(buf, sizeof(buf), file);
      sscanf(buf, "%s %s", buf, buf);
      nummaterials++;
      model->materials[nummaterials].name = stralloc(buf);
      break;
    case 'N':
      fscanf(file, "%f", &model->materials[nummaterials].shininess);
      /* wavefront shininess is from [0, 1000], so scale for OpenGL */
      model->materials[nummaterials].shininess /= 1000.0;
      model->materials[nummaterials].shininess *= 128.0;
      break;
    case 'K':
      switch(buf[1]) {
      case 'd':
	fscanf(file, "%f %f %f",
	       &model->materials[nummaterials].diffuse[0],
	       &model->materials[nummaterials].diffuse[1],
	       &model->materials[nummaterials].diffuse[2]);
	break;
      case 's':
	fscanf(file, "%f %f %f",
	       &model->materials[nummaterials].specular[0],
	       &model->materials[nummaterials].specular[1],
	       &model->materials[nummaterials].specular[2]);
	break;
      case 'a':
	fscanf(file, "%f %f %f",
	       &model->materials[nummaterials].ambient[0],
	       &model->materials[nummaterials].ambient[1],
	       &model->materials[nummaterials].ambient[2]);
	break;
      default:
	/* eat up rest of line */
	fgets(buf, sizeof(buf), file);
	break;
      }
      break;
    default:
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      break;
    }
  }
}


int IDTextura(GLMmodel* modelo, char* textura, GLuint *largura, GLuint *altura)
{
   /* procura pela textura */
   GLuint aux=0;
   GLMtexture* tex = modelo->texturas;
   while(aux<modelo->numtexturas)
   {
      if(!strcmp(tex->nome, textura))
      {
        *largura = tex->w;
        *altura = tex->h;
         return(tex->indice); //a textura ja esta presente 
      }
      tex = tex->proximo;
      aux++;
   }
   return(-1);
}

/* Insere a textura dentre as utilizadas pelo objeto */
void InsereTextura(GLMmodel* modelo, char* textura)
{
   /* Ignora os nulos */
   if(!strcmp(textura,"(null)")) 
     return;

   /* procura pela textura */
   GLuint aux=0;
   GLMtexture* tex = modelo->texturas;
   GLMtexture* ant = NULL;
   while(aux < modelo->numtexturas)
   {
      if(!strcmp(tex->nome, textura))
         return; //a textura ja esta presente 
      ant = tex;
      tex = tex->proximo;
      aux++;
   }

   /* carrega a textura do arquivo */
   char* arq = (char*)malloc(sizeof(char) * 
           (strlen(modelo->diretorioTexturas) + strlen(textura) + 1));
   strcpy(arq, modelo->diretorioTexturas);
   strcat(arq, textura);
   SDL_Surface* img = IMG_Load(arq);
   if(!img)
   {
      printf("Erro ao abrir textura: %s\n",arq);
      free(arq);
      return;
   }

   
   /* Transforma a textura em potencia de 2 */
   printf("%s: X:%d Y:%d\n",arq,img->w,img->h);
   SDL_Surface *imgPotencia = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   //SDL_Rect ret;
   //ret.w = 0; ret.h = 0; ret.x = img->w; ret.y = img->h;
   SDL_BlitSurface(img,NULL,imgPotencia,NULL);  
   //interpolaTextura(img,imgPotencia);

   /* Insere realmente a textura */ 
   tex = (GLMtexture*) malloc(sizeof(GLMtexture));
   tex->proximo = NULL;
   if(modelo->numtexturas == 0)
   {
      modelo->texturas = tex;
   }
   else
   {
     ant->proximo = tex;
   }

   tex->nome = stralloc(textura);
   glGenTextures(1, &(tex->indice));
   glBindTexture(GL_TEXTURE_2D, tex->indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imgPotencia->w,imgPotencia->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, imgPotencia->pixels);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );
   //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   modelo->numtexturas++;

   /* Libera a memoria utilizada */
   free(arq);
   SDL_FreeSurface(img);
   SDL_FreeSurface(imgPotencia);
}

/* _glmFirstPass: first pass at a Wavefront OBJ file that gets all the
 * statistics of the model (such as #vertices, #normals, etc)
 *
 * model - properly initialized GLMmodel structure
 * file  - (fopen'd) file descriptor 
 */
GLvoid
_glmFirstPass(GLMmodel* model, FILE* file) 
{
  GLuint    numvertices;		/* number of vertices in model */
  GLuint    numnormals;			/* number of normals in model */
  GLuint    numtexcoords;		/* number of texcoords in model */
  GLuint    numtriangles;		/* number of triangles in model */
  GLMgroup* group;			/* current group */
  unsigned  v, n, t;
  char      buf[128];

  /* make a default group */
  group = _glmAddGroup(model, "default");

  numvertices = numnormals = numtexcoords = numtriangles = 0;
  while(fscanf(file, "%s", buf) != EOF) {
    switch(buf[0]) {
    case '#':				/* comment */
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      break;
    case 'v':				/* v, vn, vt */
      switch(buf[1]) {
      case '\0':			/* vertex */
	/* eat up rest of line */
	fgets(buf, sizeof(buf), file);
	numvertices++;
	break;
      case 'n':				/* normal */
	/* eat up rest of line */
	fgets(buf, sizeof(buf), file);
	numnormals++;
	break;
      case 't':				/* texcoord */
	/* eat up rest of line */
	fgets(buf, sizeof(buf), file);
	numtexcoords++;
	break;
      default:
	printf("_glmFirstPass(): Token desconhecido \"%s\".\n", buf);
	exit(1);
	break;
      }
      break;
    case 'm':
      fgets(buf, sizeof(buf), file);
      sscanf(buf, "%s %s", buf, buf);
      model->mtllibname = stralloc(buf);
      _glmReadMTL(model, buf);
      break;
    case 'u':
      if ((strncmp(buf,"usemat",6)==0) || (strncmp(buf,"usemap",6)==0))
      {
          fgets(buf, sizeof(buf), file);
          sscanf(buf, "%s %s", buf, buf);
          InsereTextura(model, buf);
          //texture = inserirTextura(buf);
      }
      break;
    case 'o':				/* group */
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      sscanf(buf, "%s", buf);
      group = _glmAddGroup(model, buf);
      break;
    case 'f':				/* face */
      v = n = t = 0;
      fscanf(file, "%s", buf);
      /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
      if (strstr(buf, "//")) {
	/* v//n */
	sscanf(buf, "%d//%d", &v, &n);
	fscanf(file, "%d//%d", &v, &n);
	fscanf(file, "%d//%d", &v, &n);
	numtriangles++;
	group->numtriangles++;
	while(fscanf(file, "%d//%d", &v, &n) > 0) {
	  numtriangles++;
	  group->numtriangles++;
	}
      } else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
	/* v/t/n */
	fscanf(file, "%d/%d/%d", &v, &t, &n);
	fscanf(file, "%d/%d/%d", &v, &t, &n);
	numtriangles++;
	group->numtriangles++;
	while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
	  numtriangles++;
	  group->numtriangles++;
	}
      } else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
	/* v/t */
	fscanf(file, "%d/%d", &v, &t);
	fscanf(file, "%d/%d", &v, &t);
	numtriangles++;
	group->numtriangles++;
	while(fscanf(file, "%d/%d", &v, &t) > 0) {
	  numtriangles++;
	  group->numtriangles++;
	}
      } else {
	/* v */
	fscanf(file, "%d", &v);
	fscanf(file, "%d", &v);
	numtriangles++;
	group->numtriangles++;
	while(fscanf(file, "%d", &v) > 0) {
	  numtriangles++;
	  group->numtriangles++;
	}
      }
      break;

    default:
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      break;
    }
  }


  /* set the stats in the model structure */
  model->numvertices  = numvertices;
  model->numnormals   = numnormals;
  model->numtexcoords = numtexcoords;
  model->numtriangles = numtriangles;

  /* allocate memory for the triangles in each group */
  group = model->groups;
  while(group) {
    group->triangles = (GLuint*)malloc(sizeof(GLuint) * group->numtriangles);
    group->numtriangles = 0;
    group = group->next;
  }
}


/* _glmSecondPass: second pass at a Wavefront OBJ file that gets all
 * the data.
 *
 * model - properly initialized GLMmodel structure
 * file  - (fopen'd) file descriptor 
 */
GLvoid
_glmSecondPass(GLMmodel* model, FILE* file) 
{
  GLuint    numvertices;		/* number of vertices in model */
  GLuint    numnormals;			/* number of normals in model */
  GLuint    numtexcoords;		/* number of texcoords in model */
  GLuint    numtriangles;		/* number of triangles in model */
  GLfloat*  vertices;			/* array of vertices  */
  GLfloat*  normals;			/* array of normals */
  GLfloat*  texcoords;			/* array of texture coordinates */
  GLMgroup* group;			/* current group pointer */
  GLuint    material, largura, altura;			/* current material */
  int    texture;                    /* textura atual */
  GLuint    v, n, t;
  char      buf[128];


  /* set the pointer shortcuts */
  vertices     = model->vertices;
  normals      = model->normals;
  texcoords    = model->texcoords;
  group        = model->groups;

  /* on the second pass through the file, read all the data into the
     allocated arrays */
  numvertices = numnormals = numtexcoords = 1;
  numtriangles = 0;
  material = 0;
  texture = -1;

  while(fscanf(file, "%s", buf) != EOF) {
    switch(buf[0]) {
    case '#':				/* comment */
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      break;
    case 'v':				/* v, vn, vt */
      switch(buf[1]) {
      case '\0':			/* vertex */
      {
 	fscanf(file, "%f %f %f", 
	       &vertices[3 * numvertices + X], 
	       &vertices[3 * numvertices + Y], 
	       &vertices[3 * numvertices + Z]);
        if(! model->boundingDefinido)
        {
           model->x1 = vertices[3 * numvertices + X];
           model->x2 = vertices[3 * numvertices + X];
           model->z1 = vertices[3 * numvertices + Z];
           model->z2 = vertices[3 * numvertices + Z];
           model->boundingDefinido = 1;
        }
        else if((model->x1 > vertices[3 * numvertices + X]) ||
                (model->x2 < vertices[3 * numvertices + X]) ||
                (model->z1 > vertices[3 * numvertices + Z]) ||
                (model->z2 < vertices[3 * numvertices + Z]) )
        {
           if(model->x1 > vertices[3 * numvertices + X])
              model->x1 = vertices[3 * numvertices + X];
           else if(model->x2 < vertices[3 * numvertices + X])
              model->x2 = vertices[3 * numvertices + X];
           else if(model->z1 > vertices[3 * numvertices + Z])
              model->z1 = vertices[3 * numvertices + Z];
           else
              model->z2 = vertices[3 * numvertices + Z];
        }
        
	numvertices++;
	break;
      }
      case 'n':				/* normal */
	fscanf(file, "%f %f %f", 
	       &normals[3 * numnormals + X],
	       &normals[3 * numnormals + Y], 
	       &normals[3 * numnormals + Z]);
	numnormals++;
	break;
      case 't':				/* texcoord */
	fscanf(file, "%f %f", 
	       &texcoords[2 * numtexcoords + X],
	       &texcoords[2 * numtexcoords + Y]);
 
        texcoords[2 * numtexcoords + Y] = 1 - texcoords[2 * numtexcoords + Y];
	numtexcoords++;
	break;
      }
      break;
    case 'u':
      if (strncmp(buf,"usemtl",6) == 0)
      {
         fgets(buf, sizeof(buf), file);
         sscanf(buf, "%s %s", buf, buf);
         /*group->material =*/ 
         material = _glmFindMaterial(model, buf);
      }
      else if ((strncmp(buf,"usemat",6) ==0) || (strncmp(buf,"usemap",6) ==0))
      {
         //carregamaterialdetextura
          fgets(buf, sizeof(buf), file);
          sscanf(buf, "%s %s", buf, buf);
          texture = IDTextura(model, buf, &largura, &altura);
      }
      break;
    case 'o':				/* group */
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      sscanf(buf, "%s", buf);
      group = _glmFindGroup(model, buf);
      //group->material = material;
      break;
    case 'f':				/* face */
      v = n = t = 0;
      fscanf(file, "%s", buf);
      /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
      if (strstr(buf, "//")) {
	/* v//n */
	sscanf(buf, "%d//%d", &v, &n);
	T(numtriangles).vindices[0] = v;
	T(numtriangles).nindices[0] = n;
	fscanf(file, "%d//%d", &v, &n);
	T(numtriangles).vindices[1] = v;
	T(numtriangles).nindices[1] = n;
	fscanf(file, "%d//%d", &v, &n);
	T(numtriangles).vindices[2] = v;
	T(numtriangles).nindices[2] = n;
        T(numtriangles).material = material;
        //T(numtriangles).texture = texture;
	group->triangles[group->numtriangles++] = numtriangles;
	numtriangles++;
	while(fscanf(file, "%d//%d", &v, &n) > 0) {
	  T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
	  T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
	  T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
	  T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
	  T(numtriangles).vindices[2] = v;
	  T(numtriangles).nindices[2] = n;
          T(numtriangles).material = material;
          //T(numtriangles).texture = texture;
	  group->triangles[group->numtriangles++] = numtriangles;
	  numtriangles++;
	}
      } else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
	/* v/t/n */
	T(numtriangles).vindices[0] = v;
	T(numtriangles).tindices[0] = t;
	T(numtriangles).nindices[0] = n;
	fscanf(file, "%d/%d/%d", &v, &t, &n);
	T(numtriangles).vindices[1] = v;
	T(numtriangles).tindices[1] = t;
	T(numtriangles).nindices[1] = n;
	fscanf(file, "%d/%d/%d", &v, &t, &n);
	T(numtriangles).vindices[2] = v;
	T(numtriangles).tindices[2] = t;
	T(numtriangles).nindices[2] = n;
        T(numtriangles).material = material;
        T(numtriangles).texture = texture;
	group->triangles[group->numtriangles++] = numtriangles;
	numtriangles++;
	while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
	  T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
	  T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
	  T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
	  T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
	  T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
	  T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
	  T(numtriangles).vindices[2] = v;
	  T(numtriangles).tindices[2] = t;
	  T(numtriangles).nindices[2] = n;
          T(numtriangles).material = material;
          T(numtriangles).texture = texture;
	  group->triangles[group->numtriangles++] = numtriangles;
	  numtriangles++;
	}
      } else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
	/* v/t */
	T(numtriangles).vindices[0] = v;
	T(numtriangles).tindices[0] = t;
	fscanf(file, "%d/%d", &v, &t);
	T(numtriangles).vindices[1] = v;
	T(numtriangles).tindices[1] = t;
	fscanf(file, "%d/%d", &v, &t);
	T(numtriangles).vindices[2] = v;
	T(numtriangles).tindices[2] = t;
        T(numtriangles).material = material;
        T(numtriangles).texture = texture;
	group->triangles[group->numtriangles++] = numtriangles;
	numtriangles++;
	while(fscanf(file, "%d/%d", &v, &t) > 0) {
	  T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
	  T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
	  T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
	  T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
	  T(numtriangles).vindices[2] = v;
	  T(numtriangles).tindices[2] = t;
          T(numtriangles).material = material;
          T(numtriangles).texture = texture;
	  group->triangles[group->numtriangles++] = numtriangles;
	  numtriangles++;
	}
      } else {
	/* v */
	sscanf(buf, "%d", &v);
	T(numtriangles).vindices[0] = v;
	fscanf(file, "%d", &v);
	T(numtriangles).vindices[1] = v;
	fscanf(file, "%d", &v);
	T(numtriangles).vindices[2] = v;
        T(numtriangles).material = material;
        //T(numtriangles).texture = texture;
	group->triangles[group->numtriangles++] = numtriangles;
	numtriangles++;
	while(fscanf(file, "%d", &v) > 0) {
	  T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
	  T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
	  T(numtriangles).vindices[2] = v;
          T(numtriangles).material = material;
          //T(numtriangles).texture = texture;
	  group->triangles[group->numtriangles++] = numtriangles;
	  numtriangles++;
	}
      }
      break;

    default:
      /* eat up rest of line */
      fgets(buf, sizeof(buf), file);
      break;
    }
  }

}

/* glmDelete: Deletes a GLMmodel structure.
 *
 * model - initialized GLMmodel structure
 */
GLvoid
glmDelete(GLMmodel* model)
{
  GLMgroup* group;
  GLuint i;

  if (model->pathname)   free(model->pathname);
  if (model->mtllibname) free(model->mtllibname);
  if (model->vertices)   free(model->vertices);
  if (model->normals)    free(model->normals);
  if (model->texcoords)  free(model->texcoords);
  if (model->facetnorms) free(model->facetnorms);
  if (model->triangles)  free(model->triangles);
  if (model->materials) {
    for (i = 0; i < model->nummaterials; i++)
      free(model->materials[i].name);
  }
  free(model->materials);
  while(model->groups) {
    group = model->groups;
    model->groups = model->groups->next;
    free(group->name);
    free(group->triangles);
    free(group);
  }
  GLMtexture* tex = model->texturas;
  GLMtexture* au;
  for(i=0;i<model->numtexturas;i++)
  {
     au = tex;
     tex = tex->proximo;
     free(au->nome);
     glDeleteTextures(1,&(au->indice));
     free(au);
  }

  free(model);
}

/* glmReadOBJ: Reads a model description from a Wavefront .OBJ file.
 * Returns a pointer to the created object which should be free'd with
 * glmDelete().
 *
 * filename - name of the file containing the Wavefront .OBJ format data.  
 */
GLMmodel* 
glmReadOBJ(char* filename, char* diretorioTex)
{
  GLMmodel* model;
  FILE*     file;

  /* open the file */
  file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "glmReadOBJ() falhou: nao consegui abrir arquivo de dados \"%s\".\n",
	    filename);
    exit(1);
  }

#if 0
  /* announce the model name */
  printf("Model: %s\n", filename);
#endif

  /* allocate a new model */
  model = (GLMmodel*)malloc(sizeof(GLMmodel));
  model->pathname      = stralloc(filename);
  model->diretorioTexturas = stralloc(diretorioTex);
  model->mtllibname    = NULL;
  model->numvertices   = 0;
  model->vertices      = NULL;
  model->numnormals    = 0;
  model->normals       = NULL;
  model->numtexcoords  = 0;
  model->texcoords     = NULL;
  model->numfacetnorms = 0;
  model->facetnorms    = NULL;
  model->numtriangles  = 0;
  model->triangles     = NULL;
  model->nummaterials  = 0;
  model->materials     = NULL;
  model->numgroups     = 0;
  model->groups        = NULL;
  model->numtexturas   = 0;
  model->texturas      = NULL;
  model->position[0]   = 0.0;
  model->position[1]   = 0.0;
  model->position[2]   = 0.0;
  model->boundingDefinido = 0;
  model->x1 = model->x2 = model->z1 = model->z2 = 0.0;

  /* make a first pass through the file to get a count of the number
     of vertices, normals, texcoords & triangles */
  _glmFirstPass(model, file);

  /* allocate memory */
  model->vertices = (GLfloat*)malloc(sizeof(GLfloat) *
				     3 * (model->numvertices + 1));
  model->triangles = (GLMtriangle*)malloc(sizeof(GLMtriangle) *
					  model->numtriangles);
  if (model->numnormals) {
    model->normals = (GLfloat*)malloc(sizeof(GLfloat) *
				      3 * (model->numnormals + 1));
  }
  if (model->numtexcoords) {
    model->texcoords = (GLfloat*)malloc(sizeof(GLfloat) *
					2 * (model->numtexcoords + 1));
  }

  /* rewind to beginning of file and read in the data this pass */
  rewind(file);

  _glmSecondPass(model, file);

  /* close the file */
  fclose(file);

  glmPrecomputaListas(model, GLM_NONE | GLM_COLOR | GLM_SMOOTH | GLM_TEXTURE);

  /* //"Normaliza" NAO USADO!!!
  model->x2 -= model->x1;
  model->z2 -= model->z1;
  model->x1 = 0;
  model->z1 = 0;*/

  printf("%s : %.3f,%.3f %.3f,%.3f\n",filename,model->x1,model->z1,model->x2, model->z2);
  return model;
}


void glmPrecomputaListas(GLMmodel* model, GLuint mode)
{
    GLuint i;
    GLMgroup* group;
    int texturaAtual = -1;

    /* do a bit of warning */
    if (mode & GLM_FLAT && !model->facetnorms) {
      mode &= ~GLM_FLAT;
    }
    if (mode & GLM_SMOOTH && !model->normals) {
      mode &= ~GLM_SMOOTH;
    }
    if (mode & GLM_TEXTURE && !model->texcoords) {
      mode &= ~GLM_TEXTURE;
    }
    if (mode & GLM_FLAT && mode & GLM_SMOOTH) {
      mode &= ~GLM_FLAT;
    }
    if (mode & GLM_COLOR && !model->materials) {
      mode &= ~GLM_COLOR;
    }
    if (mode & GLM_MATERIAL && !model->materials) {
      mode &= ~GLM_MATERIAL;
    }
    if (mode & GLM_COLOR && mode & GLM_MATERIAL) {
       mode &= ~GLM_COLOR;
    }
    if (mode & GLM_COLOR)
      glEnable(GL_COLOR_MATERIAL);
    if (mode & GLM_MATERIAL)
      glDisable(GL_COLOR_MATERIAL);

  group = model->groups;


  while (group) {
    
    /*if (mode & GLM_COLOR) {
      glColor3fv(model->materials[group->material].diffuse);
    }*/

    group->listaDesenhar = glGenLists(1);
    glNewList(group->listaDesenhar,GL_COMPILE);

    glBegin(GL_TRIANGLES); 
  
    for (i = 0; i < group->numtriangles; i++) {
 
      /* Habilita Nova Textura, se necessario */
      if ((mode & GLM_TEXTURE) && (T(group->triangles[i]).texture!=-1) &&
          (T(group->triangles[i]).texture!=texturaAtual) )
      {
         glEnd();
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, T(group->triangles[i]).texture);
         texturaAtual = T(group->triangles[i]).texture;
         glBegin(GL_TRIANGLES);
      }
      /* Desabilita Textura Existente, caso necessario */
      else if( (texturaAtual == -1) && (T(group->triangles[i]).texture==-1))
      {
         texturaAtual = -1;
         glDisable(GL_TEXTURE_2D);
      }
      /* Define o Material */
      if (mode & GLM_MATERIAL) 
      {
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, 
	   	   model->materials[T(group->triangles[i]).material].ambient);
         glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, 
		   model->materials[T(group->triangles[i]).material].diffuse);
         glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, 
		   model->materials[T(group->triangles[i]).material].specular);
         glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 
   		   model->materials[T(group->triangles[i]).material].shininess);
      }
      /* Define a Cor */
      if (mode & GLM_COLOR) 
        glColor3fv( model->materials[T(group->triangles[i]).material].diffuse);
      /**/
      if (mode & GLM_FLAT)
	glNormal3fv(&model->facetnorms[3 * T(group->triangles[i]).findex]);
      
      if (mode & GLM_SMOOTH)
	glNormal3fv(&model->normals[3 * T(group->triangles[i]).nindices[0]]);
      if (mode & GLM_TEXTURE)
	glTexCoord2fv(&model->texcoords[2*T(group->triangles[i]).tindices[0]]);
      
      glVertex3fv(&model->vertices[3 * T(group->triangles[i]).vindices[0]]);
      
      if (mode & GLM_SMOOTH)
	glNormal3fv(&model->normals[3 * T(group->triangles[i]).nindices[1]]);
      if (mode & GLM_TEXTURE)
	glTexCoord2fv(&model->texcoords[2*T(group->triangles[i]).tindices[1]]);
      
      glVertex3fv(&model->vertices[3 * T(group->triangles[i]).vindices[1]]);
      
      if (mode & GLM_SMOOTH)
	glNormal3fv(&model->normals[3 * T(group->triangles[i]).nindices[2]]);
      if (mode & GLM_TEXTURE)
	glTexCoord2fv(&model->texcoords[2*T(group->triangles[i]).tindices[2]]);

      glVertex3fv(&model->vertices[3 * T(group->triangles[i]).vindices[2]]);

    }
    glEnd();
    glEndList();

    group = group->next;
  }
  if (texturaAtual != -1)
     glDisable(GL_TEXTURE_2D);
}


/* glmDraw: Renders the model to the current OpenGL context using the
 * mode specified.
 *
 * model    - initialized GLMmodel structure
 */
GLvoid
glmDraw(GLMmodel* model)
{
  GLMgroup* group;
  
  glPushMatrix();
  glTranslatef(model->position[0], model->position[1], model->position[2]);

  group = model->groups;


  while (group) {
    
    glPushMatrix();
    glScalef(group->escala[0],group->escala[1],group->escala[2]);
    glTranslatef(group->translacao[0],group->translacao[1],group->translacao[2]);
    glRotatef(group->rotacao[0],1,0,0);
    glRotatef(group->rotacao[1],0,1,0);
    glRotatef(group->rotacao[2],0,0,1);
    /*if (mode & GLM_COLOR) {
      glColor3fv(model->materials[group->material].diffuse);
    }*/
    glCallList(group->listaDesenhar);    

    glPopMatrix();
    group = group->next;
  }
  glPopMatrix();
  return;
}

