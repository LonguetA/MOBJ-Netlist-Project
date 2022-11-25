#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "XmlUtil.h"
#include <string>

namespace Netlist {

    //-----------------------TOXML------------------------------//
    void  Term::toXml ( std::ostream& stream ) const {
      stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_) << "\" x=\"" << getPosition().getX() << "\" y=\""<< getPosition().getY() <<"\"/>" << std::endl;
    }

    //-----------------------CTOR------------------------------//
    Term::Term ( Cell*  c  , const std::string& name, Direction d )
                :owner_(c),
                name_(name),
                direction_(d),
                type_(External),
                net_(),
                node_(this,Node::noid){

                  //Ajout du term à la cell
                  c->add(this);
                }

    
    Term::Term ( Instance* i, const Term* modelTerm )
                :owner_(i),
                name_(modelTerm->name_),
                direction_(modelTerm->direction_),
                type_(Internal),
                net_(),
                node_(this,Node::noid){

                  //Ajout du term à l'instance
                  i->add(this);
                }

    //-----------------------DTOR------------------------------//
    Term::~Term (){
      //Deconnection du term
      if (net_ != NULL) setNet(NULL);

      //Retrait du term à l'instance ou à la cell en fonction de son type
      if (isInternal()){
        Instance *ins = static_cast<Instance*> (owner_);
        ins->remove(this);
      }
      else{
        Cell *cell = static_cast<Cell*> (owner_);
        cell->remove(this);
      }
    }

    //-----------------------TOSTRING------------------------------//
    std::string  Term::toString    ( Type t){
      if (t == External)return "External";
      return "Internal";
    }
    std::string  Term::toString    ( Direction d ){
      switch (d)
      {
      case In:
        return "In";
      case Out:
        return "Out";
      case Inout:
        return "Inout";
      case Tristate:
        return "Tristate";
      case Transcv:
        return "Transcv";
      default:
        return "Unknown";
      }
    }
    Term::Direction    Term::toDirection ( std::string str){
      if (str.compare("In") == 0)return In;
      if (str.compare("Out") == 0)return Out;
      if (str.compare("Inout") == 0)return Inout;
      if (str.compare("Tristate") == 0)return Tristate;
      if (str.compare("Transcv") == 0)return Transcv;
      return Unknown;
    }

    Term::Type Term::toType(std::string str){
      if (str.compare("Internal") == 0) return Internal;
      return External;
    }


    //-----------------------GETTEUR------------------------------//
    Cell*              Term::getOwnerCell () const{
      if (isExternal()){
        return static_cast<Cell *>(owner_);
      }
      return static_cast<Instance *>(owner_)->getCell();
    }


    //-----------------------SETTEUR------------------------------//
    void  Term::setNet       ( Net* n){

      //Si on doit deconnecter
      if (n == NULL){
        //Insertion de Null dans la case du tableau de node de net correspondant au pointeur vers le node de notre Term
        std::vector<Node *>vec = net_->getNodes();
        vec.insert(vec.begin() + node_.getId(),NULL);

        //Deconnection du net
        net_ = NULL;
      }

      //Sinon
      else{

        if (n->getCell() != getOwnerCell()){
          std::cerr << "ERROR" << std::endl;
          return;
        }

        net_ = n;
        n->add(&node_);
      }

    }

    void  Term::setNet       ( const std::string& str ){

      //Recuperation du node correspondant 
      setNet(getOwnerCell()->getNet(str));
    }

    void  Term::setPosition  ( const Point& p){
      node_.setPosition(p);
    }

    void  Term::setPosition  ( int x, int y ){
      node_.setPosition(x,y);
    }

    //------------FROM XML-----------//
    Term *Term::fromXml(Cell* c, xmlTextReaderPtr reader){

      //Recuperation des parametres
      std::string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
      std::string dirName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" ) );
      std::string xName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
      std::string yName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );

      //Si les parametres sont vides 
      if (termName.empty() or dirName.empty() or xName.empty() or yName.empty()){
        std::cerr << "[ERROR] Term::fromXml(): Missing parameter" << std::endl;
        return NULL;
      }

      //Creation du Term
      Term *t = new Term(c,termName,toDirection(dirName));
      t->setPosition(atoi(xName.c_str()),atoi(yName.c_str()));

      return t;
    }
}
