#include "tarjeta.hpp"

using namespace std;

bool luhn(const Cadena& numero);

bool Numero::EsBlanco::operator()(char c) const {return(isspace(c));}

bool Numero::EsDigito::operator()(char c) const {return(isdigit(c));}

//---------------------------------------------------------------------Numero--------------------------------------------------------------
Numero::Numero(const Cadena& num): num_{num}
{
    Cadena aux{num};
    
    Cadena::iterator it = remove_if(aux.begin(), aux.end(), Numero::EsBlanco());
    if (it!=aux.end()) //Si se ha quitado algún espacio
    {
        *it='\0';   //Se marca el nuevo final de la cadena
        aux = Cadena(aux.c_str());  //Actualizamos el tamaño de la cadena
    }

    //Comprobaciones
    not_fn<Numero::EsDigito>(EsDigito());
    Cadena::const_iterator i = find_if(aux.begin(), aux.end(), not_fn<Numero::EsDigito>(EsDigito()));
    if(i!=aux.cend())
    {
        Numero::Incorrecto fail(DIGITOS);
        throw fail;
    }

    if(aux.length()<13 || aux.length()>19)
    {
        Numero::Incorrecto fail(LONGITUD);
        throw fail;
    }

    if(!luhn(aux))
    {
        Numero::Incorrecto fail(NO_VALIDO);
        throw fail;
    }
    //Fin comprobaciones

    if(aux != num)
        num_=aux;
}

bool operator<(const Numero& a, const Numero& b)
{
    return(strcmp(a, b) < 0);
}

//-------------------------------------------------------------Tarjeta-------------------------------------------------------------------------------------------------------
//Global de números
set<Numero> Tarjeta::conjunto; //Como es static tiene que inicializarse en el cpp

//Constructor
Tarjeta::Tarjeta(const Numero& num, Usuario& user, const Fecha& caducidad): numero_{num}, titular_{&user}, caducidad_{caducidad}, activa_{true}
{
    //Si esta fecha es anterior a la actual, el constructor lanzará la excepción Tarjeta::Caducada.
    if(caducidad_< Fecha())
    {
        Tarjeta::Caducada caducada(caducidad_);
        throw caducada;
    }
    
    //Si el numero insertado ya estaba incluido, salta excepción
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

//Desatructor
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
    os << t.tipo() << endl;
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
//Para mostrar el tipo
std::ostream& operator <<(std::ostream& os, const Tarjeta::Tipo& t) //Escritura de tipo
{
    switch (t)
    {
    case 1:
        os<< "VISA";
        break;
    case 2:
        os<< "Mastercard";
        break;
    case 3:
        os<< "Maestro";
        break;
    case 4:
        os<< "JCB";
        break;
    case 5:
        os<< "AmericanExpress";
        break;
    default:
        os<< "Tipo indeterminado";
        break;
    }
    return (os);
}