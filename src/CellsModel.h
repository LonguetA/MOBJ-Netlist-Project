
#ifndef NETLIST_CELLSMODEL_H
#define NETLIST_CELLSMODEL_H

#include <QAbstractTableModel>
#include <vector>

namespace Netlist {

class Cell;
  class CellsModel : public QAbstractTableModel {
    Q_OBJECT ;
    public :
                        CellsModel  ( QObject * parent = NULL );
                        ~CellsModel ();
              Cell*     getModel        ( int row );
              int       rowCount        ( const QModelIndex & parent = QModelIndex ()) const;
              int       columnCount     ( const QModelIndex & parent = QModelIndex ()) const;
              QVariant  data            ( const QModelIndex & index , int role = Qt :: DisplayRole) const;
              QVariant  headerData      ( int section, Qt :: Orientation orientation, int role = Qt :: DisplayRole ) const ;
    public slots:
              void updateDatas();
    private :
        std::vector<Cell*> cells_ ;
  };
}

#endif
