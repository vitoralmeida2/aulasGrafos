#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <list>
#include "Grafo.h"
#include "Aresta.h"
#include "No.h"

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
    visitado = new int[numVertices+1];
    adjList = new list<No*>[numVertices+1];
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

    delete visitado;
    delete adjList;
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
            removeAresta(noAux, noParaRemover->getIdNo());
            noAux = noAux->getProxNo();
        }
        
        Aresta *arestaParaRemover = noParaRemover->getPrimeiraAresta();
        No *noDestino = NULL;
        
        while (noParaRemover->getPrimeiraAresta() != NULL) // removendo arestas de saida
        {
            noDestino = noParaRemover->getPrimeiraAresta()->getNoDestino();
            removeAresta(noParaRemover, noDestino->getIdNo());
        }

    } else // se nao e Digrafo
        {
            No *noDestino = NULL;

            while (noParaRemover->getPrimeiraAresta() != NULL) // removendo arestas do No
            {
                noDestino = noParaRemover->getPrimeiraAresta()->getNoDestino();
                removeAresta(noParaRemover, noDestino->getIdNo());
                removeAresta(noDestino, noParaRemover->getIdNo());
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
            novaAresta = new Aresta(noDestino, NULL, pesoAresta);
            adjList[idNoOrigem].push_back(noDestino); // adicionando adjacencia na lista

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

            // inc graus
            noFonte->incGrauSaida();
            noDestino->incGrauEntrada();
        } else // se existe
            {
                novaAresta->setPeso(pesoAresta);
            }
    } else // se nao e Digrafo
        {
            if (novaAresta == NULL) // se aresta nao existe
            {
                novaAresta = new Aresta(noDestino, NULL, pesoAresta);
                adjList[idNoOrigem].push_back(noDestino); // adicionando adjacencia na lista

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
                
                Aresta *novaAresta2 = new Aresta(noFonte, NULL, pesoAresta); // aresta sentido contrario 
                adjList[idNoDestino].push_back(noFonte); // adicionando adjacencia na lista

                // ajusta arestas
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
                    novaAresta->setPeso(pesoAresta);

                    Aresta *auxAresta = noDestino->getPrimeiraAresta();
                    while (auxAresta->getNoDestino()->getIdNo() != noFonte->getIdNo())
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
bool Grafo::removeAresta(No *noFonte, int idNoDestino)
{   
    Aresta *arestaParaRemover = noFonte->getPrimeiraAresta();
    Aresta *arestaAnterior = NULL;

    while (arestaParaRemover != NULL) // procurando aresta para remover
    {
        if (arestaParaRemover->getNoDestino()->getIdNo() == idNoDestino) // aresta encontrada
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
        cout << i << " -> ";
        for (No *adj:adjList[i])
        {
            cout << adj->getIdNo() << " -> ";
        }
        cout << "//" << endl;
    }
}

/*
    Funcao que inicializa busca em profundidade
*/
void Grafo::buscaProfundidade(int idNoInicial)
{
    // inicializa todas posicoes do vetor como false
   for (int i = 0; i <= this->numNos; i++)
   {
        visitado[i] = false;
   }

    // visita Nos a partir do idNoInicial
   for (int i = 1; i <= this->numNos; i++)
   {
        if (visitado[i] == false)
        {
            buscaProfundidadeVisita(idNoInicial);
        }
   }
}

/*
    Visita nos em profundidade
*/
void Grafo::buscaProfundidadeVisita(int idNoInicial)
{
    visitado[idNoInicial] = true;
    cout << idNoInicial << endl;

    for (No *adj:adjList[idNoInicial])
    {
        if (visitado[adj->getIdNo()] == false)
        {
            buscaProfundidadeVisita(adj->getIdNo());
        }
    }
}

/*
    Funcao para identificar quantas componentes conexas ha no grafo
*/
void Grafo::componentesConexas()
{
    for (int i = 0; i <= this->numNos; i++)
    {
        visitado[i] = 0;
    }

    int componente = 0;
    for (int i = 1; i <= this->numNos; i++)
    {
        if (visitado[i] == 0)
        {
            componente = componente + 1;
            componentesConexasVisita(i, componente);
        }
    }
}

/*
    Visita nos para identificar componentes conexas
*/
void Grafo::componentesConexasVisita(int v, int marca)
{
    visitado[v] = marca;

    for (No *adj:adjList[v])
    {
        if (visitado[adj->getIdNo()] == 0)
        {
            componentesConexasVisita(adj->getIdNo(), marca);
        }
    }
}