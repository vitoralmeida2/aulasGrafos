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
    // vector<string> vetorInstancias;
    // string pastaInstancias = "./instancias";
    // int numInstancia = 0;
    // DIR *dir;
    // struct dirent *ent;

    // auto start = chrono::high_resolution_clock::now(); // Comeca contar tempo a partir daqui

    // if ((dir = opendir(pastaInstancias.c_str())) != nullptr)
    // {
    //     while ((ent = readdir(dir)) != nullptr)
    //     {
    //         if (ent->d_type == DT_REG) // Verifica se e um arquivo regular
    //         {
    //             vetorInstancias.push_back(ent->d_name);
    //             numInstancia++;
    //         }
    //     }
    //     closedir(dir);
    // } else
    //     {
    //         cout << "Erro ao abrir a pasta." << endl;
    //         return 1;
    //     }

    // for (int i = 0; i < numInstancia; i++)
    // {
    //     Grafo *g;
    //     if (vetorInstancias[i][0] == 'X')
    //     {
    //         cout << pastaInstancias + vetorInstancias[i] << endl;
    //         g = readFile2(pastaInstancias + "/" + vetorInstancias[i]);
    //     } else if (vetorInstancias[i][0] == 'G')
    //         {
    //             cout << pastaInstancias + vetorInstancias[i] << endl;
    //             g = readFile3(pastaInstancias + "/" + vetorInstancias[i]);
    //         } else
    //             {
    //                 cout << pastaInstancias + vetorInstancias[i] << endl;
    //                 g = readFile(pastaInstancias + "/" + vetorInstancias[i]);
    //             }
    // }

    // auto stop = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    // cout << "Tempo decorrido: " << duration.count() << endl;

    Grafo *grafo;
    Solution guloso, randomizado, reativo;
    double alpha = 0.3;
    vector<double> alfas = {0.05, 0.10, 0.15, 0.30, 0.50};
    string nomeArquivo = "instancias/modeloInstancia.txt";
    srand(time(NULL));

    grafo = readFile3(nomeArquivo);
    demandasReadFile(grafo, nomeArquivo);

    guloso = grafo->guloso();
    verificaSolution(guloso);
    
    randomizado = grafo->randomizado(alpha);
    verificaSolution(randomizado);

    reativo = grafo->reativo(alfas);
    verificaSolution(reativo);

    // cout << " --- Guloso --- " << endl << endl;
    // guloso = grafo->gulosoCVRP();
    // verificaSolution(guloso);
    // cout << endl << endl;

    // cout << " --- Guloso Randomizado --- " << endl << endl;
    // randomizado = grafo->gulosoRandomizadoCVRP(alpha);
    // verificaSolution(randomizado);
    // cout << endl << endl;

    // cout << " --- Guloso Randomizado Reativo --- " << endl << endl;
    // reativo = grafo->gulosoRandomizadoReativoCVRP(alfas);
    // verificaSolution(reativo);
    // cout << endl << endl;

    system("pause");
    delete grafo;
    return 0;
}