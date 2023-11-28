#include <iostream>
#include "Grafo.h"

using namespace std;

int main(int argc, const char* argv[])
{
    int menu;
    int numNos = 6;
    Grafo grafo(6, true, false, true);
    vector<int> Dijkstra, Centros, Periferia;
    
    do
    {
        cout << " --- Trabalho de Grafos - GRUPO 37 ---" << endl << endl;
        cout << "              --- MENU ---" << endl;
        cout << "1. Criar Grafo e carregar instancias." << endl;
        cout << "2. Fecho trasitivo direto do Grafo." << endl;
        cout << "3. Fecho trasitivo indireto do Grafo." << endl;
        cout << "4. Caminho Minimo entre dois pontos - Dijkstra." << endl;
        cout << "5. Caminho Minimo entre dois pontos - Floyd." << endl;
        cout << "6. Arvore Geradora Minima - Prim" << endl;
        cout << "7. Arvore Geradora Minima - Kruskal" << endl;
        cout << "8. Arvore dada pelo caminhamento dem profundidade" << endl;
        cout << "9. Ordenacao Topologica" << endl;
        cout << "10. Raio, diametro, centro e periferia." << endl;
        cout << "11. Vertices de Articulacao." << endl;
        cout << "0. Encerrar programa." << endl << endl;
        cout << "Insira a opcao desejada: ";
        cin >> menu;
        cout << endl;

        int aux;
        int aux2;

        switch (menu)
        {
            case 1:

                // le arquivo instancias
                grafo.insereAresta(1, 3, 3);
                grafo.insereAresta(3, 2, 2);
                grafo.insereAresta(2, 1, 2);
                grafo.insereAresta(1, 4, 1);
                grafo.insereAresta(4, 5, 4);
                grafo.insereAresta(5, 6, 2);
                grafo.insereAresta(6, 4, 1);
                grafo.insereAresta(4, 3, 1);

                break;
            
            case 2:
                cout << "Insira o ID do vertice incial: ";
                cin >> aux;
                grafo.fechoTransitivoDireto(aux);
                cout << endl;
                break;

            case 3:
                cout << "Insira o ID do vertice inicial: ";
                cin >> aux;
                grafo.fechoTransitivoIndireto(aux);
                cout << endl;
                break;

            case 4:
                cout << "Insira os ID de origem e destino, respectivamente: ";
                cin >> aux;
                cin >> aux2;
                Dijkstra = grafo.Dijkstra(aux);
                cout << "Caminho entre " << aux << " - " << aux2 << ": " << Dijkstra[aux2];
                cout << endl;
                break;

            case 5:
                cout << "Insira os ID de origem e destino, respectivamente: ";
                cin >> aux;
                cin >> aux2;
                cout << "Caminho entre " << aux << " - " << aux2 << ": " << grafo.Floyd(aux, aux2);
                cout << endl;
                break;

            case 6:
                cout << "Insira o ID de origem: ";
                cin >> aux;
                grafo.Prim(aux);
                cout << endl;
                break;

            case 7:
                cout << "Insira o ID de origem: ";
                cin >> aux;
                grafo.Kruskal();
                cout << endl;
                break;

            case 8:
                cout << "Insira o ID de origem: ";
                cin >> aux;
                grafo.buscaProfundidade(aux);
                cout << endl;
                break;

            case 9:
                cout << "Insira o ID para ordenacao topologica: ";
                cin >> aux;
                grafo.ordenacaoTopologica();
                cout << endl;
                break;

            case 10:
                cout << "Raio do Grafo: " << grafo.encontrarRaio() << endl;
                cout << "Diametro do Grafo: " << grafo.encontrarDiametro() << endl;
                cout << "Centros do Grafo: ";
                Centros = grafo.encontrarCentro();
                for (int i = 0; i < Centros.size(); i++)
                {
                    cout << "(" << Centros[i] << ")" << " ";
                }
                cout << endl;

                cout << "Periferia do Grafo: ";
                Periferia = grafo.encontrarPeriferia();
                for (int i = 0; i < Periferia.size(); i++)
                {
                    cout << "(" << Periferia[i] << ")" << " ";
                }
                cout << endl << endl;
                break;

            case 11:
                cout << "Vertices de articulacao: ";
                grafo.nosArticulacao();
                cout << endl;
                break;

            
            default:
                break;
        }

    } while (menu != 0);
    
    return 0;
}