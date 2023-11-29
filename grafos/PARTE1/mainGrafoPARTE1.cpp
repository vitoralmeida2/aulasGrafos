#include <iostream>
#include <fstream>
#include "Grafo.h"

using namespace std;

Grafo *criaGrafoLeInformacoes()
{
    ifstream arq("instancias/grafo_teste1.txt");
    
    if (!arq.is_open())
    {
        cout << "Erro ao abrir arquivo." << endl;
        return NULL;
    }

    int numVertices, idOrigem, idDestino, pesoAresta;
    arq >> numVertices;

    Grafo *g = new Grafo(numVertices, true, false, true);

    while(arq >> idOrigem >> idDestino >> pesoAresta)
    {
        g->insereAresta(idOrigem, idDestino, pesoAresta);
    }

    arq.close();

    return g;
}

int main(int argc, const char* argv[])
{
    int menu, Dijkstra;
    int numNos = 6;
    bool digrafo, pesoNos, pesoArestas;
    
    Grafo *grafo;
    
    vector<int> Centros, Periferia;
    
    do
    {
        cout << " --- Trabalho de Grafos - GRUPO 37 (PARTE 1) ---" << endl << endl;
        cout << "                  --- MENU ---" << endl << endl;
        cout << "1-> Criar Grafo e carregar instancias." << endl;
        cout << "2-> Imprimir Lista de Adjacencia." << endl;
        cout << "3-> Fecho trasitivo direto do Grafo." << endl;
        cout << "4-> Fecho trasitivo indireto do Grafo." << endl;
        cout << "5-> Caminho Minimo entre dois pontos - Dijkstra." << endl;
        cout << "6-> Caminho Minimo entre dois pontos - Floyd." << endl;
        cout << "7-> Arvore Geradora Minima - Prim." << endl;
        cout << "8-> Arvore Geradora Minima - Kruskal." << endl;
        cout << "9-> Arvore dada pelo caminhamento dem profundidade." << endl;
        cout << "10-> Ordenacao Topologica." << endl;
        cout << "11-> Raio, diametro, centro e periferia." << endl;
        cout << "12-> Vertices de Articulacao." << endl;
        cout << "0-> Encerrar programa." << endl << endl;
        cout << "Insira a opcao desejada: ";
        cin >> menu;
        cout << endl;

        int aux;
        int aux2;

        switch (menu)
        {
            case 1:
                grafo = criaGrafoLeInformacoes();
                break;

            case 2:
                grafo->imprimeGrafo();
                cout << endl;
                break;
            
            case 3:
                cout << "Insira o ID do vertice incial: ";
                cin >> aux;
                grafo->fechoTransitivoDireto(aux);
                cout << endl;
                break;

            case 4:
                cout << "Insira o ID do vertice inicial: ";
                cin >> aux;
                grafo->fechoTransitivoIndireto(aux);
                cout << endl;
                break;

            case 5:
                cout << "Insira os ID de origem e destino, respectivamente: ";
                cin >> aux >> aux2;
                Dijkstra = grafo->Dijkstra(aux, aux2);
                cout << "Caminho entre " << aux << " - " << aux2 << ": " << Dijkstra;
                cout << endl;
                break;

            case 6:
                cout << "Insira os ID de origem e destino, respectivamente: ";
                cin >> aux >> aux2;
                cout << "Caminho entre " << aux << " - " << aux2 << ": " << grafo->Floyd(aux, aux2);
                cout << endl;
                break;

            case 7:
                cout << "Insira o ID de origem: ";
                cin >> aux;
                grafo->Prim(aux);
                cout << endl;
                break;

            case 8:
                cout << "Insira o ID de origem: ";
                cin >> aux;
                grafo->Kruskal();
                cout << endl;
                break;

            case 9:
                cout << "Insira o ID de origem: ";
                cin >> aux;
                grafo->buscaProfundidade(aux);
                cout << endl;
                break;

            case 10:
                cout << "Insira o ID para ordenacao topologica: ";
                cin >> aux;
                grafo->ordenacaoTopologica();
                cout << endl;
                break;

            case 11:
                cout << "Raio do Grafo: " << grafo->encontrarRaio() << endl;
                cout << "Diametro do Grafo: " << grafo->encontrarDiametro() << endl;
                cout << "Centros do Grafo: ";
                Centros = grafo->encontrarCentro();
                for (int i = 0; i < Centros.size(); i++)
                {
                    cout << "(" << Centros[i] << ")" << " ";
                }
                cout << endl;

                cout << "Periferia do Grafo: ";
                Periferia = grafo->encontrarPeriferia();
                for (int i = 0; i < Periferia.size(); i++)
                {
                    cout << "(" << Periferia[i] << ")" << " ";
                }
                cout << endl << endl;
                break;

            case 12:
                cout << "Vertices de articulacao: ";
                grafo->nosArticulacao();
                cout << endl;
                break;

            
            default:
                break;
        }

    } while (menu != 0);

    delete grafo;
    
    return 0;
}