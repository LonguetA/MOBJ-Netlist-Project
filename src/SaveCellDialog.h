#ifndef NETLIST_SAVECELLDIALOG_H
#define NETLIST_SAVECELLDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "Cell.h"

namespace Netlist {

  class Cell;
  class SaveCellDialog : public QDialog {
    Q_OBJECT ;
    public :
                      SaveCellDialog  ( QWidget * parent = NULL );
              bool    run             ( QString & name );
        const QString getCellName     () const ;
              void    setCellName     ( const QString & );
    protected :
              QLineEdit * lineEdit_ ;
  };
}

#endif