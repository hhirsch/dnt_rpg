/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _cores_h
#define _cores_h

/*! A color Definition */
class farso_color
{
   public:
      int R, /**< Red [0,127] */
          G, /**< Green [0,127] */
          B; /**< Blue [0,127] */
};

/*! The GUI Colors Definition */
class farso_colors
{
   public:
      farso_color colorCont[3],       /**< Contorn Colors */
                  colorWindow,        /**< Window Color */ 
                  colorButton,        /**< Button Color */
                  colorMenu,          /**< Menu Color */
                  colorText,          /**< Text Color */
                  colorBar,           /**< Bar Color */
                  colorSelText;       /**< Color Selected Text */
      
       /*! Constructor */
       farso_colors();
};



#endif
