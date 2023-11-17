#include <iostream>
#include "Grafo.h"

using namespace std;

int main(int argc, const char* argv[])
{
    Grafo g(5, true, false, false);
    
    g.insereAresta(1, 5, 2);
    g.insereAresta(5, 2, 1);
    g.insereAresta(2, 4, 4);
    g.insereAresta(4, 3, 3);
    
    cout << g.Floyd(1, 3) << endl;
    
   

    system("pause");
    return 0;
}