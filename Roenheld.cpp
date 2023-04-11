#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

enum class StatEnum{
    STR,
    INT,
    CHR
};

enum class FoodEnum{
    CARNE,
    PESTE,
    LEGUME,
    PAINE,
    UNK
};

FoodEnum fromString(string const& s){
    if(s == "CARNE") return FoodEnum::CARNE;
    if(s == "PESTE") return FoodEnum::PESTE;
    if(s == "LEGUME") return FoodEnum::LEGUME;
    if(s == "PAINE") return FoodEnum::PAINE;
    return FoodEnum::UNK;
}

class Mancare{
    string nume{};
    int cantitate{}, pret{};

public:
    friend class ComportamentMancare;

    Mancare(string nume, int cantitate, int pret) : nume(move(nume)), cantitate(cantitate), pret(pret) {}

    string getNume(){
        return nume;
    }

    int getCantitate() const{
        return cantitate;
    }

    void setCantitate(int cant){
        cantitate = cant;
    }

    Mancare* addStat(map<StatEnum, int> &atribute){
        FoodEnum foodEnum = fromString(nume);
        switch(foodEnum){
            case FoodEnum::CARNE: atribute[StatEnum::STR] += 2; break;
            case FoodEnum::PESTE: atribute[StatEnum::INT] += 2; break;
            case FoodEnum::LEGUME:l. atribute[StatEnum::CHR] += 2; break;
            case FoodEnum::PAINE:
            default: break;
        }
        return this;
    }

    friend void substractStat(map<StatEnum, int> &atribute);
};

void substractStat(map<StatEnum, int> &atribute){
    for (auto & [atribut, valoare] : atribute){
        if(atribute[atribut] > 0){
            --atribute[atribut];
        }
    }
}

class ComportamentMancare{
    vector<Mancare*> mancare{};

public:
    ComportamentMancare() = default;
    explicit ComportamentMancare(vector<Mancare*> &mancare) : mancare(mancare) {};

    Mancare* cautaMancare(vector<FoodEnum> const &preferinte, int &bani){
        Mancare* ans = nullptr;
        for(auto &a : preferinte){
            for(auto &b : mancare){
                if(a == fromString(b->nume) && b->pret <= bani && b->cantitate > 0){
                    bani -= b->pret;
                    ans = b;
                    return ans;
                }
            }
        }
        return ans;
    }
};

class Munca {
    int plataDeBaza{};
    map<StatEnum, int> bonusPerAtribut{};

public:
    Munca(int plataDeBaza, const map<StatEnum, int> &bonusPerAtribut) :
        plataDeBaza(plataDeBaza), bonusPerAtribut(bonusPerAtribut) {}

    int calculeazaPlataPentru(map<StatEnum, int> const &atribute){
        int plata = plataDeBaza;
        for (auto const & [atribut, valoare]: atribute) {
            if (bonusPerAtribut[atribut] > 0) {
                plata += atribute.at(atribut) / bonusPerAtribut[atribut];
            }
        }
        return plata;
    }
};

class ComportamentMunca {
public:
    static Munca* cautaMunca(vector<Munca*> const &munci, map<StatEnum, int> const &atribute){
        Munca* ans = nullptr;
        for (Munca* munca : munci) {
            if (ans == nullptr || munca->calculeazaPlataPentru(atribute) > ans->calculeazaPlataPentru(atribute)) {
                ans = munca;
            }
        }
        return ans;
    }
};

class Jucator{
    string nume{};
    int bani{};
    vector<Munca*> munci{};
    map<StatEnum, int> atribute{};
    ComportamentMunca* comportamentMunca{};
    vector<FoodEnum> preferinte{};
    ComportamentMancare* comportamentMancare{};

public:
    friend vector<Jucator*> sortBeforeFood(vector<Jucator*> &jucatori);
    friend vector<Jucator*> sortBeforeOutput(vector<Jucator*> &jucatori);

    Jucator(string &nume, int bani, const vector<Munca*> &munci, map<StatEnum, int> const &atribute,
            ComportamentMunca* comportamentMunca, const vector<FoodEnum> &preferinte,
            ComportamentMancare* comportamentMancare) :
            nume(nume), bani(bani), munci(munci), atribute(atribute), comportamentMunca(comportamentMunca),
            preferinte(preferinte), comportamentMancare(comportamentMancare) {}

    void munceste(){
        auto muncaAleasa = comportamentMunca->cautaMunca(munci, atribute);
        bani += muncaAleasa->calculeazaPlataPentru(atribute);
    }

    string cumpara(){
        Mancare* mancareAleasa = comportamentMancare->cautaMancare(preferinte, bani);
        string mancareCump = "Nicio mancare!!";
        if(mancareAleasa != nullptr){
            mancareAleasa->addStat(atribute);
            mancareCump = mancareAleasa->getNume();
        } else{
            substractStat(atribute);
        }
        return mancareCump;
    }

    void afiseaza(){
        cout << nume << " " << bani << " " << atribute[StatEnum::STR] << " " << atribute[StatEnum::INT] << " "
        << atribute[StatEnum::CHR] << endl;
    }

    void deleteComportament(){
        delete comportamentMancare;
        comportamentMancare = nullptr;
    }

    void setComportament(ComportamentMancare* comp){
        comportamentMancare = comp;
    }
};

vector<Jucator*> sortBeforeFood(vector<Jucator*> &jucatori){
    sort(jucatori.begin(), jucatori.end(), [](auto &a, auto &b){
        if(a->atribute[StatEnum::CHR] == b->atribute[StatEnum::CHR]){
            if(a->bani == b->bani){
                return a->nume < b->nume;
            }
            return a->bani > b->bani;
        }
        return a->atribute[StatEnum::CHR] > b->atribute[StatEnum::CHR];
    });
    return jucatori;
}

vector<Jucator*> sortBeforeOutput(vector<Jucator*> &jucatori){
    sort(jucatori.begin(), jucatori.end(), [](auto &a, auto&b){
        if(a->bani == b->bani){
            return a->nume < b->nume;
        }
        return a->bani > b->bani;
    });
    return jucatori;
}

int main() {
    int N, M, T;
    cin >> N >> M >> T;
    vector<Munca*> munci;
    vector<Jucator*> jucatori;
    for(int i = 0; i < M; ++i){
        int plata, b1, b2, b3;
        cin >> plata >> b1 >> b2 >> b3;
        munci.push_back(new Munca(plata,
                                  {{StatEnum::STR, b1}, {StatEnum::INT, b2}, {StatEnum::CHR, b3}}));
    }
    for(int i = 0; i < N; ++i){
        string nume, prf1, prf2, prf3, prf4;
        int nrMunci;
        vector<Munca*> munciJuc;
        cin >> nume >> nrMunci >> prf1 >> prf2 >> prf3 >> prf4;
        for(int j = 0; j < nrMunci; ++j){
            int index;
            cin >> index;
            munciJuc.push_back(munci[index]);
        }
        jucatori.push_back(new Jucator(nume, 20, munciJuc,
                                       {{StatEnum::STR, 0}, {StatEnum::INT, 0}, {StatEnum::CHR, 0}},
                                       new ComportamentMunca(),
                                       {fromString(prf1), fromString(prf2), fromString(prf3), fromString(prf4)},
                                       new ComportamentMancare()));
    }
    for(int i = 0; i < T; ++i){
        int c1, p1, c2, p2, c3, p3, c4, p4;
        cin >> c1 >> p1 >> c2 >> p2 >> c3 >> p3 >> c4 >> p4;
        vector<Mancare*> mancare = {new Mancare("CARNE", c1, p1), new Mancare("PESTE", c2, p2),
                                    new Mancare("LEGUME", c3, p3), new Mancare("PAINE", c4, p4)};
        sortBeforeFood(jucatori);
        for(auto &a : jucatori){
            a->deleteComportament();
            a->setComportament(new ComportamentMancare(mancare));
            string mancareCump = a->cumpara();
            a->munceste();
            for(auto &b : mancare){
                if(mancareCump == b->getNume()){
                    int cant = b->getCantitate();
                    --cant;
                    b->setCantitate(cant);
                    break;
                }
            }
        }
    }
    sortBeforeOutput(jucatori);
    for(auto &a : jucatori){
        a->afiseaza();
    }
    return 0;
}
