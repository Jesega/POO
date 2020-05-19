#include <iostream>
#include <iomanip>
#include "cadena.hpp"
#include "fecha.hpp"
#include "articulo.hpp"
#include "tarjeta.hpp"
#include "usuario.hpp"

using namespace std;

int main()
{
  Usuario sabacio("sabacio", "Sabacio", "Garibay Menchaca", "Plaza Mayor, 1 (Hinojosa de Duero)", "vaeg0Quo");
  Articulo poo("100", "Programación Orientada a Objetos", "01/08/2000", 32.50, 5);
  cout << '[' << poo.referencia() <<"] "<< '\"' << poo.titulo() << '\"' << ", "<< poo.f_publi().anno() 
  << ". " << fixed << setprecision(2) << poo.precio()<< " €"<< endl;

  sabacio.compra(poo, 2);
  mostrar_carro(cout, sabacio);
  return(0);
}

   