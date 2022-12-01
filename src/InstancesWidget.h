
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
                  InstancesWidget ( QWidget* parent=NULL );
            void  setCellViewer   ( CellViewer* );
            int   getSelectedRow  () const;
    inline void  setCell         ( Cell* );
            //bool  run             ();
    protected:
      //virtual void    paintEvent         ( QPaintEvent* );

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
