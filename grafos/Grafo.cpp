#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include "Grafo.h"

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
    se encontrar - return true
    se nao encontrar - return false
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

    return NULL;
}

/*
    removeNo -
    Remove No do Grafo
*/
bool Grafo::removeNo(int idNo, bool isDigrafo) 
{
    // Pesquisa o No a ser excluido

    No *noParaRemover = procurarNoPeloId(idNo);

    if (noParaRemover == NULL)
    {
        return false; // No nao esta no grafo
    }

    // Remove todas as arestas/arcos onde este nó ocorre


    if (!isDigrafo) // se nao e digrafo
    {
        Aresta *arestaParaRemover = noParaRemover->getPrimeiraAresta();
        Aresta *auxAresta = NULL;

        while (noParaRemover->getPrimeiraAresta() != NULL) // removendo arestas 
        {
            No *noDestino = arestaParaRemover->getNoDestino();
            removeAresta(noParaRemover, noDestino->getIdNo());
            removeAresta(noDestino, noParaRemover->getIdNo());
            arestaParaRemover = noParaRemover->getPrimeiraAresta();
        }
    } else // se e digrafo
        {
            No *noAux = noRaiz;
            while (noParaRemover->getGrauEntrada() != 0 && noAux != NULL) // removendo arestas de entrada
            {
                removeAresta(noAux, noParaRemover->getIdNo());
                noAux = noAux->getProxNo();
            }

            Aresta *arestaParaRemover = noParaRemover->getPrimeiraAresta();
            No *noDestino = NULL;

            while (noParaRemover->getPrimeiraAresta() != NULL)
            {
                noDestino = arestaParaRemover->getNoDestino();
                removeAresta(noParaRemover, noDestino->getIdNo());
                arestaParaRemover = arestaParaRemover->getProxAresta();
            }
        }

    // Remove o no

    No *noAux = noRaiz;
    while (noAux->getProxNo() != noParaRemover)
    {
        noAux = noAux->getProxNo();
    }

    noAux->setProxNo(noParaRemover->getProxNo());
    delete noParaRemover;
    this->decOrdem();

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

    if (isDigrafo()) // se e Digrafo
    {
        if (novaAresta == NULL) // se aresta nao existe
        {
            novaAresta = new Aresta(noDestino, NULL, pesoAresta);
            noFonte->setPrimeiraAresta(novaAresta);
        } else // se existe
            {
                novaAresta->setPeso(pesoAresta);
            }
    } else // se nao e Digrafo
        {
            if (novaAresta == NULL) // se aresta nao existe
            {
                novaAresta = new Aresta(noDestino, NULL, pesoAresta);
                noFonte->setPrimeiraAresta(novaAresta);
                
                Aresta *novaAresta2 = noDestino->getPrimeiraAresta(); // aresta sentido contrario

                if (novaAresta2 == NULL) // se aresta destino -> fonte nao existe
                {
                    novaAresta2 = new Aresta(noFonte, NULL, pesoAresta);
                    noDestino->setPrimeiraAresta(novaAresta2);
                } else // se existe
                    {
                        novaAresta2->setPeso(pesoAresta);
                    }
            } else // se existe
                {
                    novaAresta->setPeso(pesoAresta);
                }
        }

    // incrementa Graus

    if (isDigrafo())
    {
        noFonte->incGrauSaida();
        noDestino->incGrauEntrada();
    } else
        {
            noFonte->incGrauEntrada();
            noDestino->incGrauEntrada();
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

    while (arestaParaRemover != NULL)
    {
        if (arestaParaRemover->getNoDestino()->getIdNo() == idNoDestino)
        {
            // decrementa graus
            arestaParaRemover->getNoDestino()->decGrauEntrada();
            if (isDigrafo())
            {
                noFonte->decGrauSaida();
            } else
                {
                    noFonte->decGrauEntrada();
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
}

// --- Caracteristica do Grafo ---

/*
    GetNumAresta
    Retorna o numero de aresta do grafo.
*/
int Grafo::getNumAresta() 
{
    No *noAux = noRaiz;
    int numArestas = 0;

    while (noAux != NULL) 
    {
        numAresta = noAux->getGrau() + numAresta;
        noAux = noAux->getProxNo();
    }
    return numAresta / 2;
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
    Retorna grau de entrada.
*/
int Grafo::getGrauEntrada() {
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
    Retorna grau de saida.
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