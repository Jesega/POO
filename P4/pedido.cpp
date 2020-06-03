#include "pedido.hpp"

int Pedido::n_total_pedidos_{0};

//Constructor
Pedido::Pedido(Usuario_Pedido& UP, Pedido_Articulo& PA, Usuario& U, const Tarjeta& T, const Fecha& f):
numero_{Pedido::n_total_pedidos_ + 1},  tarjeta_{&T}, fecha_{f}, total_{0.0}
{
    //COMPROBACIONES

    //¿El carrito a priori está vacío?
    if(U.compra().empty())
    {
        Pedido::Vacio fallo{U};
        throw fallo;
    }

    //¿El usuario es el titular de la tarjeta?
    if(&U!=T.titular())
    {
        Pedido::Impostor fallo{U};
        throw fallo;
    }

    //¿La tarjeta está caducada?
    if(T.caducidad() <= f)
    {
        Tarjeta::Caducada fallo{T.caducidad()};
        throw fallo;
    }

    //¿La tarjeta está activa?
    if(!T.activa())
    {
        Tarjeta::Desactivada fallo{};
        throw fallo;
    }

    //Comprobamos el carro artículo por artículo
    bool PedidoVacio{true};
    Usuario::Articulos Carrito{U.compra()};
    for (Usuario::Articulos::const_iterator i = U.compra().cbegin(); i != U.compra().cend(); i++)
    {
        //El artículo es un libro digital (no tiene stock)
        if(LibroDigital * ld = dynamic_cast<LibroDigital*>(i->first))   
        {
            //Si expira más tarde que hoy, es correcto, por tanto el carrito no está vacío
            if(ld->f_expir() > Fecha()) 
                PedidoVacio=false; 
            else    //Si no, borramos el libro vacío del carrito
                Carrito.erase(i->first);
        }
        else
        {
            //El artículo es un articulo almacenable (tiene stock)
            if(ArticuloAlmacenable * aa = dynamic_cast<ArticuloAlmacenable*>(i->first))     
            {
                //El stock del artículo debe ser mayor o igual que la cantidad que quiere comprar el usuario
                if (aa->stock() >= i->second) 
                    PedidoVacio = false;
                else
                {
                    //Si no, vaciamos el carrito del usuario
                    Pedido::SinStock fallo{*(i->first)};
                    const_cast<Usuario::Articulos&>(U.compra()).clear();
                    throw fallo; 
                }
            }
            else    
            {
                //No se reconoce el tipo de Artículo
                throw logic_error("Pedido: Artículo no reconocido");
            }
        }
    }
    
    //Si el pedido solo contenía Libros Digitales expirados, lanzamos una excepción
    if(PedidoVacio)
    {
        Pedido::Vacio fallo{U};
        throw fallo;
    }
    //FIN COMPROBACIONES

    //Calculamos el importe
    double importe_total{0};
    for (Usuario::Articulos::const_iterator i = Carrito.cbegin(); i != Carrito.cend(); i++)
    {
        //Actualizamos el importe del pedido
        importe_total += (i->second) * (i->first->precio());
        
        //Actualizamos el stock solo si el articulo es almacenable 
        if(ArticuloAlmacenable * aa = dynamic_cast<ArticuloAlmacenable*>(i->first))
            aa->stock() -= i->second;
        
        PA.pedir(*(i->first), *this, i->first->precio(), i->second);
    }

    //Vaciamos el carrito
    const_cast<Usuario::Articulos&>(U.compra()).clear(); 

    //Establecemos el precio del pedido
    total_ = importe_total;

    //Asociamlos el pedido al usuario
    UP.asocia(*this, U);

    //Aumentamos el número total de pedidos
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