#include "CellViewer.h"
#include "Error.h"

namespace Netlist {

  //-----------------------CTOR------------------------------//
  CellViewer :: CellViewer ( QWidget * parent ) 
  : QMainWindow ( parent )
  , cellWidget_ ( NULL )
  , saveCellDialog_ ( NULL )
  , cellsLib_ (NULL)
  , instancesWidget_ (NULL)
  {

    //Creation des differentes fenetres secondaires
    cellWidget_ = new CellWidget ();
    instancesWidget_ = new InstancesWidget ();
    cellsLib_ = new CellsLib();
    saveCellDialog_ = new SaveCellDialog ( this );

    //Ajout du cellViewer aux fenetres secondaires
    instancesWidget_->setCellViewer(this);
    cellsLib_ -> setCellViewer(this);
    

    setCentralWidget ( cellWidget_ );

    //Menu file
    QMenu * fileMenu = menuBar () -> addMenu ( "&File" );


    //Option Open cell du Menu file
    QAction * action = new QAction ( "&Open Cell" , this );
    action -> setStatusTip ( "Load a Cell from disk" );
    action -> setShortcut ( QKeySequence ( "CTRL+O" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );

    //Connection entre l'event de click sur l'option et le slot openCell
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( openCell()) );


    //Option Save As du Menu file
    action = new QAction ( "&Save As" , this );
    action -> setStatusTip ( "Save to disk ( rename ) the Cell" );
    action -> setShortcut ( QKeySequence ( "CTRL+S" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );

    //Connection entre l'envent de click sur l'option et le slot saveCell
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( saveCell ()) );
    

    //Option qui affiche une fenetre des diffrentes instances dans la Cell courante
    action = new QAction ( "&Instances" , this );
    action -> setStatusTip ( "Show instances list" );
    action -> setShortcut ( QKeySequence ( "CTRL+I" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );

    //Connection entre l'event de click sur l'option et le slot showInstancesWidget
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( showInstancesWidget()) );
    

    //Option qui affiche une fenetre des differentes Cell chargees
    action = new QAction ( "&Cell" , this );
    action -> setStatusTip ( "Load a Cell from disk" );
    action -> setShortcut ( QKeySequence ( "CTRL+C" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );

    //Connection entre l'event click sur l'option et le slot showCellsLib
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( showCellsLib  ()) );
    

    //Option pour quitter l'application
    action = new QAction ( "&Quit" , this );
    action -> setStatusTip ( "Exit the Netlist Viewer" );
    action -> setShortcut ( QKeySequence ( "CTRL+Q" ) );
    action -> setVisible ( true );
    fileMenu -> addAction ( action );

    //Connection entre l'event click sur l'option et le slot close
    connect ( action , SIGNAL ( triggered ()) , this , SLOT ( close ()) );
    

    //Connection entre l'event cellLoaded et le slot updateData de CellModel
    connect (this,SIGNAL(cellLoaded()),cellsLib_->getBaseModel() ,SLOT(updateDatas()));

  }


  //-----------------------DTOR------------------------------//
  CellViewer::~CellViewer(){}

  //-----------------------GETTEUR------------------------------//
  Cell *  CellViewer::getCell() const { return cellWidget_->getCell();}
  
  //-----------------------SLOT------------------------------//
  void CellViewer::showInstancesWidget (){
   instancesWidget_->show();
  }


  //Sauvegarde d'une Cell
  void CellViewer :: saveCell (){

    //Recuperation de la cell courante
    Cell * cell = getCell ();

    //Si elle est NULL
    if ( cell == NULL ) return ;

    //Recuperation du nom de la cell
    QString cellName = cell -> getName (). c_str ();

    //Si on click sur le bouton save dans la fenetre Save As
    if ( saveCellDialog_ -> run ( cellName )) {

      //Sauvegarde
      cell -> setName ( cellName . toStdString () );
      cell -> save ( cellName . toStdString () );
    }
  }

  //Mise a jour de la cell courante
  void    CellViewer::setCell ( Cell* c){

    //On doit aussi modifer la cell de CellWidget et InstanceWidget
    cellWidget_->setCell(c);
    instancesWidget_->setCell(c);
  }


  //Ouverture d'une nouvelle Cell
  void CellViewer::openCell (){

    QString cellName;

    //Recuperation du nom de la cell depuis la fenetre Open Cell
    if ( OpenCellDialog::run ( cellName)) {
      
      //On cherche si la Cell est deja en memoire
      Cell *c = Cell::find(cellName.toStdString());

      //Si c'est le cas on ne la load pas
      if (c) {
        setCell(c);
      }

      //Sinon
      else {

        //On load
        Cell *c1;
        c1 = Cell::load(cellName.toStdString());
        if (c1) setCell(c1);

        //On envoie le signal pour que la table de la liste de Cell soit mise a jour dans CellsModel
        emit cellLoaded();
      }
    }
  }
  
  void     CellViewer:: showCellsLib        (){
  	cellsLib_->show();
  }

}
