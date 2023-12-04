#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Grafo.h"
#define INFINITO INT32_MAX

using namespace std;

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
    int numNos, veiculos, capacity, id, x, y, demand;

    while (getline(arq, linha))
    {
        if (linha.find("DIMENSION") != string::npos)
        {
            stringstream ss(linha);
            string dimensionStr;
            ss >> dimensionStr >> numNos;
            grafo = new Grafo(numNos, false, true, true);
        } else if (linha.find("No of trucks") != string::npos)
            {
                size_t posInicio = linha.find("No of trucks:");
                if (posInicio != string::npos)
                {
                    size_t posFim = linha.find(",", posInicio);
                    if (posFim != string::npos)
                    {
                        string truckStr = linha.substr(posInicio + 13, posFim - posInicio - 13);
                        veiculos = stoi(truckStr);
                    }
                }
            } else if (linha.find("CAPACITY") != string::npos)
                {
                    istringstream iss(linha);
                    string palavra, espaco, valor;
                    iss >> palavra >> espaco >> valor;
                    capacity = stoi(valor);
                } else if (linha.find("NODE_COORD_SECTION") != string::npos)
                    {
                        while (arq >> id >> x >> y)
                        {
                            grafo->insereNo(id, x, y, 0);
                        }
                    } else if (linha.find("DEMAND_SECTION") != string::npos)
                        {
                            cout << "teste";
                            while (arq >> id >> demand)
                            {
                                grafo->atualizaPesoNos(id, demand);
                            }
                        }
    }

    grafo->setCapacidade(capacity);
    grafo->setVeiculos(veiculos);
    
    arq.close();
    return grafo;
}

void leDemandas(Grafo *grafo, string nomeArquivo)
{
    ifstream arq(nomeArquivo);
    
    if (!arq.is_open())
    {
        cout << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string linha;
    int id, demand;

    while (getline(arq, linha))
    {
        if (linha.find("NAME : ") != string::npos)
        {
            size_t pos = linha.find(":");
            if (pos != string::npos)
            {
                string nomeInstancia = linha.substr(pos + 2);
                grafo->setInstanceName(nomeInstancia);
            }
        }
        if (linha.find("DEMAND_SECTION") != string::npos)
        {
            // leitura demandas
            while (arq >> id >> demand)
            {
                grafo->atualizaPesoNos(id, demand);
            }
        }
    }

    arq.close();
}

int main(int argc, const char* argv[])
{
    string nomeArquivo = "instancias/modeloInstancia.txt";
    Grafo *grafo;
    grafo = criaGrafoLeInstancia(nomeArquivo);
    leDemandas(grafo, nomeArquivo);
    double alpha = 0.3;

    cout << " --- Guloso --- " << endl << endl;
    grafo->gulosoCVRP();
    cout << endl << endl;

    cout << " --- Guloso Randomizado --- " << endl << endl;
    grafo->gulosoRandomizadoCVRP(alpha);
    cout << endl << endl;

    system("pause");
    delete grafo;
    return 0;
}