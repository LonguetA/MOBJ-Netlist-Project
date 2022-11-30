#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "Shape.h"

namespace Netlist {

    //------------TOXML-----------//
    void  Instance::toXml ( std::ostream& stream ) const{
      stream << indent << "<instance name=\"" << name_ << "\" mastercell=\"" << masterCell_->getName() <<"\" x=\""<< position_.getX() << "\" y=\""<< position_.getY() << "\"/>" << std::endl;
    }

    //------------FROMXML-----------//
    Instance *Instance::fromXml(Cell* c, xmlTextReaderPtr reader){ 
      
      //Recuperation des parametres
      std::string instName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
      std::string masterName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell" ) );
      std::string xName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
      std::string yName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );

      //Si les parametres sont vides 
      if (instName.empty() or masterName.empty() or xName.empty() or yName.empty()){
        std::cerr << "[ERROR] Instance::fromXml(): Missing parameter" << std::endl;
        return NULL;
      }

      //Creation de l instance
      Instance *ins = new Instance(c,Cell::find(masterName),instName);
      ins->setPosition(atoi(xName.c_str()),atoi(yName.c_str()));

      return ins;

    }

    //------------CTOR-----------//
    Instance::Instance      ( Cell* owner, Cell* model, const std::string& name)
                            :owner_(owner),
                            masterCell_(model),
                            name_(name),
                            terms_(),
                            position_()
                            {   
                              //Recopie du vecteur de term avec le constructeur de term avec instance
                              for (std::vector<Term *>::const_iterator it = (model->getTerms()).begin() ; it != (model->getTerms()).end() ; it++){
                                new Term(this,*it);
                              }

                              //Ajout de l'instance à la cell
                              owner->add(this);
                            }
    
    //------------DTOR-----------//
    Instance::~Instance      (){
      while ( not terms_     .empty() ) delete *terms_     .begin();

      //Retrait de l'instance à la cell
      owner_->remove(this);
    }

    //------------GETTEUR-----------//
    const std::string&        Instance::getName       () const{
      return name_;
    }
    Cell*                     Instance::getMasterCell () const{
      return masterCell_;
    }
    Cell*                     Instance::getCell       () const{
      return owner_;
    }
    const std::vector<Term*>& Instance::getTerms      () const{
      return terms_;
    }
    Term*                     Instance::getTerm       ( const std::string& str) const{

      //On recherche le term dans notre liste de term
      for (std::vector<Term *>::const_iterator it = terms_.begin() ; it != terms_.end() ; it++){

          //Si on trouve le term avec le bon nom
          if ((*it)->getName() == str){
              return (*it);
          }
      }

      //Si aucun term avec ce nom
      return NULL;
    }
    
    Point                     Instance::getPosition   () const{
      return position_;
    }

    //------------FONCTION DE GESTION-----------//
    bool  Instance::connect       ( const std::string& name, Net* n){
        
      //On recupere le term
      Term * t = getTerm(name);

      if (t == NULL) return false;
      
      t->setNet(n);
      return true;

    }

    void  Instance::add           ( Term* t){
      terms_.push_back(t);
    }


    void  Instance::remove        ( Term* t){
      for (std::vector<Term *>::iterator it = terms_.begin() ; it != terms_.end() ; it++){
          if (*it == t){
              terms_.erase(it);
              break;
          }
      }
    }

    //------------SETTEUR-----------//
    void  Instance::setPosition   ( const Point& p){
      position_ = p;

      for (std::vector<Term *>::iterator it = terms_.begin() ; it != terms_.end() ; it++){
        TermShape *tr = getMasterCell()->getSymbol()->getTermShape((*it));
        int x = tr->getX();
        int y = tr->getY();
        (*it)->setPosition(x + position_.getX(), y + position_.getY());
      }
    }
    void  Instance::setPosition   ( int x, int y ){
      setPosition(Point(x,y));
    }

}