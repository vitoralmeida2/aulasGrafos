#ifndef ARESTA_H
#define ARESTA_H
#include <iostream>
#include "No.h"

using namespace std;

class Aresta
{
    public:
        Aresta(int NoDestino); // incializa aresta
        Aresta(int NoDestino, int pesoAresta); // inicializa aresta com peso

        int getPesoAresta(); // retorna peso aresta
        
};