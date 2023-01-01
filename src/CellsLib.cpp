#include "CellsLib.h"
#include "CellViewer.h"
#include <QHeaderView>

namespace Netlist{

  //-----------------------CTOR------------------------------//
  CellsLib::CellsLib(QWidget* parent): QWidget( parent )
    , cellViewer_ ( NULL )
    , baseModel_ ( new CellsModel ( this ))
    , view_ ( new QTableView ( this ))
    , load_ ( new QPushButton ( this ))
    {

      //Option de la fenetre
      setAttribute ( Qt::WA_QuitOnClose , false );
      setAttribute ( Qt::WA_DeleteOnClose , false );
      setContextMenuPolicy( Qt::ActionsContextMenu);


      //Option de la table 
      view_  -> setShowGrid ( false );
      view_ -> setAlternatingRowColors ( true );
      view_ -> setSelectionBehavior ( QAbstractItemView :: SelectRows );
      view_ -> setSelectionMode( QAbstractItemView :: SingleSelection );
      view_ -> setSortingEnabled ( true );
      view_ -> setModel ( baseModel_ ); // On associe le modele .

      
      //Entete de la table
      QHeaderView * horizontalHeader = view_ -> horizontalHeader ();
      horizontalHeader -> setDefaultAlignment ( Qt :: AlignHCenter );
      horizontalHeader -> setMinimumSectionSize ( 300 );
      horizontalHeader -> setStretchLastSection ( true );
      QHeaderView * verticalHeader = view_ -> verticalHeader ();
      verticalHeader -> setVisible ( false );

      //Option du button
      load_ -> setText ( " Load " );
      load_ -> setDefault ( true );


      //Horizontal layout qui contient le button load au centre
      QHBoxLayout * hLayout = new QHBoxLayout ();
      hLayout -> addStretch ();
      hLayout -> addWidget ( load_ );
      hLayout -> addStretch ();

      //Vertical layout qui contient la table et l'horizontal layout
      QVBoxLayout * vLayout = new QVBoxLayout ();
      vLayout -> addWidget(view_);
      vLayout -> addLayout(hLayout);
      setLayout ( vLayout );
      
      //Conndction entre l'event clicket du button et la fonction load de cette class
      connect ( load_ , SIGNAL ( clicked ()) , this , SLOT ( load ()) );
    }

  //-----------------------SETTEUR------------------------------//
  void CellsLib::setCellViewer(CellViewer* c){ 
    cellViewer_ = c;
  }

  //-----------------------GETTEUR------------------------------//
  int CellsLib::getSelectedRow()const {
    QModelIndexList selecteds = view_ -> selectionModel ()
    -> selection ().indexes ();
    if ( selecteds.empty ()) return -1;
    return selecteds.first().row ();
  }

  //-----------------------SLOT------------------------------//
  void CellsLib::load(){

    //On recupere la ligne cliquee par l'utilisateur
    int selectedRow = getSelectedRow ();
    if ( selectedRow < 0) return ;

    //On affiche la cell qui correspont
    cellViewer_ -> setCell ( baseModel_ -> getModel ( selectedRow ) );
  }


}
