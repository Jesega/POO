#ifndef ARTICULO_HPP_
#define ARTICULO_HPP_

#include <iostream>
#include "cadena.hpp"
#include "fecha.hpp"

class Articulo 
{
    private:
        const Cadena referencia_;
        const Cadena titulo_;
        const Fecha f_publi_;
        double precio_;
        unsigned int stock_;
    public:
        explicit Articulo(const Cadena& ref, const Cadena& tit, const Fecha& f, double price, unsigned int s=0);
        const Cadena referencia() const {return(referencia_);}
        const Cadena titulo() const {return(titulo_);}
        const Fecha f_publi() const {return(f_publi_);}
        double& precio(){return(precio_);}                  //Modificadora
        const double& precio() const {return(precio_);}     //Consultora
        unsigned int& stock(){return(stock_);}              //Modificadora
        const unsigned int& stock() const {return(stock_);} //Consultora
};

std::ostream& operator <<(std::ostream& os, const Articulo& art); //Escritura

#endif