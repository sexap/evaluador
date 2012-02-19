
bool personalizado(string archSalidaEsperada, string archAlum)
{
    ofstream cout("logJuezPersonalizado.txt", fstream::app);

    bool accepted = true;

    ifstream respCorrecta(archSalidaEsperada.c_str());
    ifstream respAlumno(archAlum.c_str());

    cout << "******************************" << endl;
    if(!respAlumno.good())
    {
        cout << "No se puede leer el archivo de resultado del alumno." << endl;
    }
    else
    {
        cout << "Abrí el archivo del alumno." << endl;
    }
    if(!respCorrecta.good())
    {
        cout << "No se puede abrir la salida esperada." << endl;
        return false;
    }
    else
    {
        cout << "Abrí la respuesta correcta." << endl;
    }

    /***
    *
    *
    *  Su código personalizado va aquí.
    *
    *
    ***/

    if(accepted) cout << "Caso " << archSalidaEsperada << " correcto" << endl;
    else         cout << "Caso " << archSalidaEsperada << " mal" << endl;

    respCorrecta.close();
    respAlumno.close();
    cout.close();

    return accepted;
}
