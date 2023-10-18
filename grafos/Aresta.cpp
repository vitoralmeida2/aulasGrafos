#include <iostream>
#include <stdlib.h>
#include <string>
#include "Aresta.h"

using namespace std;

// --- CONSTRUTOR E DESTRUTOR ---

Aresta::Aresta(No *NoDest, Aresta *proxArest, int peso)
{
    this->NoDestino = NoDest;
    this->proxAresta = proxArest;
    this->pesoAresta = peso;
}

Aresta::~Aresta() 
{
    //destrutor
}

// --- SET ---

void Aresta::setProxAresta(Aresta *valor)
{
    this->proxAresta = valor;
}

void Aresta::setNoDestino(No *valor)
{
    this->NoDestino = valor;
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

No *Aresta::getNoDestino()
{
    return this->NoDestino;
}

Aresta *Aresta::getProxAresta()
{
    return this->proxAresta;
}