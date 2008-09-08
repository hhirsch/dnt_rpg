#include "mapEditor.h"
#include "../../etc/dirs.h"
#include "../../etc/extensions.h"
#include "../../engine/options.h"

/************************************************************************
 *                              Main                                    *
 ************************************************************************/
int main(int argc, char **argv)
{

   dirs dir;
   dir.findDataDirectories();
   options opt;
   opt.load();
   editor* edt;

   edt = new(editor);
   edt->run();
   delete(edt);

   return(1);
}


 
