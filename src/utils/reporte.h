#ifndef REPORTE_H
#define REPORTE_H

#include <string>
#include <sstream>
#include <list>
#include "calificacion.h"
#include "casoPrueba.h"

#include <ctime> //obtener la fecha y hora

using namespace std;

namespace seap_implement {
    class Reporte{
            private:
                string nombreArchivo;
                string nombreProblema;
                string fecha;
                string hora;
                string juez;
                list<seap_implement::Calificacion> calificaciones;
                Calificacion calificacionTemporal;

                Reporte(){};
            public:
                Reporte(const string& nombreDelArchivo, const string& nombreDelProblema,const string& nombreJuez);
                void setNombreProblema(const string& nombreDelProblema);
                void setJuez(const string& nombreJuez);
                string getNombreProblema();
                string getJuez();
                string getFecha();
                string getHora();
                void nuevoAlumno(const string& nombreDelUsuario,const string& compilador);
                void agregarResultadoCasoPrueba(const int resultado, const double tiempo);
                void agregarResultadoCasoPrueba(const string& resultado);
                //void terminarEvaluacionUsuario(const int resultado,const string& veredicto);

                void terminarEvaluacionUsuario(const int veredictoFinal);
                void terminarEvaluacionUsuario(const string& veredictoFinal);

                void imprimirResultadoAmigable();
                void imprimirResultadoCVS();
    };
}
#endif
