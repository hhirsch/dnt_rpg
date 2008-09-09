#ifndef _dnt_btreecell_h
#define _dnt_btreecell_h

/*! A binary tree cell definition  */
class bTreeCell
{
   public:
      /*! Constructor */
      bTreeCell();
      /*! Desctuctor */
      virtual ~bTreeCell();

      /*! Gets the left son 
       * \return the cell's left son */
      bTreeCell* getLeft();
      /*! Sets the left son 
       * \param cell -> reference to the son to be */
      void setLeft(bTreeCell* cell);
      /*! Gets the right son 
       * \return the cell's right son */
      bTreeCell* getRight();
      /*! Sets the right son 
       * \param right -> reference to the son to be */
      void setRight(bTreeCell* right);

      /*! Compare with another cell
       * \param cell -> pointer to cell to compare this one to
       * \return == 0 if equal, < 0  if lesser or > 0 if greater */
      virtual int compare(bTreeCell* cell) = 0;

      /*! Merge the current bTreeCell with another
       * \param cell -> cell to merge the current one with */
      virtual void merge(bTreeCell* cell) = 0;

   private:
      bTreeCell* left;   /**< The cell's left son */
      bTreeCell* right;  /**< The cell's right son */ 
};

#endif

