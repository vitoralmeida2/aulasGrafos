#include <iostream>
#include "Grafo.h"

using namespace std;

int main(int argc, const char* argv[])
{
    Grafo g(6, false, false, true);
    vector<int> dijkstra;
    vector<int> centros;
    vector<int> periferia;
    
    g.insereAresta(1, 2, 3);
    g.insereAresta(1, 4, 2);
    g.insereAresta(2, 3, 2);
    g.insereAresta(2, 5, 2);
    g.insereAresta(3, 1, 1);
    g.insereAresta(3, 5, 1);
    g.insereAresta(4, 3, 3);
    g.insereAresta(4, 6, 6);
    g.insereAresta(5, 6, 4);
    g.insereAresta(6, 3, 3);

    g.buscaProfundidade(1);
    g.Prim(1);
    g.Kruskal();
    cout << endl;
    cout << "Raio : " << g.encontrarRaio() << endl;
    cout << "Diamentro: " << g.encontrarDiametro() << endl;

    centros = g.encontrarCentro();
    cout << "Centro: ";
    for (int i = 0; i < centros.size(); i++)
    {
        cout << centros[i] << " ";
    }
    cout << endl;

    periferia = g.encontrarPeriferia();
    cout << "Periferia: ";
    for (int i = 0; i < periferia.size(); i++)
    {
        cout << periferia[i] << " ";
    }
    cout << endl;

    /*
    
    dijkstra = g.Dijkstra(5);
    cout << "distancias do No 5: ";
    for (int i = 1; i <= 6; i++)
    {
        cout << "(" << i << ", " << dijkstra[i] << ")" << " ";
    }
    cout << endl << endl;

    cout << "Distancia No 5 a 2 (Floyd): " << g.Floyd(5, 2) << endl;
    
    */

    system("pause");
    return 0;
}