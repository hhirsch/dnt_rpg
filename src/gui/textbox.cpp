/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "textbox.h"

/*******************************************************
 *                       Constructor                   *
 *******************************************************/
textBox::textBox(int xa, int ya, int xb, int yb, int frameType,
                 SDL_Surface *screen)
{
   wSurface = screen;
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
 *                      getTextLine                    *
 *******************************************************/
string textBox::getTextLine(int line)
{
   int i;
   if(line < totalLines)
   {
      /* Get the line */
      textLine* tmp = fullText;
      for(i = 0; i < line; i++)
      {
         tmp = fullText;
         tmp = tmp->next;
      }
      /* Get the text from line */
      return(tmp->text);
   }

   return("");
}

/*******************************************************
 *                        setText                      *
 *******************************************************/
void textBox::setText(string txt)
{
   if(text != txt)
   {
      clear();
      text = txt;
      createLines(txt, fontName, fontSize, fontAlign, fontStyle,
                  Colors.colorText.R, Colors.colorText.G,
                  Colors.colorText.B);
      draw();
   }
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void textBox::addText(string txt, string font, int size,
                      int align, int style, int R, int G, int B)
{
   text += txt;
   createLines(txt, font, size, align, style, R, G, B);
   draw();
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

   /* Verify if exist lines */
   if(!fullText)
   {
      return(0);
   }

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
void textBox::draw(SDL_Surface* screen)
{
   draw();
}

/*******************************************************
 *                          draw                       *
 *******************************************************/
int textBox::draw()
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
                   Colors.colorButton.B,
                   Colors.colorButton.A);
         rectangle_Fill(wSurface,x1+1,y1+1,x2-1,y2-1);
      }
      color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G,
                Colors.colorCont[0].B, Colors.colorCont[0].A);
      rectangle_2Colors(wSurface,x1,y1,x2,y2,
                       Colors.colorCont[1].R, Colors.colorCont[1].G,
                       Colors.colorCont[1].B, Colors.colorCont[1].A);
   }

   if(fullText)
   {
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
         color_Set(line->R,line->G,line->B,255);
         fnt.defineFont(line->fontName, line->fontSize);
         fnt.defineFontAlign(line->fontAlign);
         fnt.defineFontStyle(line->fontStyle);
         fnt.write(wSurface, x1+2, y, line->text, x1+2, y, x2, y2);
         y += line->height;
         height = line->height;
         line = line->next;
      }
      setChanged();
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
 *                      setFirstLine                   *
 *******************************************************/
void textBox::setLastLine(int line)
{
   int cur = line;
   
   /* Find the lesser first line that the last one is
    * still the desired. */
   setFirstLine(line);
   while( (lastDrawableLine() >= line) && (cur > 0) )
   {
      setFirstLine(cur);
      cur--;
   }

   if(cur < line) //to avoid can't write the last!
   {
      /* So, it's the previous one  */
      cur--;
      setFirstLine(cur);
   }
}

/*******************************************************
 *                        setFont                      *
 *******************************************************/
void textBox::setFont(string name, int size, int align)
{
   setFont(name, size, align, DNT_FONT_STYLE_NORMAL);
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

   /* Change all current Lines */
   int i;
   textLine* line = fullText;
   for(i = 0; i < totalLines; i++)
   {
      line->fontName = fontName;
      line->fontAlign = fontAlign;
      line->fontSize = fontSize;
      line->fontStyle = fontStyle;
      line = line->next;
   }
}

/*******************************************************
 *                       setColor                      *
 *******************************************************/
void textBox::setColor(int R, int G, int B)
{
   Colors.colorText.R = R;
   Colors.colorText.G = G;
   Colors.colorText.B = B;

   /* Change all current Lines */
   int i;
   textLine* line = fullText;
   for(i = 0; i < totalLines; i++)
   {
      line->R = R;
      line->G = G;
      line->B = B;
      line = line->next;
   }
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

