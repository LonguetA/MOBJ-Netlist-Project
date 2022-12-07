#include <exception>
class Error : public std :: exception {
  private :
      std :: string message_ ;
  public :
      Error ( std::string message ) throw () { message_ = message ; };
  public :
      ~Error () throw () {};
  public :
  const char * what () const throw () { return message_ . c_str (); };
};