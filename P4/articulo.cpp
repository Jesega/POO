#include "articulo.hpp"

using namespace std;

//-----------------------------------------------------Auor-------------------------------------------------------------------------------
Autor::Autor(const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion): 
nombre_{nombre}, apellidos_{apellidos}, direccion_{direccion}{}

//-----------------------------------------------------Articulo-------------------------------------------------------------------------------
Articulo::Articulo(const Articulo::Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price):
autores_{as}, referencia_(ref), titulo_(tit), f_publi_(f), precio_(price)
{
    if (as.empty())
    {
        Articulo::Autores_vacios fallo{};
        throw fallo;
    }
}

std::ostream& operator <<(std::ostream& os, const Articulo& art)
{
    //Imprimimos la referencia y el título
    os << '[' << art.referencia() <<"] "<< '\"' << art.titulo() << '\"' << ", de ";

    //Imprimimos los autores
    Articulo::Autores::const_iterator i = art.autores().cbegin();
    while(i != art.autores().cend())
    {
        os<<(*i)->apellidos();
        i++;
        if(i == art.autores().cend()) //Si es el último autor ponemos un punto
            os << ". ";
        else //Si no, ponemos una coma
            os << ", ";
    }

    //Imprimimos el año de publicación y el precio
    os<< art.f_publi().anno() << ". " << fixed << setprecision(2) <<art.precio()<< " €" << endl;
    
    //Impresion específica de cada clase
    os << '\t';
    art.impresion_especifica(os);
    return(os);
}

//-----------------------------------------------------ArticuloAlmacenable-------------------------------------------------------------------------------
ArticuloAlmacenable::ArticuloAlmacenable(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price,int s): 
Articulo(as, ref, tit, f, price), stock_{s}{}


//-----------------------------------------------------Libro-------------------------------------------------------------------------------
Libro::Libro(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price,int n_pag, int s): 
ArticuloAlmacenable(as, ref, tit, f, price, s), n_pag_{n_pag}{}

void Libro::impresion_especifica(ostream& os) const
{
    os << n_pag_ << " págs., "<< stock_<<  " unidades.";
}

//-----------------------------------------------------Cederron-------------------------------------------------------------------------------
Cederron::Cederron(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price,int tam, int s): 
ArticuloAlmacenable(as, ref, tit, f, price, s), tam_{tam}{}

void Cederron::impresion_especifica(ostream& os) const
{
    os <<  tam_ << " MB, "<< stock_<< " unidades.";
}

//-----------------------------------------------------LibroDigital-------------------------------------------------------------------------------
LibroDigital::LibroDigital(const Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price, const Fecha& f_expir): 
Articulo(as, ref, tit, f, price), f_expir_{f_expir}{}

void LibroDigital::impresion_especifica(ostream& os) const
{
    os <<  "A la venta hasta el "<< f_expir_ <<'.';
}