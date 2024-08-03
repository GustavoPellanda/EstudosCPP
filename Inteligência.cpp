#include <vector>
#include <list>
#include <set>
#include <stdlib.h>
#include <iostream>

using namespace std;

namespace agentes{

class Agente{
protected:
    static int cont_id;
    int id;
    int idade;

public:
    Agente(const int idd = 21): id(cont_id++), idade(idd){
        if(idd < 21) idade = 21;
        if(idd > 59) idade = 59;
    } // Idade default é 21, min 21 e max 59
    
    virtual ~Agente(){ }

    virtual void agir() = 0;
    const int getIdade() const { return idade; }
    const int getId() const { return id; }
    void operator++(){ idade++; }
};
int Agente::cont_id(0);

class Espiao; 

class Secreto : public Agente{
protected:
    vector<Espiao*> espias;
    float forca;

public:
    Secreto(const int idd = 21): forca(0), Agente(idd){ espias.clear(); }
    virtual ~Secreto(){ espias.clear(); }
    virtual void agir();
    const float getForca() const { return forca; }
    virtual void incluirEspiao(Espiao* p){ 
        if(!p){
            cout<< "Erro: Espiao nao existe.";
            return;
        }
        espias.push_back(p);
    }
    void operator--(){ forca--; }
};

class Espiao : public Agente{
private:
    list<Secreto*> secres;
    bool boco;

public:
    Espiao(const int idd = 21): boco(rand()%100 < 90), Agente(idd){ secres.clear(); } // 90% dos espiões começam bocós
    ~Espiao(){ secres.clear(); }
    void agir();
    void incluirSecreto(Secreto* p){
        if(!p){
            cout<< "Erro: Secreto nao existe.";
            return;
        }
        secres.push_back(p);
    }
    void setBoco(const bool b){ boco = b; }
    const bool getBoco() const { return boco; }
};

class Duplo : public Secreto {
private:
    Espiao* pContato;

public:
    Duplo(const int idd = 21): pContato(nullptr), Secreto(idd){}
    ~Duplo(){ pContato = nullptr; }
    void agir();
    void incluirEspiao(Espiao* p){
        if(!p){
            cout<< "Erro: Duplo nao existe.";
            return;
        }
        pContato = p;
    }
};

void Secreto::agir(){
    if(espias.empty()){
            cout<< "Erro: Nenhum espiao na lista.";
        return;
    } 

    // Determina o número de espiões a serem considerados para remoção
    int numEspias = static_cast<int>(espias.size());
    int numRemover = std::min(static_cast<int>(forca), numEspias);

    int numRemovidos = 0;
    auto iterador = espias.begin();
    while(iterador != espias.end() && numRemovidos < numRemover){
        // Remove o espião da lista
        if((*iterador)->getBoco()){
            iterador = espias.erase(iterador);
            cout << "Espião " << (*iterador)->getId() << " neutralizado." << endl;
        }
        else ++iterador;
    }
}

void Espiao::agir(){
    auto iterador = secres.begin();
    while(iterador != secres.end()){
        if((*iterador)->getForca() > 0.0) (*iterador)->operator--();
        else{
            secres.erase(iterador);
            cout << "Agente secreto " << (*iterador)->getId() << " neutralizado." << endl;
        }
        iterador++;
    }
}

void Duplo::agir(){
    if(pContato->getBoco()){
        pContato->setBoco(false);
        cout << "Espião " << pContato->getId() << " deixou de ser boco." << endl;
    }
}

} 

using namespace agentes;

class Inteligencia {
private:
    set<Agente*> colecao;

public:
    Inteligencia(){ colecao.clear(); }
    ~Inteligencia(){ colecao.clear(); }

    void criarAgentes(){

    }

    void executar(){

    }
};

int main(){
    Inteligencia intel;
    return 0;
}
