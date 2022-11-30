
#ifndef NETLIST_INSTANCESMODEL_H
#define NETLIST_INSTANCESMODEL_H

#include <QDialog>
#include <QLineEdit>
#include "Cell.h"
#include "CellViewer.h"
#include "Instance.h"
#include "InstancesModel.h"
#include <QAbstractTableModel>

namespace Netlist {
  class InstancesModel : public QAbstractTableModel {
    Q_OBJECT ;
    public :
                        InstancesModel  ( QObject * parent = NULL );
                        ~InstancesModel ();
              void      setCell         ( Cell * );
              Cell *    getModel        ( int row );
              int       rowCount        ( const QModelIndex & parent = QModelIndex ()) const;
              int       columnCount     ( const QModelIndex & parent = QModelIndex ()) const;
              QVariant  data            ( const QModelIndex & index , int role = Qt :: DisplayRole) const;
              QVariant  headerData      ( int section, Qt :: Orientation orientation, int role = Qt :: DisplayRole ) const ;
    private :
        Cell * cell_ ;
  };
}

#endif