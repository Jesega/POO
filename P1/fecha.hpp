#ifndef FECHA_HPP_
#define FECHA_HPP_

#include <iostream>
using namespace std;
class Fecha
{
	public:
		//Constantes públicas y comunes a todos los objetos
		static const int AnnoMinimo=1902;
		static const int AnnoMaximo=2037;

		//Clase Inválida para las excepciones
		class Invalida
		{
			public:
				Invalida(const char * s): cad(s){};
				const char * por_que() const {return(cad);};
			private:
				const char * cad;
		};

		//Constructores
		explicit Fecha(int dd=0, int mm=0, int aa=0); //Constructor para 3, 2, 1 o ningún parámetro (constructor por defecto)
		Fecha(const char* cad); //Constructor de fecha a partir de cadena


		//Operadores de conversión 
		const char* cadena() const; //Conversión a const char* (Cadena de bajo nivel)

		//Observadores
		const int dia() const noexcept {return(dd);}; 
		const int mes() const noexcept {return(mm);};
		const int anno() const noexcept {return(aa);};

		//Operadores aritméticos no simétricos
		Fecha& operator++(); //Preincremento ++f
		Fecha operator++(int); //Postincremento f++
		Fecha& operator--(); //Predecremento --f
		Fecha operator--(int); //Postdecremento f--
		Fecha& operator+=(int dias); //f+=i
		Fecha& operator-=(int dias); //f-=i	
		Fecha operator+(int dias) const; //f+i
		Fecha operator-(int dias) const;  //f-i	

	private:
		int dd, mm, aa;
		void normalizar_fecha();
		void comprobar_fecha() const;
};

//Operadores lógicos simétricos
bool operator<(const Fecha& a, const Fecha& b) noexcept;
bool operator==(const Fecha& a, const Fecha& b) noexcept;
bool operator!=(const Fecha& a, const Fecha& b) noexcept;
bool operator>(const Fecha& a, const Fecha& b) noexcept;
bool operator<=(const Fecha& a, const Fecha& b) noexcept;
bool operator>=(const Fecha& a, const Fecha& b) noexcept;

//Operadores de extracción e inserción
std::istream& operator >>(std::istream& is, Fecha& f); //Lectura (debe tratar las excepciones)
std::ostream& operator <<(std::ostream& os, const Fecha& f); //Escritura

#endif