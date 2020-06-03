#include "usuario.hpp"

//Constructor
Clave::Clave(const char* cad)
{
    static char caracteres[65]{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/."};
    static std::random_device gen;
    static std::uniform_int_distribution<std::size_t> dist(0, 63);

    if(strlen(cad)<5)
    {
        Clave::Incorrecta fallo{CORTA};
        throw fallo;
    }
    
    int a{};
    char sal[3]{};
    int i{0};
    while(i<2)
    {
        a=dist(gen);
        sal[i] = caracteres[a];
        i++;
    }
    sal[i]='\0';
    
    char * p = crypt(cad, sal); //crypt(cadena en claro, la sal)
    clave_=p;
}

//Método verifica que devuelve true or false si la cadena que le paso es la clave almacenada
bool Clave::verifica(const char * cad) const
{
    char sal[3]{};
    int i;
    for (i = 0; i < 2; i++)
    {
        sal[i]=clave_[i];
    }

    sal[i]='\0';
    
    char * p = crypt(cad, sal);
    if(p==nullptr)
    {
        Clave::Incorrecta fallo{ERROR_CRYPT};
        throw fallo;
    }

    return(strcmp(p, clave_.c_str())==0);
}

//------------------------------------------------------USUARIO---------------------------------------------------------------------

unordered_set<Cadena> Usuario::conjunto_ids; //Como es static lo inicializo aquí

Usuario::Usuario(const Cadena& id, const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion, const Clave& pass): id_{id}, nombre_{nombre}, apellidos_{apellidos}, direccion_{direccion}, pass_{pass}
{
    if(!conjunto_ids.insert(id).second) //Si el id ya existe el segundo dato del par que devuelve el insert será falso
    {
        Usuario::Id_duplicado fail{id};
        throw fail;
    }
}

//Métodos de asociación con tarjeta
void Usuario::es_titular_de(const Tarjeta& t)
{
    if(t.titular()==this)
        tarjetas_[t.numero()]=const_cast<Tarjeta*>(&t);
}

void Usuario::no_es_titular_de(const Tarjeta& t)
{
    tarjetas_.erase(t.numero());
}

//Asociación con artículos
void Usuario::compra(const Articulo& a, unsigned int c)
{
    Articulos::iterator i=this->carrito_.end();
    if (carrito_.find(const_cast<Articulo*>(&a)) != i)
    {
        if(c==0)
        {
            carrito_.erase(const_cast<Articulo*>(&a));
        }
        else
        {
            carrito_[const_cast<Articulo*>(&a)]=c;
        }
    }   
    else
    {
        if(c!=0)
        {
            carrito_.insert( std::pair<Articulo * const, unsigned int>(const_cast<Articulo*>(&a), c));
        }   
    }
}

//Número de elementos en el carrito
size_t Usuario::n_articulos() const noexcept
{
    return (carrito_.size());
}

//El destructor deberá llamar a anula_titular PARA TODAS LAS TARJETAS
Usuario::~Usuario()
{
    Tarjetas::const_iterator i=tarjetas_.cbegin();
    while(i!=tarjetas_.cend())
    {
        i->second->anula_titular();
        i++;
    }  
    conjunto_ids.erase(id_);
    carrito_.clear();
    tarjetas_.clear();
}

std::ostream& operator <<(std::ostream& os, const Usuario& u) //Escritura de Usuario
{
    os << u.id() << ' ' << '[' << u.pass_.clave() << ']' << ' ' << u.nombre() << ' '<<u.apellidos() << endl;
    os << u.direccion() << endl;
    os << "Tarjetas:" <<endl;

    Usuario::Tarjetas::const_iterator i=u.tarjetas().cbegin();
    while(i!=u.tarjetas().cend())
    {
        os << i->second[0];
        i++;
    }  
    return(os);
}

std::ostream& mostrar_carro(std::ostream& os, const Usuario& u) //Escritura del carro
{
    os<<"Carrito de la compra de "<< u.id() << " [Artículos: " << u.n_articulos() << ']'<<endl;

    //Mostramos el carro solo si tiene algún artículo
    if(u.n_articulos()!=0)
    {
        os << "Cant." << '\t' << "Artículo" << endl;

        Usuario::Articulos::const_iterator i=u.compra().cbegin();
        while(i!=u.compra().cend())
        {
            os << i->second<< '\t' << '[' << i->first[0].referencia() << "] \""
            << i->first[0].titulo() << "\", "<< i->first[0].f_publi().anno()<<". "
            <<fixed << setprecision(2) <<i->first[0].precio()<<" €"<< endl;
            i++;
        }  
    }
    return(os);
}

