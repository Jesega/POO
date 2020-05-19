#ifndef CADENA_HPP_
#define CADENA_HPP_

#include <cstring>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;

class Cadena
{
	public:
		//Constructores
		explicit Cadena(size_t t=0, char c=' '); //Si no le pasas nada crea una cadena vacía de tamaño cero.
		Cadena(const Cadena& cad);	//Constructor de copia
		Cadena(const char * cad);	//Constructor a partir de cadena de bajo nivel
		Cadena(Cadena&& cad); //Constructor de movimiento


		//Observadores 
		size_t length() const noexcept {return(tam_);};

		//Conversión explícita
		const char* c_str() const noexcept;	//Conversión a cadena de bajo nivel

		//Operador aritmético de la clase
		void operator +=(const Cadena& cad); //Concatena las cadenas

		//Operadores de asignación
		Cadena& operator=(const Cadena& cad); 	//Operador de asignación a partir de una Cadena 
		Cadena& operator=(const char * cad);	//Operador de asignación a partir de una cadena de bajo nivel
		Cadena& operator=(Cadena&& cad); 		//Operador de asignación con movimiento

		//Operadores de índice
		const char operator [](unsigned int i) const noexcept;  //Entrada 
   		char & operator [](unsigned int i) noexcept; //Entrada y Salida
   		const char at(unsigned int i) const; //Entrada
   		char & at(unsigned int i); //Entrada y Salida

   		//Método para sustraer cadenas
   		Cadena substr(unsigned int i, size_t t) const;//Saca una nueva cadena a partir de la cadena_implícita[i] con t caracteres

		//Destructor
		~Cadena();

		//Tipos de iteradores
		typedef char * iterator;
		typedef const char * const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator; 
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		//Funciones con iteradores 
		iterator begin() noexcept{ return(s_);}
		iterator begin() const noexcept{ return(s_);}
		iterator end() noexcept{ return(s_+tam_);}
		iterator end() const noexcept{ return(s_+tam_);}
		const_iterator cbegin() const noexcept{return const_iterator(begin());}
		const_iterator cend() const noexcept{return const_iterator(end());}
		reverse_iterator rbegin() noexcept{return reverse_iterator(end());}
		reverse_iterator rbegin() const noexcept{return reverse_iterator(end());}
		reverse_iterator rend() noexcept{return reverse_iterator(begin());}
		reverse_iterator rend() const noexcept{return reverse_iterator(begin());}
		const_reverse_iterator crbegin() const noexcept{return const_reverse_iterator(end());}
		const_reverse_iterator crend() const noexcept{return const_reverse_iterator(begin());}

	private:
		char * s_; //Apunta a una cadena de caracteres que acaba con NULL o '\0'. La cadena formada por 'h', 'o', 'l' y 'a' es "hola\0" (ocupa 5 bytes)
		size_t tam_; //Acordarse que s_[tam_]='\0'; "hola\0" tiene tam_=4
};

//Operador aritmético externo
Cadena operator +(const Cadena& a, const Cadena& b); //Concatena cad al final del objeto

//Comparación de cadenas
bool operator ==(const Cadena& a, const Cadena& b) noexcept;	
bool operator !=(const Cadena& a, const Cadena& b) noexcept;
bool operator >(const Cadena& a, const Cadena& b) noexcept;
bool operator >=(const Cadena& a, const Cadena& b) noexcept;
bool operator <(const Cadena& a, const Cadena& b) noexcept;
bool operator <=(const Cadena& a, const Cadena& b) noexcept;

//Operadores de extracción (entrada) e inserción (salida)
std::istream& operator >>(std::istream& is, Cadena& cad); //Lectura 
std::ostream& operator <<(std::ostream& os, const Cadena& cad); //Escritura

// Para P2 y ss.
// Especialización de la plantilla hash<T>para definir la
// función hash a utilizar con contenedores desordenados de
// Cadena, unordered_[set|map|multiset|multimap].
namespace std // Estaremos dentro del espacio de nombres std
{ 
	template <> // Es una especialización de una plantilla para Cadena
		struct hash<Cadena> // Es una clase con solo un operador publico
		{ 
			size_t operator() (const Cadena& cad) const // El operador función
			{
				hash<string> hs; // Creamos un objeto hash de string
				const char* p = cad.c_str(); // Obtenemos la cadena de la Cadena
				string s(p); // Creamos un string desde una cadena
				size_t res = hs(s); // El hash del string. Como hs.operator()(s);
				return res; // Devolvemos el hash del string
			}
		};
}

#endif