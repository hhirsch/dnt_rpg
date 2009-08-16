/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "defparser.h"
#include "dirs.h"

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                defTuple                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
defTuple::defTuple()
{
   value = "";
   key = "";
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
defTuple::~defTuple()
{
}

/***********************************************************************
 *                              setValue                               *
 ***********************************************************************/
void defTuple::setValue(string v)
{
   value = v;
}

/***********************************************************************
 *                               setKey                                *
 ***********************************************************************/
void defTuple::setKey(string k)
{
   key = k;
}

/***********************************************************************
 *                              getValue                               *
 ***********************************************************************/
string defTuple::getValue()
{
   return(value);
}


/***********************************************************************
 *                               getKey                                *
 ***********************************************************************/
string defTuple::getKey()
{
   return(key);
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                               defParser                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
defParser::defParser():dntList(DNT_LIST_TYPE_ADD_AT_END)
{
   curTuple = NULL;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
defParser::~defParser()
{
   clearList();
}

/***********************************************************************
 *                           freeElement                               *
 ***********************************************************************/
void defParser::freeElement(dntListElement* obj)
{
   defTuple* tuple = (defTuple*)obj;
   delete(tuple);
}

/***********************************************************************
 *                            insertTuple                              *
 ***********************************************************************/
void defParser::insertTuple(string key, string value)
{
   defTuple* tuple = new defTuple();
 
   /* Set tuple values */
   tuple->setKey(key);
   tuple->setValue(value);

   insert(tuple);
}

/***********************************************************************
 *                            getNextTuple                             *
 ***********************************************************************/
bool defParser::getNextTuple(string& key, string& value)
{
   if(total <= 0)
   {
      /* No tuples to get */
      return(false);
   }

   if(curTuple == NULL)
   {
      /* Will get first tuple */
      curTuple = (defTuple*)first;
   }
   else if(curTuple->getNext() == first)
   {
      /* No more tuples to get */
      return(false);
   }
   else
   {
      /* Get the Next Tuple */
      curTuple = (defTuple*)curTuple->getNext();
   }

   /* Set the tuple */
   key = curTuple->getKey();
   value = curTuple->getValue();
  
   return(true);

}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool defParser::load(string fileName, bool fullPath)
{
   ifstream file;
   string strBuffer, strKey, strData;
   string::size_type pos;
   int line;
   dirs dir;

   /* Open the Definition File */
   if(fullPath)
   {
      file.open(fileName.c_str(), ios::in | ios::binary);
   }
   else 
   {
      file.open(dir.getRealFile(fileName).c_str(), ios::in | ios::binary);
   }

   if(!file)
   {
      cerr << "Error Opening file: " << fileName << endl;
      return(false);
   }

   /* Get the first line */
   getline(file, strBuffer);
   for(line = 1; (!file.eof()); line++)
   {
      /* Find First non white space character */ 
      pos = strBuffer.find_first_not_of(" \t");

      /* Ignore empty lines and comented ones */
      if( (pos != string::npos) && (strBuffer[pos] != '\n') && 
          (strBuffer[pos] != '\r') && (strBuffer[pos] != 0) && 
          (strBuffer[pos] != '#') )
      {
          /* Get the key */
          strKey = strBuffer.substr(pos, 
                              strBuffer.find_first_of(" =\t\n\r", pos) - pos);
          pos += strKey.size();

          /* Get the '=' character */
          pos = strBuffer.find_first_not_of(" \t", pos);
          if((pos == std::string::npos) || (strBuffer[pos] != '='))
          {
             file.close();
             cerr << fileName << "(" << line << "): Invalid syntax." << endl;
             return false;
          }

          /* find the first non-whitespace after the '=' */
          pos = strBuffer.find_first_not_of(" \t", pos + 1);

          /* get the data */
          if(pos != string::npos)
          {
             strData = strBuffer.substr(pos, strBuffer.length() - pos);
          }
          else
          {
             strData = "";
          }

          /* Delete last character if it is an \n or #13.
           * This is an issue that happens mainly on windows.
           * Also, delete it while is an space */
          while( (strData.length() > 0) &&
                 ( (strData[strData.length()-1] == '\n') ||
                   (strData[strData.length()-1] == 13) ||
                   (strData[strData.length()-1] == ' ') ) )
          {
             strData.erase(strData.length()-1);
          }

          /* Insert a tuple */
          insertTuple(strKey, strData);
      }

      /* Get Next Line  */
      getline(file, strBuffer);
   }

   file.close();
   return(true);
}


