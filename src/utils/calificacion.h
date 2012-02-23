#ifndef CALIFICACION_H
#define CALIFICACION_H

#include <string>
#include <sstream>
#include <list>
#include "casoPrueba.h"

using namespace std;

namespace seap_implement{

    class Calificacion{
            private:
                string nombreUsuario;
                list<seap_implement::CasoPrueba> casosPrueba;
                string veredicto;
                string compilador;

            public:

                Calificacion(){};
                Calificacion(const string& nombreDelUsuario,const string& compiler);
                void agregarCasoPrueba(const int resultado, const double tiempo);
                void agregarCasoPrueba(const string& resultado);
                void setVeredicto(const string& veredictoFinal);
                void setVeredicto(const int veredictoFinal);
                void setCompilador(const string& compiladorUsado);
                void setNombreAlumno(const string& nombreDelUsuario);
                string getNombreUsuario();
                string getCompilador();
                list<seap_implement::CasoPrueba> getCasosPrueba();
                int getCalificacion();
                string getVeredicto();
                string toString(const int numero);
                string toString(const double numero);
    };

}

#endif
