#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

class Formiga {
protected:
    static int comidas;

public:
    Formiga(): comidas(0){}
    virtual ~Formiga(){}

    void operator++(){ comidas++; }
    const int getComidas() static { return comidas; }
};

class Operaria : public Formiga {
private:
    bool exploradora;
    static int feromonio;

public:
    Operaria(): exploradora(false), feromonio(0), Formiga(){}
    ~Operaria(){}

    void operator++(){ if(exploradora) feromonio++; }
    const bool getExploradora() const { return exploradora; }
    const int getFeromonio() static { return feromonio; }
};

class Formigueiro;

class Rainha : public Formiga {
private:    
    Formigueiro* pLar;

public:
    Rainha(): pLar(nullptr), Formiga(){}
    ~Rainha(){ pLar = nullptr; }

    void setPLar(Formigueiro* pF){ pLar = pF; }
    void nascerOperarias(){ new Operaria(); }
};

class Formigueiro {
private:
    vector<Operaria*> formigal;
    vector<Operaria*>::iterator itFormigal;
    Rainha rainha;

protected:
    Formigueiro(): itFormigal(nullptr), rainha(){ formigal.clear(); }
    ~Formigueiro(){
        formigal.clear();
        itFormigal = nullptr;
    }

    void inserir(Operaria* pOp){
        if(!pOp){
            cout << Erro: Operaria nao existe. << endl;
            return;
        }
        formigal.push_back(pOp);
    }
    
    void inicio(){ itFormigal == formigal.begin(); }
    void proximo(){ if(itFormigal) itFormigal++; }
    Formiga* atual() const { return itFormigal; }

    const bool fim() const { 
        if(itFormigal == formigal.end()) return true;
        return false; 
    }

    const bool vivo() const {
        int countOperarias = 0;
        int countComidas = 0;

        Formigueiro.inicio();
        while(!Formigueiro.fim()){
            countOperarias++;
            countComidas += itFormigal->getComidas();
            Formigueiro.proximo();
        }

        if((countComidas/10) >=  countOperarias) return true;
        else return false;
    }
};

class Jardim {
private:
    Formigueiro formigueiro;
    int quant_folhas;

protected:
    Jardim(const int quant_f): quant_folhas(quant_f), formigueiro(){}
    ~Jardim(){}

    // Passa pela lista de formigas do formigueiro, incrementando o feromônio para cada folha do jardim:
    void exploracao(){
        formigaAtual = formigueiro->inicio();
        while(!formigueiro->fim()){
            formigaAtual = formigueiro->atual();
            for(i = 0; i <= quant_folhas; i++) formigaAtual->operator++();
            formigueiro->proximo();
        }
    }

    // Passa pela lista de formigas do formigueiro, incrementando o valor de comidas equivalente ao valor de feromonios:
    void carreira(){
        formigaAtual = formigueiro->inicio();
        while(!formigueiro->fim()){
            formigaAtual = formigueiro->atual();
            for(i = 0; i <= formigaAtual->getFeromonio(); i++) formigaAtual->operator++();
            formigueiro->proximo();
        }
    }

    void executar(){
        self.exploracao();
        self.carreira();
        if(formigueiro.vivo) cout << "Formigueiro continua vivo" << endl;
        else cout << "O Formigueiro morreu :-(" << endl;
    }
};

int main(){
    for(int i = 0 i < 20; i++){
        Jardim jardim(i);
        cout << endl;
    }
    return 0;
}