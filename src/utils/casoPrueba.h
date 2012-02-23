#ifndef CASOPRUEBA_H
#define CASOPRUEBA_H

using namespace std;

namespace seap_implement{

    class CasoPrueba{
            private:
                string memoria;
                string evaluacionCasoPrueba;
                //CasoPrueba(){};

            public:
                CasoPrueba(){};
                CasoPrueba(const string& evaluacion);
                CasoPrueba(const int resultado, const double tiempo);
                string getEvaluacionCasoPrueba();
                string getMemoria();
    };

}

#endif
