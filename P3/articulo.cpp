#include "articulo.hpp"

using namespace std;

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
    os << '[' << art.referencia() <<"] "<< '\"' << art.titulo() << '\"' << ", de ";
    for( Articulo::Autores::const_iterator i = art.autores().cbegin(); i != art.autores().cend(); i++)
    {
        if(++i == art.autores().cend())
            os << (*i)->apellidos() << '.';
        else
            os << (*i)->apellidos() << ',';   
    }
    os<< art.f_publi().anno() << ". " << fixed << setprecision(2) <<art.precio()<< " €" << endl;
    
    os << '\t';
    art.impresion_especifica(os);
    os << endl;
    
    return(os);
}

void Libro::impresion_especifica(ostream& os) const
{
    os << n_pag() << " págs., "<< stock()<< "unidades.";
}

void Cederron::impresion_especifica(ostream& os) const
{
    os <<  tam() << " MB, "<< stock()<< "unidades.";
}

void LibroDigital::impresion_especifica(ostream& os) const
{
    os <<  "A la venta hasta el "<< f_expir()<< '.';
}