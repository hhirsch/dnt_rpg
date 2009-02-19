#ifndef dnt_journal_window_h
#define dnt_journal_window_h

#include "../gui/farso.h"
#include "../etc/btree.h"

/*! The journalDesc keeps information about a single mission */
class journalDesc
{
   public:
      string text;    /**< Description Text */
      bool completed; /**< If completed or not */

      journalDesc* next;     /**< Next on list */
      journalDesc* previous; /**< Previous on list */
};

/*! The journalArea keeps information about
 * all missions on an specific area */
class journalArea : public bTreeCell
{
   public:
      string text;  /**< Area text */

      journalDesc* descriptions; /**< Descriptions list */
      int totalDescriptions;     /**< Total Descriptions */

      /*! Constructor */
      journalArea();

      /*! Destructor */
      ~journalArea();
 
      /*! Compare with another cell
       * \param cell -> pointer to cell to compare this one to
       * \return == 0 if equal, < 0  if lesser or > 0 if greater */
      int compare(bTreeCell* cell);

      /*! Merge the current bTreeCell with another
       * \param cell -> cell to merge the current one with */
      void merge(bTreeCell* cell);

      /*! Insert a mission at the area
       * \param desc -> mission description
       * \param comp -> true if completed */
      void insertMission(string desc, bool comp);
};

/*! The journalAreas keeps all journalArea informations */
class journalAreas : public bTree
{
   public:
      /*! Constructor */
      journalAreas();
      /*! Destructor */
      ~journalAreas();

      /*! Insert a journalArea at the bTree
       * \param title -> journalArea text 
       * \return -> pointer to the inserted journal area */
      journalArea* insert(string title);

      /*! Insert a journalArea at the bTree
       * \param title -> journalArea text 
       * \return -> pointer to the inserted journal area */
      journalArea* search(string title);

      /*! Get the journalArea at the num position
       * \param num -> position to get
       * \return -> pointer to the area got */
      journalArea* get(int num);

      /*! Get the journalArea at the num position
       * \param num -> position to get
       * \param curRoot -> current node used as root
       * \param cur -> current node position 
       * \return -> pointer to the area got */
      journalArea* get(int num, journalArea* curRoot, int& cur);

      /*! Dupplicate a cell (aka: alloc a new one with same values)
       * \param cell -> pointer to the cell to dupplicate */
      bTreeCell* dupplicateCell(bTreeCell* cell);

      /*! Delete the actual cell 
       * \param cell -> the cell to delete */
      void freeCell(bTreeCell* cell);
};

/*! The Journal Window definition.
 * This window shows current, finished and aborted missions
 * sorted by game area.
 * \todo -> TODO: the AREA idea implementation. Currently it's only a 
 *                separator at missions definitions. */
class journalWindow
{
   public:
      /*! Constructor */
      journalWindow();
      /*! Destructor */
      ~journalWindow();

      /*! Open, if not already opened, the journal window
       * \param inter -> pointer to the used guiInterface */
      void open(guiInterface* inter);

      /*! Verify if the window is opened
       * \return true if the journalWindow is opened */
      bool isOpen();
      /*! Close, if opened, the journal window */
      void close();

   protected:
      /*! Create the Journal missions lists per area */
      void createLists();

      /*! Show current Area missions at the window */
      void showArea();

      journalAreas* areas;       /**< The missions per areas */
      int curArea;               /**< Current area displayed */

      guiInterface* gui;         /**< The gui used */
      window* internalWindow;    /**< The window pointer */
      button* nextButton;        /**< The Next Area Button */
      button* previousButton;    /**< The Previous Area Button */
      textBox* areaText;         /**< The Area text */
      rolBar* missionsText;      /**< The missons text */
};

#endif

