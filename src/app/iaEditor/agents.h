#ifndef _agents_h
#define _agents_h

#include "../../ia/potentAgent.h"
#include "../../ia/pattAgent.h"
#include "../../map/map.h"
#include "pf.h"
#include "briefcase.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;


#define AGENT_TYPE_POTENT  1
#define AGENT_TYPE_PATTERN 2

/* TP3 Agents */
#define AGENT_TYPE_ROGUE   3
#define AGENT_TYPE_POLICE  4

#define AGENT_POTENT_HIGHT  6.0

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
      void actualize(Map* actualMap);
      /*! Draw All Agents */
      void draw();
      /*! Add Agent to the Editor */
      void addAgent(int type, GLfloat x, GLfloat z, bool oriented, 
                    GLfloat stepSize, GLfloat goalX, GLfloat goalZ, 
                    GLfloat sightDist, GLfloat sightAng);
      /*! Verify Agents Editor Actions */
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int tool, Map* actualMap, 
                        mapObjeto** actualObject);

      /*! Saves the current State to file
       * \param fileName -> name of the file to be saved */
      string saveState(string fileName);
      /*! Loads state from file
       * \param fileName -> name of the file to be loaded */
      string loadState(string fileName);

      int scores[3];

   private:
      potentAgent* potentAgents;    /**< List of Potential Functions Agents */
      int totalPotentAgents;        /**< Total Potential Functions Agents */
      GLMmodel* modelPot;           /**< Model Used by Potential Agents */

      pattAgent* pattAgents;        /**< List of Pattern Agents */
      int totalPattAgents;          /**< Total Pattern Agents on List */
      GLMmodel* modelPatt;          /**< Model Used by Pattern Agents */

      agent* actualAgent;           /**< Currenct on Edition Agent */
      mapObjeto* actualObstacle;    /**< Current on Edition Obstacle */
      int obstacleOrientation;      /**< Current Obstacle Orientation */
      GLfloat obstacleX;            /**< Current Obstacle X position */
      GLfloat obstacleZ;            /**< Current Obstacle Z position */
      
      int state;                    /**< Current Editor State */

      GLfloat goalX;                /**< Defined X Coordinate of Goal */
      GLfloat goalZ;                /**< Defined Z Coordinate of Goal */
      GLuint goalTexture;           /**< Goal Texture */

      /*! Draw one Pontential Agent on Origin */
      void drawPotentAgent();       
      /*! Draw One Pattern Agent on Origin */
      void drawPattAgent();
      /*! Add Visible Agents to agent obstacle
       * \param ag -> active agent
       * \param actualMap -> opened Map */
      void addVisibleAgents(agent* ag, Map* actualMap);
      void addSquareObstacles(agent* ag, Square* saux);
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

      /*! Insert Object on Map (why here?? cause of the TP2)  */
      void insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                        Map* map, mapObjeto* obj, int qx, int qz);

      /*! Load a texture */
      void loadTexture(GLuint* texture, string name, bool alpha);

      /*! Draws some texture on "Floor" */
      void drawTexture(GLuint texture, GLfloat posX, GLfloat posZ, 
                         GLfloat varX, GLfloat varZ);

      /*! Remove a politic agent from list */
      politic* removePoliticAgent(politic* polAg);

      /*! Remove a federal police agent from list */
      pf* removePfAgent(pattAgent* polAg);

      void redefineFederal(GLfloat x, GLfloat z);
      void redefineBase(int groupNum, GLfloat x, GLfloat z);
      void redefineCongress(GLfloat x, GLfloat z);


      /*! Draw a politic agent */
      void drawPolitic();

      /*! Draw a Federal Police Agent */
      void drawPf();

      politic* getPoliticWithCaseInArea(GLfloat x1, GLfloat z1,
                                        GLfloat x2, GLfloat z2);



      /* TP3 Things */
      
      politic* politics;      /**< politics agents List */
      int totalPolitics;      /**< total politics on list */
      GLMmodel* politicModel; /**< politic 3D model */
      
      pf* pfs;                /**< federal Police Agents */
      int totalPfs;           /**< total federal agents */
      GLMmodel* pfModel;      /**< federal police 3D model */
      
      briefCases* brief;      /**< briefCases List */
      
      int lastCongressTime;   /**< Last Time Congress called all politics */

      GLuint tp3Textures[5];  /**< Groups Base Textures */
      GLfloat tp3X[5];        /**< Groups Base Coordinates */
      GLfloat tp3Z[5];        /**< Groups Base Coordinates */

      int groupAdd;           /**< Last group of rogue added  to */

      
};

#endif

