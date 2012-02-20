#include "reporte.h"
#include "calificacion.h"
#include "casoPrueba.h"
#include <ctime> //Para obtener la fecha y hora de evaluacion
#include <iostream>

using namespace seap_implement;

Reporte::Reporte(const string& nombreDelArchivo,const string& nombreDelProblema,const string& nombreJuez){
    //Se obtienen la fecha y hora de la evaluación
    char dateNow [10];
    char timeNow [10];
    time_t now = time(NULL);

    strftime(dateNow, 50, "%d-%m-%Y", localtime(&now));
    strftime(timeNow, 50, "%I:%M %p", localtime(&now));

    //inicialización de variables

    nombreProblema = nombreDelProblema;
    juez = nombreJuez;
    nombreArchivo = nombreDelArchivo;
    hora = string (timeNow);
    fecha = string (dateNow);
}

void Reporte::setNombreProblema(const string& nombreDelProblema){
    nombreProblema = nombreDelProblema;
}

void Reporte::setJuez(const string& nombreJuez){
    juez = nombreJuez;
}

string Reporte::getJuez(){
    return juez;
}

string Reporte::getNombreProblema(){
    return nombreProblema;
}

string Reporte::getFecha(){
    return fecha;
}

string Reporte::getHora(){
    return hora;
}

void Reporte::nuevoAlumno(const string& nombreDelUsuario){
    calificacionTemporal = Calificacion(nombreDelUsuario);
}

void Reporte::agregarResultadoCasoPrueba(const int resultado, const double tiempo){
    calificacionTemporal.agregarCasoPrueba(resultado,tiempo);
}

void Reporte::agregarResultadoCasoPrueba(const string& resultado){
    calificacionTemporal.agregarCasoPrueba(resultado);
}

void Reporte::terminarEvaluacionUsuario(const int resultado,const string& veredicto){
    calificacionTemporal.setCalificacion(resultado);
    calificacionTemporal.setVeredicto(veredicto);
    calificaciones.push_back(calificacionTemporal);
}

void Reporte::imprimirResultadoAmigable(){

    list<seap_implement::CasoPrueba> casosPrueba;

    cout << "Nombre del archivo: " << nombreArchivo <<endl;
    cout << "Nombre del problema: " << nombreProblema <<endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Hora: " << hora << endl;
    cout << "Juez: " << juez << endl;

    while (!calificaciones.empty()) {

        calificacionTemporal = calificaciones.front();

        cout << "Nombre Alumno: " << calificacionTemporal.getNombreUsuario() << endl;

        casosPrueba = calificacionTemporal.getCasosPrueba();

        while (!casosPrueba.empty()) {
            cout << "Calificacion: " << casosPrueba.front().getEvaluacionCasoPrueba() << endl;
            casosPrueba.pop_front();
        }

        cout << "Calificacion: " << calificacionTemporal.getCalificacion() << endl;
        cout << "Veredicto: " << calificacionTemporal.getVeredicto() << endl;

        cout << endl;
        calificaciones.pop_front();
    }
}

void Reporte::imprimirResultadoCVS(){

    list<seap_implement::CasoPrueba> casosPrueba;

    cout << "Nombre del archivo: " << nombreArchivo <<endl;
    cout << "Nombre del problema: " << nombreProblema <<endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Hora: " << hora << endl;
    cout << "Juez: " << juez << endl;

    while (!calificaciones.empty()) {

        calificacionTemporal = calificaciones.front();

        cout << "Nombre Alumno: " << calificacionTemporal.getNombreUsuario() << endl;

        casosPrueba = calificacionTemporal.getCasosPrueba();

        while (!casosPrueba.empty()) {
            cout << "Calificacion: " << casosPrueba.front().getEvaluacionCasoPrueba() << endl;
            casosPrueba.pop_front();
        }

        cout << "Calificacion: " << calificacionTemporal.getCalificacion() << endl;
        cout << "Veredicto: " << calificacionTemporal.getVeredicto() << endl;

        cout << endl;
        calificaciones.pop_front();
    }
}
