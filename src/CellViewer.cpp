#include "CellViewer.h"


namespace Netlist {
  CellViewer :: CellViewer ( QWidget * parent ) 
  : QMainWindow ( parent )
  , cellWidget_ ( NULL )
  , saveCellDialog_ ( NULL )
  , cellsLib_ (NULL)
  , instancesWidget_ (NULL)
  {
    cellWidget_ = new CellWidget ();
    instancesWidget_ = new InstancesWidget ();
    cellsLib_ = new CellsLib();
    saveCellDialog_ = new SaveCellDialog ( this );

    instancesWidget_->setCellViewer(this);
    cellsLib_ -> setCellViewer(this);
    
    setCentralWidget ( cellWidget_ );
    QMenu * fileMenu = menuBar () -> addMenu ( "&File" );

    QAction * action = new QAction ( "&Open Cell" , this );
    action -> setStatusTip ( "Load a Cell from disk" );
    action -> setShortcut ( QKeySequence ( "CTRL+O" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( openCell()) );

    action = new QAction ( "&Save As" , this );
    action -> setStatusTip ( "Save to disk ( rename ) the Cell" );
    action -> setShortcut ( QKeySequence ( "CTRL+S" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( saveCell ()) );

    action = new QAction ( "&Instances" , this );
    //action -> setStatusTip ( "Save to disk ( rename ) the Cell" );
    action -> setShortcut ( QKeySequence ( "CTRL+I" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( showInstancesWidget()) );
    
    action = new QAction ( "&Cell" , this );
    action -> setStatusTip ( "Load a Cell from disk" );
    action -> setShortcut ( QKeySequence ( "CTRL+O" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( showCellsLib  ()) );

    action = new QAction ( "&Quit" , this );
    action -> setStatusTip ( "Exit the Netlist Viewer" );
    action -> setShortcut ( QKeySequence ( "CTRL+Q" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( close ()) );
    
    connect (this,SIGNAL(cellLoaded()),cellsLib_->getBaseModel() ,SLOT(updateDatas()));

  }

  void CellViewer::showInstancesWidget (){
   instancesWidget_->show();
  }


  void CellViewer :: saveCell (){
    Cell * cell = getCell ();
    if ( cell == NULL ) return ;

    QString cellName = cell -> getName (). c_str ();

    if ( saveCellDialog_ -> run ( cellName )) {
      cell -> setName ( cellName . toStdString () );
      cell -> save ( cellName . toStdString () );
    }
  }

  CellViewer::~CellViewer(){}

  Cell *  CellViewer::getCell() const { return cellWidget_->getCell();}

  void    CellViewer::setCell ( Cell* c){
    cellWidget_->setCell(c);
    instancesWidget_->setCell(c);
    
  }



  void CellViewer::openCell (){
    QString cellName;
    if ( OpenCellDialog::run ( cellName)) {
      
      Cell *c = Cell::find(cellName.toStdString());
      if (c) {
        setCell(c);
      }
      else {
        setCell(Cell::load(cellName.toStdString()));
        emit cellLoaded();
      }
    }
  }
  
  void     CellViewer:: showCellsLib        (){
  	cellsLib_->show();
  }

}
