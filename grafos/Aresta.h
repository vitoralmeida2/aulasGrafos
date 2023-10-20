#ifndef ARESTA_H
#define ARESTA_H
#include <iostream>
#include <stdlib.h>
#include "No.h"

using namespace std;

class No;

// --- Definindo class Aresta ---

class Aresta 
{
    public:
        Aresta(No *noDestino, Aresta *proxAresta, int peso);
        ~Aresta();

        // --- SET ---
        void setProxAresta(Aresta *valor);
        void setNoDestino(No *valor);
        void setPeso(int valor);

        // --- GET ---
        int getPesoAresta();
        No *getNoDestino();
        Aresta *getProxAresta();

    private:
        int pesoAresta;
        int idNoDestino;
        No *NoDestino;
        Aresta *proxAresta;

};

#endif // ARESTA_H