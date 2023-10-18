#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "No.h"
#include "Aresta.h"

using namespace std;

// --- Definindo class Grafo ---

class Grafo 
{
    public:
        // --- Construtor e Destrutor ---
        Grafo(bool isDigrafo, bool pesoNo, bool pesoArco);
        ~Grafo();

        // --- SET ---
        void incOrdem();
        void decOrdem();

        // --- Funcoes do No ---
        No *procurarNoPeloId(int idFindNo);
        No *insereNo(int idNo, int pesoNo);
        bool removeNo(int idNo, bool isDigrafo);

        // --- Funcoes de Aresta ---
        bool insereAresta(int idNoOrigem, int idNoDestino, int pesoAresta);
        bool removeAresta(No *noFonte, int idNoDestino);

        // --- Caracteristica do Grafo ---
        int getNumAresta();
        No *getNoRaiz();
        int getOrdem();
        int getGrauEntrada();
        int getGrauSaida();
        bool isDigrafo();
    
    private:
        int ordem;
        int numAresta;
        bool digrafo;
        bool pesoNo;
        bool pesoArco;
        No *noRaiz;

};

#endif // GRAFO_H