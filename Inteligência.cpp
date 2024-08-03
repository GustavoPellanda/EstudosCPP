#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>
#include <time.h>

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
            cout<< "Erro: Espiao nao existe." << endl;
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
            cout<< "Erro: Secreto nao existe." << endl;
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
            cout<< "Erro: Duplo nao existe." << endl;
            return;
        }
        pContato = p;
    }
};

void Secreto::agir(){
    if(espias.empty()){
        cout<< "Erro: Nenhum espiao na lista." << endl;
        return;
    } 

    // Determina o número de espiões a serem considerados para remoção
    int numEspias = static_cast<int>(espias.size());
    int numRemover = min(static_cast<int>(forca), numEspias);

    int numRemovidos = 0;
    auto iterador = espias.begin();
    while(iterador != espias.end() && numRemovidos < numRemover){
        // Salva o próximo iterador antes de remover o atual
        auto proximo = next(iterador);

        // Remove o espião da lista
        if((*iterador)->getBoco()){
            cout << "Espiao " << (*iterador)->getId() << " neutralizado." << endl;
            espias.erase(iterador);
            numRemovidos++;
        }
        iterador = proximo;
    }
}

void Espiao::agir(){
    auto iterador = secres.begin();
    while(iterador != secres.end()){
        auto proximo = next(iterador);
        if((*iterador)->getForca() > 0.0) (*iterador)->operator--();
        else{
            cout << "Agente secreto " << (*iterador)->getId() << " neutralizado." << endl;
            secres.erase(iterador);
        }
        iterador = proximo;
    }
}

void Duplo::agir(){
    if(pContato->getBoco()){
        pContato->setBoco(false);
        cout << "Espiao " << pContato->getId() << " deixou de ser boco." << endl;
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
        // Limpar coleções antigas
        for (auto& agente : colecao) {
            delete agente;
        }
        colecao.clear();

        // Inicializar o gerador de números aleatórios
        srand(static_cast<unsigned>(time(nullptr)));

        // Variáveis para definir o número de agentes
        const int maxSecretos = rand() % 5 + 1;  // 1 a 5 agentes secretos
        const int maxEspioes = rand() % 5 + 1;   // 1 a 5 espiões
        const int maxDuplos = rand() % 5 + 1;    // 1 a 5 duplos

        // Vetores para armazenar agentes criados
        vector<Secreto*> vetSecretos;
        vector<Espiao*> vetEspioes;
        vector<Duplo*> vetDuplos;

        // Criar agentes secretos
        for (int i = 0; i < maxSecretos; i++) {
            Secreto* pS = new Secreto(rand() % 39 + 21);  // Idade entre 21 e 59
            vetSecretos.push_back(pS);
            colecao.insert(static_cast<Agente*>(pS));
        }

        // Criar espiões
        for (int i = 0; i < maxEspioes; i++) {
            Espiao* pE = new Espiao();  // Estado `boco` é aleatório
            vetEspioes.push_back(pE);
            colecao.insert(static_cast<Agente*>(pE));
        }

        // Criar duplos
        for (int i = 0; i < maxDuplos; i++) {
            Duplo* pD = new Duplo(rand() % 39 + 21);  // Idade entre 21 e 59
            vetDuplos.push_back(pD);
            colecao.insert(static_cast<Agente*>(pD));
        }

        // Associar espiões aos secretos
        for (auto& secreto : vetSecretos) {
            for (auto& espiao : vetEspioes) {
                if (rand() % 2) {  // 50% de chance de inclusão
                    secreto->incluirEspiao(espiao);
                    espiao->incluirSecreto(secreto);
                }
            }
        }

        // Associar duplos a espiões
        for (auto& duplo : vetDuplos) {
            if (!vetEspioes.empty()) {
                duplo->incluirEspiao(vetEspioes[rand() % vetEspioes.size()]);
            }
        }
    }

    void executar(){
        set<Agente*>::iterator iterador;

        for(int i = 0; i < 10; i++){ // 10 rodadas
            iterador = colecao.begin();
            while(iterador != colecao.end()){
                (*iterador)->operator++();
                cout << "Executando agente " << (*iterador)->getId() << endl;
                (*iterador)->agir();
                iterador++;
            }
        }
    }
};

int main(){
    time_t t;
    srand((unsigned) time(&t));
    Inteligencia intel;
    intel.criarAgentes();
    intel.executar();
    return 0;
}
