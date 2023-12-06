#include <iostream>
#include <stdlib.h>
#include <string>
#include "Aresta.h"

// --- CONSTRUTOR E DESTRUTOR ---

Aresta::Aresta(No *NoOrig, No *NoDest, int pesoArest, Aresta *proxArest)
{
    this->NoOrigem = NoOrig;
    this->NoDestino = NoDest;
    this->idNoOrigem = NoOrig->getIdNo();
    this->idNoDestino = NoDest->getIdNo();
    this->proxAresta = proxArest;
    this->pesoAresta = pesoArest;
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

void Aresta::setNoOrigem(No *novaOrigem)
{
    this->NoOrigem = novaOrigem;
    this->idNoOrigem = novaOrigem->getIdNo();
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

int Aresta::getIdNoOrigem()
{
    return this->idNoOrigem;
}

int Aresta::getIdNoDestino()
{
    return this->idNoDestino;
}

int Aresta::getPesoAresta()
{
    return this->pesoAresta;
}

No *Aresta::getNoOrigem()
{
    return this->NoOrigem;
}

No *Aresta::getNoDestino()
{
    return this->NoDestino;
}

Aresta *Aresta::getProxAresta()
{
    return this->proxAresta;
}