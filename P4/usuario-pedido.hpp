#ifndef USUARIO_PEDIDO_HPP
#define USUARIO_PEDIDO_HPP

#include <map>
#include <set>
#include "usuario.hpp"
#include "pedido.hpp" 

class Pedido;

class Usuario_Pedido //Clase para implementar la asociación
{
    private:
        typedef std::set<Pedido*> Pedidos;
        std::map<Usuario*, Pedidos> UaPs;   //Mapa de Usuario a Pedidos (1 a N)
        std::map<Pedido*, Usuario*> PaU;    //Mapa de Pedido a Usuario (1 a 1)
        
    public:
        //Métodos encargados de hacer las asociaciones
        void asocia(Usuario& u, Pedido& p);
        void asocia(Pedido& p, Usuario& u);
        
        //Metodos encargados de devolver las asociaciones
        Pedidos pedidos(Usuario& u);
        const Usuario* cliente(Pedido& p);
};

#endif