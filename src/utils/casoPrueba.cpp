#include "reporte.h"
#include "calificacion.h"
#include "casoPrueba.h"

using namespace seap_implement;

// FUNCIONES DE CASO DE PRUEBA

CasoPrueba::CasoPrueba(const int resultado, const double tiempo){
    ostringstream strs;
    string str1, str2, str3;

    strs.str("");
    strs << resultado;
    str1 = strs.str();

    strs.str("");
    strs << tiempo;
    str2 = strs.str();

    str3 = str1 + " (" + str2 + " ms)";

    evaluacionCasoPrueba = str3;
}

CasoPrueba::CasoPrueba(const string& evaluacion){
    evaluacionCasoPrueba = evaluacion;
}

string CasoPrueba::getEvaluacionCasoPrueba(){
    return evaluacionCasoPrueba;
}

string CasoPrueba::getMemoria(){
    return memoria;
}
