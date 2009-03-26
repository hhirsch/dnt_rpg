#ifndef _dnt_map_sound_h
#define _dnt_map_sound_h

#include "../sound/sound.h"

/*! The soundInfo class keeps informations about a sound on a map */
class soundInfo
{
   public:
      string fileName;   /**< Ogg File Name */
      ALfloat x,         /**< Sound X Position */
              y,         /**< Sound Y Position */
              z;         /**< Sound Z Position */
      int loopInterval;  /**< Sound Loop Interval */

      soundInfo* next;     /**< Next Sound Info */
      soundInfo* previous; /**< Previous Sound Info */
};

/*! The mapSound class contains information of all sounds related to the 
 * current map (position, ogg file, etc.) */
class mapSound
{
   public:
      /*! Constructor */
      mapSound();

      /*! Destructor */
      ~mapSound();

      /*! Load mapSound definition file
       * \param fileName -> file name of the definition to load
       * \return -> true if load was successfull, falst otherwise */
      bool load(string fileName);

      /*! Save mapSound definition file with the current sound list
       * \param fileName -> name of the file to save
       * \return -> true if save was successfull */
      bool save(string fileName);

      /*! Flush Sounds, adding it to the OpenAL sound controller */
      void flush();

      /*! Add a new undefined sound to the definition.
       * \note -> used at mapEditor
       * \return -> pointer to the added soundInfo structure */
      soundInfo* newSound();

   protected:

      /*! Add sound to the soundMap
       * \param snd -> pointer to the soundInfo to add */
      void addSound(soundInfo* snd);

      soundInfo* sounds;  /**< soundInfo List */
      int totalSounds;    /**< Total Sounds on the list */
};

#endif

