#include "usuario-pedido.hpp"

//------------------------------------------------------------------Usuario_Pedido--------------------------------------------------------

//Métodos encargados de hacer las asociaciones
void Usuario_Pedido::asocia(Usuario& u, Pedido& p)
{
    UaPs[&u].insert(&p);
    PaU[&p]=&u;
}

void Usuario_Pedido::asocia(Pedido& p, Usuario& u)
{
    asocia(u,p);
}

//Metodos encargados de devolver las asociaciones
Usuario_Pedido::Pedidos Usuario_Pedido::pedidos(Usuario& u)
{
    std::map<Usuario*, Pedidos>::const_iterator it = UaPs.find(&u);
    if(it != UaPs.end())
        return(it->second);
    else 
        return(std::set<Pedido*>());
}

const Usuario* Usuario_Pedido::cliente(Pedido& p)
{
    std::map<Pedido*, Usuario*>::const_iterator it= PaU.find(&p);
    if(it != PaU.end())
        return(it->second);
    else 
        return(nullptr);
}

