bool juezNormal(bool estricto, string nomArchCorr, int dArchAlum)
{
    ofstream cout("logJN.txt", fstream::app);
    char buffer[2];
    int leidos;
    ifstream respCorrecta(nomArchCorr.c_str());

    cout << "******************************" << endl;
    if(!respCorrecta.good())
    {
        cout << "No pude abrir la respuesta correcta." << endl;
        return false;
    }

    bool accepted = true;
    char otroC;
    if(estricto)
    {
        cout << "Comparo en modo estricto con " << nomArchCorr << endl;

        while((leidos = read(dArchAlum, buffer, 1)))
        {
            buffer[leidos] = 0;

            //Comparo caracter por caracter
            if(respCorrecta.good())
            {
                respCorrecta.get(otroC);
                cout << "Comparo " << otroC << " con " << buffer[0];
                if(otroC != buffer[0])
                {
                    accepted = false;
                    cout << "\tMal" << endl;
                }
                else
                {
                    cout << "\tBien" << endl;
                }
            }
            else
            {
                cout << "El archivo de respuesta del alumno es más largo que el correcto." << endl;
                cout << "Sugerencia: Quite un salto de linea o espacio al final del archivo del alumno." << endl;
                accepted = false;
                break;
            }
        }
        if(respCorrecta.peek() != EOF)
        {
            accepted = false;
            cout << "El archivo de correcto es más largo que el de respuesta del alumno." << endl;
            cout << "Sugerencia: Agregue salto de linea al final del archivo del alumno." << endl;
        }
    }
    else
    {
        cout << "Comparo en modo normal con: "  << nomArchCorr << endl;

        //Leo un caracter de cada uno.

        // Caso 1: Si uno de los 2 es espacio o salto de linea.
            //Doy chance de avanzar hasta que no haya de esos.
        // Caso 2: Si alguno de los 2 es EOF,
            //Checho que el otro sólo le queden espacios o saltos de línea
        //Caso 3: Los 3 tienen un caracter normal.
            //Comparo y doy veredicto.
        while(1)
        {
            leidos = read(dArchAlum, buffer, 1);
            buffer[leidos] = 0;
            if(leidos)
            {
                if(respCorrecta.good())     //Caso 1 o 3. Ninguno de los 2 terminó
                {
                    respCorrecta.get(otroC);
                    //cout << "Los 2 tienen caracteres.\t";
                    while(otroC == ' ' || otroC == '\n' || otroC == '\t')     //Caso 1. El archivo correcto tiene espacio.
                    {
                        //cout << "Corr tiene espacio." << endl;
                        //Avanzo todos los espacios de Corr
                        if(respCorrecta.good())
                        {
                            respCorrecta.get(otroC);
                        }
                        else        //Caso 2. El archivo de respuesta correcta ya terminó.
                        {
                            //cout << "Se terminón Corr." << endl;
                            //Checo que al archivo del alumno sólo le queden saltos y espacios. Si no, WA.
                            do
                            {
                                if(buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\t')
                                {
                                    accepted = false;
                                    cout << "Encontré otro caracter normal en Alum cuando ya no debería." << endl;
                                    break;
                                }
                                leidos = read(dArchAlum, buffer, 1);
                            }
                            while(leidos);
                            break;  //Sólo le quedaron espacios al otro.
                        }
                    }   //Si hay otro caracter normal, el while terminará y me dejará con él.
                    //cout << "Corr encontró otro caracter normal." << endl;
                    while(buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\t')     //Caso 1. El archivo del alumno tiene espacio.
                    {
                        //cout << "Alum tiene espacio." << endl;
                        if(leidos)
                        {
                            leidos = read(dArchAlum, buffer, 1);
                        }
                        else        //Caso 2. El archivo del alumno ya terminó.
                        {
                            //cout << "Se terminón Alum." << endl;
                            //Checo que al archivo correcto sólo le queden saltos y espacios. Si no WA.
                            do
                            {
                                if(otroC != ' ' && otroC != '\n' && otroC != '\t')
                                {
                                    accepted = false;
                                    cout << "Encontré otro caracter normal en Corr cuando ya no debería." << endl;
                                    break;
                                }
                                respCorrecta.get(otroC);
                            }
                            while(respCorrecta.good());
                            break;  //Sólo le quedaron espacios al otro.
                        }
                    } //Si hay otro caracter normal, el while terminará y me dejará con él.
                    //cout << "Alum encontró otro caracter normal." << endl;
                    cout << "Comparo " << otroC << " con " << buffer[0];    //Caso 3. Los 2 tienen un caracter normal.
                    if(otroC != buffer[0])
                    {
                        accepted = false;
                        cout << "\tMal" << endl;
                        break;
                    }
                    else
                    {
                        cout << "\tBien" << endl;
                    }
                }
                else    //Caso 2. El archivo de respuesta correcta ya terminó.
                {
                    //cout << "Corr ya termino. Checho por sólo espacios en Alum." << endl;
                    //Checo que al archivo del alumno sólo le queden saltos y espacios.
                    do
                    {
                        if(buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\t')
                        {
                            accepted = false;
                            cout << "Encontré otro caracter normal en Alum cuando ya no debería." << endl;
                            break;
                        }
                        leidos = read(dArchAlum, buffer, 1);
                    }
                    while(leidos);
                    break; //Sólo le quedaron espacios al otro.
                }
            }
            else    //Caso 2. El archivo de salida del alumno ya terminó.
            {
                //cout << "Alum ya termino. Checho por sólo espacios en Corr." << endl;
                if(!respCorrecta.good()) //Los dos terminan al mismo tiempo.
                {
                    break;
                }
                else
                {
                    respCorrecta.get(otroC);
                    //Checo que al archivo correcto sólo le queden saltos y espacios.
                    do
                    {
                        if(otroC != ' ' && otroC != '\n' && otroC != '\t')
                        {
                            cout << "Encontré otro caracter normal en Corr cuando ya no debería." << endl;
                            accepted = false;
                            break;
                        }
                        respCorrecta.get(otroC);
                    }
                    while(respCorrecta.good());
                }
                break;      //Sólo le quedaron espacios al otro.
            }
        }
    }
    respCorrecta.close();
    if(accepted) cout << "Caso " << nomArchCorr << " correcto" << endl;
    else         cout << "Caso " << nomArchCorr << " mal" << endl;
    cout.close();
    return accepted;
}
