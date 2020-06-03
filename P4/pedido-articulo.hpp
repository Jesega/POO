#ifndef PEDIDO_ARTICULO_HPP
#define PEDIDO_ARTICULO_HPP

#include <iomanip>
#include <iostream>
#include <map>
#include "articulo.hpp"
#include "pedido.hpp"

using namespace std;

class Pedido;

class LineaPedido   //Clase que nace de la asociación
{
    public:
        //Constructor 
        explicit LineaPedido(double p, unsigned int c=1): precio_venta_{p}, cantidad_{c}{}; 

        //Observadores
        double precio_venta() const {return(precio_venta_);}
        unsigned int cantidad() const {return(cantidad_);}

    private:
        double precio_venta_;
        unsigned int cantidad_;
};

std::ostream& operator <<(std::ostream& os, const LineaPedido& l); //Inserción en flujo

struct OrdenaArticulos
{
    bool operator()(Articulo* A1, Articulo* A2) const;
};

struct OrdenaPedidos
{
    bool operator()(Pedido* P1, Pedido* P2) const;
};

class Pedido_Articulo //Clase para implementar la asociación
{
    public:
        typedef std::map<Articulo *, LineaPedido, OrdenaArticulos> ItemsPedido;
        typedef std::map<Pedido*, LineaPedido, OrdenaPedidos> Pedidos;

        //Asociación
        void pedir(Pedido& P, Articulo& A, double precio, unsigned int c=1);
        void pedir(Articulo& A, Pedido& P, double precio, unsigned int c=1);

        //detalle devolverá una referencia constante a ItemsPedido a partir de un pedido que se pasa por referencia
        const ItemsPedido& detalle(Pedido&);

        //ventas recibe un artículo por referencia y devuelve un Pedidos.
        Pedidos ventas(Articulo&);

        //Para mostrar
        void mostrarDetallePedidos(ostream& os);
        void mostrarVentasArticulos(ostream& os);

    private:
        std::map<Pedido *, ItemsPedido, OrdenaPedidos> Ped_to_Arts;   //Mapa de Pedido a Artículos (1 a N)
        std::map<Articulo*, Pedidos, OrdenaArticulos> Arts_to_Peds; //Mapa de Articulo a Pedidos (1 a N)
};

std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::ItemsPedido& I); //Inserción en flujo de salida de ItemsPedido
std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::Pedidos& P); //Inserción en flujo de salida de Pedidos


#endif