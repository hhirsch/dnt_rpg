#include "feats.h"

int numberClass(string buffer)
{
   int ret = -1;
   if(buffer.compare(STR_EADM) == 0)
     ret = CLASS_EADM;
   else if(buffer.compare(STR_EBIOLOGIA) == 0)
     ret = CLASS_EBIOLOGIA;
   else if(buffer.compare(STR_EDCC) == 0)
     ret = CLASS_EDCC;
   else if(buffer.compare(STR_EDFISICA) == 0)
     ret = CLASS_EDFISICA;
   else if(buffer.compare(STR_EMECANICA) == 0)
     ret = CLASS_EMECANICA;
   else if(buffer.compare(STR_EFILOSOFIA) == 0)
     ret = CLASS_EFILOSOFIA;
   else if(buffer.compare(STR_EFISIOTERAPIA) == 0)
     ret = CLASS_EFISIOTERAPIA;
   else if(buffer.compare(STR_EMEDICINA) == 0)
     ret = CLASS_EMEDICINA;
   else if(buffer.compare(STR_EMUSICA) == 0)
     ret = CLASS_EMUSICA;
   else if(buffer.compare(STR_ETO) == 0)
     ret = CLASS_ETO;

   return(ret);
}

int numberSkill(string buffer)
{
   int ret = -1;
   if(buffer.compare(SKILL_STR_ACROBACIAS) == 0)
      ret = SKILL_ACROBACIAS;
   else if(buffer.compare(SKILL_STR_ADESTRAR_BICHOS) == 0)
      ret = SKILL_ADESTRAR_BICHOS;
   else if(buffer.compare(SKILL_STR_ARROMBAR) == 0)
      ret = SKILL_ARROMBAR;
   else if(buffer.compare(SKILL_STR_ARTE_DO_ESCAPE) == 0)
      ret = SKILL_ARTE_DO_ESCAPE;
   else if(buffer.compare(SKILL_STR_BLEFAR) == 0)
      ret = SKILL_BLEFAR;
   else if(buffer.compare(SKILL_STR_BOVINICE) == 0)
      ret = SKILL_BOVINICE;
   else if(buffer.compare(SKILL_STR_CNH_SOFRIMENTO) == 0)
      ret = SKILL_CNH_SOFRIMENTO;
   else if(buffer.compare(SKILL_STR_CURAR) == 0)
      ret = SKILL_CURAR;
   else if(buffer.compare(SKILL_STR_DIPLOMACIA) == 0)
      ret = SKILL_DIPLOMACIA;
   else if(buffer.compare(SKILL_STR_DISCURSO_PROF) == 0)
      ret = SKILL_DISCURSO_PROF;
   else if(buffer.compare(SKILL_STR_DISFARCES) == 0)
      ret = SKILL_DISFARCES;
   else if(buffer.compare(SKILL_STR_EQUILIBRIO) == 0)
      ret = SKILL_EQUILIBRIO;
   else if(buffer.compare(SKILL_STR_ESCALAR) == 0)
      ret = SKILL_ESCALAR;
   else if(buffer.compare(SKILL_STR_ESCONDER) == 0)
      ret = SKILL_ESCONDER;
   else if(buffer.compare(SKILL_STR_FALAR_IDIOMA) == 0)
      ret = SKILL_FALAR_IDIOMA;
   else if(buffer.compare(SKILL_STR_FALSIFICAR) == 0)
      ret = SKILL_FALSIFICAR;
   else if(buffer.compare(SKILL_STR_FURTIVIDADE) == 0)
      ret = SKILL_FURTIVIDADE;
   else if(buffer.compare(SKILL_STR_INTIMIDAR) == 0)
      ret = SKILL_INTIMIDAR;
   else if(buffer.compare(SKILL_STR_MONTAR) == 0)
      ret = SKILL_MONTAR;
   else if(buffer.compare(SKILL_STR_NADAR) == 0)
      ret = SKILL_NADAR;
   else if(buffer.compare(SKILL_STR_OBSERVAR) == 0)
      ret = SKILL_OBSERVAR;
   else if(buffer.compare(SKILL_STR_OP_OBJ_ELETRICO) == 0)
      ret = SKILL_OP_OBJ_ELETRICO;
   else if(buffer.compare(SKILL_STR_OP_OBJ_MECANICO) == 0)
      ret = SKILL_OP_OBJ_MECANICO;
   else if(buffer.compare(SKILL_STR_OBTER_INFO) == 0)
      ret = SKILL_OBTER_INFO;
   else if(buffer.compare(SKILL_STR_OUVIR) == 0)
      ret = SKILL_OUVIR;
   else if(buffer.compare(SKILL_STR_PASSE_LIVRE) == 0)
      ret = SKILL_PASSE_LIVRE;
   else if(buffer.compare(SKILL_STR_PE_NO_SACO) == 0)
      ret = SKILL_PE_NO_SACO;
   else if(buffer.compare(SKILL_STR_PERFORMAR) == 0)
      ret = SKILL_PERFORMAR;
   else if(buffer.compare(SKILL_STR_PRESTIDIG) == 0)
      ret = SKILL_PRESTIDIG;
   else if(buffer.compare(SKILL_STR_ROUPAS_BIZARRAS) == 0)
      ret = SKILL_ROUPAS_BIZARRAS;
   else if(buffer.compare(SKILL_STR_SEM_TEMPO) == 0)
      ret = SKILL_SEM_TEMPO;
   else if(buffer.compare(SKILL_STR_CNH_TAG_MEDIC) == 0)
      ret = SKILL_CNH_TAG_MEDIC;
   else if(buffer.compare(SKILL_STR_TOCAR_INST) == 0)
      ret = SKILL_TOCAR_INST;

   return(ret);
}
