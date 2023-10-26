#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include "Grafo.h"
#include "Aresta.h"
#include "No.h"

using namespace std;

// --- Construtor ---

Grafo::Grafo(bool isDigrafo, bool pesoNO, bool pesoArc) 
{
    this->ordem = 0;
    this->noRaiz = NULL;
    this->digrafo = isDigrafo;
    this->pesoNo = pesoNO;
    this->pesoArco = pesoArc;
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
*/
No *Grafo::insereNo(int idNo, int peso) 
{
    No *aux = procurarNoPeloId(idNo);

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
    } else
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
    
    // Remove o no
    
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
            } else // se existe
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

    while (arestaParaRemover != NULL) // procurando aresta
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
    int numArest = 0;

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