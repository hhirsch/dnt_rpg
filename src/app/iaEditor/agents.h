#ifndef _agents_h
#define _agents_h

#include "../../ia/potentAgent.h"
#include "../../ia/pattAgent.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define AGENT_TYPE_POTENT  1
#define AGENT_TYPE_PATTERN 2

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
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int tool);

   private:
      potentAgent* potentAgents;
      int totalPotentAgents;
      GLuint potentialTexture;

      pattAgent* pattAgents;
      int totalPattAgents;
      GLuint pattTexture;

      agent* actualAgent;
      int state;

      GLfloat goalX;
      GLfloat goalZ;

      void drawPotentAgent();
      void drawPattAgent();
      void addVisibleAgents(agent* ag);
};

#endif

