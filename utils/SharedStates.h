#pragma once
#include <fstream>
#include "../data/userGameData.h"
using namespace std;

class SharedStates
{

private:
    int vida = 0;
    int nivel = 0;

    UserGameData userGameData;

public:
    UserGameData userModel;
    SharedStates( )
    {
        vida = 0;
        nivel = 0;
    }

    bool createDefaultUserGameData( )
    {
        try
        {
            ofstream archivo_e; // Variable para escritura de datos del archivo
            archivo_e.open("recursos/informacion.txt" , ios::out);
            archivo_e << "1"
                << " "
                << "500" << endl;
            userGameData.nivel = 1;
            userGameData.vida = 500;

            archivo_e.close( ); // Cerrar archivo de escritura
            return true;
        }
        catch (...)
        {
            throw runtime_error("Error al crear el archivo de informacion.");
        }
    }

    void readUserGameData( )
    {
        try
        {
            ifstream archivo_l;                                               // Variable para lectura de datos del archivo
            archivo_l.open("recursos/informacion.txt" , ios::in);              // Abrir el Archivo "informacion.txt" en modalidad de entrada de datos
            archivo_l >> this->userGameData.nivel >> this->userGameData.vida; // Almacenamiento de datos en el archivo, dentro de las variables locales
            if (this->userGameData.vida == 0)                                 // Si la vida es 0, se reestablecen a los valores iniciales predeterminados y almacenarlos en el archivo
            {
                archivo_l.close( ); // Cerrar archivo de lectura
                createDefaultUserGameData( );
            }
        }
        catch (...)
        {
            throw runtime_error("Error al leer el archivo de informacion.");
        }
    }

    UserGameData getUserGameData( )
    {
        return userGameData;
    }

    bool createEspecificUserGameData(int nivel , int vida)
    {
        try
        {
            ofstream archivo_e; // Variable para escritura de datos del archivo
            archivo_e.open("recursos/informacion.txt" , ios::out);
            archivo_e << nivel
                << " "
                << vida << endl;
            userGameData.nivel = nivel;
            userGameData.vida = vida;

            archivo_e.close( ); // Cerrar archivo de escritura
            return true;
        }
        catch (...)
        {
            throw runtime_error("Error al crear el archivo de informacion.");
        }
    }
};