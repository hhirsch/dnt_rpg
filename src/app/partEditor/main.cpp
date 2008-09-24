
#include "editor.h"

/***********************************************************************
 *                            Main Method                              *
 ***********************************************************************/
int main(int argc, char **argv)
{
   editor* partEditor = new editor();

   partEditor->run();

   delete(partEditor);
}


