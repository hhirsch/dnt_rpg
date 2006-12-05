#ifndef _fuzzyvariable_h
#define _fuzzyvariable_h

/*! Fuzzy Variable (usually destiny one of a rule) */
class fuzzyVariable
{
   public:
      /*! Constructor */
      fuzzyVariable();
      /*! Destructor */
      ~fuzzyVariable();

      /*! Clear the Value of Variable */
      void clearValue();
      
      /*! Set Value of the Variable */
      void setValue(float val);
      
      /*! Get value of the variable */
      float getValue();

      fuzzyVariable* next;
      fuzzyVariable* previous;
      
   private:
      float value; /**< value of the variable */
};

#endif

