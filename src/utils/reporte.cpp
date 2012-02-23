#include "reporte.h"
#include "calificacion.h"
#include "casoPrueba.h"
#include <ctime> //Para obtener la fecha y hora de evaluacion
#include <fstream>
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

void Reporte::nuevoAlumno(const string& nombreDelUsuario, const string& compilador){
    calificacionTemporal = Calificacion(nombreDelUsuario,compilador);
}

void Reporte::agregarResultadoCasoPrueba(const int resultado, const double tiempo){
    calificacionTemporal.agregarCasoPrueba(resultado,tiempo);
}

void Reporte::agregarResultadoCasoPrueba(const string& resultado){
    calificacionTemporal.agregarCasoPrueba(resultado);
}

void Reporte::terminarEvaluacionUsuario(const int veredictoFinal){
    calificacionTemporal.setVeredicto(veredictoFinal);
    calificaciones.push_back(calificacionTemporal);
}

void Reporte::terminarEvaluacionUsuario(const string& veredictoFinal){
    calificacionTemporal.setVeredicto(veredictoFinal);
    calificaciones.push_back(calificacionTemporal);
}

void Reporte::imprimirResultadoAmigable(){

    seap_implement::CasoPrueba casoPrueba;
    list<seap_implement::CasoPrueba> casosPrueba;
    list<seap_implement::CasoPrueba>::iterator ita;
    list<seap_implement::Calificacion>::iterator it;

    //Archivo de los resultados en csv
    ofstream outputResults((nombreArchivo + ".txt").c_str());

    if (outputResults.fail()) {
        cerr << "No se pudo abrtir el archivo";
    }

    outputResults << "Calificaciones de " << nombreProblema << " con SEAP.\n\n";

    for( it=calificaciones.begin() ; it != calificaciones.end(); it++ ){
        calificacionTemporal = *it;

        outputResults << "Estudiante " << calificacionTemporal.getNombreUsuario();
        outputResults << "\t\tCalificación  " << calificacionTemporal.getVeredicto() << "\t(" << calificacionTemporal.getCompilador() << ")" << endl;
    }
    outputResults << endl << fecha << " " << hora;

    outputResults.close();
}

void Reporte::imprimirResultadoCVS(){

    seap_implement::CasoPrueba casoPrueba;
    list<seap_implement::CasoPrueba> casosPrueba;
    list<seap_implement::CasoPrueba>::iterator ita;
    list<seap_implement::Calificacion>::iterator it;

    //Archivo de los resultados en csv
    ofstream outputResults((nombreArchivo + ".csv").c_str());

    if (outputResults.fail()) {
        cerr << "No se pudo abrtir el archivo";
    }

    for( it=calificaciones.begin() ; it != calificaciones.end(); it++ ){
        calificacionTemporal = *it;

        outputResults << calificacionTemporal.getNombreUsuario() << "," << calificacionTemporal.getVeredicto() << ",";

        casosPrueba = calificacionTemporal.getCasosPrueba();

        for(ita=casosPrueba.begin() ; ita != casosPrueba.end(); ita++){
            casoPrueba = *ita;
            outputResults << casoPrueba.getEvaluacionCasoPrueba() << ",";
        }
        outputResults << endl;
    }
    outputResults.close();
}
