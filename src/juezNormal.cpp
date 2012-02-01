bool juezNormal(bool estricto, string nomArchCorr, int dArchAlum)
{
    char buffer[128];
    int leidos;
    ifstream respCorrecta(nomArchCorr.c_str());
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

        while((leidos = read(dArchAlum, buffer, 127)))
        {
            buffer[leidos] = 0;
            //cout << buffer;

            //Comparo caracter por caracter
            for(int c = 0; c < leidos; c++)
            {
                if(respCorrecta.good())
                {
                    respCorrecta.get(otroC);
                    //cout << "Comparo " << otroC << " con " << buffer[c] << endl;
                    if(otroC != buffer[c])
                    {
                        accepted = false;
                    }
                }
                else
                {
                    accepted = false;
                    break;
                }
            }
        }
        if(respCorrecta.peek() != EOF)
            accepted = false;
    }
    else
    {
        cout << "Comparo en modo normal con: "  << nomArchCorr << endl;
        while((leidos = read(dArchAlum, buffer, 127)))
        {
            buffer[leidos] = 0;
            //cout << buffer;

            //Comparo caracter por caracter
            for(int c = 0; c < leidos; c++)
            {
                if(respCorrecta.good())
                {
                    respCorrecta.get(otroC);
                    //cout << "Comparo " << otroC << " con " << buffer[c] << endl;
                    if(otroC != buffer[c])
                    {
                        accepted = false;
                    }
                    else
                    {
                        bool huboSig = false;

                        while(otroC == ' ')
                        {
                            respCorrecta.get(otroC);
                        }
                        //respCorrecta.putback(otroC);

                        while(buffer[c] == ' ')
                        {
                            //Busco el siguiente caracter para ver si es un espacio
                            //Si hay más caracteres en la tanda actual, pues paso al siguiente.
                            if(c+1 < leidos)
                            {
                                c++;
                            }
                            //Si ya no hay más caracteres en la tanda actual, pues pido una nueva tanda, mientras haya nuevas tandas.
                            else
                            {
                                if(leidos = read(dArchAlum, buffer, 127))
                                {
                                    c = 0;
                                }
                                else
                                {
                                    c = leidos;
                                }
                            }
                        }
                    }
                }
                else
                {
                    accepted = false;
                    break;
                }
            }
        }
        if(respCorrecta.peek() != EOF)
            accepted = false;
    }
    respCorrecta.close();
    return accepted;
}
