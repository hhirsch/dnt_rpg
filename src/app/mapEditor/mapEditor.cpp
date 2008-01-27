#include "mapEditor.h"
#include "../../etc/dirs.h"

/************************************************************************
 *                              Main                                    *
 ************************************************************************/
int main(int argc, char **argv)
{
   dirs dir;
   dir.findDataDirectories();
   editor* edt;
   
   edt = new(editor);
   edt->run();
   delete(edt);

   return(1);
}


 
