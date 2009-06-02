
#ifndef _dnt_minimap_window_h
#define _dnt_minimap_window_h

#include "../gui/farso.h"
#include "../map/map.h"

/*! The minimap window */
class miniMapWindow
{
   public:
      /*! Constructor */
      miniMapWindow();
      /*! Destructor */
      ~miniMapWindow();

      /*! Open the minimap window (if not already opened)
       * \param gui -> guiInterface where the window will open
       * \param posX -> character X position
       * \param posZ -> character Z position 
       * \param openedMap -> current opened map */
      void open(guiInterface* gui, float posX, float posZ,
                Map* openedMap);
      /*! Close the minimap window (if opened)
       * \param gui -> guiInterface where the window is */
      void close(guiInterface* gui);

      /*! Update the active character map position
       * \param posX -> character X position
       * \param posZ -> character Z position */
      void updateCharacterPosition(float posX, float posZ);

      /*! Verify if the minimap window is opened
       * \return -> true if is opened, false otherwise */
      bool isOpened();

   protected:
      static int width;             /**< Current picture width */
      static int height;            /**< Current picture height */
      static picture* fig;          /**< The current minimap picture */
      static window* mapWindow;     /**< MiniMap Window */
      static button* charPosition;  /**< Character MiniMap Button */
      static Map* curMap;           /**< Current Opened Map */
};

#endif

