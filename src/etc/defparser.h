#ifndef _dnt_def_parser_h
#define _dnt_def_parser_h

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

/*! The defTuple class is a representaton of a tuple "key = value"  */
class defTuple
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

      /*! Set the next tuple on list
       * \param n -> pointer to the next tuple */
      void setNext(defTuple* n);
      /*! Set the previous tuple on list
       * \param p -> pointer to the previous tuple */
      void setPrevious(defTuple* p);

      /*! Get the next tuple on list
       * \return -> pointer to the next tuple */
      defTuple* getNext();
      /*! Get the previous tuple on list
       * \return -> pointer tot the previous tuple */
      defTuple* getPrevious();

   protected:
      string value;         /**< Tuple Value */
      string key;           /**< Tuple Key */

      defTuple* next;       /**< Next Tuple */
      defTuple* previous;   /**< Previous Tuple */
};

/*! The defParser class is the implementation of a definitions
 * file parser (those with "key = value"). It generate tuples of 
 * keys and values to be interpreted by the real file interpreter. */
class defParser
{
   public:
      /*! Constructor */
      defParser();
      /*! Destructor */
      ~defParser();

      /*! Load File 
       * \param -> fileName -> file name to load */
      bool load(string fileName);

      /*! Get the next tuple from the list
       * \param key -> string with the next key
       * \param value -> string with the next value 
       * \return -> true if can get the next tuple, false if no 
       *            more tuples to get */
      bool getNextTuple(string& key, string& value);

   protected:

      /*! Insert a tuple into linked list
       * \param key -> tuple key
       * \param value -> tuple value */
      void insertTuple(string key, string value);

      defTuple* firstTuple;    /**< Tuples Circular Linked List */
      int totalTuples;         /**< Total Tuples on list */
      
      defTuple* curTuple;      /**< Current Tuple */
};

#endif

