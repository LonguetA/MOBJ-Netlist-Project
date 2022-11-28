

#include  <cstdlib>
#include  <fstream>
#include  "XmlUtil.h"
#include  "Cell.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include "Symbol.h"
#include "Shape.h"


namespace Netlist {

  using namespace std;

Symbol::Symbol ( Cell* c):owner_(c),shapes_(){}

Symbol::~Symbol (){ while ( not shapes_     .empty() ) delete *shapes_     .begin(); }

Cell*  Symbol::getCell         () const { return owner_; }

Box    Symbol::getBoundingBox  () const { 
    Box b ;
    for (std::vector<Shape*>::const_iterator it = shapes_.begin() ; it != shapes_.end() ; it++){
        b.merge((*it)->getBoundingBox());
    }
    return b;

}

Point   Symbol::getTermPosition ( Term* t ) const{
  for (std::vector<Shape*>::const_iterator it = shapes_.begin() ; it != shapes_.end() ; it++){
    TermShape *tr = dynamic_cast<TermShape *>(*it);
    if (tr and tr->getTerm() == t){
      return tr->getTerm()->getPosition();
    }
  }
  return Point();
}

TermShape* Symbol::getTermShape    ( Term* t) const{
  for (std::vector<Shape*>::const_iterator it = shapes_.begin() ; it != shapes_.end() ; it++){
    TermShape *tr = dynamic_cast<TermShape *>(*it);
    if (tr and (tr->getTerm()->getName() == t->getName())) return tr;
  }
  return NULL;
}
void Symbol::add             ( Shape* s){
    shapes_.push_back(s);
}
void Symbol::remove          ( Shape* s){
  for (std::vector<Shape*>::iterator it = shapes_.begin() ; it != shapes_.end() ; it++){
    if ((*it) == s) {
      shapes_.erase(it);
      return;
    }
  }
}
void Symbol::toXml ( std::ostream& os) const {
    os << indent++ << "<symbol>" << std::endl;
    for (std::vector<Shape*>::const_iterator it = shapes_.begin() ; it != shapes_.end() ; it++){
        (*it)->toXml(os);
    }
    os << --indent << "</symbol>" << std::endl;
}
Symbol* Symbol::fromXml ( Cell*c, xmlTextReaderPtr reader){
    //Creation des balises xmlChar 
      const xmlChar* symbolTag      = xmlTextReaderConstString( reader, (const xmlChar*)"symbol" );

        Symbol *s = c->getSymbol(); 

      //Boucle de recuperation des infos de Node
      while(true){
        int status = xmlTextReaderRead(reader);

        if (status != 1) {
          if (status != 0) {
            std::cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << std::endl;
          }
          break;
        }

        const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

        //PASS si commentaire, espace
        switch ( xmlTextReaderNodeType(reader) ) {
          case XML_READER_TYPE_COMMENT:
          case XML_READER_TYPE_WHITESPACE:
          case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
          continue;
        }


        //Si on tombe sur une balise de Node
        if ((xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)){
          if(Shape::fromXml(s,reader)) continue;
        }

        //Si on tombe sur la balise fermante du Net
        if ((nodeName ==  symbolTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT)){
          break;
        }
        //ERREUR
        std::cerr << "[ERROR] Symbol::fromXml(): Unknown or misplaced tag <" << nodeName
        << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
        return NULL;
      } 
      return s;
    }                                    
Symbol::Symbol ( const Symbol& s):shapes_(){
  owner_ = s.getCell();

  for (std::vector<Shape*>::const_iterator it = shapes_.begin() ; it != shapes_.end() ; it++){
    shapes_.push_back(*it);
  }
}


  Symbol& Symbol::operator=       ( const Symbol& sym){
  Symbol *s = new Symbol(sym);
  return *s;

  }
}