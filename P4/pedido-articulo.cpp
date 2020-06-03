#include "pedido-articulo.hpp"
using namespace std;


//-----------------------------------------------------OrdenaArticulos-------------------------------------------------------------------------------
bool OrdenaArticulos::operator()(Articulo* A1, Articulo* A2) const
{
    return(A1->referencia() < A2->referencia());
}

//-----------------------------------------------------OrdenaPediods-------------------------------------------------------------------------------
bool OrdenaPedidos::operator()(Pedido* P1, Pedido* P2) const
{
    return(P1->numero() < P2->numero());
}

//-----------------------------------------------------LineaPedido-------------------------------------------------------------------------------
//Inserción en flujo
std::ostream& operator <<(std::ostream& os, const LineaPedido& l)
{
    os<< std::fixed << std::setprecision(2) << l.precio_venta() << " €\t" << l.cantidad();
    return(os);
}

//-----------------------------------------------------Pedido_Articulo-------------------------------------------------------------------------------
//Asociación
void Pedido_Articulo::pedir(Pedido& P, Articulo& A, double precio, unsigned int c)
{
    LineaPedido L(precio,c);
    Ped_to_Arts[&P].insert(std::make_pair(&A,L));
    Arts_to_Peds[&A].insert(std::make_pair(&P, L));
}

void Pedido_Articulo::pedir(Articulo& A, Pedido& P, double precio, unsigned int c)
{
    pedir(P,A,precio, c);
}

const Pedido_Articulo::ItemsPedido& Pedido_Articulo::detalle(Pedido& p)
{
    return(Ped_to_Arts.find(&p)->second);
}

Pedido_Articulo::Pedidos Pedido_Articulo::ventas(Articulo& a)
{
    std::map<Articulo*, Pedidos, OrdenaArticulos>::const_iterator it = Arts_to_Peds.find(&a);
    if(it != Arts_to_Peds.cend())
        return(Arts_to_Peds.find(&a)->second);
    else
        return(Pedidos());
}

std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::ItemsPedido& I)
{
    os<< "PVP\tCantidad\tArtículo"<<endl;
    os << "============================================"<<endl;
    
    double total{0};
    for(Pedido_Articulo::ItemsPedido::const_iterator it = I.cbegin(); it!=I.cend(); it++)
    {
        os<<it->second.precio_venta()<<" €\t"<<it->second.cantidad()<<"\t\t["<<it->first->referencia()<<"] \""<<it->first->titulo()<<'\"'<<endl;
        total+=it->second.precio_venta() * it->second.cantidad();
    }
    os << "============================================"<<endl;
    os << "Total\t"<< total<< " €"<< endl;
    os << endl;
    return(os);
}

std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::Pedidos& P)
{
    os << "[Pedidos: " << P.size() <<"]"<< endl;
    os << "===========================================================================================" << endl;
    os << "PVP\tCantidad\tFecha de venta"<<endl;
    os << "===========================================================================================" << endl;

    double importetotal{0};
    size_t cantidadtotal{0};
    for(Pedido_Articulo::Pedidos::const_iterator it = P.cbegin(); it!=P.cend(); it++)
    {
        os<< it->second.precio_venta()<<" €\t"<<it->second.cantidad()<<"\t\t"<< it->first->fecha();
        importetotal += it->second.precio_venta()*it->second.cantidad();
        cantidadtotal += it->second.cantidad();
        os << endl;
    }
    os << "===========================================================================================" << endl;
    os << fixed<<setprecision(2)<<importetotal <<" €\t"<< cantidadtotal<<endl;
    return(os);
}

void Pedido_Articulo::mostrarDetallePedidos(ostream& os)
{
    double importetotal{0};
    for(std::map<Pedido *, ItemsPedido, OrdenaPedidos>::const_iterator it = Ped_to_Arts.cbegin();it!= Ped_to_Arts.cend(); it++)
    {
        os << "Pedido núm. "<< it->first->numero() << endl;
        os << "Cliente: "<< *(it->first->tarjeta()->titular()) <<"Fecha: "<< it->first->fecha()<< endl;
        os << detalle(*(it->first));
        importetotal += it->first->total();
    }
    os << "===========================================================================================" << endl;
    os << "Total ventas\t"<<importetotal <<" €\t"<<endl;
    os << "===========================================================================================" << endl;
    os << endl;
}

void Pedido_Articulo::mostrarVentasArticulos(ostream& os)
{
    for(std::map<Articulo*, Pedidos, OrdenaArticulos>::const_iterator it = Arts_to_Peds.cbegin(); it!=Arts_to_Peds.cend(); it++)
    {
        os<<"Ventas de [" << it->first->referencia()<< "] \""<< it->first->titulo()<<'\"'<< endl;
        os << ventas(*(it->first)); 
        os << "===========================================================================================" << endl;
        os << endl;
    }
}


