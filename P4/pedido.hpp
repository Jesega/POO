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
        size_t numero() const noexcept{return(numero_);}
        const Tarjeta* tarjeta() const noexcept{return tarjeta_;}
        const Fecha& fecha() const noexcept{return(fecha_);}
        double total() const noexcept{return total_;}
        static int n_total_pedidos() noexcept{return n_total_pedidos_;}
            

        //Clases para las excepciones 
        class Vacio
        {
            private:
                const Usuario* u_;
            public:
                Vacio(const Usuario& u) noexcept : u_{&u}{}
                const Usuario& usuario() const noexcept {return(*u_);};
        };

        class Impostor
        {
            private:
                const Usuario* u_;
            public:
                Impostor(const Usuario& u) noexcept: u_{&u}{}
                const Usuario& usuario() const noexcept{return(*u_);};
        };

        class SinStock
        {
            private:
                const Articulo* art_;
            public:
                SinStock(const Articulo& art) noexcept:  art_{&art} {}
                const Articulo& articulo() const noexcept{return(*art_);};
        };

    private:
        int numero_; //Correlativo, va cogiendo el n_total_pedidos
        const Tarjeta* tarjeta_;
        Fecha fecha_;
        double total_; //Importe total del Pedido
        static int n_total_pedidos_; //Es el número del último pedido realizado o la cantidad de pedidos hechos
};

std::ostream& operator <<(std::ostream& os, const Pedido& P) noexcept; //Inserción en flujo de salida

#endif