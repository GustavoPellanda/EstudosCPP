#include <iostream>
#include <stdlib.h>
#include <vectot>

using namespace std;

class Sensor {
protected:
    static vector<Sensor*> colecao_sensores;
    static int cont;
    bool ativo;

public:
    Sensor() : ativo(rand()%10 = false), cont(++cont) { colecao_sensores.clear(); }
    virtual ~Sensor(){ colecao_sensores.clear(); }

    const bool getAtivo() const { return ativo; }
    
    static const bool TodosInativos() {
        for (int i = 0; i < colecao_sensores.size(); i++) {
            if (colecao_sensores[i]->getAtivo()) {
                return false;
            }
        }
        return true;
    }

    static void incluirSensor(Sensor* pS) { 
        if(!pS) return;
        colecao_sensores.push_back(pS); 
    }

    static void percorrer() { for (int i = 0; i < colecao_sensores.size(); i++) colecao_sensores[i]->operator++();}

    virtual void operator++() = 0;

};
vector<Sensor*> Sensor::colecao_sensores;
int Sensor::cont(0);

class Temperatura;

class Presenca : public Sensor {
private:
    Temperatura* pTemper;
    bool alguem;

public:
    Presenca() : pTemper(nullptr) {
        (0 == cont%2) ? alguem = true : alguem = false;
        pTemper = new Temperatura;
    }
    ~Presenca() { delete pTemper; }

    const bool getAlguem() const { return alguem; }
    const int getTemperatura() const { return pTemper->getTemperatura(); }
    
    const bool getAmbosAtivos() const { 
        if(pTemper->getAtivo() && this->getAtivo()) return true;
        return false;
    }

    void operator++() { alguem++; }
};

class Temperatura : public Sensor {
private:
    int temperatura;

public:
    Temperatura() : temperatura(cont) {}
    ~Temperatura() { temperatura = 0; }

    operator++() { ++temperatura; }
    const int getTemperatura() const { return temperatura; }
};

#define max_andares 5

class Controle_Predio;

class Controle_Andar{
private:
    Presenca sens_pres;
    Controle_Predio* pCtrlPred;
    float oxigenio;
    bool incendio;

public:
    Controle_Andar(float O = 100.0): sens_pres(), pCtrlPred(nullptr), oxigenio(O), incendio(false) {}
    ~Controle_Andar() { delete pCtrlPred; }

    void setControlePredio(Controle_Predio* pCP) { pCtrlPred = pCP; }
    const float getOxigenio() const { return oxigenio; }
    const bool getIncendio() const { return incendio; }
    
    void controlar(){
        if(sens_pres.getAmbosAtivos() 
        && (sens_pres.getTemperatura() > 55) 
        &&  (sens_pres.getTemperatura >= pCtrlPred->getTemperatura())
        &&  sens_pres.getAlguem()) 
            incendio = true;

        if(sens_pres.getAmbosAtivos()
        && (sens_pres.getTemperatura() > 55) 
        && !(sens_pres.getAlguem())){
            oxigenio = 0.0;
            incendio = false;
        }
    }
};

class Controle_Predio {
private:
    int temperatura;
    Controle_Andar[max_andares] andares;

public:
    Controle_Predio(int T = 0): temperatura(T), andares() {
        for (int i = 0; i < max_andares; i++) andares[i].setControlePredio(this);
        executar();
    }
    ~Controle_Predio() { temperatura = 0; }

    const int getTemperatura() const { return temperatura; }
    
    void informar_Colapso const {
        if(sensor::TodosInativos()){
            cout << "Alerta de colapso!" << endl;
            return;
        }
        cout << "Colapso nao detectado." << endl;
    }

    void informar_Incendio() const {
        for (int i = 0; i < max_andares; i++){
            if(andares[i].getIncendio()) cout << "Incendio detectado no andar " << i << endl;
            else cout << "Incendio nao detectado no andar " << i << endl;
        }    
    }

    void executar(){
        for (int i = 0; i < temperatura; i++){
            Sensor::percorrer();
            for (int j = 0; j < max_andares; j++) andares[j].controlar();
        }
        informar_Colapso();
    }

};

int main() {
    Controle_Predio CP1(1);
    Controle_Predio CP2(30);
    Controle_Predio CP1(60);
    return 0;
}