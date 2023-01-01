# include <QLabel>
# include <QLineEdit>
# include <QPushButton>
# include <QHBoxLayout>
# include <QVBoxLayout>
#include <QMenu>
#include "ErrorCellDialog.h"



namespace Netlist {

  //-----------------------CTOR------------------------------//
  ErrorCellDialog::ErrorCellDialog(QWidget * parent,std::string msg)
  :QDialog ( parent )
  , lineEdit_ ( NULL ){
    setWindowTitle ( tr ( "ERROR" ) );

    QLabel * label = new QLabel ();
    label -> setText ( tr ( msg.c_str() ) );


    QPushButton * okButton = new QPushButton ();
    okButton -> setText ( "OK" );
    okButton -> setDefault ( true );


    QHBoxLayout * hLayout = new QHBoxLayout ();
    hLayout -> addStretch ();
    hLayout -> addWidget ( okButton );
    hLayout -> addStretch ();


    QFrame * separator = new QFrame ();
    separator -> setFrameShape ( QFrame :: HLine );
    separator -> setFrameShadow ( QFrame :: Sunken );


    QVBoxLayout * vLayout = new QVBoxLayout ();
    vLayout -> setSizeConstraint ( QLayout :: SetFixedSize );
    vLayout -> addWidget ( label );
    vLayout -> addLayout ( hLayout );
    setLayout ( vLayout );

    
    connect ( okButton , SIGNAL ( clicked ()) , this , SLOT ( accept ()) );
  }

  //-----------------------AFFICHAGE------------------------------//
  bool  ErrorCellDialog::run ( std::string &msg ){
    ErrorCellDialog *op = new ErrorCellDialog(NULL,msg);
    int dialogResult = op->exec ();
    delete op;
    return ( dialogResult == Accepted );
  }
}