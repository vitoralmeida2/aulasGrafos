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
#include <algorithm>
#include <random>
#include <chrono>
#include "No.h"
#include "Aresta.h"

using namespace std;

class No;
class Aresta;

typedef list<No*> NodeList; // lista de adjancencia

// --- Definindo class Grafo ---

struct Rota
{
    vector<No*> clientes;
    int capacityUsed;
    double custo;
};

struct Solution
{
    vector<Rota> rotas;
    double cost;
    vector<No*> clientesRestantes;
    double bestAlfa;
};

struct Probabilidade {
    double alfa;
    double probabilidade;
};

struct QualidadeAlfa {
    double alfa;
    double soma;
    double quantidadeElementos;
    double qi;
};

struct Bloco{
    Solution bestSol;
    vector<QualidadeAlfa*> qualidadeAlfa;
};

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
        No *insereNo(int idNo, float x, float y, int pesoNo);
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
        void buscaProfundidadeVisita(int idNoInicial, vector<bool> &visitado, int noAnt);
        pair<int, int> buscaLargura(int idNoInicial);
        int encontrarRaio();
        int encontrarDiametro();
        vector<int> encontrarCentro();
        vector<int> encontrarPeriferia();
        bool isCiclo();
        bool isCicloAux(int i, vector<bool> &visitado, vector<bool> &visiting);
        bool isConexo();
        int componentesConexas();
        void componentesConexasVisita(int i, int marca, vector<int> &visitado);
        void ordenacaoTopologica();
        void ordenacaoTopologicaVisita(int i, vector<bool> &visitado, stack<int> &pilhaOrdenacao);
        void fechoTransitivoDireto(int idNoInicial);
        void fechoTransitivoDiretoVisita(int idNoInicial, vector<bool> &visitado);
        void fechoTransitivoIndireto(int idNoInicial);
        void nosArticulacao();
        void nosArticulacaoVisita(int v, vector<int> &desc, vector<int> &low, vector<bool> &visitado, vector<int> &pai, vector<bool> &pontoArticulacao, int &tempo);
        int Dijkstra(int idNoInicial, int idNoDestino);
        int Floyd(int idOrigem, int idDestino);
        void Prim(int idNoInicial);
        void Kruskal();

        // Auxiliares Kruskal
        int encontrarConjunto(int parent[], int i);
        void unirConjunto(int parent[], int x, int y);
        
        // Auxiliares CVRP
        void setInstanceName(string name);
        string getInstanceName();
        void atualizaPesoNos(int idNo, int novoPeso);
        void setCapacidade(int capacidade);
        int getCapacidade();
        void setVeiculos(int num);
        int getVeiculos();
        vector<No*> getNos();
        double distance(No *a, No *b);
        int encontraClienteProximo(No *clienteAtual, vector<No*> clientes);
        Solution gulosoCVRP(ofstream& arquivo);
        Solution gulosoRandomizadoCVRP(double alpha, ofstream& arquivo);
        Solution gulosoRandomizadoReativoCVRP(vector<Probabilidade*> alfas, ofstream& arquivo);
        double calculaDistanciaRota(vector<No*> rota);
        double calculateSolutionCost(Solution &sol);
        No* encontraProxClienteAleatorio(vector<No*> clientesRestantes, No *clienteAtual, double alpha, int capacidadeUsada);
        void setNosNaoVisitados(vector<No*> clientes);
        Solution guloso(ofstream& arquivo);
        Solution randomizado(double alpha, ofstream& arquivo);
        Solution reativo(vector<Probabilidade*> alfas, ofstream& arquivo);
        void atualizarProbabilidade(Bloco bloco, vector<Probabilidade*> alfas);
        void normalizarProbabilidades(vector<Probabilidade*> probabilidades);
        Probabilidade* escolheAlfaAleatorio(vector<Probabilidade*> probabilidadeAlfa);
        void adicionArestasDaRota(vector<Rota> rotas);


        Bloco inicializaBloco(vector<Probabilidade*> alfas);
        
    private:
        int ordem;
        int numNos;
        int numArest;
        bool digrafo;
        bool pesoNo;
        bool pesoArco;
        int numVeiculos;
        int capacidade;
        No *noRaiz;
        Aresta *arestaMenorPeso;
        NodeList *adjList;
        vector<Aresta> arestList;
        int **distanceMat;
        string instanceName;
};

#endif // GRAFO_H