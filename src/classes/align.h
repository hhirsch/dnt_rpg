#ifndef _align_h
#define _align_h

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
using namespace std;

/*! Single Alignment Definition */
class align
{
   public:
      int intID;              /**< Integer Identifier of the alignment */
      string strID;           /**< String Identifier of the alignment */
      SDL_Surface* image;     /**< Image of the Aligment */
      
      string name;            /**< Name of the Aligment */
      string description;     /**< Description of the Aligment */
      
      align* previous;        /**< Previous Aligment on list */
      align* next;            /**< Next Aligment on list */
};

/**! The List of Alignments Class */
class aligns
{
   public:
      /*! Constructor
       * \param fileListName -> name of the file containing the list of all
       *         aligments (usually ../data/alignment/alignment.lst)  
       * \param directory -> directory of the Descriptions to use. */
      aligns(string directory, string fileListName);
      /*! Destructor */
      ~aligns();

      /*! Get an Alignment from list
       * \param id -> integer ID of the alignment
       * \return aligment found, if found one. */
      align* getAlignByInteger(int id);
      /*! Get an Aligment from list 
       * \param id -> string ID of the alignment
       * \return aligment found, if found one. */
      align* getAlignByString(string id);
      
   private:
      int totalAlignments;          /**< Total Alignments on List */
      align* first;                 /**< First Alignment on List */

      /*! Insert an alignment on List
       * \param fileName -> fileName of the alignment to insert 
       * \param idString -> String Identificator of the alignment 
       * \param idInt -> Integer Identificator of the the alignment */
      void insertAlign(string fileName, string idString, int idInt);
};


#endif
