#ifndef _defs_h
#define _defs_h

/** \file defs.h
 *  Here is where all game rules definitions resides. Again, some of them are define two times, one with the internal number, other with the definition string.
 */

#include "dices.h"
#include <string>
using namespace std;

/******************************************************************
 *                        ATTRIBUTES CONSTANTS                    *
 ******************************************************************/
#define ATT_LEVEL        0
#define ATT_STRENGHT     1
#define ATT_DEXTERY      2
#define ATT_CONSTITUTION 3
#define ATT_INTELIGENCY  4
#define ATT_WISDOW       5
#define ATT_CHARISM      6
#define ATT_SKILL_FIRST  7
#define ATT_SKILL_LAST   39

#define ATT_STR_LEVEL        "ATT_LEVEL"
#define ATT_STR_STRENGHT     "ATT_STRENGHT"
#define ATT_STR_DEXTERY      "ATT_DEXTERY"
#define ATT_STR_CONSTITUTION "ATT_CONSTITUTION"
#define ATT_STR_INTELIGENCY  "ATT_INTELIGENCY"
#define ATT_STR_WISDOW       "ATT_WISDOW"
#define ATT_STR_CHARISM      "ATT_CHARISM"


#define VAR_AUX1         40
#define VAR_AUX2         41
#define VAR_AUX3         42
#define VAR_AUX4         43
#define VAR_AUX5         44
#define VAR_AUX6         45

/******************************************************************
 *                         SKILL CONSTANTS                        *
 ******************************************************************/
#define SKILL_ACROBACIAS       7
#define SKILL_ADESTRAR_BICHOS  8
#define SKILL_ARROMBAR         9
#define SKILL_ARTE_DO_ESCAPE  10
#define SKILL_BLEFAR          11
#define SKILL_BOVINICE        12
#define SKILL_CNH_SOFRIMENTO  13
#define SKILL_CURAR           14
#define SKILL_DIPLOMACIA      15
#define SKILL_DISCURSO_PROF   16
#define SKILL_DISFARCES       17
#define SKILL_EQUILIBRIO      18
#define SKILL_ESCALAR         19
#define SKILL_ESCONDER        20
#define SKILL_FALAR_IDIOMA    21
#define SKILL_FALSIFICAR      22
#define SKILL_FURTIVIDADE     23
#define SKILL_INTIMIDAR       24
#define SKILL_MONTAR          25
#define SKILL_NADAR           26
#define SKILL_OBSERVAR        27
#define SKILL_OP_OBJ_ELETRICO 28
#define SKILL_OP_OBJ_MECANICO 29
#define SKILL_OBTER_INFO      30
#define SKILL_OUVIR           31
#define SKILL_PASSE_LIVRE     32
#define SKILL_PE_NO_SACO      33
#define SKILL_PERFORMAR       34
#define SKILL_PRESTIDIG       35
#define SKILL_ROUPAS_BIZARRAS 36
#define SKILL_SEM_TEMPO       37
#define SKILL_CNH_TAG_MEDIC   38
#define SKILL_TOCAR_INST      39

#define SKILL_STR_ACROBACIAS      "SKILL_ACROBACIAS" 
#define SKILL_STR_ADESTRAR_BICHOS "SKILL_ADESTRAR_BICHOS"
#define SKILL_STR_ARROMBAR        "SKILL_ARROMBAR"
#define SKILL_STR_ARTE_DO_ESCAPE  "SKILL_ARTE_DO_ESCAPE"
#define SKILL_STR_BLEFAR          "SKILL_BLEFAR"
#define SKILL_STR_BOVINICE        "SKILL_BOVINICE"
#define SKILL_STR_CNH_SOFRIMENTO  "SKILL_CNH_SOFRIMENTO"
#define SKILL_STR_CURAR           "SKILL_CURAR"
#define SKILL_STR_DIPLOMACIA      "SKILL_DIPLOMACIA"
#define SKILL_STR_DISCURSO_PROF   "SKILL_DISCURSO_PROF"
#define SKILL_STR_DISFARCES       "SKILL_DISFARCES"
#define SKILL_STR_EQUILIBRIO      "SKILL_EQUILIBRIO"
#define SKILL_STR_ESCALAR         "SKILL_ESCALAR"
#define SKILL_STR_ESCONDER        "SKILL_ESCONDER"
#define SKILL_STR_FALAR_IDIOMA    "SKILL_FALAR_IDIOMA"
#define SKILL_STR_FALSIFICAR      "SKILL_FALSIFICAR"
#define SKILL_STR_FURTIVIDADE     "SKILL_FURTIVIDADE"
#define SKILL_STR_INTIMIDAR       "SKILL_INTIMIDAR"
#define SKILL_STR_MONTAR          "SKILL_MONTAR"
#define SKILL_STR_NADAR           "SKILL_NADAR"
#define SKILL_STR_OBSERVAR        "SKILL_OBSERVAR"
#define SKILL_STR_OP_OBJ_ELETRICO "SKILL_OP_OBJ_ELETRICO"
#define SKILL_STR_OP_OBJ_MECANICO "SKILL_OP_OBJ_MECANICO"
#define SKILL_STR_OBTER_INFO      "SKILL_OBTER_INFO"
#define SKILL_STR_OUVIR           "SKILL_OUVIR"
#define SKILL_STR_PASSE_LIVRE     "SKILL_PASSE_LIVRE"
#define SKILL_STR_PE_NO_SACO      "SKILL_PE_NO_SACO"
#define SKILL_STR_PERFORMAR       "SKILL_PERFORMAR"
#define SKILL_STR_PRESTIDIG       "SKILL_PRESTIDIG"
#define SKILL_STR_ROUPAS_BIZARRAS "SKILL_ROUPAS_BIZARRAS"
#define SKILL_STR_SEM_TEMPO       "SKILL_SEM_TEMPO"
#define SKILL_STR_CNH_TAG_MEDIC   "SKILL_CNH_TAG_MEDIC"
#define SKILL_STR_TOCAR_INST      "SKILL_TOCAR_INST"

/******************************************************************
 *                        CLASSES CONSTANTS                       *
 ******************************************************************/
#define CLASS_EADM          300
#define CLASS_EBIOLOGIA     301
#define CLASS_EDCC          302
#define CLASS_EDFISICA      303
#define CLASS_EMECANICA     304
#define CLASS_EFILOSOFIA    305
#define CLASS_EFISIOTERAPIA 306
#define CLASS_EMEDICINA     307
#define CLASS_EMUSICA       308
#define CLASS_ETO           309
#define STR_EADM          "EADM"
#define STR_EBIOLOGIA     "EBIOLOGIA"
#define STR_EDCC          "EDCC"
#define STR_EDFISICA      "EDFISICA"
#define STR_EMECANICA     "EMECANICA"
#define STR_EFILOSOFIA    "EFILOSOFIA"
#define STR_EFISIOTERAPIA "EFISIOTERAPIA"
#define STR_EMEDICINA     "EMEDICINA"
#define STR_EMUSICA       "EMUSICA"
#define STR_ETO           "ETO"

/******************************************************************
 *                         PC RACES CONSTANTS                     *
 ******************************************************************/
#define RACE_AUTISTA_ESTRANHO 100
#define RACE_BOY_PATY         101
#define RACE_GOTICO           102
#define RACE_HIPPIE           103
#define RACE_HUMANO           104
#define RACE_LLAMA_HUMANA     105
#define RACE_METALEIRO        106
#define RACE_MANIACO_SKINHEAD 107
#define RACE_MUTANTE          108

#define RACE_STR_AUTISTA_ESTRANHO "RACE_AUTISTA_ESTRANHO"
#define RACE_STR_BOY_PATY         "RACE_BOY_PATY"
#define RACE_STR_GOTICO           "RACE_GOTICO"
#define RACE_STR_HIPPIE           "RACE_HIPPIE"
#define RACE_STR_HUMANO           "RACE_HUMANO"
#define RACE_STR_LLAMA_HUMANA     "RACE_LLAMA_HUMANA"
#define RACE_STR_METALEIRO        "RACE_METALEIRO"
#define RACE_STR_MANIACO_SKINHEAD "RACE_MANIACO_SKINHEAD"
#define RACE_STR_MUTANTE          "RACE_MUTANTE"


/******************************************************************
 *                        NPC RACES CONSTANTS                     *
 ******************************************************************/
#define RACE_RATAZANA         120
#define RACE_ENTE             121
#define RACE_AMEIVA           122

#define RACE_STR_RATAZANA  "RACE_RATAZANA"
#define RACE_STR_ENTE      "RACE_ENTE"
#define RACE_STR_AMEIVA    "RACE_AMEIVA"


/******************************************************************
 *                         TENDENCY CONSTANTS                     *
 ******************************************************************/
#define TEND_SL_LIBERTARIO     200
#define TEND_SL_CENTRISTA      201
#define TEND_SL_CAPITALISTA    202
#define TEND_NEUTRO_FUNCIONAL  203
#define TEND_NEUTRO_CENTRISTA  204
#define TEND_NEUTRO_CAOTICO    205
#define TEND_SP_CAPITALISTA    206
#define TEND_SP_NEUTRO         207
#define TEND_SP_CAOTICO        208

#define TEND_STR_SL_LIBERTARIO     "TEND_SL_LIBERTARIO"
#define TEND_STR_SL_CENTRISTA      "TEND_SL_CENTRISTA"
#define TEND_STR_SL_CAPITALISTA    "TEND_SL_CAPITALISTA"
#define TEND_STR_NEUTRO_FUNCIONAL  "TEND_NEUTRO_FUNCIONAL"
#define TEND_STR_NEUTRO_CENTRISTA  "TEND_NEUTRO_CENTRISTA"
#define TEND_STR_NEUTRO_CAOTICO    "TEND_NEUTRO_CAOTICO"
#define TEND_STR_SP_CAPITALISTA    "TEND_SP_CAPITALISTA"
#define TEND_STR_SP_NEUTRO         "TEND_SP_NEUTRO"
#define TEND_STR_SP_CAOTICO        "TEND_SP_CAOTICO"


#define WORK_ON_ALL            10000
#define STR_WORK_ON_ALL        "WORK_ON_ALL"
#define NONE                   10001
#define STR_NONE               "NONE" 

/******************************************************************
 *                            SEX CONSTANTS                       *
 ******************************************************************/
#define SEX_MALE        0
#define SEX_FEMALE      1
#define SEX_OTHER       2


/*!
 **************************************************************** 
 * Convert the concept with name buffer to its internal number  
 * \param buffer -> string with the name of the concept
 * \return the internal number of the constant.
 ***************************************************************/
int numberConstant(string buffer);

/*!
 **************************************************************** 
 * Verifies if the number represents a skill  
 * \param number -> a number that represents some concept
 * \return \c true if is a Skill, \c false otherwise.
 ***************************************************************/
bool isSkill(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents a class 
 * \param number -> a number that represents some concept
 * \return \c true if is a Class, \c false otherwise.
 ***************************************************************/
bool isClass(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents a tendency.  
 * \param number -> a number that represents some concept
 * \return \c true if is a Tendency, \c false otherwise.
 ***************************************************************/
bool isTendency(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents a race  
 * \param number -> a number that represents some concept
 * \return \c true if is a Race, \c false otherwise.
 ***************************************************************/
bool isRace(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents an attribute  
 * \param number -> a number that represents some concept
 * \return \c true if is an Attribute, \c false otherwise.
 ***************************************************************/
bool isAttribute(int number);

/*!
 ************************************************************** 
 * Verifies if the number represents a variable  
 * \param number -> a number that represents some concept
 * \return \c true if is a Variable, \c false otherwise.
 ***************************************************************/
bool isVariable(int number);


#endif
