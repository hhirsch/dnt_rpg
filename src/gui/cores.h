#ifndef _cores_h
#define _cores_h

typedef struct{
   int R, G, B;
} Cor;


class cores{
   public:
      Cor corCont[3], 
          corJan, 
          corBot, 
          corMenu, 
          corTexto, 
          corBarra, 
          corTextoSel;
       void Iniciar();
};



#endif
