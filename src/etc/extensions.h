#ifndef _extensions_h
#define _extensions_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

/*! The extensons class load, if avalaible, openGL extensions
 * functions used by DNT. */
class extensions
{
   public:
      /* Texture per points functions */
      static PFNGLPOINTPARAMETERFARBPROC arbPointParameterf;   
      static PFNGLPOINTPARAMETERFVARBPROC arbPointParameterfv;

      /* Multi Textures Functions */
      static PFNGLACTIVETEXTUREPROC arbActiveTexture;
      static PFNGLCLIENTACTIVETEXTUREPROC arbClientActiveTexture;
      static PFNGLMULTITEXCOORD2FPROC arbMultiTexCoord2f;
      static PFNGLMULTITEXCOORD2FVPROC arbMultiTexCoord2fv;
      static PFNGLMULTITEXCOORD2DPROC arbMultiTexCoord2d;
      static PFNGLMULTITEXCOORD2DVPROC arbMultiTexCoord2dv;

      /* Shader functions */
      static PFNGLCREATESHADEROBJECTARBPROC arbCreateShaderObject;
      static PFNGLCREATEPROGRAMOBJECTARBPROC arbCreateProgramObject;
      static PFNGLDELETEOBJECTARBPROC arbDeleteObject;
      static PFNGLSHADERSOURCEARBPROC arbShaderSourceObject;
      static PFNGLCOMPILESHADERARBPROC arbCompileShader;
      static PFNGLATTACHOBJECTARBPROC arbAttachObject;
      static PFNGLLINKPROGRAMARBPROC arbLinkProgram;
      static PFNGLUSEPROGRAMOBJECTARBPROC arbUseProgram;
      static PFNGLGETHANDLEARBPROC arbGetHandle;
      static PFNGLGETUNIFORMLOCATIONARBPROC arbGetUniformLocation;
      static PFNGLGETOBJECTPARAMETERFVARBPROC arbGetObjectParameterfv;
      static PFNGLGETOBJECTPARAMETERIVARBPROC arbGetObjectParamenteriv;
      static PFNGLUNIFORM1FARBPROC arbUniform1f;
      static PFNGLUNIFORM2FARBPROC arbUniform2f;
      static PFNGLUNIFORM1FARBPROC arbUniform3f;


      /*! Gets all Extended Functions that DNT uses. Usually call this after
       * the init of the Engine. */
      void defineAllExtensions();

      /*! Verify if the machine have the point texture extension
       * \return true if the extension is available */
      bool hasPointTexture();
      /*! Verify if the machine have the multi texture extension
       * \return true if the extension is available */
      bool hasMultiTexture();
      /*! Verify if the machine have the shader extension
       * \return true if the extension is available */
      bool hasShader();

   protected:
      /*! Define Point per Texture functions
       * \param ext -> extensions string */
      void definePointTexture(string ext);
      /*! Define MultiTexture functions
       * \param ext -> extensions string */
      void defineMultiTexture(string ext);
      /*! Define shader functions
       * \param ext -> extensions string */
      void defineShader(string ext);

      /*! Get OpenGL Function
       * \param functionName -> function to get
       * \return -> pointer to the function found or NULL */
      void* getFunction(string functionName);

      /*! Sow an warning about not found some extension function 
       * \param functionName -> string wit function name to displa warning of*/
      void showWarning(string functionName);

};

#endif

