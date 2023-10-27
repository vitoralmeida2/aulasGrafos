#include <iostream>
#include "Grafo.h"

using namespace std;

int main(int argc, const char* argv[])
{
    Grafo g(5, false, false, false);
    
    g.insereAresta(1, 2, 0);
    g.insereAresta(3, 5, 0);
    g.insereAresta(2, 3, 0);
    g.insereAresta(4, 3, 0);
    g.insereAresta(1, 3, 0);
    g.insereAresta(5, 4, 0);

    g.buscaProfundidade(1);


    system("pause");
    return 0;
}