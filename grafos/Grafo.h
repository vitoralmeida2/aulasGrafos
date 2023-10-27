#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <list>
#include "No.h"
#include "Aresta.h"


using namespace std;

class No;
class Aresta;

// --- Definindo class Grafo ---

class Grafo 
{
    public:
        // --- Construtor e Destrutor ---
        Grafo(int numVertices, bool isDigrafo, bool pesoNo, bool pesoArco);
        ~Grafo();

        // --- SET ---
        void incOrdem();
        void decOrdem();

        // --- Funcoes do No ---
        No *procurarNoPeloId(int idFindNo);
        No *insereNo(int idNo, int pesoNo);
        bool removeNo(int idNo);

        // --- Funcoes de Aresta ---
        bool insereAresta(int idNoOrigem, int idNoDestino, int pesoAresta);
        bool removeAresta(No *noFonte, int idNoDestino);

        // --- Caracteristica do Grafo ---
        int getNumAresta();
        No *getNoRaiz();
        int getOrdem();
        int getGrau();
        int getGrauEntrada();
        int getGrauSaida();
        bool isDigrafo();

        // --- Funcoes Grafo ---
        void imprimeGrafo(); // imprime grafo lista de adjacencia
        void buscaProfundidade(int idNoInicial);
        void buscaProfundidadeVisita(int idNoInicial);
        void componentesConexas();
        void componentesConexasVisita(int v, int marca);
    
    private:
        int ordem;
        int numNos;
        int numArest;
        bool digrafo;
        bool pesoNo;
        bool pesoArco;
        int *visitado;
        No *noRaiz;
        list<No*> *adjList;

};

#endif // GRAFO_H