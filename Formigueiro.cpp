#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>

using namespace std;

class Formiga {
protected:
    static int comidas;

public:
    Formiga(){}
    virtual ~Formiga(){}

    void operator++(){ comidas++; }
    static const int getComidas(){ return comidas; }
};

int Formiga::comidas = 0;

class Operaria : public Formiga {
private:
    bool exploradora;
    static int feromonio;

public:
    Operaria() : exploradora(rand() % 2 == 0){}
    ~Operaria(){}

    void operator++(){ if (exploradora) feromonio++; }
    const bool getExploradora() const { return exploradora; }
    static const int getFeromonio(){ return feromonio; }
};

int Operaria::feromonio = 0;

class Formigueiro;

class Rainha : public Formiga {
private:
    Formigueiro* pLar;

public:
    Rainha() : pLar(nullptr){}
    ~Rainha(){ pLar = nullptr; }

    void setPLar(Formigueiro* pF){ pLar = pF; }
    void nascerOperarias(){ new Operaria(); }
};

class Formigueiro {
private:
    vector<Operaria*> formigal;
    vector<Operaria*>::iterator itFormigal;
    Rainha rainha;

public:
    Formigueiro() : rainha(){ formigal.clear(); }
    ~Formigueiro(){
        for(auto op : formigal) delete op;
        formigal.clear();
    }

    void inserir(Operaria* pOp){
        if(!pOp){
            cout << "Erro: Operaria nao existe." << endl;
            return;
        }
        formigal.push_back(pOp);
    }
    
    void inicio(){ itFormigal = formigal.begin(); }
    void proximo(){ if(itFormigal != formigal.end()) itFormigal++; }
    Formiga* atual() const { return (itFormigal != formigal.end()) ? *itFormigal : nullptr; }

    const bool fim() const { return itFormigal == formigal.end(); }

    const bool vivo() const {
        int countOperarias = formigal.size();
        int countComidas = Formiga::getComidas();
        return (countComidas / 10) >= countOperarias;
    }
};

class Jardim {
private:
    Formigueiro formigueiro;
    int quant_folhas;

public:
    Jardim(const int quant_f) : quant_folhas(quant_f){}
    ~Jardim(){}

    // Passa pela lista de formigas do formigueiro, incrementando o feromônio para cada folha do jardim:
    void exploracao(){
        formigueiro.inicio();
        while(!formigueiro.fim()){
            Formiga* formigaAtual = formigueiro.atual();
            for(int i = 0; i < quant_folhas; i++) formigaAtual->operator++();
            formigueiro.proximo();
        }
    }

    // Passa pela lista de formigas do formigueiro, incrementando o valor de comidas equivalente ao valor de feromonios:
    void carreira(){
        formigueiro.inicio();
        while(!formigueiro.fim()){
            Formiga* formigaAtual = formigueiro.atual();
            for(int i = 0; i < Operaria::getFeromonio(); i++) formigaAtual->operator++();
            formigueiro.proximo();
        }
    }

    void executar(){
        exploracao();
        carreira();
        if(formigueiro.vivo()) cout << "Formigueiro continua vivo" << endl;
        else cout << "O Formigueiro morreu :-(" << endl;
    }
};

int main(){
    srand(time(NULL));
    for(int i = 0; i < 20; i++){
        Jardim jardim(i);
        jardim.executar();
        cout << endl;
    }
    return 0;
}
