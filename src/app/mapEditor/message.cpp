#include "message.h"

string messageForTool(int tool)
{
   switch(tool)
   {
      case TOOL_TERRAIN_UP: 
         return("Select Square Vertex to Up"); 
      break;
      case TOOL_TERRAIN_NIVELATE:
         return("Select Area to Nivelate");
      break;
      case TOOL_TERRAIN_DOWN:
         return("Select Square Vertex to Down");
      break;
      case TOOL_TERRAIN_TEXTURE:
         return("Select Square to Texturize");
      break;
      case TOOL_PORTAL_ADD:
         return("Select Area to Add Portal (Max One portal per Square)");
      break;
      case TOOL_PORTAL_TAG:
         return("Select Portal to Define Destiny");
      break;
   }
   return("DccNiTghtmare!");
}

