#include "iaEditor.h"

/************************************************************************
 *                              Main                                    *
 ************************************************************************/
int main(int argc, char **argv)
{
   editor* edt;

   edt = new(editor);
   edt->run();
   delete(edt);

   return(1);
}


 
