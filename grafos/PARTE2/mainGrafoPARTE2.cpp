#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
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

void leDemandas(Grafo *g, string nomeArquivo)
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
                g->atualizaPesoNos(id, demand);
            }
        }
    }

    arq.close();
}

struct Point
{
    double x, y;
};

struct Cliente
{
    int idCliente;
    Point location;
    int demand;
    bool visited;
};

struct Rota
{
    vector<Cliente> clientes;
    int capacity;
};

double distance (Point &a, Point &b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
};

int encontraClienteProximo(Point &location, vector<Cliente> &clientes)
{
    int clienteProximo = -1;
    double minDistance = INFINITO;

    for (int i = 0; i < clientes.size(); i++)
    {
        if (!clientes[i].visited)
        {
            double dist = distance(location, clientes[i].location);
            if (dist < minDistance)
            {
                minDistance = dist;
                clienteProximo = i;
            }
        }
    }

    return clienteProximo;
}

vector<Rota> gulosoVRP(vector<Cliente> &clientes, int capacidadeVeiculo)
{
    vector<Rota> rotas;
    vector<Cliente> clientesRestantes = clientes;

    while (!clientesRestantes.empty())
    {
        Rota rota;
        rota.capacity = capacidadeVeiculo;

        // inicio no deposito
        rota.clientes.push_back(clientes[0]);
        clientesRestantes[0].visited = true;

        while (true)
        {
            int clienteProximo = encontraClienteProximo(rota.clientes.back().location, clientesRestantes);
            
            if (clienteProximo == -1)
            {
                break; // nao ha clientes que nao foram visitados
            }

            Cliente &clienteProx = clientesRestantes[clienteProximo];

            // verificando se excede capacidade do veiculo
            if (rota.capacity >= clienteProx.demand)
            {
                rota.capacity -= clienteProx.demand;
                rota.clientes.push_back(clienteProx);
                clienteProx.visited = true;
            } else
                {
                    break; // cliente passa para proxima rota
                }
        }
        
        rotas.push_back(rota);
    }

    return rotas;
}

int main()
{
    string nomeArquivo = "instancias/modeloInstancia.txt";
    Grafo *grafo;
    grafo = criaGrafoLeInstancia(nomeArquivo);
    leDemandas(grafo, nomeArquivo);

    vector<Rota> rotas;
    vector<Cliente> clientes;
    vector<No*> nosClientes;
    Cliente cl;
    nosClientes = grafo->getNos();
    reverse(nosClientes.begin(), nosClientes.end());

    for(No *no:nosClientes)
    {
        cl.idCliente = no->getIdNo();
        cl.location.x = no->getCordX();
        cl.location.y = no->getCordY();
        cl.demand = no->getPeso();
        clientes.push_back(cl);
    }

    system("pause");
    delete grafo;
    return 0;
}