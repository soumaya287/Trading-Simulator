#ifndef SIMAULATION_H_INCLUDED
#define SIMAULATION_H_INCLUDED
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <string.h>
#include<cstdlib> //aleatoire
#include<time.h>
#include<list>
using namespace std;
/////////////////////partie class prixJournalier/////////////
///////////////partie date///////////////
////////////class date///////////////
class date
{
private:
    int jour,mois,an;
public:

    void saisir(void);
    int valide(void);
    int longueurMois(void);
    void ajouter(int nbjours);
    int bissextile(void);
    int comparerA(date d);
    friend istream& operator>> (istream&f, date &d);
    friend ostream& operator<< (ostream &f, date &d);
};
/////////////////////PrixJournalier///////////////
class PrixJournalier
{
public:
    date date_;
    string action;
    double prix;
    friend ostream& operator<< (ostream& flux, PrixJournalier  & p);

    friend istream& operator>> (istream& flux, PrixJournalier  & p);
    friend class Bourse;
};
//////////////////gestionPrix/////////
class GestionHistorique
{
public:
    vector<PrixJournalier> static import(string chemin);
};
//////////////////////////////////Class Bourse//////////
class Bourse
{
protected:
    date dateActuelle;
public:
    Bourse(date d)
    {
        dateActuelle=d;
    };
    date getDate()
    {
        return this->dateActuelle;
    }

    virtual PrixJournalier* getPrixAction(date d,string nomAction)=0;


    virtual vector<string> getActionDisponible(date dateDonne)=0;
    virtual bool chercherAction(date d,string s)=0;
    virtual vector<PrixJournalier> getListePrixJourDisp(string ac)=0;
    virtual PrixJournalier getDernierPrixJournalierDe(string symbolAction)=0;
    friend class Simulation;

};
/////////////////////////////VECTOR//////////////////////////
class Vector:public Bourse
{
private:
    vector<PrixJournalier> pj;
    public:
    Vector(vector<PrixJournalier> pj,date d);

    int getTaille()
    {
        return pj.size();
    }

    vector<string> getActionDisponible(date dateDonne);

    PrixJournalier* getPrixAction(date d,string nomAction);
    bool chercherAction(date d,string s);
    vector<PrixJournalier> getListePrixJourDisp(string ac);
    PrixJournalier getDernierPrixJournalierDe(string symbolAction);
};
//////////////class Liste///////////
class ListVersion:public Bourse
{
private:
    list<PrixJournalier> listB;

public:
    ListVersion(date dateActuelleBourse,vector<PrixJournalier> pj);
    int getTaille(){return listB.size();};
      PrixJournalier* getPrixAction(date d,string nomAction);
    vector<string> getActionDisponible(date dateDonne);
     bool chercherAction(date d,string s);
     vector<PrixJournalier> getListePrixJourDisp(string ac);
    PrixJournalier getDernierPrixJournalierDe(string symbolAction);
    friend class Simulation;
};

////////////////////class Titre//////////
class Titre
{
private:
    string nom;
    int quantite;


public:
    Titre();
    Titre(string nom,int quantite=0);
    string getNom();
    int getQuantite();
    void ajouterQuantite(int);
    void retirerQuantite(int);
    friend ostream& operator <<(ostream&,Titre const&);
    friend istream& operator>>(istream&,Titre &);
    friend class Portefeuille;
    friend class Simulation;//dans la methode actionen possession la simulation accède aux titres
};
///////////portefeuille/////////
class Portefeuille
{
private:
    double sold;//initialiser par le sold initiale et change chaque vente ou achat


    int  nbtitres;
    Titre * tab[100];


public:

    Portefeuille(double);
    double getSold()const
    {
        return sold;
    };
    int chercherUnTitre(string);
    void acheterUneAction(string,int);
    void vendreUneAction(string,int);
    int getnombre() const;//on fait appel dans le trader qui utilise une copie constante du portefeuille
    friend ostream& operator <<(ostream&,Portefeuille&);
    vector<Titre> getValeursTitres()const;

    ~Portefeuille()
    {
        cout << "Destructeur de Portefeuille.\n";
        for(int i=0; i<nbtitres; i++)
        {
            delete tab[i];
            nbtitres=0;
        }
    };
    friend class Simulation;
};
///////////Transaction////////
enum TypeTransaction {VENTE=2,ACHAT=1,RIEN=0};
class Transaction
{
private:
    TypeTransaction type;
    string action;
    int qtite;

public:
    Transaction() {};

    Transaction(string nomAction,int quantite,TypeTransaction t)
    {

        action=nomAction;
        qtite=quantite;
        type=t;
    }

    string getNomAction()
    {
        return action;
    };
    TypeTransaction getType()
    {
        return type;
    };
    int getQuantite()const
    {
        return qtite;
    };
    friend istream& operator>> ( istream& in, TypeTransaction& x );
    friend ostream& operator<<( ostream& out,TypeTransaction& x);
    bool operator==(const  Transaction&tr){
    if ((qtite==tr.qtite) && (action==tr.action) &&(type==tr.type));
    return true;

    };
    friend class Simulation;
    friend class Trader;


};
//////////Class Trader///////////////
class Trader
{

protected:
    string id;
public:
    Trader(string i)
    {
        id=i;
    };
    virtual Transaction decider(Bourse& b,const Portefeuille  & p)=0;//une copie constante du portefeuille pour forcer au trader de ne pas changer les données du porteuille(accés sauf aux membres statique)



};
//////////////////HUMAIN/////////
////////////////trader humain////////
class TraderHumain: public Trader
{
public:
    TraderHumain(string n):Trader(n) {};
    Transaction decider(Bourse & b,const Portefeuille &p);
};
/////////tarder aleatoire/////////////
class TraderAleatoire:public Trader
{
public:
    TraderAleatoire(string c):Trader(c) {};
    Transaction decider(Bourse& b,const Portefeuille&  p);

};
/////////////Tarder Algo////
class Algo1:public Trader
{
    vector<Transaction> transactionsDejour;
public:
    Algo1(string n):Trader(n) {};
    Transaction decider(Bourse & b,const Portefeuille &p);
    double moyPrixAction(string ac, Bourse&b );
    ~Algo1();
};
////////class Simulation//////
class Simulation
{
private:
    date dateAujour;
    date dateDebut;
    double soldeInitiale;
    Portefeuille port;
    int nbrJour;
    Trader* trd;
    Bourse* b;
public:
    Simulation(double soldeInitiale,int nbrJour,Bourse *b,Trader* trd,Portefeuille p,date dateDebut);
    Portefeuille getPortefeuille()
    {
        return port;
    };
    Trader* getModeDecision()
    {
        return trd;
    };
    date getDateAujour()
    {
        return dateAujour;
    };
    void simulerUnJour();
    void lancerSimulation();
    double MontantRestant();
    double Montanttotale();
    double Montantgain();
    double GainEnPourcentage();
    vector<string> ActionEnPocession();
    string HistroiqueSimulations();

};
class Trace
{ date db;
   double soldedb;
double montantgain;
float gainP;
int periode;

 public:
    Trace(date d,int p,double s,double mg,float g):db(d),periode(p),soldedb(s),montantgain(mg),gainP(g){};

	 friend ostream& operator<< (ostream& flux, Trace& e);
	 friend istream& operator>> (istream& flux, Trace& e);

};



#endif // SIMAULATION_H_INCLUDED
