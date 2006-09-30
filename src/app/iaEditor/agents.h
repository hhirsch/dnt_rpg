#ifndef _agents_h
#define _agents_h

#include "../../ia/potentAgent.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define MAX_AGENTS_POTENT 15

#define AGENT_TYPE_POTENT 1

class agents
{
   public:
      agents();
      ~agents();

      void actualize();
      void draw();
      void addAgent(int type, GLfloat x, GLfloat z, bool oriented, 
                    GLfloat stepSize, GLfloat goalX, GLfloat goalZ, 
                    GLfloat sightDist, GLfloat sightAng);

   private:
      potentAgent* potentAgents;
      int totalPotentAgents;

      void drawPotentAgent();
};

#endif

