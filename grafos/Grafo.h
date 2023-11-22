#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <limits>
#include "No.h"
#include "Aresta.h"

using namespace std;

class No;
class Aresta;

typedef list<No*> NodeList; // lista de adjancencia
typedef list<pair<int, int>> DistancesList; // lista de distancia entre os vizinhos <id vizinho, distancia para aquele vizinho>

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
        bool removeAresta(No *noFonte, No *noDestino);

        // --- Caracteristica do Grafo ---
        int getNumAresta();
        No *getNoRaiz();
        int getOrdem();
        int getGrau();
        int getGrauEntrada();
        int getGrauSaida();
        bool isDigrafo();

        // --- Funcoes Grafo ---
        void imprimeGrafo();
        void buscaProfundidade(int idNoInicial);
        void buscaProfundidadeVisita(int idNoInicial, vector<bool> &visitado);
        bool isCiclo();
        bool isCicloAux(int i, vector<bool> &visitado, vector<bool> &visiting);
        bool isConexo();
        int componentesConexas();
        void componentesConexasVisita(int i, int marca, vector<int> &visitado);
        void ordenacaoTopologica();
        void ordenacaoTopologicaVisita(int i, vector<bool> &visitado, stack<int> &pilhaOrdenacao);
        void fechoTransitivoDireto(int idNoInicial);
        void fechoTransitivoIndireto();
        vector<int> Dijkstra(int idNoInicial);
        int Floyd(int idOrigem, int idDestino);
        void Prim();
        void Kruskal();
    
    private:
        int ordem;
        int numNos;
        int numArest;
        bool digrafo;
        bool pesoNo;
        bool pesoArco;
        No *noRaiz;
        NodeList *adjList;
        DistancesList *distanceList;
};

#endif // GRAFO_H