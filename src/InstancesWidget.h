
#ifndef NETLIST_INSTANCESWIDGET_H
#define NETLIST_INSTANCESWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QTableView>
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Cell.h"
#include "CellViewer.h"
#include "InstancesModel.h"

namespace Netlist {

  class CellViewer;
  class InstancesModel;

  class InstancesWidget : public QWidget {
    Q_OBJECT;
    public:
                  InstancesWidget ( QWidget* parent=NULL );
            void  setCellViewer   ( CellViewer* );
            int   getSelectedRow  () const;
    void  setCell         ( Cell* );
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
}
#endif