#include "CellsLib.h"
#include "CellViewer.h"
#include <QHeaderView>

namespace Netlist{

CellsLib::CellsLib(QWidget* parent): QWidget( parent )
  , cellViewer_ ( NULL )
  , baseModel_ ( new CellsModel ( this ))
  , view_ ( new QTableView ( this ))
  , load_ ( new QPushButton ( this ))
  {
    setAttribute ( Qt::WA_QuitOnClose , false );
    setAttribute ( Qt::WA_DeleteOnClose , false );
    setContextMenuPolicy( Qt::ActionsContextMenu);

    view_  -> setShowGrid ( false );
    view_ -> setAlternatingRowColors ( true );
    view_ -> setSelectionBehavior ( QAbstractItemView :: SelectRows );
    view_ -> setSelectionMode( QAbstractItemView :: SingleSelection );
    view_ -> setSortingEnabled ( true );
    view_ -> setModel ( baseModel_ ); // On associe le modele .


    QHeaderView * horizontalHeader = view_ -> horizontalHeader ();
    horizontalHeader -> setDefaultAlignment ( Qt :: AlignHCenter );
    horizontalHeader -> setMinimumSectionSize ( 300 );
    horizontalHeader -> setStretchLastSection ( true );
    QHeaderView * verticalHeader = view_ -> verticalHeader ();
    verticalHeader -> setVisible ( false );
    load_ -> setText ( " Load " );

    QVBoxLayout * vLayout = new QVBoxLayout ();
    vLayout -> addWidget(view_);
    vLayout -> addWidget(load_);
    setLayout ( vLayout );
    
    connect ( load_ , SIGNAL ( clicked ()) , this , SLOT ( load ()) );
  }

void CellsLib::setCellViewer(CellViewer* c){ 
	cellViewer_ = c;
}
int CellsLib::getSelectedRow()const {return 1;}
void CellsLib::load(){}


}
