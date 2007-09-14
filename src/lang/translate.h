#ifndef _translate_h
#define _translate_h

#include <libintl.h>
#include <string>
using namespace std;

/*! Translate a data string readed from a data file.
 * \param dataString -> translatable data string readed from a data file 
 * \return -> translated string for the input data. */
string translateDataString(string dataString);


#endif

