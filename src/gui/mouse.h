/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _mouse_h
#define _mouse_h

/*! Verify if the mouse coordinate is indoor to the are defined
 * \param x1 -> x1 coordinate
 * \param y1 -> y1 coordinate
 * \param x2 -> x2 coordinate
 * \param y2 -> y2 coordinate
 * \param mouseX -> mouse X coordinate
 * \param mouseY -> mouse Y coordinate 
 * \return true id on area, false otherwise */
bool isMouseIn(int x1,int y1,int x2,int y2, int mouseX, int mouseY);

#endif

