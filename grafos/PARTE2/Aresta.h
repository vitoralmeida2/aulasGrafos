#ifndef ARESTA_H
#define ARESTA_H
#include <iostream>
#include <stdlib.h>
#include "No.h"

class No;

// --- Definindo class Aresta ---

class Aresta 
{
    public:
        Aresta(No *noOrigem, No *noDestino, int pesoAresta, Aresta *proxAresta);
        ~Aresta();

        // --- SET ---
        void setProxAresta(Aresta *valor);
        void setNoOrigem(No *valor);
        void setNoDestino(No *valor);
        void setPeso(int valor);

        // --- GET ---
        int getIdNoOrigem();
        int getIdNoDestino();
        int getPesoAresta();
        No *getNoOrigem();
        No *getNoDestino();
        Aresta *getProxAresta();

        bool operator < (const Aresta &a) const
        {
            return pesoAresta > a.pesoAresta;
        }

    private:
        int idNoOrigem;
        int idNoDestino;
        No *NoOrigem;
        No *NoDestino;
        int pesoAresta;
        Aresta *proxAresta;

};

#endif // ARESTA_H