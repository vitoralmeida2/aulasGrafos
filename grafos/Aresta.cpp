#include <iostream>
#include <stdlib.h>
#include <string>
#include "Aresta.h"

using namespace std;

// --- CONSTRUTOR E DESTRUTOR ---

Aresta::Aresta(No *NoDest, Aresta *proxArest, int peso)
{
    this->NoDestino = NoDest;
    this->idNoDestino = NoDest->getIdNo();
    this->proxAresta = proxArest;
    this->pesoAresta = peso;
}

Aresta::~Aresta() 
{
    //destrutor
}

// --- SET ---

void Aresta::setProxAresta(Aresta *proxArest)
{
    this->proxAresta = proxArest;
}

void Aresta::setNoDestino(No *novoDestino)
{
    this->NoDestino = novoDestino;
    this->idNoDestino = novoDestino->getIdNo();
}

void Aresta::setPeso(int valor) 
{
    this->pesoAresta = valor;
}

// --- GET ---

int Aresta::getPesoAresta()
{
    return this->pesoAresta;
}

int Aresta::getIdNoDestino()
{
    return this->idNoDestino;
}

No *Aresta::getNoDestino()
{
    return this->NoDestino;
}

Aresta *Aresta::getProxAresta()
{
    return this->proxAresta;
}