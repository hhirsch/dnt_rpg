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
   next = NULL;
   previous = NULL;
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

/***********************************************************************
 *                              setNext                                *
 ***********************************************************************/
void defTuple::setNext(defTuple* n)
{
   next = n;
}

/***********************************************************************
 *                             setPrevious                             *
 ***********************************************************************/
void defTuple::setPrevious(defTuple* p)
{
   previous = p;
}

/***********************************************************************
 *                              getNext                                *
 ***********************************************************************/
defTuple* defTuple::getNext()
{
   return(next);
}

/***********************************************************************
 *                             getPrevious                             *
 ***********************************************************************/
defTuple* defTuple::getPrevious()
{
   return(previous);
}


///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                               defParser                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
defParser::defParser()
{
   totalTuples = 0;
   firstTuple = NULL;
   curTuple = NULL;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
defParser::~defParser()
{
   int i;
   defTuple* aux;
   /* Remove all tuples */
   for(i=0; i < totalTuples; i++)
   {
      aux = firstTuple;
      firstTuple = firstTuple->getNext();
   }
}

/***********************************************************************
 *                            insertTuple                              *
 ***********************************************************************/
void defParser::insertTuple(string key, string value)
{
   defTuple* tuple = new defTuple();
 
   //cout << "Inserting tuple '" << key << "' value '" << value << "'" << endl;

   /* Set tuple values */
   tuple->setKey(key);
   tuple->setValue(value);

   if(firstTuple == NULL)
   {
      /* It's the first tuple */
      firstTuple = tuple;

      tuple->setNext(tuple);
      tuple->setPrevious(tuple);
   }
   else
   {
      /* Insert it at list's end */
      tuple->setNext(firstTuple);
      tuple->setPrevious(firstTuple->getPrevious());

      tuple->getNext()->setPrevious(tuple);
      tuple->getPrevious()->setNext(tuple);
   }
 
   /* Inc it */
   totalTuples++;
}

/***********************************************************************
 *                            getNextTuple                             *
 ***********************************************************************/
bool defParser::getNextTuple(string& key, string& value)
{
   if(totalTuples <= 0)
   {
      /* No tuples to get */
      return(false);
   }

   if(curTuple == NULL)
   {
      /* Will get first tuple */
      curTuple = firstTuple;
   }
   else if(curTuple->getNext() == firstTuple)
   {
      /* No more tuples to get */
      return(false);
   }
   else
   {
      /* Get the Next Tuple */
      curTuple = curTuple->getNext();
   }

   /* Set the tuple */
   key = curTuple->getKey();
   value = curTuple->getValue();
  
   return(true);

}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool defParser::load(string fileName)
{
   ifstream file;
   string strBuffer, strKey, strData;
   string::size_type pos;
   int line;
   dirs dir;

   /* Open the Book Definition File */
   file.open(dir.getRealFile(fileName).c_str(), ios::in | ios::binary);
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
          strData = strBuffer.substr(pos, strBuffer.length() - pos);

          /* Insert a tuple */
          insertTuple(strKey, strData);
      }

      /* Get Next Line  */
      getline(file, strBuffer);
   }

   file.close();
   return(true);
}


