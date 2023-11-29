#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Grafo.h"

using namespace std;

struct Caminhao
{
    int capacidade;
};

Grafo *criaGrafoLeInstancia(string nomeArquivo)
{
    ifstream arq(nomeArquivo);

    if (!arq.is_open())
    {
        cout << "Erro ao abrir o arquivo." << endl;
        return NULL;
    }

    Grafo *grafo;
    
    string linha;
    int numNos, id, x, y, demand, capacity;
    while(getline(arq, linha))
    {
        if (linha.find("DIMENSION") != string::npos)
        {
            // Extrai quantidade de nos
            stringstream ss(linha);
            string dimensionStr;
            ss >> dimensionStr >> numNos;
            grafo = new Grafo(numNos, false, true, true);
        } else if (linha.find("CAPACITY") != string::npos)
                {
                    // Extrai capacidade
                    stringstream ss(linha);
                    string capacidadeStr;
                    ss >> capacidadeStr >> capacity;
                    grafo->atualizaCapacidade(capacity);
                } else if (linha.find("NODE_COORD_SECTION") != string::npos)
                        {
                            // leitura coordenadas
                            while (arq >> id >> x >> y)
                            {
                                grafo->insereNo(id, x, y, 0);
                            }
                        } else if (linha.find("DEMAND_SECTION") != string::npos)
                                {
                                    // leitura demandas
                                    while (arq >> id >> demand)
                                    {
                                        grafo->atualizaPesoNos(id, demand);
                                    }
                                }
    }
    
    return grafo;
}

int main()
{
    string nomeArquivo = "instancias/modeloInstancia.txt";
    Grafo *grafo;
    grafo = criaGrafoLeInstancia(nomeArquivo);
    cout << grafo->getOrdem() << endl;

    delete grafo;
    return 0;
}