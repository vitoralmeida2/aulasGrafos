#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "Grafo.h"
#include "Aresta.h"
#include "No.h"
#define INFINITO INT32_MAX
#include <fstream>

// --- Construtor ---

Grafo::Grafo(int numVertices, bool isDigrafo, bool pesoNO, bool pesoArc) 
{
    this->ordem = 0;
    this->numNos = numVertices;
    this->numArest = 0;
    this->digrafo = isDigrafo;
    this->pesoNo = pesoNO;
    this->pesoArco = pesoArc;
    this->capacidade = 0;
    this->noRaiz = NULL;
    this->arestaMenorPeso = NULL;
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
    
    delete [] distanceMat;
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
No *Grafo::insereNo(int idNo, float x, float y, int peso) 
{
    No *aux = procurarNoPeloId(idNo);

    // se No nao existe
    if (aux == NULL)
    {
        No *novoNo = new No(idNo, x, y, peso);

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
        noFonte = insereNo(idNoOrigem, 0, 0, 0);
    }

    if (noDestino == NULL)
    {
        noDestino = insereNo(idNoDestino, 0, 0, 0);
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
            arestList.push_back(*novaAresta); // lista de Arestas - Kruskal
            
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
            
            // verifica se e Aresta de menor peso
            if (this->arestaMenorPeso == NULL)
            {
                this->arestaMenorPeso = novaAresta;
            } else
                {
                    if (this->arestaMenorPeso->getPesoAresta() > pesoAresta)
                    {
                        this->arestaMenorPeso = novaAresta;
                    }
                }

            // inc graus
            noFonte->incGrauSaida();
            noDestino->incGrauEntrada();
        } else // se existe
            {
                novaAresta->setPeso(pesoAresta);
                distanceMat[idNoOrigem][idNoDestino] = pesoAresta; // atualizando distancia na matriz de distancias

                // verifica se e Aresta de menor peso
                if (this->arestaMenorPeso->getPesoAresta() > pesoAresta)
                {
                    this->arestaMenorPeso = novaAresta;
                }
            }
    } else // se nao e Digrafo
        {
            if (novaAresta == NULL) // se aresta nao existe
            {
                novaAresta = new Aresta(noFonte, noDestino, pesoAresta, NULL);
                adjList[idNoOrigem].push_back(noDestino); // adicionando adjacencia na lista
                distanceMat[idNoOrigem][idNoDestino] = pesoAresta; // adicionando distancia na matriz de distancias
                arestList.push_back(*novaAresta); // lista de Arestas - Kruskal

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
                
                // verifica se e Aresta de menor peso
                if (this->arestaMenorPeso == NULL)
                {
                    this->arestaMenorPeso = novaAresta;
                } else
                    {
                        if (this->arestaMenorPeso->getPesoAresta() > pesoAresta)
                        {
                            this->arestaMenorPeso = novaAresta;
                        }
                    }
                
                // cria Aresta sentido contrario
                Aresta *novaAresta2 = new Aresta(noDestino, noFonte, pesoAresta, NULL); 
                adjList[idNoDestino].push_back(noFonte); // adicionando adjacencia na lista
                distanceMat[idNoDestino][idNoOrigem] = pesoAresta; // adicionando distancia na matriz de distancias
                arestList.push_back(*novaAresta2); // lista de Arestas - Kruskal

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

                    // verifica se e Aresta de menor peso
                    if (this->arestaMenorPeso->getPesoAresta() > pesoAresta)
                    {
                        this->arestaMenorPeso = novaAresta;
                    }

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
    cout << endl;

    return;
}

/*
    Funcao que inicia busca em profundidade
*/
void Grafo::buscaProfundidade(int idNoInicial)
{
    // inicia vetor de visitados com posicoes = false
    vector<bool> visitado(this->numNos+1, false);
    buscaProfundidadeVisita(idNoInicial, visitado, idNoInicial);
    cout << endl;
}

/*
    Funcao para visitar em profundidade
*/
void Grafo::buscaProfundidadeVisita(int idNoInicial, vector<bool> &visitado, int noAnt)
{
    // Marca o No atual como visitado
    visitado[idNoInicial] = true;
    if (idNoInicial == noAnt)
    {
        cout << "(" << idNoInicial << ")" << " ";
    } else
        {
            cout << "(" << noAnt << ", " << idNoInicial << ")" << " ";
        }
    noAnt = idNoInicial;

    // Percorre todos os vertices adjacentes do vertice atual
    for (No *adj:adjList[idNoInicial])
    {
        // visitado adjacente se nao visitado
        if (visitado[adj->getIdNo()] == false)
        {
            buscaProfundidadeVisita(adj->getIdNo(), visitado, noAnt);
        }
    }
}

/*
    Funcao para realizar a busca em largura para de encontrar excentricidade 
*/
pair<int, int> Grafo::buscaLargura(int idNoInicial)
{
    vector<bool> visitado(this->numNos+1, false);
    vector<int> distancia(this->numNos+1, INFINITO);

    queue<int> fila;
    fila.push(idNoInicial);
    visitado[idNoInicial] = true;
    distancia[idNoInicial] = 0;

    while (!fila.empty())
    {
        int atual = fila.front();
        fila.pop();

        // adicionando nos na fila e calculando distancias
        for (No *adj:adjList[atual])
        {
            if(!visitado[adj->getIdNo()])
            {
                visitado[adj->getIdNo()] = true;
                fila.push(adj->getIdNo());
                distancia[adj->getIdNo()] = distancia[atual] + 1;
            }
        }
    }

    // Encontrar o vertice mais distante do vertice atual
    int maxDistancia = 0;
    int verticeDistante = idNoInicial;
    for (int i = 1; i <= this->numNos; i++)
    {
        if (distancia[i] > maxDistancia)
        {
            maxDistancia = distancia[i];
            verticeDistante = i;
        }
    }

    return {verticeDistante, maxDistancia}; // retorna vertice mais distante de vertice inicial e sua distancia ate ele
}

int Grafo::encontrarRaio()
{
    int raio = INFINITO;
    for (int i = 1; i <= this->numNos; i++)
    {
        pair<int, int> resultado = buscaLargura(i);
        raio = min(raio, resultado.second);
    }

    return raio; // retorna raio do grafo
}

int Grafo::encontrarDiametro()
{
    int diametro = 0;
    for (int i = 1; i <= this->numNos; i++)
    {
        pair<int, int> resultado = buscaLargura(i);
        diametro = max(diametro, resultado.second);
    }

    return diametro; // retorna diametro do grafo
}

vector<int> Grafo::encontrarCentro()
{
    int raio = encontrarRaio();
    vector<int> centros;

    for (int i = 1; i <= this->numNos; i++)
    {
        pair<int, int> resultado = buscaLargura(i);
        if (resultado.second == raio)
        {
            centros.push_back(i);
        }
    }

    return centros; // retorna vertices que sao centros do grafo
}

vector<int> Grafo::encontrarPeriferia()
{
    int diametro = encontrarDiametro();
    vector<int> periferia;

    for (int i = 1; i <= this->numNos; i++)
    {
        pair<int, int> resultado = buscaLargura(i);
        if (resultado.second == diametro)
        {
            periferia.push_back(i);
        }
    }

    return periferia; // retorna vertices que sao periferia do grafo
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
        cout << "O grafo contem um ciclo. Nao e possivel ordenar topologicamente." << endl;
        return;
    }

    if (!isDigrafo())
    {
        cout << "O grafo nao e direcionado." << endl;
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

    return;
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

    return;
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
        fechoTransitivoDiretoVisita(idNoInicial, visitado);

        cout << "Fecho transitivo direto a partir do No " << idNoInicial << ": ";
        for (int i = 1; i <= this->numNos; i++)
        {
            if (visitado[i] == true && i != idNoInicial)
            {
                cout << "(" << i << ")" << " ";
            }
        }
        cout << endl;

    } else
        {
            cout << "Grafo nao e direcionado" << endl;
        }

    return;
}

/*
    Funcao para visitar fecho transitivo direto
*/
void Grafo::fechoTransitivoDiretoVisita(int idNoInicial, vector<bool> &visitado)
{
    // Marca o No atual como visitado
    visitado[idNoInicial] = true;

    // Percorre todos os vertices adjacentes do vertice atual
    for (No *adj:adjList[idNoInicial])
    {
        // visitado adjacente se nao visitado
        if (visitado[adj->getIdNo()] == false)
        {
            fechoTransitivoDiretoVisita(adj->getIdNo(), visitado);
        }
    }
}

/*
    Funcao para encontrar fechamentro transitivo indireto usando floyd-warshall
*/
void Grafo::fechoTransitivoIndireto(int idNoInicial)
{
    if (isDigrafo())
    {
        // inicia matriz distancias
        vector<vector<int>> fechamento(this->numNos+1, vector<int>(this->numNos+1, INFINITO));
        for (int i = 1; i <= this->numNos; i++)
        {
            for (int j = 1; j <= this->numNos; j++)
            {
                fechamento[i][j] = distanceMat[i][j];
            }
        }

        // implementa algoritmo floyd
        for (int k = 1; k <= this->numNos; k++)
        {
            for (int i = 1; i <= this->numNos; i++)
            {
                for (int j = 1; j <= this->numNos; j++)
                {
                    fechamento[i][j] = fechamento[i][j] || (fechamento[i][k] && fechamento[k][j]);
                }
            }
        }

        // imprime Nos que existe caminho para idNoInicial
        cout << "Fechamento transitivo indireto para o No" << idNoInicial << ": ";
        for (int i = 1; i <= this->numNos; i++)
        {
            if (fechamento[idNoInicial][i] != INFINITO)
            {
                if (idNoInicial == i) continue;
                cout << "(" << i << ")" << " ";
            }
        }
        cout << endl;

    } else
        {
            cout << "Grafo nao e direcionado" << endl;
        }

    return;
}

/*
    Algoritimo de Dijkstra usado para retorna as distancias entre o idNoInicial e todos os outros vertices
*/
int Grafo::Dijkstra(int idNoInicial, int idNoDestino)
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

    return distancesDijkstra[idNoDestino]; // retorna distancia idNoInicial - idNoDestino
}

/*
    Algoritmo de Floyd para retornar o caminho minimo entre dois Vertices
*/
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

    // Algoritmo de Floyd
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

    return distancesFloyd[idNoOrigem][idNoDestino]; // retorna distancia origem - destino
}

/*
    Algoritmo de Prim para encontrar Arvore Geradora Minima do grafo nao orientado
*/
void Grafo::Prim(int idNoInicial)
{
    int pesoAGM = 0;
    vector<Aresta> AGM; // conjunto de Arestas que representa a arvore
    vector<bool> visited(this->numNos+1, false); // vetor visitados
    priority_queue<Aresta> pq; // min Heap para ordenar arestas com base em seu peso

    No *noAux = procurarNoPeloId(idNoInicial);
    Aresta *arestAux = noAux->getPrimeiraAresta();

    // adiciona arestas saindo do vertice inicial na minHeap
    visited[idNoInicial] = true;
    while (arestAux != nullptr)
    {
        pq.push(*arestAux);
        arestAux = arestAux->getProxAresta();
    }

    // enquanto minHeap nao estiver vazia
    while (!pq.empty())
    {
        Aresta arestAux2 = pq.top();
        pq.pop();

        int vertex = arestAux2.getIdNoDestino();

        // se formar ciclo, continua para verificar proxima aresta
        if (visited[vertex])
            continue;

        // se nao, marca como visitado e adiciona na AGM
        visited[vertex] = true;
        AGM.push_back(arestAux2);

        // arestas para os adjacentes de vertex sao adicionadas na minHeap
        No *noAux2 = procurarNoPeloId(vertex);
        arestAux = noAux2->getPrimeiraAresta();

        while (arestAux != NULL)
        {
            pq.push(*arestAux);
            arestAux = arestAux->getProxAresta();
        }
    }

    // Imprimindo AGM
    cout << "Arvore geradora minima - Prim: ";
    for (Aresta arest:AGM)
    {
        cout << "(" << arest.getIdNoOrigem() << ", " << arest.getIdNoDestino() << ")" << " ";
        pesoAGM = pesoAGM + arest.getPesoAresta();
    }
    cout << endl << "Peso AGM: " << pesoAGM << endl;

    return;
}

void Grafo::Kruskal()
{
    int pesoAGM = 0;
    vector<Aresta> AGM;
    sort(arestList.begin(), arestList.end());

    int *parent = new int[this->numNos+1];
    for (int i = 0; i < this->numNos; i++)
    {
        parent[i] = -1;
    }

    for (Aresta arest:arestList)
    {
        int origem = arest.getIdNoOrigem();
        int dest = arest.getIdNoDestino();

        int conjuntoOrigem = encontrarConjunto(parent, origem);
        int conjuntoDest = encontrarConjunto(parent, dest);

        if (conjuntoOrigem != conjuntoDest)
        {
            AGM.push_back(arest);
            unirConjunto(parent, conjuntoOrigem, conjuntoDest);
        }
    }

    // Imprime AGM
    cout << "Arvore geradora minima - Kruskal: ";
    for (Aresta arest:AGM)
    {
        cout << "(" << arest.getIdNoOrigem() << ", " << arest.getIdNoDestino() << ")" << " ";
        pesoAGM = pesoAGM + arest.getPesoAresta();
    }
    cout << endl << "Peso AGM: " << pesoAGM << endl;

    return;
}

/*
    Funcoes Auxiliares Kruskal
*/

int Grafo::encontrarConjunto(int parent[], int i)
{
    if (parent[i] == -1)
    {
        return i;
    }
    return encontrarConjunto(parent, parent[i]);
}

void Grafo::unirConjunto(int parent[], int x, int y)
{
    int conjuntoX = encontrarConjunto(parent, x);
    int conjuntoY = encontrarConjunto(parent, y);
    parent[conjuntoX] = conjuntoY;

    return;
}

void Grafo::nosArticulacao()
{
    vector<int> desc(this->numNos+1, -1);
    vector<int> low(this->numNos+1, -1);
    vector<bool> visitado(this->numNos+1, false);
    vector<int> pai(this->numNos+1, -1);
    vector<bool> pontoArticulacao(this->numNos+1, false);

    int tempo = 0;

    for (int i = 1; i <= this->numNos; i++)
    {
        if (!visitado[i])
        {
            nosArticulacaoVisita(i, desc, low, visitado, pai, pontoArticulacao, tempo);
        }
    }

    cout << "Vertices de articulacao: ";
    for (int i = 1; i<= this->numNos; i++)
    {
        if(pontoArticulacao[i])
        {
            cout << "(" << i << ")" << " ";
        }
    }

    return;
}

void Grafo::nosArticulacaoVisita(int v, vector<int> &desc, vector<int> &low, vector<bool> &visitado, vector<int> &pai, vector<bool> &pontoArticulacao, int &tempo)
{
    int filhos = 0;

    visitado[v] = true;
    desc[v] = low[v] = ++tempo;

    for (No *adj:adjList[v])
    {
        if (!visitado[adj->getIdNo()])
        {
            filhos++;
            pai[adj->getIdNo()] = v;
            nosArticulacaoVisita(adj->getIdNo(), desc, low, visitado, pai, pontoArticulacao, tempo);

            low[v] = min(low[v], low[adj->getIdNo()]);

            if (pai[v] == -1 && filhos > 1)
            {
                pontoArticulacao[v] = true;
            }

            if (pai[v] != -1 && low[adj->getIdNo()] >= desc[v])
            {
                pontoArticulacao[v] = true;
            } else
                {
                    if (adj->getIdNo() != pai[v])
                    {
                        low[v] = min(low[v], desc[adj->getIdNo()]);
                    }
                }
        }
    }
}

/*
    Auxliares CVRP
*/

void Grafo::setInstanceName(string name)
{
    this->instanceName = name;
}

string Grafo::getInstanceName()
{
    return this->instanceName;
}

void Grafo::atualizaPesoNos(int idNo, int novoPeso)
{
    No *noAux;
    noAux = procurarNoPeloId(idNo);
    noAux->setPesoNo(novoPeso);
}

void Grafo::setCapacidade(int capacity)
{
    this->capacidade = capacity;
}

int Grafo::getCapacidade()
{
    return this->capacidade;
}

void Grafo::setVeiculos(int num)
{
    this->numVeiculos = num;
}

int Grafo::getVeiculos()
{
    return this->numVeiculos;
}

vector<No*> Grafo::getNos()
{
    vector<No*> nos;
    No *auxNo = this->noRaiz;

    while (auxNo != NULL)
    {
        nos.push_back(auxNo);
        auxNo = auxNo->getProxNo();
    }
    reverse(nos.begin(), nos.end());

    return nos;    
}

void Grafo::setNosNaoVisitados(vector<No*> clientes)
{
    for (No* cl:clientes)
    {
        cl->setVisitado(false);
    }
}

/*
    --- Gulosos ---
*/

double Grafo::distance(No *a, No *b)
{
    return sqrt(pow(a->getCordX() - b->getCordX(), 2) + pow(a->getCordY() - b->getCordY(), 2));
};

double Grafo::calculaDistanciaRota(vector<No*> rota)
{
    double totalDistance = 0.0;

    for (int i = 1; i < rota.size(); i++)
    {
        totalDistance += distance(rota[i-1], rota[i]);
    }

    return totalDistance;
}

void Grafo::adicionArestasDaRota(vector<Rota> rotas)
{
    double distancia = 0.0;
    for(Rota rota : rotas) {
        for (int i = 1; i < rota.clientes.size(); i++) {
            distancia = distance(rota.clientes[i-1], rota.clientes[i]);
            this->insereAresta(rota.clientes[i-1]->getIdNo(), rota.clientes[i]->getIdNo(), distancia);
        }
    }
}

double Grafo::calculateSolutionCost(Solution &sol)
{
    double custoTotal = 0.0;
    for (auto rota:sol.rotas)
    {
        custoTotal += calculaDistanciaRota(rota.clientes);
    }

    return custoTotal;
}

int Grafo::encontraClienteProximo(No *clienteAtual, vector<No*> clientes)
{
    int clienteProximo = -1;
    double minDistance = INFINITO;

    for (int i = 0; i < clientes.size(); i++)
    {
        if (!clientes[i]->getVisitado())
        {
            double dist = distance(clienteAtual, clientes[i]);
            if (dist < minDistance)
            {
                minDistance = dist;
                clienteProximo = i;
            }
        }
    }

    return clienteProximo;
}

No* Grafo::encontraProxClienteAleatorio(vector<No*> clientesRestantes, No *clienteAtual, double alpha, int capacidadeUsada)
{
    vector<No*> clientesNaoVisitados;

    for (int i = 0; i < clientesRestantes.size(); i++)
    {
        if(!clientesRestantes[i]->getVisitado() && (capacidadeUsada + clientesRestantes[i]->getPeso() <= this->getCapacidade()))
        {
           clientesNaoVisitados.push_back(clientesRestantes[i]);
        }
    }

    if (clientesNaoVisitados.empty())
    {
        return NULL; // nao ha clientes nao visitados
    }

    sort(clientesNaoVisitados.begin(), clientesNaoVisitados.end(), [this,clienteAtual](No* a, No* b) {
        return distance(clienteAtual, a) < distance(clienteAtual, b);
    });

    // Calcula novo tamanho
    size_t novoTamanho = static_cast<size_t>(alpha * clientesNaoVisitados.size());

    // Reajusta tamanho vector
    clientesNaoVisitados.resize(novoTamanho);

    random_shuffle(clientesNaoVisitados.begin(), clientesNaoVisitados.end());

    return clientesNaoVisitados[0];
}

Solution Grafo::gulosoCVRP(ofstream& arquivo)
{
    Solution sol;
    sol.cost = 0.0;
    vector<No*> clientesRestante = this->getNos();
    vector<Rota> rotas(this->getVeiculos());

    for (int i = 0; i < this->getVeiculos(); i++)
    {
        Rota &rotaTemp = rotas[i];
        rotaTemp.capacityUsed = 0;

        // Iniciando no deposito
        rotaTemp.clientes.push_back(clientesRestante[0]);
        clientesRestante[0]->setVisitado(true);

        while (true) // constroi rotas
        {
            int clienteProximo = encontraClienteProximo(rotaTemp.clientes.back(), clientesRestante);
            
            if (clienteProximo == -1)
                break; // nao ha Clientes nao visitados

            No *clientProx = clientesRestante[clienteProximo];

            // testando capacidade
            if (rotaTemp.capacityUsed + clientProx->getPeso() <= this->getCapacidade())
            {
                rotaTemp.capacityUsed += clientProx->getPeso();
                rotaTemp.clientes.push_back(clientProx);
                clientesRestante.erase(clientesRestante.begin() + clienteProximo);
                clientProx->setVisitado(true);
            } else
                {
                    break; // rota cheia passa pro proximo veiculo
                }
        }
        rotaTemp.clientes.push_back(clientesRestante[0]);
    }

    sol.rotas = rotas;
    sol.clientesRestantes = clientesRestante;
    sol.cost = calculateSolutionCost(sol);

    this->adicionArestasDaRota(sol.rotas);

    // Imprimindo rotas
    //arquivo.open("main.cpp", ios::app);
    cout << "Instancia: " << this->getInstanceName() << endl;
    if (arquivo.is_open()) { 
        arquivo << "Resultados Instancia " << this->getInstanceName() << " em:" << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "   RESULTADOS         Guloso   " << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "ROTAS: " << endl;
    }
    for (int i = 0; i < this->numVeiculos; i++)
    {
        cout << "Rota #" << i+1 << ": ";
        if (arquivo.is_open()) { 
            arquivo << "Rota #" << i+1 << ": ";
        }
        for (No *client:sol.rotas[i].clientes)
        {
            cout << client->getIdNo() << " ";
            if (arquivo.is_open()) { 
                arquivo << client->getIdNo() << " ";
            }
        } 
        if (arquivo.is_open()) { 
            arquivo << "" << endl;
        }
        cout << endl;
    }
    cout << "Custo total: " << sol.cost << endl << endl;
    if (arquivo.is_open()) { 
        arquivo << "Custo total: " << sol.cost << endl;
    }
    return sol;
}

Solution Grafo::gulosoRandomizadoCVRP(double alpha, ofstream& arquivo)
{
    Solution bestSolution;
    bestSolution.cost = INFINITO;
    vector<No*> clientes = this->getNos();

    for (int exec = 0; exec < 1000; exec++) // numero de execucoes do algoritmo
    {
        Solution solAtual;
        vector<No*> clientesRestante = clientes;
        setNosNaoVisitados(clientesRestante);
        vector<Rota> rotas(this->getVeiculos());

        for (int i = 0; i < this->getVeiculos(); i++)
        {
            Rota &rotaTemp = rotas[i];
            rotaTemp.capacityUsed = 0;

            // Iniciando no deposito
            rotaTemp.clientes.push_back(clientesRestante[0]);
            clientesRestante[0]->setVisitado(true);

            while (true)
            {
                // Escolhendo aleatoriamente um cliente proximo nao visitado
                No* randomClient = encontraProxClienteAleatorio(clientesRestante, rotaTemp.clientes.back(), alpha, rotaTemp.capacityUsed);

                if (randomClient == NULL)
                    break; // nao ha clientes nao visitados

                // testando capacidade
                if (rotaTemp.capacityUsed + randomClient->getPeso() <= this->getCapacidade())
                {
                    rotaTemp.capacityUsed += randomClient->getPeso();
                    rotaTemp.clientes.push_back(randomClient);
                    clientesRestante.erase(remove_if(clientesRestante.begin(), clientesRestante.end(), [randomClient](No* no) {
                        return no->getIdNo() == randomClient->getIdNo();
                    }), clientesRestante.end());
                    randomClient->setVisitado(true);
                } else
                    {
                        break; // rota cheia passa pro proximo veiculo
                    }
            }
            // adicionando retorno ao deposito
            rotaTemp.clientes.push_back(clientes[0]);
        }

        // comparando solucao atual com a melhor solucao
        solAtual.rotas = rotas;
        solAtual.clientesRestantes = clientesRestante;
        solAtual.cost = calculateSolutionCost(solAtual);

        if (solAtual.cost < bestSolution.cost && solAtual.clientesRestantes.size() == 1) // se solucao atual tem o custo menor que a melhor solucao e se tem como clientes restantes somete o deposito
        {
            bestSolution.cost = solAtual.cost;
            bestSolution.rotas = solAtual.rotas;
            bestSolution.clientesRestantes = solAtual.clientesRestantes;
        }
    }

    this->adicionArestasDaRota(bestSolution.rotas);

    // Imprimindo rotas
    cout << "Instancia: " << this->getInstanceName() << endl;
    if (arquivo.is_open()) { 
        arquivo << "Resultados Instancia " << this->getInstanceName() << " em:" << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "   RESULTADOS         Guloso Randomizado Alfa    " <<  alpha << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "ROTAS: " << endl;
    }
    for (int i = 0; i < this->numVeiculos; i++)
    {
        cout << "Rota #" << i+1 << ": ";
        if (arquivo.is_open()) { 
            arquivo << "Rota #" << i+1 << ": ";
        }
        for (No *client:bestSolution.rotas[i].clientes)
        {
            cout << client->getIdNo() << " ";
            if (arquivo.is_open()) { 
                arquivo << client->getIdNo() << " ";
            }
        }
        if (arquivo.is_open()) { 
            arquivo << "" << endl;
        }
        cout << endl;
    }
    if (arquivo.is_open()) { 
        arquivo << "Custo total: " << bestSolution.cost << endl;
    }
    cout << "Custo total: " << bestSolution.cost << endl << endl;

    return bestSolution;
}

/* ===========================================================================================================================*/

void Grafo::atualizarProbabilidade(Probabilidade* alfaAtual, double melhorDistanciaTotal, double distanciaRota, bool valida) {
    bool aumentaProb = false;
    if(valida && (distanciaRota < (melhorDistanciaTotal + 20))) {
        aumentaProb = true;
    }

    if(aumentaProb && (distanciaRota < melhorDistanciaTotal)) {
        alfaAtual->probabilidade = alfaAtual->probabilidade * 1.3;
    } else if (valida && aumentaProb) {
        alfaAtual->probabilidade = alfaAtual->probabilidade * 1.1; // Aumenta a probabilidade em 10%
    } else {
        alfaAtual->probabilidade = alfaAtual->probabilidade * 0.9;  // Diminui a probabilidade em 10%
    }
}

void Grafo::normalizarProbabilidades(vector<Probabilidade*> probabilidades) {
    double soma = 0.0;

    // Calcular a soma total das probabilidades
    for (auto* prob : probabilidades) {
        soma += prob->probabilidade;
    }

    // Normalizar as probabilidades
    for (auto* prob : probabilidades) {
        prob->probabilidade = prob->probabilidade / soma;
    }
}

Probabilidade* Grafo::escolheAlfaAleatorio(vector<Probabilidade*> probabilidadeAlfa) {
    double numeroAleatorio = (double)rand() / RAND_MAX;
    double acumulado = 0.0;

    for (const auto& prob : probabilidadeAlfa) {
        acumulado += prob->probabilidade;
        if (numeroAleatorio <= acumulado) {
            return prob;
        }
    }

    return nullptr;
}


Solution Grafo::gulosoRandomizadoReativoCVRP(vector<Probabilidade*> alfas, ofstream& arquivo) // Reativo com quantidade de veiculos
{
    Solution bestSolution;
    bestSolution.cost = INFINITO;
    vector<No*> clientes = this->getNos();

    for (int exec = 0; exec < 1000; exec++) // numero de execucoes do algoritmo
    {
        Solution solAtual;
        vector<No*> clientesRestante = clientes;
        setNosNaoVisitados(clientesRestante);
        vector<Rota> rotas(this->getVeiculos());
        Probabilidade* alfaEscolhido = this->escolheAlfaAleatorio(alfas);

        for (int i = 0; i < this->getVeiculos(); i++)
        {
            Rota &rotaTemp = rotas[i];
            rotaTemp.capacityUsed = 0;

            // Iniciando no deposito
            rotaTemp.clientes.push_back(clientesRestante[0]);
            clientesRestante[0]->setVisitado(true);

            while (true)
            {
                // Escolhendo aleatoriamente um cliente proximo nao visitado
                No* randomClient = encontraProxClienteAleatorio(clientesRestante, rotaTemp.clientes.back(), alfaEscolhido->alfa, rotaTemp.capacityUsed);

                if (randomClient == NULL)
                    break; // nao ha clientes nao visitados

                // testando capacidade
                if (rotaTemp.capacityUsed + randomClient->getPeso() <= this->getCapacidade())
                {
                    rotaTemp.capacityUsed += randomClient->getPeso();
                    rotaTemp.clientes.push_back(randomClient);
                    clientesRestante.erase(remove_if(clientesRestante.begin(), clientesRestante.end(), [randomClient](No* no) {
                        return no->getIdNo() == randomClient->getIdNo();
                    }), clientesRestante.end());
                    randomClient->setVisitado(true);
                } else
                    {
                        break; // rota cheia passa pro proximo veiculo
                    }
            }
            // adicionando retorno ao deposito
            rotaTemp.clientes.push_back(clientes[0]);
        }

        // comparando solucao atual com a melhor solucao
        solAtual.rotas = rotas;
        solAtual.clientesRestantes = clientesRestante;
        solAtual.cost = calculateSolutionCost(solAtual);

        this->atualizarProbabilidade(alfaEscolhido, bestSolution.cost, solAtual.cost, solAtual.clientesRestantes.size() == 1);
        normalizarProbabilidades(alfas);

        if (solAtual.cost < bestSolution.cost && solAtual.clientesRestantes.size() == 1) // se solucao atual tem o custo menor que a melhor solucao e se tem como clientes restantes somete o deposito
        {
            bestSolution.cost = solAtual.cost;
            bestSolution.rotas = solAtual.rotas;
            bestSolution.clientesRestantes = solAtual.clientesRestantes;
            bestSolution.bestAlfa = alfaEscolhido->alfa;
        }
    }

    this->adicionArestasDaRota(bestSolution.rotas);

    // Imprimindo rotas
    cout << "Instancia: " << this->getInstanceName() << endl;
     if (arquivo.is_open()) { 
        arquivo << "Resultados Instancia " << this->getInstanceName() << " em:" << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "   RESULTADOS        Guloso Randomizado Reativo    " << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "ROTAS: " << endl;
    }
    for (int i = 0; i < this->numVeiculos; i++)
    {
        cout << "Rota #" << i+1 << ": ";
        if (arquivo.is_open()) { 
            arquivo << "Rota #" << i+1 << ": ";
        }
        for (No *client:bestSolution.rotas[i].clientes)
        {
            cout << client->getIdNo() << " ";
            if (arquivo.is_open()) { 
                arquivo << client->getIdNo() << " ";
            }
        }
        if (arquivo.is_open()) { 
            arquivo << "" << endl;
        }
        cout << endl;
    }
    if (arquivo.is_open()) { 
        arquivo << "Custo total: " << bestSolution.cost << endl;
        arquivo << "Melhor alpha: " << bestSolution.bestAlfa << endl;
    }
    cout << "Custo total: " << bestSolution.cost << endl;
    cout << "Melhor aplha: " << bestSolution.bestAlfa << endl;

    return bestSolution;
}

Solution Grafo::guloso(ofstream& arquivo) // Sem quantidade de veiculos, minimo de rotas necessarias
{
    Solution sol;
    sol.cost = 0.0;
    vector<No*> clientesRestante = this->getNos();
    vector<Rota> rotas;

    while (clientesRestante.size() > 1)
    {
        Rota rotaTemp;
        rotaTemp.capacityUsed = 0;

        // Iniciando no deposito
        rotaTemp.clientes.push_back(clientesRestante[0]);
        clientesRestante[0]->setVisitado(true);

        while (rotaTemp.capacityUsed <= this->getCapacidade()) // constroi rotas
        {
            int clienteProximo = encontraClienteProximo(rotaTemp.clientes.back(), clientesRestante);
            
            if (clienteProximo == -1)
                break; // nao ha Clientes nao visitados

            No *clientProx = clientesRestante[clienteProximo];

            // testando capacidade
            if (rotaTemp.capacityUsed + clientProx->getPeso() <= this->getCapacidade())
            {
                rotaTemp.capacityUsed += clientProx->getPeso();
                rotaTemp.clientes.push_back(clientProx);
                clientesRestante.erase(clientesRestante.begin() + clienteProximo);
                clientProx->setVisitado(true);
            } else
                {
                    break; // rota cheia passa pro proximo veiculo
                }
        }
        rotaTemp.clientes.push_back(clientesRestante[0]);
        rotas.push_back(rotaTemp);
    }

    sol.rotas = rotas;
    sol.clientesRestantes = clientesRestante;
    sol.cost = calculateSolutionCost(sol);

    this->adicionArestasDaRota(sol.rotas);

    // Imprimindo rotas
    //arquivo.open("main.cpp", ios::app);
    int cont = 1;
    cout << "Instancia: " << this->getInstanceName() << endl;
    if (arquivo.is_open()) { 
        arquivo << "Resultados Instancia " << this->getInstanceName() << " em:" << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "   RESULTADOS         Guloso   " << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "ROTAS: " << endl;
    }
    for (auto route:sol.rotas)
    {
        cout << "Rota #" << cont << ": ";
        if (arquivo.is_open()) { 
            arquivo << "Rota #" << cont << ": ";
        }
        for (No *client:route.clientes)
        {
            cout << client->getIdNo() << " ";
            if (arquivo.is_open()) { 
                arquivo << client->getIdNo() << " ";
            }
        } 
        if (arquivo.is_open()) { 
            arquivo << "" << endl;
        }
        cont++;
        cout << endl;
    }
    cout << "Custo total: " << sol.cost << endl << endl;
    if (arquivo.is_open()) { 
        arquivo << "Custo total: " << sol.cost << endl;
    }
    return sol;
}

Solution Grafo::randomizado(double alpha, ofstream& arquivo) // Sem quantidade de veiculos, minimo de rotas necessarias
{
    Solution bestSolution;
    bestSolution.cost = INFINITO;
    vector<No*> clientes = this->getNos();

    for (int exec = 0; exec < 1000; exec++) // numero de execucoes do algoritmo
    {
        Solution solAtual;
        vector<No*> clientesRestante = clientes;
        setNosNaoVisitados(clientesRestante);
        vector<Rota> rotas(this->getVeiculos());

        while (clientesRestante.size() > 1)
        {
            Rota rotaTemp;
            rotaTemp.capacityUsed = 0;

            // Iniciando no deposito
            rotaTemp.clientes.push_back(clientesRestante[0]);
            clientesRestante[0]->setVisitado(true);

            while (rotaTemp.capacityUsed <= this->getCapacidade())
            {
                // Escolhendo aleatoriamente um cliente proximo nao visitado
                No* randomClient = encontraProxClienteAleatorio(clientesRestante, rotaTemp.clientes.back(), alpha, rotaTemp.capacityUsed);

                if (randomClient == NULL)
                    break; // nao ha clientes nao visitados

                // testando capacidade
                if (rotaTemp.capacityUsed + randomClient->getPeso() <= this->getCapacidade())
                {
                    rotaTemp.capacityUsed += randomClient->getPeso();
                    rotaTemp.clientes.push_back(randomClient);
                    clientesRestante.erase(remove_if(clientesRestante.begin(), clientesRestante.end(), [randomClient](No* no) {
                        return no->getIdNo() == randomClient->getIdNo();
                    }), clientesRestante.end());
                    randomClient->setVisitado(true);
                } else
                    {
                        break; // rota cheia passa pro proximo veiculo
                    }
            }
            // adicionando retorno ao deposito
            rotaTemp.clientes.push_back(clientes[0]);
            rotas.push_back(rotaTemp);
        }

        // comparando solucao atual com a melhor solucao
        solAtual.rotas = rotas;
        solAtual.clientesRestantes = clientesRestante;
        solAtual.cost = calculateSolutionCost(solAtual);

        if (solAtual.cost < bestSolution.cost && solAtual.clientesRestantes.size() == 1) // se solucao atual tem o custo menor que a melhor solucao e se tem como clientes restantes somete o deposito
        {
            bestSolution.cost = solAtual.cost;
            bestSolution.rotas = solAtual.rotas;
            bestSolution.clientesRestantes = solAtual.clientesRestantes;
        }
    }


    this->adicionArestasDaRota(bestSolution.rotas);

    // Imprimindo rotas
    int cont = 1;
    cout << "Instancia: " << this->getInstanceName() << endl;
    if (arquivo.is_open()) { 
        arquivo << "Resultados Instancia " << this->getInstanceName() << " em:" << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "   RESULTADOS         Guloso Randomizado Alfa    " <<  alpha << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "ROTAS: " << endl;
    }
    for (auto route:bestSolution.rotas)
    {
        cout << "Rota #" << cont << ": ";
        if (arquivo.is_open()) { 
            arquivo << "Rota #" << cont << ": ";
        }
        for (No *client:route.clientes)
        {
            cout << client->getIdNo() << " ";
            if (arquivo.is_open()) { 
                arquivo << client->getIdNo() << " ";
            }
        }
        if (arquivo.is_open()) { 
            arquivo << "" << endl;
        }
        cont++;
        cout << endl;
    }
    if (arquivo.is_open()) { 
        arquivo << "Custo total: " << bestSolution.cost << endl;
    }
    cout << "Custo total: " << bestSolution.cost << endl << endl;

    return bestSolution;
}

Solution Grafo::reativo(vector<Probabilidade*> alfas, ofstream& arquivo) // Sem quantidade de veiculos, minimo de rotas necessarias
{
    Solution bestSolution;
    bestSolution.cost = INFINITO;
    vector<No*> clientes = this->getNos();

    for (int exec = 0; exec < 1000; exec++) // numero de execucoes do algoritmo
    {
        Solution solAtual;
        vector<No*> clientesRestante = clientes;
        setNosNaoVisitados(clientesRestante);
        vector<Rota> rotas(this->getVeiculos());
        Probabilidade* alfaEscolhido = this->escolheAlfaAleatorio(alfas);

        while (clientesRestante.size() > 1)
        {
            Rota rotaTemp;
            rotaTemp.capacityUsed = 0;

            // Iniciando no deposito
            rotaTemp.clientes.push_back(clientesRestante[0]);
            clientesRestante[0]->setVisitado(true);

            while (rotaTemp.capacityUsed <= this->getCapacidade())
            {
                // Escolhendo aleatoriamente um cliente proximo nao visitado
                No* randomClient = encontraProxClienteAleatorio(clientesRestante, rotaTemp.clientes.back(), alfaEscolhido->alfa, rotaTemp.capacityUsed);

                if (randomClient == NULL)
                    break; // nao ha clientes nao visitados

                // testando capacidade
                if (rotaTemp.capacityUsed + randomClient->getPeso() <= this->getCapacidade())
                {
                    rotaTemp.capacityUsed += randomClient->getPeso();
                    rotaTemp.clientes.push_back(randomClient);
                    clientesRestante.erase(remove_if(clientesRestante.begin(), clientesRestante.end(), [randomClient](No* no) {
                        return no->getIdNo() == randomClient->getIdNo();
                    }), clientesRestante.end());
                    randomClient->setVisitado(true);
                } else
                    {
                        break; // rota cheia passa pro proximo veiculo
                    }
            }
            // adicionando retorno ao deposito
            rotaTemp.clientes.push_back(clientes[0]);
            rotas.push_back(rotaTemp);
        }

        // comparando solucao atual com a melhor solucao
        solAtual.rotas = rotas;
        solAtual.clientesRestantes = clientesRestante;
        solAtual.cost = calculateSolutionCost(solAtual);

        this->atualizarProbabilidade(alfaEscolhido, bestSolution.cost, solAtual.cost, solAtual.clientesRestantes.size() == 1);
        normalizarProbabilidades(alfas);

        if (solAtual.cost < bestSolution.cost && solAtual.clientesRestantes.size() == 1) // se solucao atual tem o custo menor que a melhor solucao e se tem como clientes restantes somete o deposito
        {
            bestSolution.cost = solAtual.cost;
            bestSolution.rotas = solAtual.rotas;
            bestSolution.clientesRestantes = solAtual.clientesRestantes;
            bestSolution.bestAlfa = alfaEscolhido->alfa;
        }
    }

    this->adicionArestasDaRota(bestSolution.rotas);

    // Imprimindo rotas
    int cont = 1;
    cout << "Instancia: " << this->getInstanceName() << endl;
     if (arquivo.is_open()) { 
        arquivo << "Resultados Instancia " << this->getInstanceName() << " em:" << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "   RESULTADOS        Guloso Randomizado Reativo    " << endl;
        arquivo << "====================================================================" << endl;
        arquivo << "ROTAS: " << endl;
    }
    for (auto route:bestSolution.rotas)
    {
        cout << "Rota #" << cont << ": ";
        if (arquivo.is_open()) { 
            arquivo << "Rota #" << cont << ": ";
        }
        for (No *client:route.clientes)
        {
            cout << client->getIdNo() << " ";
            if (arquivo.is_open()) { 
                arquivo << client->getIdNo() << " ";
            }
        }
        if (arquivo.is_open()) { 
            arquivo << "" << endl;
        }
        cont++;
        cout << endl;
    }
    if (arquivo.is_open()) { 
        arquivo << "Custo total: " << bestSolution.cost << endl;
        arquivo << "Melhor alpha: " << bestSolution.bestAlfa << endl;
    }
    cout << "Custo total: " << bestSolution.cost << endl;
    cout << "Melhor aplha: " << bestSolution.bestAlfa << endl;

    return bestSolution;
}