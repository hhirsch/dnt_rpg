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

#ifndef _dnt_def_parser_h
#define _dnt_def_parser_h

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "list.h"

/*! The defTuple class is a representaton of a tuple "key = value"  */
class defTuple: public dntListElement
{
   public:
      /*! Constructor */
      defTuple();
      /*! Destructor */
      ~defTuple();

      /*! Set the tuple value
       * \param v -> new value */
      void setValue(string v);
      /*! Set the tuple Key
       * \param k -> new key */
      void setKey(string k);

      /*! Get tuple value
       * \return -> current tuple value */
      string getValue();
      /*! Get the tuple key
       * \return -> current tuple key */
      string getKey();

   protected:
      string value;         /**< Tuple Value */
      string key;           /**< Tuple Key */
};

/*! The defParser class is the implementation of a definitions
 * file parser (those with "key = value"). It generate tuples of 
 * keys and values to be interpreted by the real file interpreter. */
class defParser: public dntList
{
   public:
      /*! Constructor */
      defParser();
      /*! Destructor */
      ~defParser();

      /*! Load File 
       * \param fileName -> file name to load
       * \param fullPath -> if the fileName is a fullPath or not
       * \note -> if not full path, will get the path with dirs */
      bool load(string fileName, bool fullPath=false);

      /*! Get the next tuple from the list
       * \param key -> string with the next key
       * \param value -> string with the next value 
       * \return -> true if can get the next tuple, false if no 
       *            more tuples to get */
      bool getNextTuple(string& key, string& value);

   protected:

      /*! Free a defTuple object
       * \param obj -> pointer to the deftuple to free */
      void freeElement(dntListElement* obj);

      /*! Insert a tuple into linked list
       * \param key -> tuple key
       * \param value -> tuple value */
      void insertTuple(string key, string value);

      defTuple* curTuple;      /**< Current Tuple */
};

#endif

