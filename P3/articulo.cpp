#include "articulo.hpp"

using namespace std;

Articulo::Articulo(const Autor::Autores& as, const Cadena& ref, const Cadena& tit, const Fecha& f, double price):
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
    os << '[' << art.referencia() <<"] "<< '\"' << art.titulo() << '\"' << ", "<< art.f_publi().anno() << ". " << fixed << setprecision(2) <<art.precio()<< " €";
    return(os);
}