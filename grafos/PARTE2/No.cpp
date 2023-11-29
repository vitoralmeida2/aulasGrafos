#include "No.h"

using namespace std;

// --- CONSTRUTOR E DESTRUTOR ---

No::No(int name, int x, int y, int peso) 
{
    this->idNo = name;
    this->cordX = x;
    this->cordY = y;
    this->pesoNo = peso;
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->proxNo = NULL;
    this->primeiraAresta = NULL;
}

No::~No()
{
    Aresta *auxAresta = getPrimeiraAresta();
    while (auxAresta != NULL) 
    {
        Aresta *auxNextAresta = auxAresta->getProxAresta();
        delete auxAresta;
        auxAresta = auxNextAresta;
    }
}

// --- SET ---

void No::setIdNo(int valor) 
{
    this->idNo = valor;
}

void No::setPesoNo(int valor)
{
    this->pesoNo = valor;
}

void No::incGrau()
{
    this->grauEntrada++;
    this->grauSaida++;
}

void No::decGrau()
{
    this->grauEntrada--;
    this->grauSaida--;
}

void No::incGrauEntrada()
{
    this->grauEntrada++;
}

void No::decGrauEntrada()
{
    this->grauEntrada--;
}

void No::incGrauSaida()
{
    this->grauSaida++;
}

void No::decGrauSaida()
{
    this->grauSaida--;
}

void No::setProxNo(No *novoNo)
{
    this->proxNo = novoNo;
}

void No::setPrimeiraAresta(Aresta *novaAresta)
{
    this->primeiraAresta = novaAresta;
}

// --- GET ---

int No::getIdNo() 
{
    return this->idNo;
}

int No::getPeso() 
{
    return this->pesoNo;
}

int No::getGrau()
{
    return this->grauEntrada;
}

int No::getGrauEntrada()
{
    return this->grauEntrada;
}

int No::getGrauSaida()
{
    return this->grauSaida;
}

No *No::getProxNo()
{
    return this->proxNo;
}

Aresta *No::getPrimeiraAresta()
{
    return this->primeiraAresta;
}