#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <dirent.h>
#include "Grafo.h"
#include <chrono>
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
    Grafo *grafo;
    Solution guloso, randomizado, reativo;
    double alpha = 0.3;
    vector<double> alfas = {0.05, 0.10, 0.15, 0.30, 0.50}; 
    vector<Probabilidade*> probailidadesAlfa = {
        new Probabilidade{0.05, 0.2},
        new Probabilidade{0.10, 0.2},
        new Probabilidade{0.15, 0.2},
        new Probabilidade{0.30, 0.2},
        new Probabilidade{0.5, 0.2}
    };
    srand(time(NULL));

    vector<string> vetorInstancias;
    string pastaInstancias = "./instancias";
    int numInstancias = 0;
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(pastaInstancias.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (ent->d_type == DT_REG) {  // Verifica se é um arquivo regular
                vetorInstancias.push_back(ent->d_name);
                numInstancias++;
            }
        }
        closedir(dir);
    } else {
        cerr << "Erro ao abrir a pasta." << endl;
        return 1;
    }

    // for(int i=0; i<numInstancias; i++) { 
    //     cout << vetorInstancias[i] << endl;
    // }

    for(int i=0; i<numInstancias; i++) {
        ofstream arquivo(vetorInstancias[i],ios::app); // Abre o arquivo em modo de adição (append)

        auto start = chrono::high_resolution_clock::now(); // Comeca contar tempo a partir daqui
        
        if(vetorInstancias[i][0] == 'X') {  
            grafo = readFile2(pastaInstancias + "/" + vetorInstancias[i]);
        } else if(vetorInstancias[i][0] == 'G') { 
            grafo = readFile3(pastaInstancias + "/" + vetorInstancias[i]);
        } else { 
            grafo = readFile(pastaInstancias + "/" + vetorInstancias[i]);
        }
        
        demandasReadFile(grafo, pastaInstancias + "/" + vetorInstancias[i]);

        if (arquivo.is_open()) { // Verifica se o arquivo foi aberto com sucesso
            arquivo << "RELATORIO FINAL COM AS ROTA DO GULOSO, RANDOMIZADO E REATIVO COM O TEMPO DE COMPILAÇÃO: " << endl;
            
            auto start = chrono::high_resolution_clock::now();
            cout << " --- Guloso --- " << endl << endl;
            cout << endl << endl;
            if (grafo->getVeiculos() == 0)
            {
                guloso = grafo->guloso(arquivo);
            } else {
                guloso = grafo->gulosoCVRP(arquivo);
            }
            if (verificaSolution(guloso))
            {
                arquivo << endl;
                arquivo << "Solucao valida, todos clientes atendidos!" << endl;
                arquivo << "Custo total da solucao: " << guloso.cost << endl << endl;
            } else {
                arquivo << endl;
                arquivo << "Solucao invalida!" << endl;
                arquivo << "Clientes nao visitados: " << endl;
                for (No *cl:guloso.clientesRestantes)
                {
                    arquivo << cl->getIdNo() << " ";
                }
                arquivo << endl;
            }

            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

            cout << "Tempo decorrido: " << duration.count() << " milisegundos " << endl;
            arquivo << "Tempo decorrido: " << duration.count() << " milisegundos " << endl;
            arquivo << endl; 
            arquivo << endl;
            arquivo << endl;

            for(int i=0; i<alfas.size(); i++) { 
                start = chrono::high_resolution_clock::now();
                cout << " --- Guloso Randomizado --- " << endl << endl;
                if (grafo->getVeiculos() == 0)
                {
                    randomizado = grafo->randomizado(alfas[i], arquivo);
                } else {
                    randomizado = grafo->gulosoRandomizadoCVRP(alfas[i], arquivo); 
                }
                if (verificaSolution(randomizado))
                {
                    arquivo << endl;
                    arquivo << "Solucao valida, todos clientes atendidos!" << endl;
                    arquivo << "Custo total da solucao: " << randomizado.cost << endl << endl;
                } else {
                    arquivo << endl;
                    arquivo << "Solucao invalida!" << endl;
                    arquivo << "Clientes nao visitados: " << endl;
                    for (No *cl:guloso.clientesRestantes)
                    {
                        arquivo << cl->getIdNo() << " ";
                    }
                    arquivo << endl;
                }
                cout << endl << endl;

                stop = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

                cout << "Tempo decorrido: " << duration.count() << " milisegundos " << endl;
                arquivo << "Tempo decorrido: " << duration.count() << " milisegundos " << endl;
                arquivo << endl; 
                arquivo << endl;
                arquivo << endl;
            } 

            start = chrono::high_resolution_clock::now();
            cout << " --- Guloso Randomizado Reativo --- " << endl << endl;
            if (grafo->getVeiculos() == 0)
            {
                reativo = grafo->reativo(probailidadesAlfa, arquivo);
            } else {
                reativo = grafo->gulosoRandomizadoReativoCVRP(probailidadesAlfa, arquivo);
            }
            if (verificaSolution(reativo))
            {
                arquivo << endl;
                arquivo << "Solucao valida, todos clientes atendidos!" << endl;
                arquivo << "Custo total da solucao: " << reativo.cost << endl << endl;
            } else {
                arquivo << endl;
                arquivo << "Solucao invalida!" << endl;
                arquivo << "Clientes nao visitados: " << endl;
                for (No *cl:guloso.clientesRestantes)
                {
                    arquivo << cl->getIdNo() << " ";
                }
                arquivo << endl;
            }
            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

            cout << "Tempo decorrido: " << duration.count() << " milisegundos " << endl;
            arquivo << "Tempo decorrido: " << duration.count() << " milisegundos " << endl;
            cout << endl << endl;
            
            arquivo.close(); // Fecha o arquivo quando terminar a interação
        } else {
        cerr << "Erro ao abrir o arquivo!" <<endl;
        return 1;
        }
    }

    for (auto prob:probailidadesAlfa)
    {
        delete [] prob;
    }
    delete grafo;
    return 0;
}