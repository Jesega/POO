#include "pedido.hpp"

int Pedido::n_total_pedidos_{0};

//Constructor
Pedido::Pedido(Usuario_Pedido& UP, Pedido_Articulo& PA, Usuario& U, const Tarjeta& T, const Fecha& f):
numero_{Pedido::n_total_pedidos_ + 1},  tarjeta_{&T}, fecha_{f}, total_{0.0}
{
    //COMPROBACIONES
    if(U.compra().empty())
    {
        Pedido::Vacio fallo{U};
        throw fallo;
    }

    if(&U!=T.titular())
    {
        Pedido::Impostor fallo{U};
        throw fallo;
    }

    if(T.caducidad() <= f)
    {
        Tarjeta::Caducada fallo{T.caducidad()};
        throw fallo;
    }

    if(!T.activa())
    {
        Tarjeta::Desactivada fallo{};
        throw fallo;
    }

    for (Usuario::Articulos::const_iterator i = U.compra().cbegin(); i != U.compra().cend(); i++)
    {
        if(i->first->stock() < i->second) //El stock del artículo debe ser mayor o igual que la cantidad que quiere comprar el usuario
        {
            Pedido::SinStock fallo{*(i->first)};
            const_cast<Usuario::Articulos&>(U.compra()).clear();
            throw fallo;
        }
    }
    //FIN COMPROBACIONES

    double importe_total{0};
    for (Usuario::Articulos::const_iterator i = U.compra().cbegin(); i != U.compra().cend(); i++)
    {
        importe_total += (i->second) * (i->first->precio());
        i->first->stock() -= i->second;
        PA.pedir(*(i->first), *this, i->first->precio(), i->second);
    }

    const_cast<Usuario::Articulos&>(U.compra()).clear(); //Vaciamos el carrito

    total_ = importe_total;

    UP.asocia(*this, U);

    n_total_pedidos_++;
}

std::ostream& operator <<(std::ostream& os, const Pedido& P) //Inserción en flujo de salida
{
    os << "Núm. pedido:\t" << P.numero() << endl;
    os << "Fecha:\t\t" << P.fecha() << endl;
    os << "Pagado con:\t" << P.tarjeta()->tipo()<<"\tnº: \t"<< P.tarjeta()->numero() << endl;
    os << "Importe:\t" << P.total() <<" €"<< endl;
    return(os);
}