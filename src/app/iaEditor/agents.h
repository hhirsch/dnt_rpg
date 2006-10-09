#ifndef _agents_h
#define _agents_h

#include "../../ia/potentAgent.h"
#include "../../ia/pattAgent.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;


#define AGENT_TYPE_POTENT  1
#define AGENT_TYPE_PATTERN 2

#define AGENT_POTENT_HALF_X 5.0
#define AGENT_POTENT_HALF_Z 5.0
#define AGENT_POTENT_HIGHT  6.0

#define AGENT_PATT_HALF_X 5.0
#define AGENT_PATT_HALF_Z 5.0
#define AGENT_PATT_HIGHT  6.0


/*! The Agents Editor Class */
class agents
{
   public:
      /*! Constructor */
      agents();
      /*! Destructor */
      ~agents();

      /*! Actualize the states of all Agents on Simulation */
      void actualize();
      /*! Draw All Agents */
      void draw();
      /*! Add Agent to the Editor */
      void addAgent(int type, GLfloat x, GLfloat z, bool oriented, 
                    GLfloat stepSize, GLfloat goalX, GLfloat goalZ, 
                    GLfloat sightDist, GLfloat sightAng);
      /*! Verify Agents Editor Actions */
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int tool);

      /*! Saves the current State to file
       * \param fileName -> name of the file to be saved */
      string saveState(string fileName);
      /*! Loads state from file
       * \param fileName -> name of the file to be loaded */
      string loadState(string fileName);

   private:
      potentAgent* potentAgents;    /**< List of Potential Functions Agents */
      int totalPotentAgents;        /**< Total Potential Functions Agents */
      GLuint potentialTexture;      /**< Texture used by Potential Agents */

      pattAgent* pattAgents;        /**< List of Pattern Agents */
      int totalPattAgents;          /**< Total Pattern Agents on List */
      GLuint pattTexture;           /**< Texture Used by Pattern Agents */

      agent* actualAgent;           /**< Currenct on Edition Agent */
      
      int state;                    /**< Current Editor State */

      GLfloat goalX;                /**< Defined X Coordinate of Goal */
      GLfloat goalZ;                /**< Defined Z Coordinate of Goal */
      GLuint goalTexture;           /**< Goal Texture */

      /*! Draw one Pontential Agent on Origin */
      void drawPotentAgent();       
      /*! Draw One Pattern Agent on Origin */
      void drawPattAgent();
      /*! Add Visible Agents to agent obstacle
       * \param ag -> active agent */
      void addVisibleAgents(agent* ag);
      /*! Remove Potential Function Agents Colliding with pattern Agent 
       * \param patAg -> pattern Agent to be verified */
      void removeColliders(pattAgent* patAg);
      /*! Remove Potential Function Agents from potential agents list 
       * \param potAg -> potential Agent to be removed */
      potentAgent* removePotentAgent(potentAgent* potAg);
      /*! Remove Pattern Agents from list 
       * \param patAg -> pattern Agent to be removed */
      pattAgent* removePattAgent(pattAgent* patAg);
      /*! Redefines potential agents goal position
       * \param x -> new x goal coordinate
       * \param z -> new z goal coordinate */
      void redefineGoal(GLfloat x, GLfloat z);

      /*! remove all agents from all agents lists */
      void removeAllAgents();
};

#endif

