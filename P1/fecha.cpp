#include <iostream>
#include <ctime>
#include <locale>
#include <cstdio>
#include "fecha.hpp"

using namespace std;

//Métodos privados
void Fecha::comprobar_fecha() const
{
	static int diadelmes[12]={31,28,31,30,31,30,31,31,30,31,30,31};

	if(aa<Fecha::AnnoMinimo||aa>Fecha::AnnoMaximo)
	{
		Fecha::Invalida a_inv("El anno debe estar comprendido entre  1902 y 2037");
		throw a_inv;
	}

	bool bisiesto=false;
	if(aa % 4 == 0 && (aa % 400 == 0 || aa % 100 != 0))
		bisiesto=true; 

	if(mm<1||mm>12)
	{
		Fecha::Invalida m_inv("El mes debe estar comprendido entre 1 y 12");
		throw m_inv;
	}
	
	if(dd<1||dd > diadelmes[mm-1])
	{
		if(!bisiesto)
		{
			Fecha::Invalida d_inv("El dia debe corresponder al numero de dias del mes");
			throw d_inv;
		}
		else
		{
			if(mm!=2)
			{
				Fecha::Invalida d_inv("El dia debe corresponder al numero de dias del mes");
				throw d_inv;
			}
			else
			{
				if(dd!=29)
				{
					Fecha::Invalida d_inv("El dia debe corresponder al numero de dias del mes");
					throw d_inv;
				}
			}
		}
	}
}

void Fecha::normalizar_fecha() //32 de marzo --> 1 de abril
{
	std::tm tmstruct{0};
	tmstruct.tm_mday= dd;
	tmstruct.tm_mon= mm-1;
	tmstruct.tm_year= aa-1900;

	std::mktime(&tmstruct);

	dd=tmstruct.tm_mday;
	mm=tmstruct.tm_mon+1;
	aa=tmstruct.tm_year+1900;

}

//Constructores
Fecha::Fecha(int d, int m, int a): dd(d), mm(m), aa(a) //d, m y a valen 0 por omisión
{
	std::time_t rawtime = std::time(nullptr);
	std::tm * tm_pointer=std::localtime(&rawtime);

	if(a==0)
		aa= tm_pointer->tm_year + 1900;	//Porque tm_year son los años que han pasado desde 1900
	if(m==0)
		mm= tm_pointer->tm_mon + 1; //Porque enero es 0 y diciembre 11 
	if(d==0)	
		dd= tm_pointer->tm_mday;
				
		
	

	//Comprobaciones
	comprobar_fecha();
}

Fecha::Fecha(const char * cad)	//Constructor a partir de una cadena
{
	int control= 0;
	control = sscanf(cad, "%d/%d/%d",&dd,&mm,&aa);	
	if(control != 3)
	{
		Fecha::Invalida form_inv("La cadena es incomprensible");
		throw form_inv;
	}
	Fecha temp(dd,mm,aa);
	dd=temp.dd;
	mm=temp.mm;
	aa=temp.aa;
}

//Operadores de conversión
const char* Fecha::cadena() const//Conversión a const char* (Cadena de bajo nivel)
{
	std::locale::global(std::locale("es_ES.UTF-8"));

	std::tm tmstruct{0};
	tmstruct.tm_mday= dd;
	tmstruct.tm_mon= mm-1;
	tmstruct.tm_year= aa-1900;

	std::mktime(&tmstruct);

	static char cad[sizeof("miércoles 23 de septiembre de 2020")]{'\0'};
	strftime(cad,sizeof(cad),"%A %e de %B de %Y",&tmstruct);

	return(cad);
}

//Operadores aritméticos

Fecha& Fecha::operator +=(int dias) //Haremos todos los operadores a partir de este. Como se normaliza y comprueba la fecha aquí, no hay que hacerlo en los demás
{
	dd += dias;
	normalizar_fecha();
	comprobar_fecha();
	return (*this);
}

Fecha& Fecha::operator++() //Preincremento ++f
{
	*this += 1;
	return(*this);
}

Fecha Fecha::operator++(int) //Postincremento f++
{
	Fecha f= *this;
	*this += 1;
	return(f);
}

Fecha& Fecha::operator--() //Predecremento --f
{
	*this += -1;
	return(*this);
}

Fecha Fecha::operator--(int) //Posstdecremento f--
{
	Fecha f = *this;
	*this += -1;
	return(f);
}

Fecha Fecha::operator+(int dias) const
{
	Fecha t= *this;
	t += dias;
	return(t);
}

Fecha Fecha::operator-(int dias) const
{
	Fecha t= *this;
	t += -dias;
	return(t);
}

Fecha& Fecha::operator-=(int dias) //f-=i
{
	*this += -dias;
	return(*this);
}


//Operadores lógicos
bool operator<(const Fecha& a, const Fecha& b) noexcept
{
	if (a.anno()<b.anno())
	{
		return(true);
	}
	else
	{
		if (a.anno()>b.anno())
		{
			return(false);
		}
		else
		{
			if (a.mes()<b.mes())
			{
				return(true);
			}
			else
			{
				if (a.mes()>b.mes())
				{
					return(false);
				}
				else
				{
					if (a.dia()<b.dia())
					{
						return(true);
					}
					else
					{
						return(false);
					}
				}
			}
		}
	}
}

bool operator==(const Fecha& a, const Fecha& b) noexcept
{
	if(a.dia()==b.dia() && a.mes()==b.mes() && a.anno()==b.anno()) 
		return(true);
	else
		return(false);
}

bool operator!=(const Fecha& a, const Fecha& b) noexcept
{
	return(!(a==b));
}

bool operator>(const Fecha& a, const Fecha& b) noexcept
{
	return(b<a);
}

bool operator<=(const Fecha& a, const Fecha& b) noexcept
{
	return(!(b<a));
}

bool operator>=(const Fecha& a, const Fecha& b) noexcept
{
	return(!(a<b));
}

//Operadores de extracción e inserción
std::istream& operator >>(std::istream& is, Fecha& f) //Lectura (debe tratar las excepciones)
{
	char* cadena = new char[11];
	is.width(11);
	is >> cadena;
	try{
		f = Fecha(cadena);
	}catch(Fecha::Invalida e){
		is.setstate(std::ios::failbit); 
		throw Fecha::Invalida("Se ha producido un error en el operator >>");
	}
	delete[] cadena;
	return(is);
}

std::ostream& operator <<(std::ostream& os, const Fecha& f) //Escritura
{
	os << f.cadena();
	return (os);
}