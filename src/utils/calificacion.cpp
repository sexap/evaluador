#include "reporte.h"
#include "calificacion.h"
#include "casoPrueba.h"

using namespace seap_implement;

// FUNCIONES DE CALIFICACION

Calificacion::Calificacion(const string& nombreDelUsuario){
    nombreUsuario = nombreDelUsuario;
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

void Calificacion::setCalificacion(const int resultadoFinal){
    calificacion = resultadoFinal;
}

string Calificacion::getNombreUsuario(){
    return nombreUsuario;
}

int Calificacion::getCalificacion(){
    return calificacion;
}

string Calificacion::getVeredicto(){
    return veredicto;
}

list<seap_implement::CasoPrueba> Calificacion::getCasosPrueba(){
    return casosPrueba;
}
