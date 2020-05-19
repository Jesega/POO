#include <stdexcept>
#include <iostream>
#include <cstring>
#include "cadena.hpp"

using namespace std;

//Constructores
Cadena::Cadena(size_t t, char c): s_{new char[t+1]}, tam_{t} //Constructor por defecto
{
	size_t i=0;
	while(i<t)
	{
		s_[i]=c;
		i++;
	}
	s_[t]='\0';
}

//Constructor de copia
Cadena::Cadena(const Cadena& cad): s_{new char[cad.length()+1]}, tam_{cad.length()}
{
	std::strcpy(s_, cad.c_str());
}

//Constructor a partir de cadena de bajo nivel
Cadena::Cadena(const char * cad): s_{new char[std::strlen(cad)+1]}, tam_{std::strlen(cad)}
{
	std::strcpy(s_, cad);
}

 //Constructor de movimiento
Cadena::Cadena(Cadena&& cad): s_{cad.s_}, tam_{cad.tam_}
{
	cad.s_=nullptr;
	cad.tam_=0;
}

//Conversión explícita
const char* Cadena::c_str () const noexcept
{
	return(s_);
}


//Operador aritmético de la clase
void Cadena::operator +=(const Cadena& cad) //Concatena las cadenas
{
	char * copia= new char[tam_+1];
	strcpy(copia, s_);
	tam_+=cad.tam_;
	delete[] s_;
	s_ = new char[tam_+1];
	strcpy(s_, copia);
	delete[] copia;
	strcat(s_,cad.c_str());
}

//Operador de asignación a partir de otra cadena
Cadena& Cadena::operator=(const Cadena& cad)
{
	if(this!=&cad) //Evitar auto asignación
	{
		delete[] s_;
		s_ = new char[cad.tam_+1];
		tam_ = cad.tam_;
		std::strcpy(s_, cad.s_);
	}
	return(*this);
}

//Operador de asignación a partir de una cadena de bajo nivel
Cadena& Cadena::operator=(const char * cad)
{
	tam_ = strlen(cad);
	delete[] s_;
	s_ = new char[tam_+1];
	std::strcpy(s_, cad);
	return(*this);
}

//Operador de asignación con movimiento
Cadena& Cadena::operator=(Cadena&& cad)
{
	tam_= cad.tam_;
	delete[] s_;
	s_=cad.s_;
	cad.tam_=0;
	cad.s_=nullptr;
	return *this;
} 		

 //Operadores de índice
const char Cadena::operator [](unsigned int i) const noexcept //Entrada
{return *(s_+i);}

char& Cadena::operator [](unsigned int i) noexcept//Entrada y salida
{return *(s_+i) ;}

const char Cadena::at(unsigned int i) const //Entrada
{
	if(i  >= tam_)
	{
		std::out_of_range Error("El índice debe estar comprendido entre cero y el tamaño de la cadena");
		throw Error;
	}
	else
		return(*(s_+i));
}

char & Cadena::at(unsigned int i) //Entrada y Salida
{

	if(i>(tam_-1))
	{
		std::out_of_range Error("El índice debe estar comprendido entre cero y el tamaño de la cadena");
		throw Error;
	}
	else
		return(*(s_+i));
}

//Método para sustraer cadenas
Cadena Cadena::substr(unsigned int i, size_t t) const
{
	//Entra
	if(i>=tam_)
	{
		std::out_of_range Error("El índice debe estar comprendido entre cero y el tamaño de la cadena");
		throw Error;
	}
	else
	{
		if( (i+t) > tam_ || t> tam_) //Cuando t es negativo se evalua como un entero enorme pero se suma como negativo
		{
			std::out_of_range Error("La subcadena pedida se sale de los límites de la cadena original");
			throw Error;
		}
		else
		{
				Cadena cad(t); 
				size_t j{0};
				while(cad[j]!='\0')
				{
					cad[j] = s_[i];
					i++;
					j++;
				}
				return(cad);
			
		}
	}
}

//Destructor
Cadena::~Cadena()
{
	delete[] s_;
}

//Operador aritmético externo
Cadena operator +(const Cadena& a, const Cadena& b) //Concatena cad al final del objeto
{
	Cadena c(a); 
	c += b;
	return(c);
}

/*Comparación de cadenas
int strcmp (const char *p1, const char *p2)
{
	const unsigned char *s1 = (const unsigned char *) p1;
	const unsigned char *s2 = (const unsigned char *) p2;
	 char c1, c2;
	do
    {
		c1 = ( char) *s1++;
		c2 = ( char) *s2++;
    	if (c1 == '\0')
        	return (c1 - c2);
    } while (c1 == c2);
    return (c1 - c2);
}*/

bool operator ==(const Cadena& a, const Cadena& b) noexcept
{
	return(strcmp(a.c_str(),b.c_str()) == 0);
}

bool operator !=(const Cadena& a, const Cadena& b) noexcept
{
	return(!(a==b));
}

bool operator >(const Cadena& a, const  Cadena& b) noexcept
{
	return(b < a);
}

bool operator >=(const Cadena& a, const Cadena& b) noexcept
{
	return(!(a<b));
}

bool operator <(const Cadena& a, const Cadena& b) noexcept
{
	return(strcmp(a.c_str(),b.c_str()) < 0);
}

bool operator <=(const Cadena& a, const Cadena& b) noexcept
{
	return(!(b<a));
}

//Operadores de extracción (entrada) e inserción (salida)
std::istream& operator >>(std::istream& is, Cadena& cad) //Lectura 
{
	char entrada[33]={};
	is.width(33);
	is >> entrada;
	cad = entrada;
	return(is);
}

std::ostream& operator <<(std::ostream& os, const Cadena& cad) //Escritura
{
	os << cad.c_str();
	return(os);
}