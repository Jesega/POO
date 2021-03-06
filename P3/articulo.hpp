#ifndef ARTICULO_HPP_
#define ARTICULO_HPP_

#include <iostream>
#include <iomanip>
#include <set>
#include <map>
#include "cadena.hpp"
#include "fecha.hpp"

using namespace std;

class Autor
{
    private:
        Cadena nombre_;
        Cadena apellidos_;
        Cadena direccion_;
    public:
        //Constructor
        Autor(const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion): nombre_{}, apellidos_{}, direccion_{}{}

        //Observador
        const Cadena& nombre() const{return(nombre_);}
        const Cadena& apellidos() const{return(apellidos_);}
        const Cadena& direccion() const{return(direccion_);}
};

class Articulo //Clase abstracta (nivel 0)
{
    public:
        //Sinónimo
        typedef set<Autor*> Autores;

        //Constructor
        explicit Articulo(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price);

        //Observadoras
        const Cadena referencia() const {return referencia_;}
        const Cadena titulo() const {return titulo_;}
        const Fecha f_publi() const {return f_publi_;}
        const double& precio() const {return precio_;} 
        const Autores& autores() const {return autores_;}

        //Modificadora
        virtual double& precio() = 0;               

        //Excepción
        class Autores_vacios
        {
 
        };

        //TODO: Los mostrar
        virtual void impresion_especifica(ostream& os) const = 0;

    private:
        const Cadena referencia_;
        const Cadena titulo_;
        const Fecha f_publi_;
        double precio_;
        const Autores& autores_;
};

ostream& operator <<(ostream& os, const Articulo& art); //Escritura

class ArticuloAlmacenable: public Articulo //Clase abstracta (nivel 1)
{
    public:
        //Constructor
        ArticuloAlmacenable(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price,int s=0): Articulo(as, ref, tit, f, price), stock_{s}{}

        //Observadores
        int stock() const {return stock_;}  

        //Modificadores 
        int& stock() {return stock_;}

    private:
        int stock_;
};

class Libro: public ArticuloAlmacenable //Clase tangible (nivel 2)
{
    public:
        //Constructor 
        Libro(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price,int n_pag, int s=0): ArticuloAlmacenable(as, ref, tit, f, price, s), n_pag_{n_pag}{}
    
        //Observadores
        int n_pag() const {return n_pag_;}

        //Impresión específica
        void impresion_especifica(ostream& os) const;

    private:
        const int n_pag_;
};

class Cederron: public ArticuloAlmacenable //Clase tangible (nivel 2)
{
    public:
        //Constructor 
        Cederron(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price,int tam, int s=0): ArticuloAlmacenable(as, ref, tit, f, price, s), tam_{tam}{}
    
        //Observador de n_pag
        int tam() const {return tam_;}

        //Impresión específica
        void impresion_especifica(ostream& os) const;

    private:
        const int tam_;
};

class LibroDigital: public Articulo //Clase tangible (nivel 1)
{
    public:
        LibroDigital(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price, const Fecha& f_expir): Articulo(as, ref, tit, f, price), f_expir_{f_expir}{}

        //Observador f_expir_
        const Fecha& f_expir() const {return f_expir_;}

        //Impresión específica
        void impresion_especifica(ostream& os) const;

    private:
        const Fecha f_expir_;
};

#endif