#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
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
    cout << grafo->getOrdem() << endl;

    vector<Rota> rotas;
    vector<Cliente> clientes;
    vector<No*> nosClientes;
    Cliente cl;
    nosClientes = grafo->getNos();

    for(No *no:nosClientes)
    {
        cl.idCliente = no->getIdNo();
        cl.location.x = no->getCordX();
        cl.location.y = no->getCordY();
        cl.demand = no->getPeso();
        clientes.push_back(cl);
    }

    rotas = gulosoVRP(clientes, grafo->getCapacidade());

    // imprimindo rotas
    for(int i = 0; i < rotas.size(); i++)
    {
        cout << "Rota [" << i+1 << "]: ";
        for (Cliente c:rotas[i].clientes)
        {
            cout << c.idCliente << " ";
        }
    }

    delete grafo;
    return 0;
}

/*

std::vector<Route> greedyVRP(const std::vector<Customer>& customers, int vehicleCapacity) {
    std::vector<Route> routes;
    std::vector<Customer> remainingCustomers = customers;

    while (!remainingCustomers.empty()) {
        Route route;
        route.capacity = vehicleCapacity;

        // Start from the depot (assumed to be at index 0)
        route.customers.push_back(customers[0]);
        remainingCustomers[0].visited = true;

        while (true) {
            int closestCustomerIndex = findClosestCustomer(route.customers.back().location, remainingCustomers);

            if (closestCustomerIndex == -1)
                break; // No more unvisited customers

            Customer& closestCustomer = remainingCustomers[closestCustomerIndex];

            // Check if adding the closest customer exceeds the vehicle capacity
            if (route.capacity >= closestCustomer.demand) {
                route.capacity -= closestCustomer.demand;
                route.customers.push_back(closestCustomer);
                closestCustomer.visited = true;
            } else {
                break; // Move to the next route
            }
        }

        routes.push_back(route);
    }

    return routes;
}

int main() {
    // Example usage
    std::vector<Customer> customers = {
        {{0, 0}, 0},    // Depot
        {{1, 2}, 10},   // Customer 1
        {{3, 4}, 5},    // Customer 2
        {{5, 6}, 8}     // Customer 3
        // Add more customers as needed
    };

    int vehicleCapacity = 15;

    std::vector<Route> routes = greedyVRP(customers, vehicleCapacity);

    // Output the routes
    for (size_t i = 0; i < routes.size(); ++i) {
        std::cout << "Route " << i + 1 << ": ";
        for (const auto& customer : routes[i].customers) {
            std::cout << "Customer " << &customer - &customers[0] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}


*/