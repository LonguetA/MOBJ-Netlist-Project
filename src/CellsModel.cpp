#include "CellsModel.h"
#include "Cell.h"

namespace Netlist {
  CellsModel :: CellsModel ( QObject * parent )
  : QAbstractTableModel ( parent )
  , cells_ (Cell::getAllCells()) { }

  CellsModel ::~CellsModel () { }

  int CellsModel :: rowCount ( const QModelIndex & parent ) const { return cells_.size(); }

  int CellsModel :: columnCount ( const QModelIndex & parent ) const { return 1; }
  
  QVariant CellsModel :: data ( const QModelIndex & index, int role ) const
  { 
    if (cells_.size() == 0 and not index . isValid ()) return QVariant ();
    if ( role == Qt :: DisplayRole ) {

      int row = index . row ();
     
      return cells_[ row ]->getName().c_str();
    }

    return QVariant ();
  }

  QVariant CellsModel :: headerData ( int section, Qt :: Orientation orientation, int role ) const {
    if ( orientation == Qt :: Vertical ) return QVariant ();
    if ( role != Qt :: DisplayRole ) return QVariant ();
    
    switch ( section ) {
      case 0: return "Cell" ;
    }
    return QVariant ();
  }

  void CellsModel ::updateDatas(){
    emit layoutAboutToBeChanged ();
    cells_ = Cell::getAllCells();
    emit layoutChanged ();
  }

  Cell*     CellsModel ::getModel        ( int row ){
    if ( cells_.size() == 0 ) return NULL ;
    if ( row >= ( int ) cells_. size ()) return NULL ;
    return cells_[ row ];
  }
}
