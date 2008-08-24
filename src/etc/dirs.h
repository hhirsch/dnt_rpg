
#ifndef _dirs_h
#define _dirs_h

#include <string>
using namespace std;


/*! The directory abstraction path */
class dirs
{
   public:
      /*! Find Where on the system data current is:
       * (first try to find at ../data, if not found, isn´t running the local
       *  state, so try at the DATADIR, if still not found, we must have an 
       *  error) */
      void findDataDirectories();

      /*! Get the real path and file of an internal file path
       * \param fileName -> internal file path
       * \return -> string with the real path of the file */
      string getRealFile(string fileName);

      /*! Get the relative (internal) file path of a real path file
       * \param fileName -> real file path
       * \return -> string with the internal path of the file */
      string getRelativeFile(string fileName);

   protected:
      static string dataPath; /**< Current Data Path */
};


#endif


