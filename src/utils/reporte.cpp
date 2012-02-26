#include "reporte.h"
#include "calificacion.h"
#include "casoPrueba.h"
#include "string_manip.h"
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

Reporte::Reporte(const string& nombreDelArchivo,const string& nombreDelProblema,const string& nombreJuez, const list<string> testCasesList){
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
    testCases = testCasesList;
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

void Reporte::imprimirResultadoCSV(){

    seap_implement::CasoPrueba casoPrueba;
    list<seap_implement::CasoPrueba> casosPrueba;
    list<seap_implement::CasoPrueba>::iterator ita;
    list<seap_implement::Calificacion>::iterator it;

    //Archivo de los resultados en csv
    ofstream outputResults((nombreArchivo + ".csv").c_str());

    if (outputResults.fail()) {
        cerr << "No se pudo abrtir el archivo";
    }

    outputResults << "Alumno,";

    for(list<string>::iterator itTC = testCases.begin(); itTC != testCases.end(); itTC++){
        outputResults << getFileName((*itTC).c_str()) << "," ;
    }

    outputResults << "Calificación" << endl;

    for( it=calificaciones.begin() ; it != calificaciones.end(); it++ ){

        calificacionTemporal = *it;

        outputResults << calificacionTemporal.getNombreUsuario() << ",";

        casosPrueba = calificacionTemporal.getCasosPrueba();

        for(ita=casosPrueba.begin() ; ita != casosPrueba.end(); ita++){
            casoPrueba = *ita;
            outputResults << casoPrueba.getEvaluacionCasoPrueba() << ",";
        }

        outputResults << calificacionTemporal.getVeredicto() << endl;
    }
    outputResults.close();
}

void Reporte::imprimirResultadoHTML(){

    seap_implement::CasoPrueba casoPrueba;
    list<seap_implement::CasoPrueba> casosPrueba;
    list<seap_implement::CasoPrueba>::iterator ita;
    list<seap_implement::Calificacion>::iterator it;
    list<string> observaciones;


    //Archivo de los resultados en csv
    ofstream outputResults((nombreArchivo + ".html").c_str());

    if (outputResults.fail()) {
        cerr << "No se pudo abrtir el archivo";
    }

    outputResults << "<!DOCTYPE html>" << endl;
    outputResults << "<html lang=\"es\">" << endl;
    outputResults << "\t<head>" << endl;
    outputResults << "\t\t<meta http-equiv=\"Content-Type\" content=\"text/html charset=utf-8\">" << endl;
    outputResults << "\t\t<title>" << "Calificaciones de " << nombreProblema << " con SEAP"<< "</title>" << endl;
    outputResults << "\t\t<link href=\"http://rafaelvargas.me/seap/css/bootstrap.min.css\" rel=\"stylesheet\">" << endl;
    outputResults << "\t</head>" << endl << endl;
    outputResults << "\t<body>" << endl;
    outputResults << "\t\t<h2>Calificaciones de " << nombreProblema << " con SEAP.</h2>" << endl;
    outputResults << "\t\t<br>" << endl;
    outputResults << "\t\t<br>" << endl;
    outputResults << "\t\t<div class=\"span9\">" << endl;
    outputResults << "\t\t\t<table class=\"table table-striped table-bordered table-condensed\">" << endl;
    outputResults << "\t\t\t\t<thead>" << endl;
    outputResults << "\t\t\t\t\t<tr>" << endl;
    outputResults << "\t\t\t\t\t\t<th>Alumno</th>" << endl;
    outputResults << "\t\t\t\t\t\t<th>Compilador</th>" << endl;
    outputResults << "\t\t\t\t\t\t<th>Calificación</th>" << endl;
    outputResults << "\t\t\t\t\t\t<th>Observaciones</th>" << endl;
    outputResults << "\t\t\t\t\t</tr>" << endl;
    outputResults << "\t\t\t\t</thead>" << endl;
    outputResults << "\t\t\t\t<tbody>" << endl;

    for( it=calificaciones.begin() ; it != calificaciones.end(); it++ ){
        calificacionTemporal = *it;

        outputResults << "\t\t\t\t\t<tr>" << endl;
        outputResults << "\t\t\t\t\t\t<td>"<< calificacionTemporal.getNombreUsuario() <<"</td>" << endl;
        outputResults << "\t\t\t\t\t\t<td>"<< calificacionTemporal.getCompilador() <<"</td>" << endl;
        outputResults << "\t\t\t\t\t\t<td>"<< calificacionTemporal.getVeredicto() <<"</td>" << endl;

        if(calificacionTemporal.getVeredicto() != "100"){
            observaciones.clear();

            outputResults << "\t\t\t\t\t\t<td>";

            casosPrueba = calificacionTemporal.getCasosPrueba();

            for(ita=casosPrueba.begin() ; ita != casosPrueba.end(); ita++){
                casoPrueba = *ita;

                if(casoPrueba.getEvaluacionCasoPrueba() == "CE" ||
                   casoPrueba.getEvaluacionCasoPrueba() == "RE" ||
                   casoPrueba.getEvaluacionCasoPrueba() == "BAD RET" ||
                   casoPrueba.getEvaluacionCasoPrueba() == "LIM TIME" ||
                   casoPrueba.getEvaluacionCasoPrueba() == "LIMIT_MEM" ||
                   casoPrueba.getEvaluacionCasoPrueba() == "LIM OUT" ||
                   casoPrueba.getEvaluacionCasoPrueba() == "ERR MEM" ||
                   casoPrueba.getEvaluacionCasoPrueba() == "ERR MATH" ||
                   casoPrueba.getEvaluacionCasoPrueba() == "SIG ?"){
                       observaciones.push_back(casoPrueba.getEvaluacionCasoPrueba());
                }

            }

            observaciones.sort();
            observaciones.unique();


            for(list<string>::iterator itOB = observaciones.begin() ; itOB != observaciones.end() ; itOB++){
                outputResults << *itOB << " ";
            }

            outputResults << "</td>" << endl;
        }
        else{
            outputResults << "\t\t\t\t\t\t<td>"<< "" <<"</td>" << endl;
        }

        outputResults << "\t\t\t\t\t</tr>" << endl;
    }

    outputResults << "\t\t\t\t</tbody>" << endl;
    outputResults << "\t\t\t</table>" << endl;
    outputResults << "\t\t</div>" << endl;
    outputResults << "\t\t<div class=\"span5\">" << endl;
    outputResults << "\t\t\t<ul>" << endl;
    outputResults << "\t\t\t\t<li><strong>CE</strong>: Error de compilaci&oacute;n.</li>" << endl;
    outputResults << "\t\t\t\t<li><strong>RE</strong>: Error en tiempo de ejecuci&oacute;n.</li>" << endl;
    outputResults << "\t\t\t\t<li><strong>BAD RET</strong>: El programa regresa un valor no reconocido.</li>" << endl;
    outputResults << "\t\t\t\t<li><strong>LIM TIME</strong>: Exceso de tiempo.</li>" << endl;
    outputResults << "\t\t\t\t<li><strong>LIMIT_MEM</strong>: Exceso de memoria.</li>" << endl;
    outputResults << "\t\t\t\t<li><strong>LIM OUT</strong>: Exceso de salida.</li>" << endl;
    outputResults << "\t\t\t\t<li><strong>ERR MEM</strong>: Violaci&oacute;n de segmento o error de bus.</li>" << endl;
    outputResults << "\t\t\t\t<li><strong>ERR MATH</strong>: Excepci&oacute;n de punto flotante.</li>" << endl;
    outputResults << "\t\t\t</ul>" << endl;
    outputResults << "\t\t\t<br>" << endl;
    outputResults << "\t\t\t<p>Si tu calificaci&oacute;n no aparece es porque no recibi tu programa o porque no compila con gcc, g++ o gcj.</p>" << endl;
    outputResults << "\t\t\t<p>Si tu calificacion es menor a lo que esperabas es probable que tu programa este mal o que imprima letreros o que no separe la salida de la forma pedida (es decir, esta mal).</p>" << endl;
    outputResults << "\t\t\t<p>Generado el " << fecha <<" a las " << hora <<".</p>" << endl;
    outputResults << "\t\t</div>" << endl;
    outputResults << "\t\t" << endl;
    outputResults << "\t</body>" << endl;
    outputResults << "<html>" << endl;

    outputResults.close();
}
