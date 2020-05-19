#include <iostream>
#include <iomanip>
#include "articulo.hpp"

using namespace std;

Articulo::Articulo(const Cadena& ref, const Cadena& tit, const Fecha& f, double price, unsigned int s):
referencia_(ref), titulo_(tit), f_publi_(f), precio_(price), stock_(s) {}

std::ostream& operator <<(std::ostream& os, const Articulo& art)
{
    os << '[' << art.referencia() <<"] "<< '\"' << art.titulo() << '\"' << ", "<< art.f_publi().anno() << ". " << fixed << setprecision(2) <<art.precio()<< " €";
    return(os);
}