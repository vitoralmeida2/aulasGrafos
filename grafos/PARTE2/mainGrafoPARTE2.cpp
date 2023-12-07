#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <dirent.h>
#include "Grafo.h"
#define INFINITO INT32_MAX

using namespace std;

Grafo *readFile(string nomeArquivo)
{
    ifstream arq(nomeArquivo);
    
    if (!arq.is_open())
    {
        cout << "Erro ao abrir o arquivo." << endl;
        return NULL;
    }

    Grafo *grafo;
    string linha, instaceName;
    int numNos, veiculos, capacity, id, x, y, demand;

    while (getline(arq, linha))
    {
        if (linha.find("NAME : ") != string::npos)
        {
            size_t pos = linha.find(":");
            if (pos != string::npos)
            {
                instaceName = linha.substr(pos + 2);
            }
        } else if (linha.find("DIMENSION") != string::npos)
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
                        }
    }

    grafo->setInstanceName(instaceName);
    grafo->setCapacidade(capacity);
    grafo->setVeiculos(veiculos);
    
    arq.close();

    return grafo;
}

void demandasReadFile(Grafo *grafo, string nomeArquivo)
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

Grafo *readFile2(string nomeArquivo)
{
    ifstream arq(nomeArquivo);

    if (!arq.is_open())
    {
        cout << "Erro ao abrir arquivo!" << endl;
        return NULL;
    }

    Grafo *grafo;
    string linha, instanceName;
    int dimension, capacity, id, x, y, demand;

    while (getline(arq, linha))
    {
        if (linha.find("NAME : ") != string::npos)
        {
            size_t pos = linha.find(":");
            if (pos != string::npos)
            {
                instanceName = linha.substr(pos + 2);
            }
        } else if (linha.find("DIMENSION") != string::npos)
            {
                istringstream iss(linha);
                string palavra, espaco, valor;
                iss >> palavra >> espaco >> valor;
                dimension = stoi(valor);
                grafo = new Grafo(dimension, false, true, false);
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
                    }
    }

    grafo->setInstanceName(instanceName);
    grafo->setVeiculos(0);
    grafo->setCapacidade(capacity);

    arq.close();

    return grafo;
}

Grafo *readFile3(string nomeArquivo)
{
    ifstream arq(nomeArquivo);

    if (!arq.is_open())
    {
        cout << "Erro ao abrir arquivo!" << endl;
        return NULL;
    }

    Grafo *grafo;
    string linha, instanceName;
    int dimension, capacity, id, demand;
    float x, y;

    while (getline(arq, linha))
    {
        if (linha.find("NAME : ") != string::npos)
        {
            size_t pos = linha.find(":");
            if (pos != string::npos)
            {
                instanceName = linha.substr(pos + 2);
            }
        } else if (linha.find("DIMENSION") != string::npos)
            {
                istringstream iss(linha);
                string palavra, espaco, valor;
                iss >> palavra >> espaco >> valor;
                dimension = stoi(valor);
                grafo = new Grafo(dimension, false, true, false);
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
                    }
    }

    grafo->setInstanceName(instanceName);
    grafo->setVeiculos(0);
    grafo->setCapacidade(capacity);

    arq.close();

    return grafo;
}

bool verificaSolution(Solution sol)
{
    if (sol.clientesRestantes.size() == 1 || sol.clientesRestantes.size() == 0) // Se tiver vazio ou apenas o deposito nos clientes restantes, solucao valida!
    {
        cout << "Solucao valida, todos clientes atendidos!" << endl;
        cout << "Custo total da solucao: " << sol.cost << endl << endl;
        return true;
    } else
        {
            cout << "Solucao invalida!" << endl;
            cout << "Clientes nao visitados: " << endl;
            for (No *cl:sol.clientesRestantes)
            {
                cout << cl->getIdNo() << " ";
            }
            cout << endl << endl;
            return false;
        }
}

int main(int argc, const char* argv[])
{
    string nomeArquivo = "instancias/B-n50-k8.txt";
    Grafo *grafo;
    Solution guloso, randomizado, reativo;
    double alpha = 0.1;
    vector<double> alfas = {0.05, 0.10, 0.15, 0.30, 0.50};
    vector<Probabilidade*> probailidadesAlfa = {
        new Probabilidade{0.05, 0.2},
        new Probabilidade{0.10, 0.2},
        new Probabilidade{0.15, 0.2},
        new Probabilidade{0.30, 0.2},
        new Probabilidade{0.5, 0.2}
    };
    srand(time(NULL));

    grafo = readFile(nomeArquivo);
    demandasReadFile(grafo, nomeArquivo);

    // guloso = grafo->guloso();
    // verificaSolution(guloso);
    
    // randomizado = grafo->randomizado(alpha);
    // verificaSolution(randomizado);

    // reativo = grafo->reativo(alfas);
    // verificaSolution(reativo);

    cout << " --- Guloso --- " << endl << endl;
    // guloso = grafo->gulosoCVRP();
    // verificaSolution(guloso);
    // cout << endl << endl;

    // cout << " --- Guloso Randomizado --- " << endl << endl;
    // randomizado = grafo->gulosoRandomizadoCVRP(alpha);
    // verificaSolution(randomizado);
    // cout << endl << endl;

    // cout << " --- Guloso Randomizado Reativo --- " << endl << endl;
    // reativo = grafo->gulosoRandomizadoReativoCVRP(alfas);
    reativo = grafo->gulosoRandomizadoReativoCVRPTeste(probailidadesAlfa);
    verificaSolution(reativo);
    cout << endl << endl;

    // system("pause");
    delete grafo;
    return 0;
}