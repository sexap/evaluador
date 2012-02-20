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
                int calificacion;
                string veredicto;

            public:

                Calificacion(){};
                Calificacion(const string& nombreDelUsuario);
                void agregarCasoPrueba(const int resultado, const double tiempo);
                void agregarCasoPrueba(const string& resultado);
                void setCalificacion(const int calificacionFinal);
                void setVeredicto(const string& veredictoFinal);
                void setNombreAlumno(const string& nombreDelUsuario);
                string getNombreUsuario();
                list<seap_implement::CasoPrueba> getCasosPrueba();
                int getCalificacion();
                string getVeredicto();
    };

}

#endif
