#include <cctype>
#include <iomanip>
#include "tarjeta.hpp"

using namespace std;

bool luhn(const Cadena& numero);

//---------------------------------------------------------------------NUMERO--------------------------------------------------------------
Numero::Numero(const Cadena& num): num_{num}
{
    char * aux = new char[num.length()+1]; //El +1 para guardar el /0
    size_t i,j{0};
    for (i = 0; i < num.length(); i++)
    {
        if(!isspace(num[i])) //Si es un espacio me salto el caracter
        {
            if(!isdigit(num[i])) //Si no es un dígito, lanzo excepción
            {
                Numero::Incorrecto fail(DIGITOS);
                throw fail;
            }
            aux[j]=num[i];
            j++;
        }
    }
    aux[j]='\0';

    Cadena cad{aux};

    if(cad.length()<13 || cad.length()>19)
    {
        Numero::Incorrecto fail(LONGITUD);
        throw fail;
    }

    if(!luhn(cad))
    {
        Numero::Incorrecto fail(NO_VALIDO);
        throw fail;
    }

    if(cad != num)
        num_=cad;
}

bool operator<(const Numero& a, const Numero& b) noexcept
{
    return(strcmp(a, b) < 0);
}

//-------------------------------------------------------------TARJETA-------------------------------------------------------------------------------------------------------
//Global de números
set<Numero> Tarjeta::conjunto; //Como es static tiene que inicializarse en el cpp

//Constructor
Tarjeta::Tarjeta(const Numero& num, Usuario& user, const Fecha& caducidad): numero_{num}, titular_{&user}, caducidad_{caducidad}, activa_{true}
{
    //Si esta fecha es anterior a la actual, el constructor lanzará la excepción Tarjeta::Caducada.
    Fecha hoy;
    if(caducidad_< hoy)
    {
        Tarjeta::Caducada caducada(caducidad_);
        throw caducada;
    }
    
    //Si el numero insertado ya estaba incluido, sale excepción
    if(!conjunto.insert(num).second)
    {
        Tarjeta::Num_duplicado dup(num);
        throw dup;
    }

    const_cast<Usuario*>(titular_)->es_titular_de(*this);
}

//Observador del tipo
Tarjeta::Tipo Tarjeta::tipo() const noexcept
{
    if(numero()[0]=='3')
    {
        if(numero()[1]=='4' || numero()[1]=='7')
            return(AmericanExpress);
        else
            return(JCB);
    }
    else
    {
        if(numero()[0]=='4')
            return(VISA);
        else
        {
            if(numero()[0]=='5')
                return(Mastercard);
            else
            {
                if(numero()[0]=='6')
                    return(Maestro);
                else
                    return(Otro);
            }
        }
    }
}

//Activar o desactivar
bool Tarjeta::activa(bool a)
{
    activa_=a;
    return(a);
}

//Para cuando se destruya el titular 
void Tarjeta::anula_titular()
{
    titular_=nullptr;
    activa_=false;
}

//Desatructor TODO
Tarjeta::~Tarjeta()
{
    if(titular_!=nullptr)
        const_cast<Usuario*>(titular_)->no_es_titular_de(*this);
    conjunto.erase(numero_);
}

//Comparación de tarjetas
bool operator<(const Tarjeta& a, const Tarjeta& b) noexcept 
{return(strcmp(a.numero(),b.numero()) < 0);}

//Para mostrar la tarjeta
std::ostream& operator <<(std::ostream& os, const Tarjeta& t)
{
    os << t.tipo();
    os << t.numero()<< endl;

    //Ponemos nombre y apellido en mayúscula
    Cadena aux = t.titular()->nombre();
    int i{0};
    while (aux[i]!='\0')
    {
        if(islower(aux[i]))
            aux[i]=toupper(aux[i]);
        i++;
    }
    os<<aux<< " ";

    i=0;
    aux = t.titular()->apellidos();
    while (aux[i]!='\0')
    {
        if(islower(aux[i]))
            aux[i]=toupper(aux[i]);
        i++;
    }
    os<<aux<<endl;

    os << "Caduca: "<< std::setw(2) << std::setfill('0')<< std::setiosflags(std::ios_base::right) << t.caducidad().mes() << '/';
    os << std::setw(2) << std::setfill('0')<< std::setiosflags(std::ios_base::right) << t.caducidad().anno()%100 << endl;
    os<<endl;
    return(os);
}

std::ostream& operator <<(std::ostream& os, const Tarjeta::Tipo& t) //Escritura de tipo
{
    switch (t)
    {
    case 1:
        os<< "VISA" <<endl;
        break;
    case 2:
        os<< "Mastercard" <<endl;
        break;
    case 3:
        os<< "Maestro" <<endl;
        break;
    case 4:
        os<< "JCB" <<endl;
        break;
    case 5:
        os<< "AmericanExpress" <<endl;
        break;
    default:
        os<< "Tipo indeterminado" <<endl;
        break;
    }
    return (os);
}