#include <iostream>
#include "Grafo.h"

using namespace std;

int main()
{
    Grafo *g = new Grafo(false, false, false);
    g->insereAresta(1, 2, 0);
    g->insereAresta(1, 3, 0);
    g->insereAresta(2, 3, 0);
    g->insereAresta(4, 2, 0);
    cout << "Raiz: " << g->getNoRaiz()->getIdNo() << endl;
    cout << g->getOrdem() << endl;
    cout << g->getNumAresta() << endl;
    cout << g->getGrau() << endl;
    g->removeNo(4);
    cout << g->getOrdem() << endl;
    cout << g->getNumAresta() << endl;
    cout << g->getGrau() << endl;

    delete g;

    system("pause");
    return 0;
}