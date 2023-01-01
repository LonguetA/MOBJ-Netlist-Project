
#ifndef NETLIST_INSTANCESWIDGET_H
#define NETLIST_INSTANCESWIDGET_H

#include <QTableView>
#include "InstancesModel.h"
#include <QPushButton>

namespace Netlist {

  class CellViewer;
  class Cell;
  
  class InstancesWidget : public QWidget {
    Q_OBJECT;
    public:
    //-----------------------CTOR------------------------------//
                  InstancesWidget ( QWidget* parent=NULL );

    //-----------------------SETTEUR------------------------------//
            void  setCellViewer   ( CellViewer* );
    inline  void  setCell         ( Cell* );

    //-----------------------GETTEUR------------------------------//
            int   getSelectedRow  () const;

    //-----------------------SLOT------------------------------//
    public slots:
            void  load            ();

            
    private:
            CellViewer*     cellViewer_;
            InstancesModel* baseModel_;
            QTableView*     view_;
            QPushButton*    load_;
  };
  inline void InstancesWidget::setCell(Cell *c){ baseModel_->setCell(c);}
}
#endif
