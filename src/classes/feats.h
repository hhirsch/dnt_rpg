#ifndef _feats_h
#define _feats_h

#include <string>
using namespace std;

typedef struct _feat
{
   int requeridedLevel;   /* Requerided Character class level to use */
   int requeridedClass;   /* Requerided Character Class */
   int quantityPerDay;    /* Quantity avaible to use per day*/
   int actualQuantity;    /* Actual quantity to use */
   int costToUse;         /* Cost, in PP to use */
   int actionType;        /* Action Type of the feat */
   int action;            /* Defined Action of the feat */
   string name;           /* Feat Name */
   string description;    /* Feat Description */
}feat;


#endif

