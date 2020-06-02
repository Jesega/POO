#ifndef TARJETA_HPP_
#define TARJETA_HPP_

#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>
#include <functional>
#include "iomanip"
#include "fecha.hpp"
#include "cadena.hpp"
#include "usuario.hpp"

class Usuario;

class Numero 
{
    private:
        Cadena num_;
    public:
        Numero(const Cadena& num);//Constructor
        
        //Operador de conversión a cadena de bajo nivel
        operator const char*() const {return(num_.c_str());}

        enum Razon {LONGITUD, DIGITOS, NO_VALIDO};  //Tipo enumerado para Incorrecto
        class Incorrecto    //Clase para excepciones
        {
            private:
                Razon razon_;
            public:
                Incorrecto(Razon r): razon_{r} {} //Constructor
                const Razon razon() const {return(razon_);} //Observador de la razón
        };

        //Predicados
        struct EsBlanco
        {
            bool operator()(char c) const;
        };
        
        struct EsDigito
        {
            bool operator()(char c) const;
        };
        
};

bool operator<(const Numero& a, const Numero& b) noexcept;

class Tarjeta 
{
    private:
        const Numero numero_;
        const Usuario* titular_;
        const Fecha caducidad_;
        bool activa_;
        static set<Numero> conjunto;

        //Para cuando se destruya el titular, solo tienen acceso los Usuario
        void anula_titular();

    public:

        friend class Usuario; 

        //Enumeración pública de tipos de Tarjeta
        enum Tipo{Otro=0, VISA=1, Mastercard=2, Maestro=3, JCB=4, AmericanExpress=5};

        //Constructor
        Tarjeta(const Numero& num, Usuario& user, const Fecha& caducidad);
            /*  TODO:  
                El constructor deberá asociar la tarjeta que se está creando con su Usuario correspondiente,
                llamando a Usuario::es_titular_de sobre su titular.
            */
        //Clase para las excepciones de tarjeta caducada
        class Caducada
        {
            private:
                Fecha caducada_;
            public:
                Caducada(const Fecha& f): caducada_{f}{}
                const Fecha cuando() const{return(caducada_);}
        };
        
        //Clase para las excepciones de numero de tarjeta repetido
        class Num_duplicado
        {
            private:
                Numero duplicado_;
            public:
                Num_duplicado(const Numero& dup): duplicado_{dup}{}
                const Numero que() const{return(duplicado_);}
        };

        //Constructor de copia anulado
        Tarjeta(const Tarjeta&) = delete;

        //Operador de asignación anulado
        Tarjeta& operator=(const Tarjeta& cad) = delete;

        //Observadores
        const Numero& numero() const noexcept{return(numero_);}
        const Usuario* titular() const noexcept{return(titular_);}
        const Fecha& caducidad() const noexcept{return(caducidad_);}
        bool activa() const noexcept{return(activa_);}
        Tipo tipo() const noexcept;

        //Activar o deszactivar tarjeta
        bool activa(bool a=true);

        //Clase de excepción Desactivada
        class Desactivada
        {
            //Vacía, para uso posterior en otros casos de uso.
        };

        //Destructor
        ~Tarjeta();
};

bool operator<(const Tarjeta& a, const Tarjeta& b) noexcept;

std::ostream& operator <<(std::ostream& os, const Tarjeta& t); //Escritura de tarjeta
std::ostream& operator <<(std::ostream& os, const Tarjeta::Tipo& t); //Escritura de tipo

#endif