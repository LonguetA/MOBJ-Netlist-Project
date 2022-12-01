#include "Cell.h"
#include "CellViewer.h"
#include <QHeaderView>
#include "InstancesWidget.h"
#include "InstancesModel.h"

namespace Netlist {

  using namespace std;

  InstancesWidget :: InstancesWidget ( QWidget * parent )
  : QWidget( parent )
  , cellViewer_ ( NULL )
  , baseModel_ ( new InstancesModel ( this ))
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

    QHBoxLayout * hLayout = new QHBoxLayout ();
    hLayout -> addStretch ();
    hLayout -> addWidget ( load_ );
    hLayout -> addStretch ();


    QVBoxLayout * vLayout = new QVBoxLayout ();
    vLayout -> addWidget(view_);
    vLayout -> addLayout(hLayout);
    setLayout ( vLayout );
    
    connect ( load_ , SIGNAL ( clicked ()) , this , SLOT ( load ()) );
  }

  int InstancesWidget :: getSelectedRow () const
  {
    QModelIndexList selecteds = view_ -> selectionModel ()
    -> selection ().indexes ();
    if ( selecteds.empty ()) return -1;
    return selecteds.first().row ();
  }

  void InstancesWidget :: load ()
  {
    int selectedRow = getSelectedRow ();
    if ( selectedRow < 0) return ;
    cellViewer_ -> setCell ( baseModel_ -> getModel ( selectedRow ) );
  }

  void InstancesWidget ::setCellViewer   ( CellViewer* c){
    cellViewer_ = c;
  }


}
