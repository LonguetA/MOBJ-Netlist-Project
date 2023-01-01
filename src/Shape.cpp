#include  <cstdlib>
#include  <fstream>
#include  "XmlUtil.h"
#include  "Cell.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include "Shape.h"
#include "Symbol.h"


namespace Netlist {

    using namespace std;

    /************************SHAPE****************************************/

    //-----------------------XML------------------------------//
    Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
    {
    // Factory-like method.
      const xmlChar* boxTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
      const xmlChar* ellipseTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
      const xmlChar* arcTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
      const xmlChar* lineTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
      const xmlChar* termTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
      const xmlChar* nodeName
          = xmlTextReaderConstLocalName( reader );

      Shape* shape = NULL;
      if (boxTag == nodeName)
        shape = BoxShape::fromXml( owner, reader );
      if (ellipseTag == nodeName)
        shape = EllipseShape::fromXml( owner, reader );
      if (arcTag == nodeName)
        shape = ArcShape::fromXml( owner, reader );
      if (lineTag == nodeName)
        shape = LineShape::fromXml( owner, reader );
      if (termTag == nodeName)
        shape = TermShape::fromXml( owner, reader );

      if (shape == NULL)
        cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

      return shape;
    }

    //-----------------------CTOR------------------------------//
    Shape::Shape ( Symbol * owner ):owner_( owner ) { owner->add( this );}

    //-----------------------DTOR------------------------------//
    Shape::~Shape (){ owner_->remove( this ); }


    /************************BOXSHAPE****************************************/

    //-----------------------CTOR------------------------------//
    BoxShape::BoxShape  ( Symbol * owner, const Box & box):Shape(owner),box_(box){}

    //-----------------------DTOR------------------------------//
    BoxShape::~BoxShape (){}

    //-----------------------GETTEUR------------------------------//
    Box BoxShape::getBoundingBox() const { return box_;}

    //-----------------------XML------------------------------//
    void BoxShape::toXml ( std::ostream& os) const{
        os << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>" << std::endl;
    }

    BoxShape *  BoxShape::fromXml ( Symbol* owner, xmlTextReaderPtr reader ){
        //Recuperation des parametres
        std::string x1Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        std::string x2Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        std::string y1Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        std::string y2Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );

        //Si les parametres sont vides 
        if (x1Name.empty() or x2Name.empty() or y1Name.empty() or y2Name.empty()){
            std::cerr << "[ERROR] BoxShape::fromXml(): Missing parameter" << std::endl;
            return NULL;
        }

        //Creation de la boxShape
        BoxShape *b = new BoxShape(owner,Box(atoi(x1Name.c_str()),atoi(y1Name.c_str()),atoi(x2Name.c_str()),atoi(y2Name.c_str())));

        return b;
    }


    /************************TERMSHAPE****************************************/

    //-----------------------CTOR------------------------------//
    TermShape::TermShape ( Symbol * owner , std::string name , int x1 , int y1 ,NameAlign align):Shape( owner ),term_( NULL ),x_( x1 ),y_( y1 ),align_(align) {
        Cell * cell = owner->getCell ();
        term_ = cell->getTerm ( name );
    }

    //-----------------------DTOR------------------------------//
    TermShape::~TermShape () { }

    //-----------------------GETTEUR------------------------------//
    Box TermShape::getBoundingBox () const { return Box( x_-2 , y_-2 , x_+2 , y_+2 ); }

    string TermShape::alignToString(NameAlign align){
        switch(align){
            case TopLeft:
                return "\"top_left\"";
            case TopRight:
                return "\"top_right\"";
            case BottomLeft:
                return "\"bottom_left\"";
            case BottomRight:
                return "\"bottom_right\"";
            default:
                return "\"unknow_align\"";
        }
    }
    TermShape::NameAlign TermShape::stringToAlign(string str) {
        if (str == "top_left")  return TopLeft;
        if (str == "top_right") return TopRight;
        if (str == "bottom_left") return BottomLeft;
        if (str == "bottom_right") return BottomRight;
    }

    //-----------------------XML------------------------------//
    void TermShape::toXml(std::ostream & os) const{
        os << indent << "<term name=\"" << term_->getName() << "\" x1=" << getX() << "\" y1=\"" << getY() << "\" align=" << alignToString(align_) << "/>" << std::endl;
    }
    TermShape * TermShape::fromXml( Symbol* owner, xmlTextReaderPtr reader ){
        //Recuperation des parametres
        std::string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        std::string alignName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align" ) );
        std::string xName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        std::string yName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );

        //Si les parametres sont vides 
        if (termName.empty() or alignName.empty() or xName.empty() or yName.empty()){
            std::cerr << "[ERROR] TermShape::fromXml(): Missing parameter" << std::endl;
            return NULL;
        }

        //Creation du TermShape
        TermShape *t = new TermShape(owner,termName,atoi(xName.c_str()),atoi(yName.c_str()),stringToAlign(alignName));

        return t;
    }

    /************************LINESHAPE****************************************/

    //-----------------------CTOR------------------------------//
    LineShape::LineShape( Symbol * owner, int x1 , int y1 , int x2 , int y2 ):Shape(owner),x1_(x1),y1_(y1),x2_(x2),y2_(y2){}
    
    //-----------------------DTOR------------------------------//
    LineShape::~LineShape(){}

    //-----------------------GETTEUR------------------------------//
    Box LineShape::getBoundingBox  () const {
        return Box(min(x1_,x2_) , min(y1_,y2_), max(x2_,x1_) ,max(y2_,y1_)) ;}


    //-----------------------XML------------------------------//
    void LineShape::toXml ( std::ostream & os) const {
        os << indent << "<line x1=" << getX1() << "\" y1=\"" << getY1() << "\" x2=" << getX2() << "\" y2=\"" << getY2() << "\"/>" << std::endl;
    }

    LineShape * LineShape::fromXml         ( Symbol* owner, xmlTextReaderPtr reader ){
        //Recuperation des parametres
        std::string x1Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        std::string x2Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        std::string y1Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        std::string y2Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );

        //Si les parametres sont vides 
        if (x1Name.empty() or x2Name.empty() or y1Name.empty() or y2Name.empty()){
            std::cerr << "[ERROR] LineShape::fromXml(): Missing parameter" << std::endl;
            return NULL;
        }

        //Creation du LineShape
        LineShape *l = new LineShape(owner,atoi(x1Name.c_str()),atoi(y1Name.c_str()),atoi(x2Name.c_str()),atoi(y2Name.c_str()));

        return l;
    }

    /************************ARCSHAPE****************************************/


    //-----------------------CTOR------------------------------//
    ArcShape::ArcShape (Symbol * owner,const Box & b,int start, int span):Shape(owner),box_(b),start_(start),span_(span){}
    
    //-----------------------DTOR------------------------------//
    ArcShape::~ArcShape (){}

    //-----------------------GETTEUR------------------------------//
    Box ArcShape::getBoundingBox () const {return box_;}

    //-----------------------XML------------------------------//
    void ArcShape::toXml(std::ostream & os) const {
        os << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << 
        "\" start=\"" << start_ << "\" span=\"" << span_<<"\"/>" << std::endl;
    }

    ArcShape *ArcShape::fromXml( Symbol* owner, xmlTextReaderPtr reader ){
       //Recuperation des parametres
        std::string x1Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        std::string x2Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        std::string y1Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        std::string y2Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
        std::string spanName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"span" ) );
        std::string startName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"start" ) );


        //Si les parametres sont vides 
        if (x1Name.empty() or x2Name.empty() or y1Name.empty() or y2Name.empty() or spanName.empty() or startName.empty()){
            std::cerr << "[ERROR] ArcShape::fromXml(): Missing parameter" << std::endl;
            return NULL;
        }

        //Creation du ArcShape
        ArcShape *arc = new ArcShape(owner,Box(atoi(x1Name.c_str()),atoi(y1Name.c_str()),atoi(x2Name.c_str()),atoi(y2Name.c_str())),atoi(startName.c_str()),atoi(spanName.c_str()));

        return arc;
    }

    /************************ELLIPSESHAPE****************************************/

    //-----------------------CTOR------------------------------//
    EllipseShape::EllipseShape (Symbol *owner,const Box &b):Shape(owner),box_(b){}

    //-----------------------DTOR------------------------------//
    EllipseShape::~EllipseShape (){}

    //-----------------------GETTEUR------------------------------//
    Box EllipseShape::getBoundingBox () const { return box_;}

    //-----------------------XML------------------------------//
    void EllipseShape::toXml (std::ostream & os) const {
        os << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>" << std::endl;
    }

    EllipseShape * EllipseShape::fromXml ( Symbol* owner, xmlTextReaderPtr reader ){
        //Recuperation des parametres
        std::string x1Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        std::string x2Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        std::string y1Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        std::string y2Name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );

        //Si les parametres sont vides 
        if (x1Name.empty() or x2Name.empty() or y1Name.empty() or y2Name.empty()){
            std::cerr << "[ERROR] LineShape::fromXml(): Missing parameter" << std::endl;
            return NULL;
        }

        //Creation du EllipseShape
        EllipseShape *e = new EllipseShape(owner,Box(atoi(x1Name.c_str()),atoi(y1Name.c_str()),atoi(x2Name.c_str()),atoi(y2Name.c_str())));

        return e;
    }


}