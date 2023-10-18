#ifndef NO_H
#define NO_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include "Aresta.h"

using namespace std;


// --- Defininindo class No ---

class No 
{
    public:
        No(int idNo);
        No(int idNo, int peso);
        
        // --- SET ---
        void setIdNo(int idNo);
        void setPesoNo(int valor);
        void incGrauEntrada();
        void decGrauEntrada();
        void incGrauSaida();
        void decGrauSaida();
        void setProxNo(No *novoNo);
        void setPrimeiraAresta(Aresta *novAresta);

        // --- GET ---
        int getIdNo();
        int getPeso();
        int getGrau();
        int getGrauEntrada();
        int getGrauSaida();
        No *getProxNo();
        Aresta *getPrimeiraAresta();

    private:
        int idNo;
        int pesoNo;
        int grauEntrada;
        int grauSaida;
        No *proxNo;
        Aresta *primeiraAresta;
};

#endif // NO_H