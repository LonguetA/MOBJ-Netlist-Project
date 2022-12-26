#include "CellsModel.h"
#include "Cell.h"

namespace Netlist {

  //-----------------------CTOR------------------------------//
  CellsModel :: CellsModel ( QObject * parent )
  : QAbstractTableModel ( parent )
  , cells_ (Cell::getAllCells()) { }

  //-----------------------DTOR------------------------------//
  CellsModel ::~CellsModel () { }


  //-----------------------GETTEUR------------------------------//

  int CellsModel :: rowCount ( const QModelIndex & parent ) const { return cells_.size(); }

  int CellsModel :: columnCount ( const QModelIndex & parent ) const { return 1; }
  
  //Info contenu dans une case de la table
  QVariant CellsModel :: data ( const QModelIndex & index, int role ) const { 
    if (cells_.size() == 0 and not index . isValid ()) return QVariant ();
    if ( role == Qt :: DisplayRole ) {

      int row = index . row ();
     
      return cells_[ row ]->getName().c_str();
    }

    return QVariant ();
  }

  //Info dans l'entete de la table
  QVariant CellsModel :: headerData ( int section, Qt :: Orientation orientation, int role ) const {
    if ( orientation == Qt :: Vertical ) return QVariant ();
    if ( role != Qt :: DisplayRole ) return QVariant ();
    
    switch ( section ) {
      case 0: return "Cell" ;
    }
    return QVariant ();
  }

  //Recuperation de la Cell qui correspond a la case cliquee par l'utilisateur
  Cell*     CellsModel ::getModel        ( int row ){
    
    //Si notre liste de Cell est vide
    if ( cells_.size() == 0 ) return NULL ;

    //Si la case selectionnee est invalide
    if ( row >= ( int ) cells_. size ()) return NULL ;


    return cells_[ row ];
  }

  //-----------------------SLOT------------------------------//

  //Mise a jour de la liste de Cell lors d'un load
  void CellsModel ::updateDatas(){
    emit layoutAboutToBeChanged ();
    
    //On recupere toutes les Cells
    cells_ = Cell::getAllCells();

    emit layoutChanged ();
  }
}
