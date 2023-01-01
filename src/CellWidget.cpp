// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  "CellWidget.h"
#include  <QPainterPath>
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"


namespace Netlist {

  using namespace std;


  
  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  //-----------------------CTOR------------------------------//
  CellWidget::CellWidget ( QWidget* parent )
    : QWidget(parent)
    , cell_  (NULL)
    , viewport_ ( Box (0 ,0 ,500 ,500))
  {
    setAttribute    ( Qt::WA_OpaquePaintEvent );
    setAttribute    ( Qt::WA_NoSystemBackground );
    setAttribute    ( Qt::WA_StaticContents );
    setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy  ( Qt::StrongFocus );
    setMouseTracking( true );
  }

  //-----------------------DTOR------------------------------//
  CellWidget::~CellWidget ()
  { }


  //-----------------------SETTEUR------------------------------//
  void  CellWidget::setCell ( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }

  //-----------------------GETTEUR------------------------------//
  QSize  CellWidget::minimumSizeHint () const
  { return QSize(500,500); }


  //-----------------------GESTION DE L'AFFICHAGE------------------------------//


  //Fonction d'affichage celon des flags
  void CellWidget :: query ( unsigned int flags , QPainter & painter ) {

    //Si aucune cell a afficher ou aucun flag
    if (( not cell_ ) or ( not flags )) return ;


    /**********************AFFICHAGE DES INSTANCES************************/
    const vector <Instance *> & instances = cell_ -> getInstances ();
    

    for ( size_t i = 0; i < instances . size () ; ++ i ) {
      

      //Boolean qui gere si l'instance courante contient un ArcShape pour l'affichage de son nom
      bool noArc = true;

      //Recuperation de la position
      Point instPos = instances [ i ]  -> getPosition ();

      //Mise a jour de la police d'ecriture
      QFont  bigFont = QFont( "URW Bookman L", 12 );
      painter.setFont      ( bigFont );
      painter . setPen ( QPen ( Qt :: darkGreen , 0 ) );
      painter . setBrush ( QBrush ( Qt :: darkGreen ) );


      const Symbol * symbol = instances [ i ] -> getMasterCell () -> getSymbol ();
      if ( not symbol ) continue ;

      
      if ( flags == 1 or flags == 4) {
        
        //Coordonnée Max et Min de l'instance pour afficher son nom 
        int max_x = -1;
        int min_y = 2000;


        const vector < Shape * >& shapes = symbol -> getShapes ();

        //On parcours les shapes
        for ( size_t j =0 ; j < shapes . size () ; ++ j ) {

          int flagTxt = Qt::AlignCenter;

          //Affichage TermShape
          TermShape * termShape = dynamic_cast<TermShape *>(shapes[j]);


          if (termShape){

            //Recuperation du Term liee au TermShape
            Term *t = termShape->getTerm();

            //Rectangle du termShape
            Box box = termShape -> getBoundingBox ();
            QRect rect = boxToScreenRect ( box . translate ( instPos ));

            if (flags == 1) continue;

            //Placement du Nom du Term en fonction de son Alignement
            QRect tag = boxToScreenRect ( Box(box.getX1()-12,box.getY1()-12,box.getX2()+12,box.getY2()+12) );
            
            //HAUT GAUCHE
            if (termShape->getAlign() == TermShape::TopLeft){
              tag = boxToScreenRect ( Box(box.getX1()-12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
              flagTxt = Qt::AlignTop;
            }

            //HAUT DROITE
            if (termShape->getAlign() == TermShape::TopRight){
              tag = boxToScreenRect ( Box(box.getX1()+12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
              flagTxt = Qt::AlignTop;
            }


            //BAS GAUCHE
            if (termShape->getAlign() == TermShape::BottomLeft){
              tag = boxToScreenRect ( Box(box.getX1()-12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
              flagTxt = Qt::AlignBottom;
            }

            //BAS DROITE
            if (termShape->getAlign() == TermShape:: BottomRight){
              tag = boxToScreenRect ( Box(box.getX1()+12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
              flagTxt = Qt::AlignBottom;
            }
            

            //Affichage
            QFont  bigFont = QFont( "URW Bookman L", 10 );
            painter.setFont      ( bigFont );
            painter . setPen ( QPen ( Qt :: red , 0 ) );
            painter . setBrush ( QBrush ( Qt :: red ) );
            painter.drawText( tag, flagTxt, QString(t->getName().c_str() ));
            painter . drawRect ( rect );

            continue;
            
          }

          if (flags == 4) continue;


          //Affichage boxShape
          BoxShape * boxShape = dynamic_cast <BoxShape *>( shapes [ j ]);


          if ( boxShape ) {

            Box box = boxShape -> getBoundingBox ();

            //Affichage
            painter . setPen ( QPen ( Qt ::  darkGreen , 2 ) );
            painter . setBrush ( QBrush ( Qt ::  black ) );
            QRect rect = boxToScreenRect ( box . translate ( instPos ));
            painter . drawRect ( rect );


            //Maj des coordonnees de l'instance
            if (max_x < rect.x() + rect.width()){
              max_x = rect.x() + rect.width();
            }

            if (min_y > rect.y()){
              min_y = rect.y() ;
            }

            continue;

          }


          //Affichage LineShape
          LineShape * lineShape = dynamic_cast<LineShape *>(shapes[j]);
          if (lineShape){       

            //Affichage   
            painter . setPen ( QPen ( Qt ::  darkGreen , 2 ) );
            painter . setBrush ( QBrush ( Qt ::  darkGreen ) );
            painter . drawLine ( xToScreenX((lineShape->getX1())+instPos.getX()) ,yToScreenY((lineShape->getY1())+instPos.getY()),xToScreenX((lineShape->getX2())+instPos.getX()) ,yToScreenY((lineShape->getY2())+instPos.getY()));


            //Maj des coordonnees de l'instance
            int xtmp = min(lineShape->getX1()+instPos.getX(),lineShape->getX2())+instPos.getX();
            int ytmp = min(lineShape->getY1()+instPos.getY(),lineShape->getY2())+instPos.getY();
            
            if (max_x < xToScreenX(xtmp)){
                max_x = xToScreenX(xtmp);
              }

            if (min_y > yToScreenY(ytmp)){
              min_y = yToScreenY(ytmp);
            }

            continue;
         }


          //Affichage EllipseShape
          EllipseShape * ellipseShape = dynamic_cast<EllipseShape *>(shapes[j]);

          if (ellipseShape){

            Box box = ellipseShape -> getBoundingBox ();
            QRect rect = boxToScreenRect ( box . translate ( instPos ));
            

            //Affichage
            painter . setPen ( QPen ( Qt ::  darkGreen , 2 ) );
            painter . setBrush ( QBrush ( Qt :: black ) );
            painter . drawEllipse ( rect );


            //Maj des coordonnees de l'instance
            if (max_x < rect.x() + rect.width( )){
              max_x = rect.x() + rect.width( );
            }

            if (min_y > rect.y()){
              min_y = rect.y() ;
            }

            continue;

          }


          //Affichage ArcShape
          ArcShape * arcShape = dynamic_cast<ArcShape *>(shapes[j]);

          if (arcShape){

            Box box = arcShape -> getBoundingBox ();
            QRect rect = boxToScreenRect ( box . translate ( instPos ));
            
            //Mise a jour du boolean
            noArc = false;

            //Affichage
            painter . setPen ( QPen ( Qt :: darkGreen , 2 ) );
            painter . setBrush ( QBrush ( Qt ::  black ) );
            painter . drawArc ( rect,arcShape->getStart()*16,arcShape->getSpan()*16);
            
            //Maj des coordonnees de l'instance
            if (max_x < rect.x() + rect.width()){
              max_x = rect.x()+ rect.width();
            }

            if (min_y > rect.y()){
              min_y = rect.y();
            }

            continue;
          }
        }

        //Affichage du nom de l'instanxe
        if (flags == 1) {

          //Si on a pas d'arc
          if (noArc) painter.drawText( QPoint(max_x+5,min_y), QString(instances [ i ]->getName().c_str() ));

          //Sinon
          else  painter.drawText( QPoint(max_x-20,min_y+60), QString(instances [ i ]->getName().c_str() ));
        }
      }
      
    }

    /**********************AFFICHAGE DES NETS************************/
    if (flags == 2){

      //Recuperation des Nets
      std::vector<Net *> nets = cell_->getNets();

      //Si aucun net
      if (nets.size() != 0){

        for (size_t i = 0; i < nets . size () ; ++ i ){

          //On recupere les Lines
          std::vector<Line *> ls = nets[i]->getLines();


          for (size_t j = 0 ; j < ls.size() ; j++){
            painter . setPen ( QPen ( Qt :: cyan , 1 ) );
            painter . setBrush ( QBrush ( Qt :: cyan ) );


            //Recuperation de la source et de la target
            Point pS = ls[j]->getSourcePosition();
            Point pT = ls[j]->getTargetPosition();


            //Si le node target a plus de 2 lines connectees on trace un rond
            if (ls[j]->getTarget()->getLines().size() > 2){
              painter. drawEllipse(xToScreenX(pT.getX()-5),yToScreenY(pT.getY()+5),10,10);
            }

            //Affichage
            painter . drawLine(pointToScreenPoint(pS),pointToScreenPoint(pT));
          }
        }
      }
    }

    /**********************AFFICHAGE DES SYMBOLS************************/
    if (flags == 3 or flags == 4){

      //Si la Cell n'a pas d'instance on affichage sa representation symbol
      if (instances.size() == 0){

        //On recupere le symbol
        Symbol * s = cell_->getSymbol();

        //On recupere les shapes
        const vector < Shape * >& shapes = s -> getShapes ();


        for ( size_t j =0 ; j < shapes . size () ; ++ j ) {

              //Affichage boxShape
              BoxShape * boxShape = dynamic_cast <BoxShape *>( shapes [ j ]);

              if ( boxShape ) {
                Box box = boxShape -> getBoundingBox ();
                QRect rect = boxToScreenRect ( box.translate(Point(250,250)) );
                painter . drawRect ( rect );

                continue;
              }

              //Affichage TermShape
              TermShape * termShape = dynamic_cast<TermShape *>(shapes[j]);

              if (termShape){
                Term *t = termShape->getTerm();
                Box box = termShape -> getBoundingBox ();
                int flagTxt;
                QRect rect = boxToScreenRect ( box.translate(Point(250,250))  );
              
                QRect tag = boxToScreenRect ( Box(box.getX1()-12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
                if (termShape->getAlign() == TermShape::TopLeft){
                  tag = boxToScreenRect ( Box(box.getX1()-12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
                  flagTxt = Qt::AlignTop;
                }

                if (termShape->getAlign() == TermShape::TopRight){
                  tag = boxToScreenRect ( Box(box.getX1()+12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
                  flagTxt = Qt::AlignTop;
                }

                if (termShape->getAlign() == TermShape::BottomLeft){
                  tag = boxToScreenRect ( Box(box.getX1()-12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
                  flagTxt = Qt::AlignBottom;
                }
              

                if (termShape->getAlign() == TermShape:: BottomRight){
                  tag = boxToScreenRect ( Box(box.getX1()+12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );
                  flagTxt = Qt::AlignBottom;
                }

                QFont  bigFont = QFont( "URW Bookman L", 10 );
                painter.setFont      ( bigFont );
                painter . setPen ( QPen ( Qt :: red , 0 ) );
                painter . setBrush ( QBrush ( Qt :: red ) );
                painter . drawRect ( rect );
                painter . setPen ( QPen ( Qt :: red , 0 ) );
                painter . setBrush ( QBrush ( Qt :: red ) );
                //painter . drawRect ( tag );
                painter.drawText( tag, flagTxt, QString(t->getName().c_str() ));

                continue;
              }

              if (flags == 4) continue;
              

              //Affichage LineShape
              LineShape * lineShape = dynamic_cast<LineShape *>(shapes[j]);

              if (lineShape){          
                painter . setPen ( QPen ( Qt ::  darkGreen , 2 ) );
                painter . setBrush ( QBrush ( Qt ::  darkGreen ) );
                painter . drawLine ( xToScreenX((lineShape->getX1()+250)) ,yToScreenY((lineShape->getY1())+250),xToScreenX((lineShape->getX2())+250) ,yToScreenY((lineShape->getY2())+250));

                continue;
              }

              //Affichage EllipseShape
              EllipseShape * ellipseShape = dynamic_cast<EllipseShape *>(shapes[j]);

              if (ellipseShape){
                Box box = ellipseShape -> getBoundingBox ();
                QRect rect = boxToScreenRect ( box.translate(Point(250,250))  );
                
                painter . setPen ( QPen ( Qt ::  darkGreen , 2 ) );
                painter . setBrush ( QBrush ( Qt :: black ) );
                painter . drawEllipse ( rect );

                continue;
              }


              //Affichage ArcShape
              ArcShape * arcShape = dynamic_cast<ArcShape *>(shapes[j]);

              if (arcShape){
                Box box = arcShape -> getBoundingBox ();
                QRect rect = boxToScreenRect ( box.translate(Point(250,250))  );
                
                painter . setPen ( QPen ( Qt ::  darkGreen , 2 ) );
                painter . setBrush ( QBrush ( Qt ::  darkGreen ) );
                painter . drawArc ( rect,arcShape->getStart()*16,arcShape->getSpan()*16);

                continue;
              }
            }
          }

          //Sinon on affiche les termShapes de la cellule
          else{


            Symbol * s = cell_->getSymbol();

            //Si on a des shapes
            if (s->getShapes().size() != 0){

              //On recupere les Shapes
              const vector < Shape * >& shapes = s -> getShapes ();


              for ( size_t j =0 ; j < shapes . size () ; ++ j ) {

                //Affichage TermShape
                TermShape * termShape = dynamic_cast<TermShape *>(shapes[j]);

                if (termShape){

                  Term *t = termShape->getTerm();
                  Box box = termShape -> getBoundingBox ();
                  int flagTxt;

                  QRect rect = boxToScreenRect ( Box(t->getPosition().getX()-10,t->getPosition().getY()-10,t->getPosition().getX()+10,t->getPosition().getY()+10) );
                  QPainterPath path;

                  //Affichage du Term en fonction de sa direction

                  if (t->getDirection() == Term::In){

                    //Dessin triangle 
                    path.moveTo(rect.left(), rect.top());
                    path.lineTo(rect.left(),rect.bottom());
                    path.lineTo(rect.right(),rect.top() + (rect.height()/2));
                    path.lineTo(rect.topLeft());

                    rect = boxToScreenRect ( Box(t->getPosition().getX()-30,t->getPosition().getY()-8,t->getPosition().getX()-10,t->getPosition().getY()+10) );
                  }


                  else{
                    //Dessin triangle 
                    path.moveTo(rect.right(), rect.top());
                    path.lineTo(rect.right(),rect.bottom());
                    path.lineTo(rect.left(),rect.top() + (rect.height()/2));
                    path.lineTo(rect.topRight());

                    rect = boxToScreenRect ( Box(t->getPosition().getX()+9,t->getPosition().getY()-8,t->getPosition().getX()+30,t->getPosition().getY()+10) );
                  
                  } 


                  painter.fillPath(path, QBrush(QColor ("red"))); 
                  painter.drawRect(rect);

                  QRect tag = boxToScreenRect ( Box(box.getX1()-12,box.getY1()-12,box.getX2()+40,box.getY2()+12) );


                  //Nom du term en fonction de son alignement et sa direction

                  //HAUT GAUCHE
                  if (termShape->getAlign() == TermShape::TopLeft){
                    if (t->getDirection() == Term::In){
                      tag = boxToScreenRect ( Box(t->getPosition().getX()-40,t->getPosition().getY(),t->getPosition().getX(),t->getPosition().getY()+40) );
                    }
                    else{
                      tag = boxToScreenRect ( Box(t->getPosition().getX()-10,t->getPosition().getY(),t->getPosition().getX()+30,t->getPosition().getY()+40) );
                    }
                    flagTxt = Qt::AlignTop;
                  }

                  //HAUT DROIT
                  if (termShape->getAlign() == TermShape::TopRight){
                    if (t->getDirection() == Term::In){
                      tag = boxToScreenRect ( Box(t->getPosition().getX()+10,t->getPosition().getY(),t->getPosition().getX()+50,t->getPosition().getY()+40) );
                    }
                    else{
                      tag = boxToScreenRect ( Box(t->getPosition().getX()+30,t->getPosition().getY(),t->getPosition().getX()+70,t->getPosition().getY()+40) );
                    }
                    flagTxt = Qt::AlignTop;
                  }

                  //BAS GAUCHE
                  if (termShape->getAlign() == TermShape::BottomLeft){
                    tag = boxToScreenRect ( Box(t->getPosition().getX(),t->getPosition().getY(),t->getPosition().getX(),t->getPosition().getY()) );
                    flagTxt = Qt::AlignBottom;
                  }
                
                  //BAS DROIT
                  if (termShape->getAlign() == TermShape:: BottomRight){
                    tag = boxToScreenRect ( Box(t->getPosition().getX(),t->getPosition().getY(),t->getPosition().getX(),t->getPosition().getY()) );
                    flagTxt = Qt::AlignBottom;
                  }

                  //Affichage
                  QFont  bigFont = QFont( "URW Bookman L", 20 );
                  painter.setFont      ( bigFont );
                  painter . setPen ( QPen ( Qt :: red , 0 ) );
                  painter . setBrush ( QBrush ( Qt :: red ) );
                  painter . setPen ( QPen ( Qt :: red , 0 ) );
                  painter . setBrush ( QBrush ( Qt :: red ) );
                  painter.drawText( tag, flagTxt, QString(t->getName().c_str() ));
                }
              }
            }

            //Sinon dessins des term de la cellule
            else{
              std::vector<Term *> terms = cell_->getTerms();

              for (size_t i = 0 ; i < terms.size() ; i++){
                Term *t = terms[i];


                Box box = Box( t->getPosition().getX()-2 , t->getPosition().getY()-2 , t->getPosition().getX()+2 ,t->getPosition().getY()+2 );
                QRect tag;
                QRect rect = boxToScreenRect ( Box(t->getPosition().getX()-10,t->getPosition().getY()-10,t->getPosition().getX()+10,t->getPosition().getY()+10) );
                QPainterPath path;


                if (t->getDirection() == Term::In){
                  path.moveTo(rect.left(), rect.top());
                  path.lineTo(rect.left(),rect.bottom());
                  path.lineTo(rect.right(),rect.top() + (rect.height()/2));
                  path.lineTo(rect.topLeft());

                  rect = boxToScreenRect ( Box(t->getPosition().getX()-30,t->getPosition().getY()-8,t->getPosition().getX()-10,t->getPosition().getY()+10) );
                  tag = boxToScreenRect ( Box(t->getPosition().getX()-60,t->getPosition().getY(),t->getPosition().getX()+10,t->getPosition().getY()+30) );
                }
                else{
                  path.moveTo(rect.right(), rect.top());
                  path.lineTo(rect.right(),rect.bottom());
                  path.lineTo(rect.left(),rect.top() + (rect.height()/2));
                  path.lineTo(rect.topRight());

                  rect = boxToScreenRect ( Box(t->getPosition().getX()+9,t->getPosition().getY()-8,t->getPosition().getX()+30,t->getPosition().getY()+10) );
                  tag = boxToScreenRect ( Box(t->getPosition().getX(),t->getPosition().getY(),t->getPosition().getX()+60,t->getPosition().getY()+40) );
                }

                //Affichage
                QFont  bigFont = QFont( "URW Bookman L", 20 );
                painter.setFont      ( bigFont );
                painter . setPen ( QPen ( Qt :: red , 0 ) );
                painter . setBrush ( QBrush ( Qt :: red ) );
                painter.fillPath(path, QBrush(QColor ("red"))); 
                painter.drawRect(rect);
                painter.drawText( tag, Qt::AlignTop, QString(t->getName().c_str() ));
              }
            }
          }
        }

  }

  void  CellWidget::resizeEvent ( QResizeEvent* event ){ 
    const QSize & size = event -> size ();
    // Assume the resize is always done by drawing the bottom right corner .
    viewport_.setX2 ( viewport_.getX1 () + size .width () );
    viewport_.setY1 ( viewport_.getY2 () - size .height () );
    std::cerr << " CellWidget :: resizeEvent ()  viewport_ : " << viewport_ << std::endl ;
    repaint(); 
  }


  //Fonction de dessin
  void  CellWidget::paintEvent ( QPaintEvent* event )
  {
    QFont  bigFont = QFont( "URW Bookman L", 36 );

    QString cellName = "NULL";
    if (cell_) cellName = cell_->getName().c_str();

    QPainter painter(this);
    painter.setFont      ( bigFont );
    painter.setBackground( QBrush( Qt::black ) );
    painter.eraseRect    ( QRect( QPoint(0,0), size() ) );
    painter . setPen ( QPen ( Qt :: white , 0 ) );

    int frameWidth  = 460;
    int frameHeight = 100;
    QRect nameRect1 ( (size().width ()-frameWidth )/2
                   , (size().height()-frameHeight)/2
                   , frameWidth
                   , frameHeight
                   );
    QRect nameRect2 ( 0
                   , 0
                   , frameWidth
                   , frameHeight
                   );
    
    //Affichage du nom de la cellule courante
    if (!cell_)painter.drawText( nameRect1, Qt::AlignCenter, "No Cell Loaded" );
    else painter.drawText(nameRect2,Qt::AlignLeft,cellName);


    //Dessin des fils
    query(2,painter);

    //Dessin des Term de la cellule
    query(3,painter);

    //Dessin des instances
    query(1,painter);
    
    //Dessin des termShape des instances
    query(4,painter);
  }

  void CellWidget :: keyPressEvent ( QKeyEvent * event ) {
    event -> ignore ();
    if ( event -> modifiers () & ( Qt :: ControlModifier | Qt::ShiftModifier )) return ;
    switch ( event -> key ()) {
      case Qt :: Key_Up : goUp (); break ;
      case Qt :: Key_Down : goDown (); break ;
      case Qt :: Key_Left : goLeft (); break ;
      case Qt :: Key_Right : goRight (); break ;
      default : return ;
    }
    event -> accept ();
  }

  void CellWidget :: goRight () {
    viewport_ . translate ( Point (20 ,0) );
    repaint ();
  }

  void CellWidget :: goUp () {
    viewport_ . translate ( Point (0 ,20) );
    repaint ();
  }

  void CellWidget :: goDown () {
    viewport_ . translate ( Point (0 ,-20) );
    repaint ();
  }

  void CellWidget :: goLeft () {
    viewport_ . translate ( Point (-20 ,0) );
    repaint ();
  }


}  // Netlist namespace.
