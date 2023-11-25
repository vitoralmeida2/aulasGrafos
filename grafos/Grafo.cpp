#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <limits>
#include "Grafo.h"
#include "Aresta.h"
#include "No.h"
#define INFINITO INT16_MAX

using namespace std;

// --- Construtor ---

Grafo::Grafo(int numVertices, bool isDigrafo, bool pesoNO, bool pesoArc) 
{
    this->ordem = 0;
    this->noRaiz = NULL;
    this->digrafo = isDigrafo;
    this->pesoNo = pesoNO;
    this->pesoArco = pesoArc;
    this->numNos = numVertices;
    adjList = new NodeList[numVertices+1];
    distanceMat = new int*[numVertices+1];
    for (int i = 0; i <= this->numNos; i++)
    {
        distanceMat[i] = new int[numVertices+1];
    }

    for (int i = 1; i <= this->numNos; i++)
    {
        for (int j = 1; j <= this->numNos; j++)
        {
            if (i == j)
            {
                distanceMat[i][j] = 0;
            } else
                {
                    distanceMat[i][j] = INFINITO;
                }
        }
    }
}

// --- Destrutor ---

Grafo::~Grafo() 
{
    No *noAux = getNoRaiz();
    No *noAuxProx;
    while (noAux != NULL) 
    {
        noAuxProx = noAux->getProxNo();
        delete noAux;
        noAux = noAuxProx;
    }

    for (int i = 0; i <= this->numNos; i++)
    {
        delete [] distanceMat[i];
    }
    
    delete [] adjList;
}

// --- SET ---

// Funcao incrementa Ordem do grafo em um.
void Grafo::incOrdem() 
{
    this->ordem++;
}

// Funcao decrementa Ordem do grafo em um.
void Grafo::decOrdem() 
{
    this->ordem--;
}

// --- FUNCOES DO NO ---

/*  procuraNoPeloId -
    Procura pelo No pelo seu ID
    retorna o No se encontrar
    retorna NULL se nao encontrar
*/
No *Grafo::procurarNoPeloId(int idFindNo) 
{
    No *noAux = noRaiz;

    while (noAux != NULL) 
    {
        if (noAux->getIdNo() == idFindNo) 
        {
            return noAux;
        }

        if (noAux->getProxNo() == NULL) 
        {
            return NULL;
        }

        noAux = noAux->getProxNo();
    }

    return NULL;
}

/*  insereNo -
    A funcao insere o No caso ele nao existe ou atualiza seu peso.
    retorna o No criado ou alterado
*/
No *Grafo::insereNo(int idNo, int peso) 
{
    No *aux = procurarNoPeloId(idNo);

    // se No nao existe
    if (aux == NULL)
    {
        No *novoNo = new No(idNo, peso);

        if (noRaiz == NULL)
        {
            noRaiz = novoNo;
        } else
            {
                novoNo->setProxNo(noRaiz);
                noRaiz = novoNo;
            }
        
        incOrdem();
        return novoNo;
    } else // se No existe
        {
            aux->setPesoNo(peso);
        }

    return aux;
}

/*
    removeNo -
    Remove No do Grafo
*/
bool Grafo::removeNo(int idNo) 
{
    // Pesquisa o No a ser excluido

    No *noParaRemover = procurarNoPeloId(idNo);

    if (noParaRemover == NULL)
    {
        return false; // No nao esta no grafo
    }

    // Remove todas as arestas/arcos onde este nó ocorre

    if (isDigrafo()) // se e digrafo
    {
        No *noAux = noRaiz;
        while (noParaRemover->getGrauEntrada() != 0 && noAux != NULL) // removendo arestas de entrada
        {
            removeAresta(noAux, noParaRemover);
            noAux = noAux->getProxNo();
        }
        
        Aresta *arestaParaRemover = noParaRemover->getPrimeiraAresta();
        No *noDestino = NULL;
        
        while (noParaRemover->getPrimeiraAresta() != NULL) // removendo arestas de saida
        {
            noDestino = noParaRemover->getPrimeiraAresta()->getNoDestino();
            removeAresta(noParaRemover, noDestino);
        }

    } else // se nao e Digrafo
        {
            No *noDestino = NULL;

            while (noParaRemover->getPrimeiraAresta() != NULL) // removendo arestas do No
            {
                noDestino = noParaRemover->getPrimeiraAresta()->getNoDestino();
                removeAresta(noParaRemover, noDestino);
                removeAresta(noDestino, noParaRemover);
            }
        }
    
    // Ajusta Nos
    
    if (noParaRemover == noRaiz)
    {
        noRaiz = noParaRemover->getProxNo();
    } else
        {
            No *noAux = noRaiz;
            while (noAux->getProxNo() != noParaRemover)
            {
                noAux = noAux->getProxNo();
            }

            noAux->setProxNo(noParaRemover->getProxNo());
        }
    
    // deleta No

    /*
        remove No da lista de adjacencia
    */
    for (int i = 1; i <= this->numNos; i ++)
    {
        if (i == idNo)
        {
            adjList[i].clear();
        }
        
        for (No *adj:adjList[i])
        {
            if (adj == noParaRemover)
            {
                adjList[i].remove(noParaRemover);
            }
        }
    }
    
    delete noParaRemover;
    decOrdem();

    return true;
}

// --- FUNCOES DE ARESTA ---

/*
    Insere a aresta de A para B se a adjacencia nao existir,
    caso exista, a funcao atualiza o peso
*/
bool Grafo::insereAresta(int idNoOrigem, int idNoDestino, int pesoAresta) 
{
    No *noFonte, *noDestino;
    noFonte = procurarNoPeloId(idNoOrigem);
    noDestino = procurarNoPeloId(idNoDestino);

    // Veririfica se ja exisitem os Nos. Cria novos Nos se nao existem

    if (noFonte == NULL)
    {
        noFonte = insereNo(idNoOrigem, 0);
    }

    if (noDestino == NULL)
    {
        noDestino = insereNo(idNoDestino, 0);
    }

    // Cria nova aresta ou altera seu peso caso ja exista

    Aresta *novaAresta = noFonte->getPrimeiraAresta();

    while (novaAresta != NULL && novaAresta->getNoDestino()->getIdNo() != idNoDestino) // procura aresta correspondente se ela existe
    {
        novaAresta = novaAresta->getProxAresta();
    }

    if (isDigrafo()) // se e Digrafo
    {
        if (novaAresta == NULL) // se aresta nao existe
        {
            novaAresta = new Aresta(noFonte, noDestino, pesoAresta, NULL);
            adjList[idNoOrigem].push_back(noDestino); // adicionando adjacencia na lista
            distanceMat[idNoOrigem][idNoDestino] = pesoAresta; // adicionando distancia na matriz de distancias
            
            // ajusta arestas do no origem
            if (noFonte->getPrimeiraAresta() == NULL)
            {
                noFonte->setPrimeiraAresta(novaAresta);
            } else
                {
                    Aresta *aux = noFonte->getPrimeiraAresta();
                    while (aux->getProxAresta() != NULL)
                    {
                        aux = aux->getProxAresta();
                    }
                    aux->setProxAresta(novaAresta);
                }

            // inc graus
            noFonte->incGrauSaida();
            noDestino->incGrauEntrada();
        } else // se existe
            {
                novaAresta->setPeso(pesoAresta);
                distanceMat[idNoOrigem][idNoDestino] = pesoAresta; // atualizando distancia na matriz de distancias
            }
    } else // se nao e Digrafo
        {
            if (novaAresta == NULL) // se aresta nao existe
            {
                novaAresta = new Aresta(noFonte, noDestino, pesoAresta, NULL);
                adjList[idNoOrigem].push_back(noDestino); // adicionando adjacencia na lista
                distanceMat[idNoOrigem][idNoDestino] = pesoAresta; // adicionando distancia na matriz de distancias

                // ajusta arestas
                if (noFonte->getPrimeiraAresta() == NULL)
                {
                    noFonte->setPrimeiraAresta(novaAresta);
                } else
                    {
                        Aresta *aux = noFonte->getPrimeiraAresta();
                        while (aux->getProxAresta() != NULL)
                        {
                            aux = aux->getProxAresta();
                        }
                        aux->setProxAresta(novaAresta);
                    }
                
                // cria Aresta sentido contrario
                Aresta *novaAresta2 = new Aresta(noDestino, noFonte, pesoAresta, NULL); 
                adjList[idNoDestino].push_back(noFonte); // adicionando adjacencia na lista
                distanceMat[idNoDestino][idNoOrigem] = pesoAresta; // adicionando distancia na matriz de distancias

                // ajusta arestas sentido contrario
                if (noDestino->getPrimeiraAresta() == NULL)
                {
                    noDestino->setPrimeiraAresta(novaAresta2);
                } else
                    {
                        Aresta *aux = noDestino->getPrimeiraAresta();
                        while (aux->getProxAresta() != NULL)
                        {
                            aux = aux->getProxAresta();
                        }
                        aux->setProxAresta(novaAresta2);
                    }

                // inc graus
                noFonte->incGrau();
                noDestino->incGrau();
            } else // se aresta existe
                {
                    // atualizando peso aresta
                    novaAresta->setPeso(pesoAresta);
                    distanceMat[idNoOrigem][idNoDestino] = pesoAresta;

                    // atualizando peso aresta contraria
                    Aresta *auxAresta = noDestino->getPrimeiraAresta();
                    while (auxAresta->getIdNoDestino() != noFonte->getIdNo())
                    {
                        auxAresta = auxAresta->getProxAresta();
                    }
                    auxAresta->setPeso(pesoAresta);
                }
        }
        
    return true;
}

/*
    Remove arestas
*/
bool Grafo::removeAresta(No *noFonte, No *noDestino)
{   
    Aresta *arestaParaRemover = noFonte->getPrimeiraAresta();
    Aresta *arestaAnterior = NULL;

    while (arestaParaRemover != NULL) // procurando aresta para remover
    {
        if (arestaParaRemover->getIdNoDestino() == noDestino->getIdNo()) // aresta encontrada
        {
            // decrementa graus
            if (isDigrafo())
            {
                noFonte->decGrauSaida();
                arestaParaRemover->getNoDestino()->decGrauEntrada();
            } else
                {
                    noFonte->decGrau();
                }
            
            // ajusta arestas
            if (arestaAnterior != NULL)
            {
                arestaAnterior->setProxAresta(arestaParaRemover->getProxAresta());
            } else
                {
                    noFonte->setPrimeiraAresta(arestaParaRemover->getProxAresta());
                }

            // deleta aresta
            delete arestaParaRemover;
            distanceMat[noFonte->getIdNo()][noDestino->getIdNo()] = INFINITO; // define distancia = INFINITO na matriz de distancias
            return true;
        }

        arestaAnterior = arestaParaRemover;
        arestaParaRemover = arestaParaRemover->getProxAresta();
    }

    return false; // aresta nao encontrada
}

// --- Caracteristica do Grafo ---

/*
    GetNumAresta
    Retorna o numero de aresta do grafo(ou arcos do digrafo).
*/
int Grafo::getNumAresta() 
{
    No *noAux = noRaiz;
    numArest = 0;

    if (isDigrafo())
    {
        while (noAux != NULL)
        {
            numArest = numArest + noAux->getGrauEntrada();
            noAux = noAux->getProxNo();
        }
        return numArest;
    } else
        {
            while (noAux != NULL) 
            {
                numArest = numArest + noAux->getGrau();
                noAux = noAux->getProxNo();
            }
            return numArest / 2;
        }
}

/*
    Retorna No raiz.
*/
No *Grafo::getNoRaiz() 
{
    return this->noRaiz;
}

/*
    Retorna ordem do grafo.
*/
int Grafo::getOrdem() 
{
    return this->ordem;
}

/*
    Retorna grau maximo de um grafo simples
*/
int Grafo::getGrau()
{
    return getGrauEntrada();
}

/*
    Retorna grau max de entrada.
*/
int Grafo::getGrauEntrada() 
{
    int grauEntrada = 0;
    No *noAux = noRaiz;

    while (noAux != NULL) 
    {
        if (noAux->getGrauEntrada() > grauEntrada) 
        {
            grauEntrada = noAux->getGrauEntrada();
        }
        noAux = noAux->getProxNo();
    }
    return grauEntrada;
}

/*
    Retorna grau max de saida.
*/
int Grafo::getGrauSaida() 
{
    int grauSaida = 0;
    No *noAux = noRaiz;

    while (noAux != NULL) 
    {
        if (noAux->getGrauSaida() > grauSaida) 
        {
            grauSaida = noAux->getGrauSaida();
        }
        noAux = noAux->getProxNo();
    }
    return grauSaida;
}

/*
    true - se grafo e digrafo.
    false - se grafo NAO e digrafo.
*/
bool Grafo::isDigrafo() 
{
    return this->digrafo;
}

// --- Funcoes Grafo ---

/*
    imprime o grafo representado em lista de adjacencia
*/
void Grafo::imprimeGrafo()
{
    cout << "Lista de Adjacencia: " << endl;
    for (int i = 1; i <= this->numNos; i++)
    {
        cout << "[" << i << "]" << " -> ";
        for (No *adj:adjList[i])
        {
            cout << adj->getIdNo() << " -> ";
        }
        cout << "//" << endl;
    }
}

/*
    Funcao que inicia busca em profundidade
*/
void Grafo::buscaProfundidade(int idNoInicial)
{
    // inicia vetor de visitados com posicoes = false
    vector<bool> visitado(this->numNos+1, false);
    buscaProfundidadeVisita(idNoInicial, visitado);
}

/*
    Funcao para visitar em profundidade
*/
void Grafo::buscaProfundidadeVisita(int idNoInicial, vector<bool> &visitado)
{
    // Marca o No atual como visitado
    visitado[idNoInicial] = true;
    // cout << idNoInicial << " ";

    // Percorre todos os vertices adjacentes do vertice atual
    for (No *adj:adjList[idNoInicial])
    {
        // visitado adjacente se nao visitado
        if (visitado[adj->getIdNo()] == false)
        {
            buscaProfundidadeVisita(adj->getIdNo(), visitado);
        }
    }
}

/*
    Funcao para verificar se ha ciclo no grafo
*/
bool Grafo::isCiclo()
{
    // inicia vetor visitado e vetor visiting com posicoes = false
    vector<bool> visitado(this->numNos+1, false);
    vector<bool> visiting(this->numNos+1, false);

    // chama funcao recursiva para verificar se ha ciclo
    for (int i = 1; i <= this->numNos; i++)
    {
        if (isCicloAux(i, visitado, visiting))
        {
            return true;
        }
    }

    return false;
}

/*
    Funcao auxiliar para verificar se ha ciclo no grafo
*/
bool Grafo::isCicloAux(int i, vector<bool> &visitado, vector<bool> &visiting)
{
    if (visiting[i])
    {
        return true; // return true se No estiver sendo visitado
    }

    if (visitado[i])
    {
        return false; // return false se No ja foi visitado
    }

    // seta true para visiting e visitado
    visitado[i] = true;
    visiting[i] = true;

    // verifica Nos adjacentes
    for (No *adj:adjList[i])
    {
        if (isCicloAux(adj->getIdNo(), visitado, visiting))
        {
            return true;    // se possui ciclo return true
        }
    }

    visiting[i] = false;

    return false;
}

/*
    Funcao para verificar se um grafo e conexo
    return true se sim
*/
bool Grafo::isConexo()
{
    if (componentesConexas() == 1)
    {
        return true;
    }

    return false;
}

/*
    Funcao para identificar quantas componentes conexas ha no grafo
*/
int Grafo::componentesConexas()
{
    // inicializa marca das componentes e vetor de visitados
    vector<int> visitado(this->numNos+1, 0);
    int componentes = 0;

    for (int i = 1; i <= this->numNos; i++)
    {
        if (visitado[i] == 0)
        {
            componentes++;
            componentesConexasVisita(i, componentes, visitado);
        }
    }

    return componentes;
}

/*
    Funcao auxiliar parar visitar componentes conexas
*/
void Grafo::componentesConexasVisita(int v, int marca, vector<int> &visitados)
{
    visitados[v] = marca;

    for (No *adj:adjList[v])
    {
        if (visitados[adj->getIdNo()] == 0)
        {
            componentesConexasVisita(adj->getIdNo(), marca, visitados);
        }
    }
}

/*
    Funcao que ordena topologicamente o grafo
*/
void Grafo::ordenacaoTopologica()
{
    if (isCiclo())
    {
        cout << "O grafo contém um ciclo. Não é possível ordenar topologicamente." << endl;
        return;
    }

    // inicia vetor visitados com posicoes = false e pilha de ordenacao
    vector<bool> visitado(this->numNos+1, false);
    stack<int> pilhaOdernacao;

    // chama funcao recursiva para ordenar topologicamente
    for (int i = 1; i <= this->numNos; i++)
    {
        if (visitado[i] == false)
        {
            ordenacaoTopologicaVisita(i, visitado, pilhaOdernacao);
        }
    }

    cout << "Odernacao Topologica do Grafo: ";
    while(!pilhaOdernacao.empty())
    {
        cout << pilhaOdernacao.top() << " ";
        pilhaOdernacao.pop();
    }
    cout << endl;
}

/*
    Funcao auxliar ordenacaoTopologica
*/
void Grafo::ordenacaoTopologicaVisita(int i, vector<bool> &visitado, stack<int> &pilhaOrdenacao)
{
    // marca No atual como visitado
    visitado[i] = true;

    // visita todos Nos adjacentes do No atual
    for (No *adj:adjList[i])
    {
        if (visitado[adj->getIdNo()] == false)
        {
            ordenacaoTopologicaVisita(adj->getIdNo(), visitado, pilhaOrdenacao);
        }
    }

    pilhaOrdenacao.push(i);
}

/*
    Funcao para obter fecho transitivo direto
*/
void Grafo::fechoTransitivoDireto(int idNoInicial)
{
    if (isDigrafo())
    {
        // vetor fecho transitivo direto para No inicial
        vector<bool> visitado(this->numNos+1, false);
        buscaProfundidadeVisita(idNoInicial, visitado);

        cout << "Fecho transitivo direto a partir do No " << idNoInicial << ": ";
        for (int i = 1; i <= this->numNos; i++)
        {
            if (visitado[i] == true && i != idNoInicial)
            {
                cout << i << " ";
            }
        }

    } else
        {
            cout << "Grafo nao e direcionado" << endl;
        }
}

void Grafo::fechoTransitivoIndireto()
{
    if (isDigrafo())
    {
        // codigo
    } else
        {
            cout << "Grafo nao e direcionado" << endl;
        }
}

vector<int> Grafo::Dijkstra(int idNoInicial)
{
    // inicial vetor de distancias e define distancia do No inicial para ele mesmo como 0
    vector<int> distancesDijkstra(this->numNos+1, INFINITO);
    distancesDijkstra[idNoInicial] = 0;

    // fila de pares que representa a distancia e o no antecedente <distancia do No inicial até No X, No que antecede X>
    queue<pair<int, int>> fila;
    fila.push({0, idNoInicial});

    // enquanto fila nao esta vazia
    while (!fila.empty())
    {
        int distance = fila.front().first;
        int vertex = fila.front().second;
        fila.pop();

        // se distancia em distantecDijkstra ja e a menor, continue
        if (distance > distancesDijkstra[vertex])
            continue;
        // se nao:
        // para cada vizinho do vertice atual, calcula nova distancia
        for(int i = 1; i <= this->numNos; i++)
        {
            int newDistance = distance + this->distanceMat[vertex][i];
            if (newDistance < distancesDijkstra[i])
            {
                distancesDijkstra[i] = newDistance;
                fila.push({newDistance, i});
            }
        }
    }

    return distancesDijkstra; // retorna vector de distancias 
}

int Grafo::Floyd(int idNoOrigem, int idNoDestino)
{
    // inicia matriz com todos valores = INFINITO
    vector<vector<int>> distancesFloyd(this->numNos+1, vector<int>(this->numNos+1, INFINITO));

    // seta diagonal = 0 e insere distancias
    for (int i = 1; i <= this->numNos; i++)
    {
        for (int j = 1; j <= this->numNos; j++)
        {
            if (i == j)
            {
                distancesFloyd[i][j] = 0;
                continue;
            }

            distancesFloyd[i][j] = this->distanceMat[i][j];
        }
    }

    for (int k = 1; k <= this->numNos; k++)
    {
        for (int i = 1; i <= this->numNos; i++)
        {
            for (int j = 1; j <= this->numNos; j++)
            {
                if (distancesFloyd[i][j] > distancesFloyd[i][k] + distancesFloyd[k][j])
                {
                    distancesFloyd[i][j] = distancesFloyd[i][k] + distancesFloyd[k][j];
                }
            }
        }
    }

    return distancesFloyd[idNoOrigem][idNoDestino];
}