#include <iostream>
#include "Grafo.h"

using namespace std;

int main()
{
    Grafo g(false, false, false);
    g.insereAresta(1, 2, 0);
    g.insereAresta(1, 5, 0);
    g.insereAresta(4, 3, 0);
    g.insereAresta(5, 4, 0);
    g.insereAresta(3, 2, 0);
    cout << g.getOrdem() << endl;
    cout << g.getNumAresta() << endl;
    cout << g.getGrau() << endl;
    g.insereNo(10, 0);
    g.insereAresta(1,10,0);
    cout << g.getOrdem() << endl;
    cout << g.getNumAresta() << endl;
    cout << g.getGrau() << endl;


    system("pause");
    return 0;
}