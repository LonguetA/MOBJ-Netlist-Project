// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"


namespace Netlist {

  class Cell;
  class NodeTerm;


  class CellWidget : public QWidget {
      Q_OBJECT;
    public:

    //-----------------------CTOR------------------------------//
                      CellWidget         ( QWidget* parent=NULL );

    //-----------------------DTOR------------------------------//
      virtual        ~CellWidget         ();

    //-----------------------SETTEUR------------------------------//
              void    setCell            ( Cell* );

    //-----------------------GETTEUR------------------------------//
      inline  Cell*   getCell            () const;
      virtual QSize   minimumSizeHint    () const;

      
    //-----------------------CONVERTION------------------------------//
      inline  int     xToScreenX ( int x ) const { return x - viewport_.getX1(); }
      inline  int     yToScreenY ( int y ) const { return viewport_.getY2() - y;}
      inline  int     screenXToX ( int x ) const { return x + viewport_.getX1();}
      inline  int     screenYToY ( int y ) const { return viewport_.getY2() - y;}
      inline  QRect   boxToScreenRect    ( const Box& b) const { return QRect(xToScreenX(b.getX1()),yToScreenY(b.getY2()),b.getWidth(),b.getHeight());}
      inline  QPoint  pointToScreenPoint ( const Point& p) const {return QPoint(xToScreenX(p.getX()),yToScreenY(p.getY()));}
      inline  Box     screenRectToBox    ( const QRect& r) const {return Box(screenXToX(r.x()),screenYToY(r.y() + r.height()),screenXToX(r.x() + r.width()),screenYToY(r.y()));}
      inline  Point   screenPointToPoint ( const QPoint& p) const {return Point(screenXToX(p.x()),screenYToY(p.y()));}
              

    //-----------------------GESTION DE L'AFFICHAGE------------------------------//
      virtual void    resizeEvent        ( QResizeEvent* );
              void    query ( unsigned int flags , QPainter & painter );
  
    protected:
      virtual void    paintEvent         ( QPaintEvent* );
      virtual void    keyPressEvent      ( QKeyEvent* );


    //-----------------------SLOT------------------------------//
    public slots:
              void    goLeft             ();
              void    goRight            ();
              void    goUp               ();
              void    goDown             ();
    private:
      Cell* cell_;
      Box viewport_;
  };


  inline Cell* CellWidget::getCell () const { return cell_; }


}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H
