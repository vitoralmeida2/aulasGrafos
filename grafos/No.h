#ifndef NO_H
#define NO_H
#include <iostream>
#include <Aresta.h>

using namespace std;

class No 
{
    public:
        No(int idNo);
        No(int idNo, int peso);
        ~No();

        int getIdNo();

    private:
        int idNo;
        int pesoNo;
        int grauEntrada;
        int grauSaida;
        No *proxNo;
        Aresta *primeiraAresta;
        Aresta *ultimaAresta;
};

#endif // NO_H