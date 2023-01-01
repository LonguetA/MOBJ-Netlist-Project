
#ifndef NETLIST_INSTANCESMODEL_H
#define NETLIST_INSTANCESMODEL_H

#include "Cell.h"
#include <QAbstractTableModel>

namespace Netlist {
  class InstancesModel : public QAbstractTableModel {
    Q_OBJECT ;
    public :

    //-----------------------CTOR------------------------------//
                        InstancesModel  ( QObject * parent = NULL );

    //-----------------------DTOR------------------------------//
                        ~InstancesModel ();

    //-----------------------SETTEUR------------------------------//
              void      setCell         ( Cell * );

    //-----------------------GETTEUR------------------------------//
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
