#ifndef PEDIDO_HPP
#define PEDIDO_HPP

#include <iostream>
#include "usuario.hpp"
#include "articulo.hpp"
#include "usuario-pedido.hpp"
#include "pedido-articulo.hpp"

class Pedido_Articulo;
class Usuario_Pedido;

class Pedido
{
    public:
        //Constructor
        Pedido(Usuario_Pedido& UP, Pedido_Articulo& PA, Usuario& U, const Tarjeta& T, const Fecha& f=Fecha());

        //Observadores
        size_t numero() const {return(numero_);}
        const Tarjeta* tarjeta() const {return tarjeta_;}
        const Fecha& fecha() const {return(fecha_);}
        double total() const {return total_;}
        static int n_total_pedidos(){return n_total_pedidos_;}
            

        //Clases para las excepciones 
        class Vacio
        {
            private:
                const Usuario* u_;
            public:
                Vacio(const Usuario& u): u_{&u}{}
                const Usuario& usuario() const {return(*u_);};
        };

        class Impostor
        {
            private:
                const Usuario* u_;
            public:
                Impostor(const Usuario& u): u_{&u}{}
                const Usuario& usuario() const {return(*u_);};
        };

        class SinStock
        {
            private:
                const Articulo* art_;
            public:
                SinStock(const Articulo& art): art_{&art} {}
                const Articulo& articulo() const {return(*art_);};
        };

    private:
        int numero_; //Correlativo, va cogiendo el n_total_pedidos
        const Tarjeta* tarjeta_;
        Fecha fecha_;
        double total_; //Importe total del Pedido
        static int n_total_pedidos_; //Es el número del último pedido realizado o la cantidad de pedidos hechos
};

std::ostream& operator <<(std::ostream& os, const Pedido& P); //Inserción en flujo de salida

#endif