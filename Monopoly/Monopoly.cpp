#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class ICamp{
public:
    virtual ~ICamp() = default;
};

class CampLiber : public ICamp{
public:
    CampLiber() = default;

    ~CampLiber() override = default;
};

class Proprietate : public ICamp{
protected:
    string nume{};
    double cost{}, chirie{};
public:
    Proprietate(string nume, double cost, double chirie) : nume(move(nume)), cost(cost), chirie(chirie) {}

    string getNume(){
        return nume;
    }

    double getChirie() const{
        return chirie;
    }

    double getCost() const{
        return cost;
    }

    ~Proprietate() override = default;
};

class ProprietateNormala : public Proprietate{
public:
    ProprietateNormala(string nume, double cost, double chirie) : Proprietate(move(nume), cost, chirie) {}
};

class Birou : public Proprietate{
public:
    Birou(string nume, double cost, double chirie) : Proprietate(move(nume), cost, chirie) {}
};

class Rezidential : public Proprietate{
    string numeComplex{};
public:
    Rezidential(string nume, double cost, double chirie, string numeComplex) : Proprietate(move(nume), cost, chirie),
                                                                                numeComplex(move(numeComplex)) {}

    string getNumeComplex(){
        return numeComplex;
    }
};

class Jucator{
protected:
    string nume{};
    double baniPerTura{};
    vector<Proprietate*> proprietatiDetinute{};
    bool statusEliminare{};
    double baniDetinuti{};
public:
    Jucator(string nume, double baniPerTura, bool statusEliminare, double baniDetinuti) : nume(move(nume)), baniPerTura(baniPerTura),
                                                                    statusEliminare(statusEliminare), baniDetinuti(baniDetinuti) {}

    virtual void interactiune(ICamp*, vector<Jucator*>) = 0;

    string getNume(){
        return nume;
    }

    double getBaniPerTura() const{
        return baniPerTura;
    }

    double getBaniDetinuti() const{
        return baniDetinuti;
    }

    void setBani(double bani){
        baniDetinuti = bani;
    }

    bool getStatusEliminare() const{
        return statusEliminare;
    }

    void eliminat(){
        statusEliminare = true;
        baniDetinuti = -1;
    }

    vector<Proprietate*> getListaProprietati(){
        return proprietatiDetinute;
    }

    void sortareProprietati(){
        sort(proprietatiDetinute.begin(), proprietatiDetinute.end(), [](auto &a, auto &b){
            return a->getNume() < b->getNume();
        });
    }
};

Jucator* verificareDetinator(vector<Jucator*> listaJucatori, Proprietate* proprietateCurenta) {
    for (auto &jucator: listaJucatori) {
        for (auto &proprietateDetinuta: jucator->getListaProprietati()) {
            if (proprietateDetinuta == proprietateCurenta) {
                return jucator;
            }
        }
    }
    return nullptr;
}

class Avar : public Jucator{
public:
    Avar(string nume, double baniPerTura, bool statusEliminare, double baniDetinuti) : Jucator(move(nume), baniPerTura,
                                                                                               statusEliminare, baniDetinuti) {}

    void interactiune(ICamp* campCurent, vector<Jucator*> listaJucatori) override{
        if(dynamic_cast<CampLiber*>(campCurent)){
            return;
        } else if(auto proprietateNormala = dynamic_cast<ProprietateNormala*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateNormala)){
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= proprietateNormala->getChirie()){
                    baniDetinuti -= proprietateNormala->getChirie();
                    double bani1 = detinator->getBaniDetinuti() + proprietateNormala->getChirie();
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                return;
            }
        } else if(auto proprietateBirou = dynamic_cast<Birou*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateBirou)){
                double sumaChirii = 0;
                for(auto &proprietateDetinuta : detinator->getListaProprietati()){
                    if(dynamic_cast<Birou*>(proprietateDetinuta)){
                        sumaChirii += proprietateDetinuta->getChirie();
                    }
                }
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= sumaChirii){
                    baniDetinuti -= sumaChirii;
                    double bani1 = detinator->getBaniDetinuti() + sumaChirii;
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                return;
            }
        } else if(auto proprietateRezidentiala = dynamic_cast<Rezidential*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateRezidentiala)){
                double sumaChirii = 0;
                for(auto &proprietateDetinuta : detinator->getListaProprietati()){
                    if(auto rezidential = dynamic_cast<Rezidential*>(proprietateDetinuta)){
                        if(rezidential->getNumeComplex() == proprietateRezidentiala->getNumeComplex()){
                            sumaChirii += proprietateDetinuta->getChirie();
                        }
                    }
                }
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= sumaChirii){
                    baniDetinuti -= sumaChirii;
                    double bani1 = detinator->getBaniDetinuti() + sumaChirii;
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                return;
            }
        } else{
            return;
        }
    }
};

class JucatorNormal : public Jucator{
public:
    JucatorNormal(string nume, double baniPerTura, bool statusEliminare, double baniDetinuti) : Jucator(move(nume), baniPerTura,
                                                                                                        statusEliminare, baniDetinuti) {}

    void interactiune(ICamp* campCurent, vector<Jucator*> listaJucatori) override{
        if(dynamic_cast<CampLiber*>(campCurent)){
            return;
        } else if(auto proprietateNormala = dynamic_cast<ProprietateNormala*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateNormala)){
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= proprietateNormala->getChirie()){
                    baniDetinuti -= proprietateNormala->getChirie();
                    double bani1 = detinator->getBaniDetinuti() + proprietateNormala->getChirie();
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                if(baniDetinuti >= proprietateNormala->getCost()){
                    proprietatiDetinute.push_back(proprietateNormala);
                    baniDetinuti -= proprietateNormala->getCost();
                    return;
                } else{
                    return;
                }
            }
        } else if(auto proprietateBirou = dynamic_cast<Birou*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateBirou)){
                double sumaChirii = 0;
                for(auto &proprietateDetinuta : detinator->getListaProprietati()){
                    if(dynamic_cast<Birou*>(proprietateDetinuta)){
                        sumaChirii += proprietateDetinuta->getChirie();
                    }
                }
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= sumaChirii){
                    baniDetinuti -= sumaChirii;
                    double bani1 = detinator->getBaniDetinuti() + sumaChirii;
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                if(baniDetinuti >= proprietateBirou->getCost()){
                    proprietatiDetinute.push_back(proprietateBirou);
                    baniDetinuti -= proprietateBirou->getCost();
                    return;
                } else{
                    return;
                }
            }
        } else if(auto proprietateRezidentiala = dynamic_cast<Rezidential*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateRezidentiala)){
                double sumaChirii = 0;
                for(auto &proprietateDetinuta : detinator->getListaProprietati()){
                    if(auto rezidential = dynamic_cast<Rezidential*>(proprietateDetinuta)){
                        if(rezidential->getNumeComplex() == proprietateRezidentiala->getNumeComplex()){
                            sumaChirii += proprietateDetinuta->getChirie();
                        }
                    }
                }
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= sumaChirii){
                    baniDetinuti -= sumaChirii;
                    double bani1 = detinator->getBaniDetinuti() + sumaChirii;
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                if(baniDetinuti >= proprietateRezidentiala->getCost()){
                    proprietatiDetinute.push_back(proprietateRezidentiala);
                    baniDetinuti -= proprietateRezidentiala->getCost();
                    return;
                } else{
                    return;
                }
            }
        } else{
            return;
        }
    }
};

class Investitor : public Jucator{
public:
    Investitor(string nume, double baniPerTura, bool statusEliminare, double baniDetinuti) : Jucator(move(nume), baniPerTura,
                                                                                                     statusEliminare, baniDetinuti) {}

    void interactiune(ICamp* campCurent, vector<Jucator*> listaJucatori) override{
        if(dynamic_cast<CampLiber*>(campCurent)){
            return;
        } else if(auto proprietateNormala = dynamic_cast<ProprietateNormala*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateNormala)){
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= proprietateNormala->getChirie()){
                    baniDetinuti -= proprietateNormala->getChirie();
                    double bani1 = detinator->getBaniDetinuti() + proprietateNormala->getChirie();
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                return;
            }
        } else if(auto proprietateBirou = dynamic_cast<Birou*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateBirou)){
                double sumaChirii = 0;
                for(auto &proprietateDetinuta : detinator->getListaProprietati()){
                    if(dynamic_cast<Birou*>(proprietateDetinuta)){
                        sumaChirii += proprietateDetinuta->getChirie();
                    }
                }
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= sumaChirii){
                    baniDetinuti -= sumaChirii;
                    double bani1 = detinator->getBaniDetinuti() + sumaChirii;
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                if(baniDetinuti >= proprietateBirou->getCost()){
                    proprietatiDetinute.push_back(proprietateBirou);
                    baniDetinuti -= proprietateBirou->getCost();
                    return;
                } else{
                    return;
                }
            }
        } else if(auto proprietateRezidentiala = dynamic_cast<Rezidential*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateRezidentiala)){
                double sumaChirii = 0;
                for(auto &proprietateDetinuta : detinator->getListaProprietati()){
                    if(auto rezidential = dynamic_cast<Rezidential*>(proprietateDetinuta)){
                        if(rezidential->getNumeComplex() == proprietateRezidentiala->getNumeComplex()){
                            sumaChirii += proprietateDetinuta->getChirie();
                        }
                    }
                }
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= sumaChirii){
                    baniDetinuti -= sumaChirii;
                    double bani1 = detinator->getBaniDetinuti() + sumaChirii;
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                return;
            }
        } else{
            return;
        }
    }
};

class Dezvoltator : public Jucator{
    int numarComplexe{};
    vector<string> complexeValabile{};
public:
    Dezvoltator(string nume, double baniPerTura, bool statusEliminare, double baniDetinuti, int numarComplexe) :
                                                                Jucator(move(nume), baniPerTura, statusEliminare, baniDetinuti),
                                                                                            numarComplexe(numarComplexe) {}

    bool verificareComplexValabil(Rezidential* proprietateCurenta){
        for(auto &complex : complexeValabile){
            if(complex == proprietateCurenta->getNumeComplex()){
                return true;
            }
        }
        return false;
    }

    void interactiune(ICamp* campCurent, vector<Jucator*> listaJucatori) override{
        if(dynamic_cast<CampLiber*>(campCurent)){
            return;
        } else if(auto proprietateNormala = dynamic_cast<ProprietateNormala*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateNormala)){
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= proprietateNormala->getChirie()){
                    baniDetinuti -= proprietateNormala->getChirie();
                    double bani1 = detinator->getBaniDetinuti() + proprietateNormala->getChirie();
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                return;
            }
        } else if(auto proprietateBirou = dynamic_cast<Birou*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateBirou)){
                double sumaChirii = 0;
                for(auto &proprietateDetinuta : detinator->getListaProprietati()){
                    if(dynamic_cast<Birou*>(proprietateDetinuta)){
                        sumaChirii += proprietateDetinuta->getChirie();
                    }
                }
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= sumaChirii){
                    baniDetinuti -= sumaChirii;
                    double bani1 = detinator->getBaniDetinuti() + sumaChirii;
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else{
                return;
            }
        } else if(auto proprietateRezidentiala = dynamic_cast<Rezidential*>(campCurent)){
            if(auto detinator = verificareDetinator(listaJucatori, proprietateRezidentiala)){
                double sumaChirii = 0;
                for(auto &proprietateDetinuta : detinator->getListaProprietati()){
                    if(auto rezidential = dynamic_cast<Rezidential*>(proprietateDetinuta)){
                        if(rezidential->getNumeComplex() == proprietateRezidentiala->getNumeComplex()){
                            sumaChirii += proprietateDetinuta->getChirie();
                        }
                    }
                }
                if(detinator == this){
                    return;
                } else if(baniDetinuti >= sumaChirii){
                    baniDetinuti -= sumaChirii;
                    double bani1 = detinator->getBaniDetinuti() + sumaChirii;
                    detinator->setBani(bani1);
                    return;
                } else {
                    double bani2 = detinator->getBaniDetinuti() + baniDetinuti;
                    detinator->setBani(bani2);
                    proprietatiDetinute.clear();
                    eliminat();
                    return;
                }
            } else {
                if (verificareComplexValabil(proprietateRezidentiala)) {
                    if (baniDetinuti >= proprietateRezidentiala->getCost()) {
                        proprietatiDetinute.push_back(proprietateRezidentiala);
                        baniDetinuti -= proprietateRezidentiala->getCost();
                        return;
                    } else {
                        return;
                    }
                } else if (!verificareComplexValabil(proprietateRezidentiala) && complexeValabile.size() < numarComplexe) {
                    if (baniDetinuti >= proprietateRezidentiala->getCost()) {
                        proprietatiDetinute.push_back(proprietateRezidentiala);
                        baniDetinuti -= proprietateRezidentiala->getCost();
                        complexeValabile.push_back(proprietateRezidentiala->getNumeComplex());
                        return;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            }
        } else{
            return;
        }
    }
};

class FabricaCamp {
    static ICamp *citesteSiConstruiesteCamp(ifstream &f) {
        string linie;
        getline(f, linie);
        stringstream s(linie);

        string tip;
        s >> tip;

        if (tip == "LIBER") {
            return new CampLiber();
        }
        if (tip == "PROPRIETATE") {
            string nume, tipProprietate;
            double cost, chirie;
            s >> nume >> cost >> chirie >> tipProprietate;

            if (tipProprietate == "NORMAL") {
                return new ProprietateNormala(nume, cost, chirie);
            }
            if (tipProprietate == "BIROU") {
                return new Birou(nume, cost, chirie);
            }
            if (tipProprietate == "REZIDENTIAL") {
                string numeComplex;
                s >> numeComplex;

                return new Rezidential(nume, cost, chirie, numeComplex);
            }
            return nullptr;
        }
        return nullptr;
    }

public:
    static vector<ICamp*> citesteCampuriDinFisier(const string& numeFisier) {
        ifstream f(numeFisier);
        vector<ICamp *> ans;

        while (true) {
            auto camp = citesteSiConstruiesteCamp(f);
            if (camp == nullptr) {
                break;
            }
            ans.push_back(camp);
        }
        return ans;
    }
};

class FabricaJucator {
    static Jucator *citesteSiConstruiesteJucator(ifstream &f) {
        string linie;
        getline(f, linie);
        stringstream s(linie);

        string nume, tip;
        double baniPerTura;
        s >> nume >> baniPerTura >> tip;

        if (tip == "AVAR") {
            return new Avar(nume, baniPerTura, false, 0);
        }
        if (tip == "NORMAL") {
            return new JucatorNormal(nume, baniPerTura, false, 0);
        }
        if (tip == "INVESTITOR") {
            return new Investitor(nume, baniPerTura, false, 0);
        }
        if (tip == "DEZVOLTATOR") {
            int numarComplexe;
            s >> numarComplexe;

            return new Dezvoltator(nume, baniPerTura, false, 0, numarComplexe);
        }
        return nullptr;
    }

public:
    static vector<Jucator*> citesteJucatoriDinFisier(const string& numeFisier) {
        ifstream f(numeFisier);
        vector<Jucator *> ans;

        while (true) {
            auto jucator = citesteSiConstruiesteJucator(f);
            if (jucator == nullptr) {
                break;
            }
            ans.push_back(jucator);
        }
        return ans;
    }
};

vector<int> citesteMutariDinFisier(const string& numeFisier) {
    ifstream f(numeFisier);
    vector<int> ans;
    int mutare;
    while (f >> mutare) {
        ans.push_back(mutare);
    }
    return ans;
}

int main() {
    string fisier1, fisier2, fisier3;
    cin >> fisier1 >> fisier2 >> fisier3;

    vector<ICamp *> tablaDeJoc = FabricaCamp::citesteCampuriDinFisier(fisier1);

    vector<Jucator *> listaJucatori = FabricaJucator::citesteJucatoriDinFisier(fisier2);

    vector<int> listaMutari = citesteMutariDinFisier(fisier3);

    int i = 0; //jucatori
    vector<int> pozitii(listaJucatori.size(), 0); //campuri

    for(auto &mutare : listaMutari){
        while(listaJucatori[i]->getStatusEliminare()){
            if(i == listaJucatori.size() - 1){
                i = 0;
            } else{
                ++i;
            }
        }
        auto& jucatorCurent = listaJucatori[i];
        double bani = jucatorCurent->getBaniDetinuti() + jucatorCurent->getBaniPerTura();
        jucatorCurent->setBani(bani);

        pozitii[i] += pozitii[i] + mutare >= tablaDeJoc.size() ? mutare - tablaDeJoc.size() : mutare;
        auto& campCurent = tablaDeJoc[pozitii[i]];

        jucatorCurent->interactiune(campCurent, listaJucatori);

        if(i == listaJucatori.size() - 1){
            i = 0;
        } else{
            ++i;
        }
    }

    for(auto &jucator : listaJucatori){
        jucator->sortareProprietati();
    }

    sort(listaJucatori.begin(), listaJucatori.end(), [](auto &a, auto &b){
        if(a->getBaniDetinuti() == b->getBaniDetinuti()){
            return a->getNume() < b->getNume();
        }
        return a->getBaniDetinuti() > b->getBaniDetinuti();
    });

    for(auto &jucator : listaJucatori){
        cout << jucator->getNume() << " " << jucator->getBaniDetinuti() << endl;
        for(auto &proprietate : jucator->getListaProprietati()){
            cout << proprietate->getNume() << " ";
        }
        cout << endl;
    }

    return 0;
}