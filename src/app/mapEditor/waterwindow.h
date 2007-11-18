#ifndef _waterwindow_h
#define _waterwindow_h

#include "../../particle/partSystem.h"
#include "../../gui/farso.h"

/*! The water controller window */
class waterWindow
{
   public:
      /*! Constructor
       * \param actualGui -> actual used interface */
      waterWindow(interface* actualGui);

      /*! Destructor */
      ~waterWindow();

      /*! Set the active Water
       * \param water -> new active water */
      void setWater(part1* water);

      /*! Set the active plane
       * \param pl -> new active Plane */
      void setPlane(interPlane* pl);

      /*! Set the used particle system contoller
       * \param pS -> the particle system controller */
      void setPartSystem(partSystem* pS);

      /*! Verify if some event is related to the window
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the window */
      bool eventGot(int type, guiObject* object);

      /*! Draw the temporary things to the screen */
      void drawTemporary();
      
   protected:

      /*! Open the water window */
      void openWindow();

      /*! Define the Values amd put on the boxes */
      void defineValues();

      part1* activeWater;                 /**< The active Water */
      interPlane* activePlane;            /**< The active Plane */
      partSystem* pSystem;                /**< The active Controller */
      interface* gui;                     /**< The used Interface */
   

      window* intWindow;                  /**< The internal window pointer */

      tabButton* waterTab;                /**< Water Tab */
      oneTabButton* buttonDestroyWater;   /**< Destroy active water */
      oneTabButton* buttonPreviousWater;  /**< Previous Water */
      oneTabButton* buttonNextWater;      /**< Next Water */

      tabButton* waterMoveTab;            /**< Water Move tab */
      oneTabButton* buttonMoveWaterUp;    /**< Water Move Up */
      oneTabButton* buttonMoveWaterDown;  /**< Water Move Down */
   
      tabButton* planesTab;               /**< Planes Tab */
      oneTabButton* buttonAddPlane;       /**< Add Plane */
      oneTabButton* buttonDestroyPlane;   /**< Destroy Active Plane */
      oneTabButton* buttonPreviousPlane;  /**< Previous Plane */
      oneTabButton* buttonNextPlane;      /**< Next Plane */

      tabButton* incTypeTab;              /**< Plane Inclination type Tab */
      oneTabButton* buttonNoInclination;  /**< No Inclination */
      oneTabButton* buttonInclinationX;   /**< X axys Inclination */
      oneTabButton* buttonInclinationZ;   /**< Z axys Inclination */

      tabButton* movXZTab;                /**< Move Plane on XZ axys Tab */
      oneTabButton* buttonMoveZMore;      /**< Move More on Z */
      oneTabButton* buttonMoveZLess;      /**< Move Less on Z */
      oneTabButton* buttonMoveXMore;      /**< Move More on X */
      oneTabButton* buttonMoveXLess;      /**< Move Less on X */

      tabButton* movYTab;                 /**< Move Plane on Y axys Tab */
      oneTabButton* buttonMoveYMore;      /**< Move More on Y */
      oneTabButton* buttonMoveYLess;      /**< Move Less on Y */
      oneTabButton* buttonMoveY1More;     /**< Move Y1 More */
      oneTabButton* buttonMoveY1Less;     /**< Move Y1 Less */
      oneTabButton* buttonMoveY2More;     /**< Move Y2 More */
      oneTabButton* buttonMoveY2Less;     /**< Move Y2 Less */

      tabButton* scaleTab;                /**< Plane Scale Tab */
      oneTabButton* buttonScaleXMore;     /**< Scale more on X */
      oneTabButton* buttonScaleXLess;     /**< Scale less on X */
      oneTabButton* buttonScaleZMore;     /**< Scale more on Z */
      oneTabButton* buttonScaleZLess;     /**< Scale less on Z */
      oneTabButton* buttonScaleYMore;     /**< Scale more on Y */
      oneTabButton* buttonScaleYLess;     /**< Scale less on Y */

      button* dxWaterflowMore;            /**< More dX flow */
      textBox* dxWaterflowText;           /**< dx flow text */
      button* dxWaterflowLess;            /**< Less dX flow */
      button* dzWaterflowMore;            /**< More dZ flow */
      textBox* dzWaterflowText;           /**< dz flow text */
      button* dzWaterflowLess;            /**< Less dZ flow */
};


#endif

