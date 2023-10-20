#include <iostream>
#include "Grafo.h"

using namespace std;

int main()
{
    Grafo g(false, false, false);
    g.insereNo(1, 0);
    g.insereNo(2, 0);
    g.insereNo(3, 0);
    g.insereNo(4, 0);
    g.insereNo(5, 0);
    g.insereAresta(1, 2, 0);
    g.insereAresta(1, 5, 0);
    g.insereAresta(4, 3, 0);
    g.insereAresta(5, 4, 0);
    g.insereAresta(3, 2, 0);
    cout << g.getOrdem() << endl;
    cout << g.getNumAresta() << endl;
    cout << g.getGrau() << endl;
    g.removeNo(3, 0);
    cout << g.getOrdem() << endl;
    cout << g.getNumAresta() << endl;
    cout << g.getGrau() << endl;


    system("pause");
    return 0;
}