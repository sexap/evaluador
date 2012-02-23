#include "reporte.h"
#include "calificacion.h"
#include "casoPrueba.h"

using namespace seap_implement;

// FUNCIONES DE CALIFICACION

Calificacion::Calificacion(const string& nombreDelUsuario, const string& compiler){
    nombreUsuario = nombreDelUsuario;
    compilador = compiler;
    casosPrueba.clear();
}

void Calificacion::setNombreAlumno(const string& nombreDelUsuario){
    nombreUsuario = nombreDelUsuario;
}

void Calificacion::agregarCasoPrueba(const int resultado, const double tiempo){
    casosPrueba.push_back(CasoPrueba(resultado,tiempo));
}

void Calificacion::agregarCasoPrueba(const string& resultado){
    casosPrueba.push_back(CasoPrueba(resultado));
}

void Calificacion::setVeredicto(const string& veredictoFinal){
    veredicto = veredictoFinal;
}

void Calificacion::setVeredicto(const int veredictoFinal){
    veredicto = toString(veredictoFinal);
}

void Calificacion::setCompilador(const string& compiladorUsado){
    compilador = compiladorUsado;
}

string Calificacion::getNombreUsuario(){
    return nombreUsuario;
}

string Calificacion::getCompilador(){
    return compilador;
}

string Calificacion::getVeredicto(){
    return veredicto;
}

list<seap_implement::CasoPrueba> Calificacion::getCasosPrueba(){
    return casosPrueba;
}

string Calificacion::toString(const double numero){
    ostringstream strs;
    string str;
    strs.str("");
    strs << numero;
    return strs.str();
}

string Calificacion::toString(const int numero){
    ostringstream strs;
    string str;
    strs.str("");
    strs << numero;
    return strs.str();
}
