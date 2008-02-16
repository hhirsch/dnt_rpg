/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "textbox.h"

/*******************************************************
 *                       Constructor                   *
 *******************************************************/
textBox::textBox(int xa, int ya, int xb, int yb, int frameType)
{
   type = GUI_TEXT_BOX;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   framed = frameType;
   fontAlign = DNT_FONT_ALIGN_LEFT;
   fontName = DNT_FONT_ARIAL;
   fontStyle = DNT_FONT_STYLE_NORMAL;
   fontSize = 10;
   fullText = NULL;
   totalLines = 0;
   firstLine = 0;
}

/*******************************************************
 *                        Destructor                   *
 *******************************************************/
textBox::~textBox()
{
   clear();
}

/*******************************************************
 *                        clear                        *
 *******************************************************/
void textBox::clear()
{
   int i;
   textLine* tmp;
   for(i = 0; i < totalLines; i++)
   {
      tmp = fullText;
      fullText = fullText->next;
      delete(tmp);
   }
   fullText = NULL;
   totalLines = 0;
}

/*******************************************************
 *                        setText                      *
 *******************************************************/
void textBox::setText(string txt)
{
   clear();
   text = txt;
   createLines(txt, fontName, fontSize, fontAlign, fontStyle,
               Colors.colorText.R, Colors.colorText.G,
               Colors.colorText.B);
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void textBox::addText(string txt, string font, int size,
                      int align, int style, int R, int G, int B)
{
   text += txt;
   createLines(txt, font, size, align, style, R, G, B);
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void textBox::addText(string txt, string font, int size,
                      int align, int style)
{
   addText(txt, font, size, align, style,
               Colors.colorText.R, Colors.colorText.G,
               Colors.colorText.B);
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void textBox::addText(string txt)
{
   addText(txt, fontName, fontSize, fontAlign, fontStyle, 
               Colors.colorText.R, Colors.colorText.G,
               Colors.colorText.B);
}

/*******************************************************
 *                  lastDrawableLines                  *
 *******************************************************/
int textBox::lastDrawableLine()
{
   int i;
   textLine* line;
   int lastLine = 0;
   int height = 0;

   /* Get first text line */
   line = fullText;
   for(i = 0; i < firstLine; i++)
   {
      height = line->height;
      line = line->next;
   }
   
   int y = y1+2;
   /* Draw the text lines */
   for(i = firstLine; ( (y+height) < y2) ; i++)
   {
      lastLine = i;
      if( i < totalLines)
      {
         y += line->height;
         height = line->height;
         line = line->next;
      }
      else
      {
         y+= height;
      }
   }
   return(lastLine);
}


/*******************************************************
 *                          draw                       *
 *******************************************************/
int textBox::draw(SDL_Surface *screen)
{
   int i;
   textLine* line;
   dntFont fnt;
   int lastLine = 0;
   int height = 0;

   /* Draw Background */
   if(framed)
   {
      if(framed == 1)
      {
         color_Set(Colors.colorButton.R,
                   Colors.colorButton.G,
                   Colors.colorButton.B);
         rectangle_Fill(screen,x1+1,y1+1,x2-1,y2-1);
      }
      color_Set(Colors.colorCont[0].R,Colors.colorCont[0].G,
                Colors.colorCont[0].B);
      rectangle_2Colors(screen,x1,y1,x2,y2,
                       Colors.colorCont[1].R,Colors.colorCont[1].G,
                       Colors.colorCont[1].B);
   }

   /* Get first text line */
   line = fullText;
   for(i = 0; i < firstLine; i++)
   {
      line = line->next;
   }
   
   int y = y1+2;
   /* Draw the text lines */
   for(i = firstLine; 
       ( (y+height < y2) && (i < totalLines) ) ; i++)
   {
      lastLine = i;
      color_Set(line->R,line->G,line->B);
      fnt.defineFont(line->fontName, line->fontSize);
      fnt.defineFontAlign(line->fontAlign);
      fnt.defineFontStyle(line->fontStyle);
      fnt.write(screen, x1+2, y, line->text, x1+2, y, x2, y2);
      y += line->height;
      height = line->height;
      line = line->next;
   }
   return(lastLine);
}

/*******************************************************
 *                     getTotalLines                   *
 *******************************************************/
int textBox::getTotalLines()
{
   return(totalLines);
}

/*******************************************************
 *                      getFirstLine                   *
 *******************************************************/
int textBox::getFirstLine()
{
   return(firstLine);
}

/*******************************************************
 *                      setFirstLine                   *
 *******************************************************/
void textBox::setFirstLine(int line)
{
   firstLine = line;
}


/*******************************************************
 *                        setFont                      *
 *******************************************************/
void textBox::setFont(string name, int size, int align)
{
   fontName = name;
   fontAlign = align;
   fontSize = size;
   fontStyle = DNT_FONT_STYLE_NORMAL;
}

/*******************************************************
 *                        setFont                      *
 *******************************************************/
void textBox::setFont(string name, int size, int align, int style)
{
   fontName = name;
   fontAlign = align;
   fontSize = size;
   fontStyle = style;
}

/*******************************************************
 *                       setColor                      *
 *******************************************************/
void textBox::setColor(int R, int G, int B)
{
   Colors.colorText.R = R;
   Colors.colorText.G = G;
   Colors.colorText.B = B;
}

/*******************************************************
 *                     setBackColor                    *
 *******************************************************/
void textBox::setBackColor(int R, int G, int B)
{
   Colors.colorButton.R = R;
   Colors.colorButton.G = G;
   Colors.colorButton.B = B;
}

/*******************************************************
 *                     insertLine                      *
 *******************************************************/
void textBox::insertLine(textLine* line)
{
   if(totalLines == 0)
   {
      line->next = line;
      line->previous = line;
      fullText = line;
   }
   else
   {
      line->next = fullText;
      line->previous = fullText->previous;
      line->next->previous = line;
      line->previous->next = line;
   }
   totalLines++;
}

/*******************************************************
 *                     createLines                     *
 *******************************************************/
void textBox::createLines(string txt, string font, int size,
                          int align, int style, int R, int G, int B)
{
   dntFont fnt;
   fnt.defineFont(font, size);
   fnt.defineFontAlign(align);
   fnt.defineFontStyle(style);

   textLine* line = NULL;
   int lastPos = 0;

   while(lastPos < (int)txt.length())
   {
      line = new textLine();
      line->R = R; 
      line->G = G;
      line->B = B;
      line->fontName = font;
      line->fontSize = size;
      line->fontAlign = align;
      line->fontStyle = style;
      line->text = fnt.getNextLine(txt, lastPos, x2-x1-2);
      line->height = fnt.getHeight();
      insertLine(line);
   }
}

