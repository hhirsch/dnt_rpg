#ifndef _pf_h
#define _pf_h

#include "../../ia/potentAgent.h"
#include "../../ia/pattAgent.h"
#include "../../ia/fuzzy.h"
#include "states.h"
#include "politic.h"

/*! The Federal Police Class */
class pf
{
   public:
      /*! Constructor */
      pf();
      /*! Destructor */
      ~pf();

      /*! Get Actual Agent Position 
       * \param x -> will have X agent coordinate
       * \param z -> will have Z agent coordinate */
      void getPosition(GLfloat& x, GLfloat& z);

      /*! Define Step Size 
       * \param size -> step size */
      void defineStepSize(GLfloat size);

      /*! Define Destiny 
       * \param x -> x destiny coordinate
       * \param z -> z destiny coordinate */
      void defineDestiny(GLfloat x, GLfloat z);

      /*! Define Actual Agent Position 
       * \param x -> x position coordinate
       * \param z -> z position coordinate */
      void definePosition(GLfloat x, GLfloat z);

      /*! Define Character Sight 
       * \param sightDist -> sight Distance 
       * \param sightAng -> sight Angle */
      void defineSight(GLfloat sightDist, GLfloat sightAng);

      /*! Gets Character Sight 
       * \param sightDist -> sight Distance 
       * \param sightAng -> sight Angle */
      void getSight(GLfloat& sightDist, GLfloat& sightAng);

      /*! Actualize the Position and State machine of the agent*/
      void actualizeMachineAndPosition();

      /*! Set the target to the politic 
       * \param tg -> politic to be the target of investigation 
       * \return True if Fuzzy Logic accepted to take the target */
      bool setTarget(politic* tg);

      /*! Gets the target of the police
       * \return current target: politic. */
      politic* getTarget();

      /*! Set Federal Police Base center point. */
      void setFederal(GLfloat x, GLfloat z);

      /*! Get State of the agent
       * \return state */
      int getState();

      float getTimePercent();
      float getCorruptPercent();


      pattAgent* patAg;       /**< Pattern Agent */
      potentAgent* potAg;     /**< Potential Agent */
      pf* next;               /**< Next Agent on List */

   private:
      stateMachine* state;    /**< State Machine */

      politic* target;        /**< Target Politic */

      GLfloat federalX,       /**< Federal Base X center coordinate */
              federalZ;       /**< Federal Base Z center coordinate */

      int corrupt;            /**< How Corrupt is the Agent */
      int lastPrison;         /**< Last time the agent capture some rogue */

      fuzzy fuzzyLogic;           /**< Fuzzy Logic Controler */
      fuzzyFunction* muchMoney;   /**< Much Money Function */
      fuzzyFunction* normalMoney; /**< Normal Money Function */
      fuzzyFunction* fewMoney;    /**< Few Money Function */
      fuzzyFunction* muchTime;    /**< Much Time Function */
      fuzzyFunction* normalTime;  /**< Normal Time Function */
      fuzzyFunction* fewTime;     /**< Few Time Function */

      /*! Init Fuzzy Logic Functions */
      void fuzzyInit();
      

};


#endif

