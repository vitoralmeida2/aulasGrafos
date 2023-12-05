#ifndef NO_H
#define NO_H
#include <iostream>
#include <stdlib.h>
#include "Aresta.h"

using namespace std;

class Aresta;


// --- Defininindo class No ---

class No 
{
    public:
        No(int idNo, int cordX, int cordY, int peso);
        ~No();
        
        // --- SET ---
        void setIdNo(int idNo);
        void setPesoNo(int valor);
        void incGrau();
        void decGrau();
        void incGrauEntrada();
        void decGrauEntrada();
        void incGrauSaida();
        void decGrauSaida();
        void setProxNo(No *novoNo);
        void setPrimeiraAresta(Aresta *novAresta);
        void setVisitado(bool v);

        // --- GET ---
        int getIdNo();
        int getPeso();
        int getGrau();
        int getGrauEntrada();
        int getGrauSaida();
        No *getProxNo();
        Aresta *getPrimeiraAresta();
        bool getVisitado();

        int getCordX();
        int getCordY();

        bool operator == (const No& no) const 
        {
            return idNo == no.idNo;
        }

    private:
        int idNo;
        int cordX, cordY;
        int pesoNo;
        bool visitado;
        int grauEntrada;
        int grauSaida;
        No *proxNo;
        Aresta *primeiraAresta;
};

#endif // NO_H