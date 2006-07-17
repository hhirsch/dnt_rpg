#ifndef _mapobjeto_h
#define _mapobjeto_h

/*   DCC Nightmare is Public Domain - Do whatever you want with this code.
 */

#include "../gui/objeto.h"
#include "../gui/lista.h"

/*!
 ****************************************************
 *               Map's Objects                      *
 ****************************************************/
class mapObjeto: public Tobjeto
{
   public:
     int deltaVariacao;   /**< For how many distance change static LOD*/
     char* nome;          /**< Name of the object  */
     char* nomeArq;       /**< File Name of the object */

     /*!
      * Draws the object
      * \param x -> X position 
      * \param z -> Z posotion
      * \param distancia -> distance from observator (defines the LOD used)
      * \param orientacao -> orientation angle of the object */
     void Desenhar(float x, float z,GLfloat distancia, float orientacao);

     void* modelo3d;     /**< Max LOD Model */
     void* modeloMedio;  /**< Med LOD Model */
     void* modeloMinimo; /**< Min LOD Model */
};

/*! Map Objects list */
class LmapObjeto: public Tlista
{
    public:
       /*! Constructor */
       LmapObjeto():Tlista(){};
       /*! Destruictor */
       ~LmapObjeto();

       /*!
        * Insert object on map list
        * \param arquivo -> filename to load
        * \param nome -> object name 
        * \return pointer to the map Object created */
       mapObjeto* InserirMapObjeto(const char* arquivo, const char* nome);
       /*!
        * Search for object on map list
        * \param nome -> object name 
        * \return pointer to the map Object founded. */
       mapObjeto* EndMapObjeto(const char* nome);

       /*!
        * Remove object from list
        * \param obj-> pointer to the object to remove.
        * \param tiraMemoria -> != 0 if is to remove the memory from list,              *                or 0 when calling by the destructor.*/
       void RetirarMapObjeto(mapObjeto* obj, int tiraMemoria);
}; 

#endif
