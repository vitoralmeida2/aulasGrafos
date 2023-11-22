#include <iostream>
#include "Grafo.h"

using namespace std;

int main(int argc, const char* argv[])
{
    Grafo g(5, true, false, true);
    vector<int> dijkstra;
    
    g.insereAresta(1, 5, 2);
    g.insereAresta(5, 2, 1);
    g.insereAresta(2, 4, 4);
    g.insereAresta(4, 3, 3);

    dijkstra = g.Dijkstra(1);
    cout << "distancias do No 1: ";
    for (int i = 1; i <= 5; i++)
    {
        cout << "(" << i << ", " << dijkstra[i] << ")" << " ";
    }
    cout << endl << endl;

    cout << "Distancia No 1 a 4 (Floyd): " << g.Floyd(1, 4) << endl;

    system("pause");
    return 0;
}