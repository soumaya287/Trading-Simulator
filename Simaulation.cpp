#include<iostream>
#include"Simaulation.h"
#include<vector>
#include<fstream>
#include<string>
#include <string.h>
#include<cstdlib> //aleatoire
#include<time.h>
#include<list>
#include<algorithm>
#include <stdexcept>
////////////////class Date//////////
int lngMois[12]= {31,28,31,30,31,30,31,31,30,31,30,31};

int  operator== (date &d1, date &d2)
{
    return d1.comparerA(d2)==0;
}

int date::comparerA(date d)
{
    int r;
    if(d.an>an)r=1;
    else if(d.an<an)r=-1;
    else if(d.mois>mois)r=1;
    else if(d.mois<mois)r=-1;
    else if(d.jour>jour)r=1;
    else if(d.jour<jour)r=-1;
    else r=0;
    return r;
}
void date::ajouter(int nbjours)
{
    jour+=nbjours;
    while(jour>longueurMois())
    {
        jour-=longueurMois();
        mois++;
        if(mois>12)
        {
            mois=1;
            an++;
        }
    }
    while(jour<1)
    {
        mois--;
        if(mois<1)
        {
            mois=12;
            an--;
        }
        jour+=longueurMois();
    }
}

int  operator<= (date &d1, date &d2)
{
    return d1.comparerA(d2)>=0;
}
int date::bissextile(void)
{
    return((an%4)==0); //retourne 0 ou 1
}
int date::longueurMois(void)
{
    int l;
    l=lngMois[mois-1];
    if(mois==2) l+=bissextile(); //ajoute 1 ou 0
    return l;
}
int date::valide(void)
{
    int ok=1;
    if (mois<=0 || mois>12)
    {
        cout<<"mois invalide\n";
        ok=0;
    }
    if(jour<0 || jour>longueurMois())
    {
        cout<<"jour invalide\n";
        ok=0;
    }
    return ok;
}
void date::saisir(void)
{
    char c; //c contiendra le caractere de separation, sans rien tester
    do
    {
        cout<<"entrez la date au format jj/mm/aaaa : ";
        cin>>jour>>c>>mois>>c>>an;
    }
    while(!valide());
}


//surcharges des operateurs
int operator< (date &d1, date &d2)
{
    return d1.comparerA(d2)>0;
}
ostream& operator<< (ostream &f, date &d)
{
    f<<d.jour<<"/"<<d.mois<<"/"<<d.an;
    return f;
}
int  operator!= (date &d1, date &d2)
{
    return d1.comparerA(d2)!=0;
}
istream& operator>> (istream&flux, date &d)
{
    char tmp[1000];
    flux.getline(tmp,1000,'/');
    d.jour=atoi(tmp);
    flux.getline(tmp,1000,'/');
    d.mois=atoi(tmp);
    flux.getline(tmp,1000,';');
    d.an=atoi(tmp);
    return flux;
}
//////////////////////PrixJournalier///////////
istream& operator>> (istream& flux, PrixJournalier& P)
{
    flux>>P.date_;
    char tmp[1000];
    flux.getline(tmp,1000,';');
    P.action=tmp;
    flux>>P.prix;
    return flux;
}

ostream& operator<< (ostream& flux, PrixJournalier  & p)
{
    flux<<  p.date_<<";"<<p.action<<";"<<p.prix<<endl;
    return flux;
}
bool myComp( PrixJournalier& a, PrixJournalier& b )
{
    return a.date_<=b.date_;

}
///////////////////////GestionHistorique//////////////
vector<PrixJournalier> GestionHistorique:: import(string chemin)

{
    vector<PrixJournalier> pp;
    ifstream f("prices_simple.csv");
    int nbPrixJournalier= 0;
    string entete;
    if(f.is_open())
    {
        f>>entete;
        while(!f.eof())
        {

            PrixJournalier* P =new PrixJournalier ;
            f>>(*P);

            pp.push_back(*P);
            nbPrixJournalier++;

        }
    }
    cout<<nbPrixJournalier<<" lignes lues"<<endl;
    return pp;
}
////////////////class Liste////
ListVersion:: ListVersion(date dateActuelleBourse,vector<PrixJournalier> pj):Bourse(dateActuelleBourse)
{

    for(int i=0; i<pj.size(); i++)

    {
        listB.push_back(pj.at(i));

    }



}
PrixJournalier* ListVersion::getPrixAction( date d,string nomAction)
{

    PrixJournalier* j=new PrixJournalier;
    for(PrixJournalier x: listB)
    {
        *j=x;
        if(x.date_==d && x.action==nomAction)
        {
            return j;
        }
    }
}

vector<string> ListVersion:: getActionDisponible(date dateDonne)
{
    vector<string> ac;
    int nbaction=0;
    if (dateDonne<=dateActuelle)
    {


        for(PrixJournalier x: listB)
        {
            if(x.date_==dateDonne)
            {
                ac.push_back(x.action);
                nbaction++;

            }
        }
    }
    return ac;



}
bool ListVersion:: chercherAction(date d,string s)
{
    list<PrixJournalier>::iterator itr;
    for (itr = listB.begin(); itr != listB.end(); ++itr)

    {
        if ((itr->date_==d) && (itr->action==s))
            return true;
        else
            return false;


    }
}

PrixJournalier ListVersion::getDernierPrixJournalierDe(string symbolAction)
{
    vector<PrixJournalier> vj= getListePrixJourDisp(symbolAction);

    sort(vj.begin(),vj.end(),myComp);
    return(vj.back());

}
vector<PrixJournalier> ListVersion:: getListePrixJourDisp(string ac)
{
    vector<PrixJournalier> pj;

    int nbPrixj=0;
    for(PrixJournalier x: listB)
    {
        if(x.action==ac )
            if(x.date_<=dateActuelle)
            {
                pj.push_back(x);
                nbPrixj++;
            }
    }
    return pj;



}
//////////////class vector////////////
bool Vector:: chercherAction(date d,string s)
{
    if (dateActuelle <d)
    {

        return false;
    }
    else
    {
        int i=0;

        while((i<getTaille()) && ((pj.at(i).date_!=d)||(pj.at(i).action!=s)))
        {
            i++;
        }
        if(i<getTaille())
        {
            return true;
        }
        else
            return false;
    }

}

Vector::Vector(vector<PrixJournalier> vec,date d):Bourse(d)
{
    pj=vec;
}


PrixJournalier* Vector::getPrixAction(date d,string nomAction)
{
    if(d<=dateActuelle)

    {
        for(int i=0; i<getTaille(); i++)
        {
            if ((pj.at(i).date_==d )&&(pj.at(i).action==nomAction))
                return &pj.at(i);

        }
    }
    else
        return nullptr;
}
vector<string> Vector:: getActionDisponible(date dateDonne)
{
    vector<string> ac;
    if(dateDonne<=dateActuelle)
    {
        for(int i=0; i<getTaille(); i++)
        {
            if (pj.at(i).date_==dateDonne)
            {
                ac.push_back(pj.at(i).action);
            }
        }
        return ac;

    }
    else
        exit(1);

}
vector<PrixJournalier> Vector:: getListePrixJourDisp(string ac)
{
    vector<PrixJournalier> v;
    int j=0;
    for(int i=0; i<pj.size(); i++)
    {
        if (pj.at(i).action==ac)

            if(pj.at(i).date_<=dateActuelle)
            {

                v.insert(v.end(),pj.at(i));
                j++;
            }


    }

    return v;
}


PrixJournalier Vector::getDernierPrixJournalierDe(string symbolAction)
{
    vector<PrixJournalier> vj= getListePrixJourDisp(symbolAction);
    sort(vj.begin(),vj.end(),myComp);
    return(vj.back());


}
///////////////class Titre/////////////////////////////////////////////
Titre::Titre()
{
    nom="google";
    quantite=1;
}

Titre::Titre(string n,int q)
{
    nom= n;
    quantite= q;
}
int Titre::getQuantite()
{
    return quantite;
}



string Titre::getNom()
{
    return nom;
}

void Titre::ajouterQuantite(int q)
{
    quantite+=q;
}
void Titre::retirerQuantite(int q)
{
    if (q>quantite)
        cout<<"quantite insuffisante!!"<<endl;
    else
        quantite-=q;
}

ostream& operator<<(ostream &flux,Titre const& ent)
{
    flux<<"("<<ent.nom<<","<<ent.quantite<<")"<<endl;
    return flux;
}

istream& operator >>(istream &in,Titre & ent)
{
    in>> ent.nom>>ent.quantite;
    return in;
}
///////////////////classPortefeuille////////////
Portefeuille::Portefeuille(double s=0)
{

    sold=s;
    nbtitres=0;

}
ostream& operator<<(ostream &flux,Portefeuille & sm)
{
    flux<<"le portefeuille est de solde iniatiale= "<<sm.sold<<" et comporte "<<sm.nbtitres<<" Titres :"<<endl;
    for(int i=0; i<sm.nbtitres; i++)
        flux<<i+1<<"-"<<*sm.tab[i];

    return flux;

}

int Portefeuille::chercherUnTitre(string nomA)
{
    int i=0;
    while(i<nbtitres && tab[i]->nom!=nomA)
    {
        i++;
    }
    if(i<nbtitres)
    {
        return i;
    }
    else
        return -1;

}
void Portefeuille::acheterUneAction(string nomA,int q)
{

    if (chercherUnTitre(nomA)==-1)
    {
        tab[nbtitres]=new Titre(nomA,q);
        nbtitres++;

    }
    else
    {
        tab[chercherUnTitre(nomA)]->ajouterQuantite(q);

    }

}
void Portefeuille::vendreUneAction(string nomA,int q)
{
    Titre nvtitre(nomA,q);
    if (chercherUnTitre(nomA)!=-1)
    {
        if (q>=(tab[chercherUnTitre(nomA)]->quantite))
        {
            delete (tab[chercherUnTitre(nomA)]);
            tab[chercherUnTitre(nomA)]=tab[nbtitres-1];
            nbtitres--;
        }
        else
        {
            tab[chercherUnTitre(nomA)]->retirerQuantite(q);
        }
    }
}
int Portefeuille::getnombre()const
{
    return nbtitres;
}
vector<Titre> Portefeuille:: getValeursTitres()const
{
    vector<Titre> vect;
    for(int i=0 ; i<nbtitres; i++)
    {
        vect.push_back(*tab[i]);

    }
    return vect;


}
/////////////////tarnsaction///////////
TypeTransaction randomType()
{
    int low(RIEN), high(VENTE);
    return TypeTransaction(low + double(rand()) / (1.0 + RAND_MAX / (high - low + 1.0)));
}
istream& operator>> ( istream& in, TypeTransaction& x )
{
    int val;

    if ( in>> val )
    {
        switch ( val )
        {
        case VENTE:
        case ACHAT:
        case RIEN:
            x = TypeTransaction(val);
            break;
        default:
            throw out_of_range ( "Invalid value for typeTransaction" );
        }
    }

    return in;
}
ostream& operator<< ( ostream& stm, TypeTransaction& clr )
{
    switch(clr)
    {
    case TypeTransaction::ACHAT :
        return stm << "ACHAT" ;
    case TypeTransaction::VENTE :
        return stm << "VENTE" ;
    case TypeTransaction::RIEN :
        return stm << "RIEN" ;

    }
}
////////////////////trader Humain//////

Transaction TraderHumain::decider(Bourse & b,const Portefeuille &p)
{
    TypeTransaction i;

    cout<<"what will you do :\n ACHAT(1)? \n VENTE(2)? \n RIEN Faire Et Passer Au Jour Suivant(0)?"<<endl;
    cin>>i;
    if( i==ACHAT)
    {

        string ac;
        cout<<"quelle action voulez-vous acheter?"<<endl;
        cin>>ac;

        cout<<"combien voulez vous acheter?"<<endl;
        int q;
        cin>>q;

        Transaction trs(ac,q,i) ;
        // cout<<"achat!!"<<endl;
        return trs;
    }

    if(i==VENTE)
    {
        string ac;
        cout<<"quelle action voulez-vous vendre?"<<endl;
        cin>>ac;

        cout<<"combien voulez vous vendre?"<<endl;
        int q;
        cin>>q;
        Transaction trs(ac,q,i) ;
        //cout<<"vente!!!!!"<<endl;
        return trs;
    }
    else

    {

        Transaction trs("",0,RIEN) ;
        return trs;
    }



}

Transaction TraderAleatoire::decider(Bourse& b,const Portefeuille & p)
{


    TypeTransaction ty;

    vector<string> disp;
    int pos1,pos2,k,d,var;
    double qmax;
    string nom;
    srand(time(0));

     ty=randomType();
    switch(ty)
    {
    case ACHAT:
    {
        vector<string> disp;
        cout<<"votre decision: "<<ty<<endl;
        disp=b.getActionDisponible(b.getDate());
        k=disp.size();
        cout<<"vous avez "<<k<<" actions aujourd'hui "<<endl;
       // for(int i=0; i<k; i++)
         //   cout<<disp.at(i)<<endl;
        if(k==0)
        {
            ty=randomType();
        }
        else
        {
            if(k==1)
                pos1=0;
            else
                pos1=rand()% k ;

            //cout<<"vous choisissez l'action: ";
            //cout<<disp.at(pos1)<<endl;
            double u=b.getPrixAction(b.getDate(),disp.at(pos1))->prix;
            // cout<<"qui a un prix=  "<<u<<endl;
            qmax=p.getSold()/ (b.getPrixAction(b.getDate(),disp.at(pos1))->prix);
            cout<<"la quantite maximale  "<<qmax<<" --> vous pouvez achetez ";
            var=static_cast<int>(qmax);
            cout<<var<<" quantite(s) de cette action "<<endl;
            if (var==0)
            {
                cout<<"Essayez encore"<<endl;


                ty=randomType();

                //pour assurer que la transaction ne sort pas =>simulerUnjour marche sinon elle sort
            }

            if(var>=1)
            {

                d=(rand()% var)+1;
                nom=disp.at(pos1);
                cout<<"l'action a acheter: "<<nom;
                cout<<"  avec une quantite= "<<d<<endl;
            }

            Transaction t(nom,d,ty);

            return t;
        }
    }
    case VENTE:
    {
        cout<<"votre decision: "<<ty<<endl;
        if (p.getnombre()!=0)
        {
            pos2=rand()% p.getnombre();

            nom=p.getValeursTitres().at(pos2).getNom();
            // cout<<"l'action a vendre: "<<nom;
            d=rand()% p.getValeursTitres().at(pos2).getQuantite() +1;//choisir une quantité aléatoire(getters puisque le trader a une copie et n'est pas friend)
            //cout<<"  a la quantite= "<<d<<endl;
            Transaction t(nom,d,ty);

            return t;
        }


        cout<<"votre portefeuille est vide!! Essayez encore"<<endl;
        ty=randomType();


    }
    case RIEN:
    {
        // cout<<"vous choisissez de rien faire "<<endl;
        d=0;
        nom="";
        Transaction t(nom,d,ty);


        return t;
    }
    }






}

/////////////////trader algo///
Transaction Algo1::decider(Bourse & b,const Portefeuille &p)
{
    TypeTransaction c;
    int n;
    date dte=b.getDate();


    vector<string> v;
    v=b.getActionDisponible(dte);
    n=v.size();

    int i;
    while (i<n)
    {
        if( (moyPrixAction(v[i],b)>b.getPrixAction(dte,v[i])->prix)&&(p.getSold()>=b.getPrixAction(dte,v[i])->prix)&&b.chercherAction(dte,v[i])==true)
        {


            c=ACHAT;
            Transaction t(v[i],2,c);
            transactionsDejour.push_back(t);

        }
        if( (moyPrixAction(v[i],b)<b.getPrixAction(dte,v[i])->prix)&&b.chercherAction(dte,v[i]))
        {

            c=VENTE;
            Transaction t(v[i],2,c);
            transactionsDejour.push_back(t);


        }
        else
        {
            break;

        }

    }
    Transaction t("",0,RIEN);
    transactionsDejour.push_back(t);
    int r=rand()%(transactionsDejour.size());

    Transaction &tt= transactionsDejour.at(r);
    vector<Transaction>::iterator it;
    it=find(transactionsDejour.begin(),transactionsDejour.end(),tt);
    transactionsDejour.erase(it);

    return tt;

}
double Algo1:: moyPrixAction(string ac,Bourse& b )
{
    vector<PrixJournalier> vt=b.getListePrixJourDisp(ac);
    double s=0;
    int l=vt.size();
    for(int i=0; i<l; i++)
        s+=vt.at(i).prix;
    return s/l;
}
Algo1::~Algo1()
{
    cout << "Destructeur de TraderAlgo.\n";
}
////////////simulation///////////
Simulation::Simulation(double s,int n,Bourse* br,Trader* tr,Portefeuille p,date d)
{
    dateDebut=d;
    soldeInitiale=s;
    nbrJour=n;
    trd=tr;
    b=br;
    port=p;
    port.sold=s;

    dateAujour=d;

}
void Simulation::simulerUnJour()//aujourd'hui
{
    double solde;
    cout<<"\n                                     \n"<<endl;
     cout<<"******** vous simulez un jour ***********"<<endl;

    Transaction i=trd->decider(*b,port);

    cout<<"la decision choisie "<<i.type<<endl;
    while (1)
    {   cout<<"\n                                     \n"<<endl;


        if (i.type==RIEN)
        {
            cout<<"vous decidez de rien faire"<<endl;
            break;
        }
        if (i.type==ACHAT)
        {
            cout<<" action choisie: "<<i.action<<endl;
            if(b->chercherAction(b->getDate(),i.action)==true)
            {
                double s;

                s=(b->getPrixAction(b->getDate(),i.action))->prix ;
                cout<<"le prix de l'action "<<s<<endl;
                double prixRecommandee=s*(i.qtite);
                cout<<"le prix necessaire pour effectuer cette transaction= "<<prixRecommandee<<endl;
                if(port.sold>= prixRecommandee)//verifier l'achat//)

                {
                    //verifier l'achat//
                    port.sold-=prixRecommandee;
                    port.acheterUneAction(i.action,i.qtite);
                    // cout<<"l'action achetee: "<<i.action<<endl;
                    cout<<"le solde du portefeuille devient= "<<port.getSold()<<endl;
                }
                else
                {
                    cout<<"votre solde est insuffisant!!!! donnez donc une autre decision"<<endl;
                    i=trd->decider(*b,port);
                    if(i.type==RIEN )break;
                }
}

            else
            {
                cout<<"cette action n'existe pas aujourd'hui donnez donc une autre decision"<<endl;

                i=trd->decider(*b,port);
                if(i.type==RIEN )
                {
                    cout<<"votre choix est rien faire "<<endl;
                    break;
                }


            }
        }



        if (i.type==VENTE )
        {
            if(b->chercherAction(dateAujour,i.action)==true)
            {
                if (port.chercherUnTitre(i.action)!=-1)

                {

                    cout<<"action a vendre: "<<i.action<<endl<<"avec une quantite= "<<i.qtite<<endl;

                    double s=(b->getPrixAction(dateAujour,i.action))->prix ;
                    cout<<"prix action: "<<s<<endl;
                    port.sold+=s*(i.qtite);
                    port.vendreUneAction(i.action,i.qtite);
                    cout<<"le solde du portefeuille devient= "<<port.getSold()<<endl;
                }
               else
                {
                    cout<<"vous n'avez pas cette action dans votre portefeuille donnez donc une autre decision"<<endl;
                    i=trd->decider(*b,port);
                    if(i.type==RIEN )break;
                }
            }
            else
            {
                cout<<"action que vous desirez vendre: "<<i.action<<endl<<"a une quantite= "<<i.qtite<<endl;
                cout<<"Transaction echouee!!"<<endl;
                cout<<"cette action n'existe pas aujourd'hui donnez donc une autre decision"<<endl;
                i=trd->decider(*b,port);
                if(i.type==RIEN )
                {
                    cout<<"votre choix est rien faire "<<endl;
                    break;
                }
            }}
 i=trd->decider(*b,port);
        }



  cout<<"\n                                     \n"<<endl;
    cout<<"FIN JOUR----->>JOUR SUIVANT ";

}
void Simulation::lancerSimulation()


{
    int i=1;
    while(i<=nbrJour)
    {  cout<<"\n                                     \n"<<endl;
        cout<<"........Chargement de la Simulation......"<<endl;
         cout<<"\n                                     \n"<<endl;
        cout<<"        Go ====> "<<" JOUR numero "<<i<<endl;
        cout<<"\n                                     \n"<<endl;
        system("PAUSE");
        system("cls");
        simulerUnJour();

        dateAujour.ajouter(1);
        cout<<dateAujour<<endl;
        b->dateActuelle.ajouter(1);

        ++i;

        system("PAUSE");
        system("cls");
    }
    cout<<"\n                                     \n"<<endl;
    cout<<"FIN SIMULATION."<<endl;
    cout<<"\n                                     \n"<<endl;
}



double Simulation::MontantRestant()
{
    return port.sold;
}
double Simulation::Montanttotale()
{
    double soldeActionTrouve,soldeActionNonTrouve,prixAction,montantT;

    vector<Titre> vec=port.getValeursTitres();

    for(int i=0; i<vec.size(); i++)
    {
        if (b->chercherAction(b->getDate(),vec.at(i).getNom())==false)
        {
            //cout<<"Action non trouvee aujourd'hui "<<endl;
            prixAction=b->getDernierPrixJournalierDe(vec.at(i).getNom()).prix;
           // cout<<"le dernier prix de l' action non trouvee= "<<prixAction<<endl;
            soldeActionNonTrouve+=prixAction*vec.at(i).getQuantite()+ MontantRestant();

        }
        else
        {
            //cout<<"Action exite aujourd'hui "<<endl;
            prixAction=b->getPrixAction(b->dateActuelle,vec.at(i).getNom())->prix;
           // cout<<"le prix de cette action= "<<prixAction;
            soldeActionTrouve+=prixAction*vec.at(i).getQuantite()+ MontantRestant();
        }
        montantT+=soldeActionNonTrouve+soldeActionTrouve;

    }
    return montantT;
}

double Simulation::Montantgain()
{
         if (soldeInitiale>=Montanttotale())
            return 0;
         else
    return (Montanttotale()- soldeInitiale);


}


double Simulation::GainEnPourcentage()
{
    double s=Montantgain();
    if (Montanttotale()==0|| soldeInitiale==0)
        return 0;
    else

        return(((s*100)/soldeInitiale));
}



vector<string>  Simulation::ActionEnPocession()
{
    vector<string> vec;
    for(int i=0; i<port.nbtitres; i++)
    {
        vec.push_back(port.tab[i]->nom );
    }
    return vec;


}
////trace////////////////
ostream& operator<< (ostream& flux, Trace& e)
{
    flux<<"date debut simulation: "<<  e.db<<";"<<" duree: "<< e.periode<<"jour(s)"<<";"<<" solde initiale: "<<e.soldedb<<";"<<"montant gain: "<< e.montantgain<<";"<<"Gain: "<< e.gainP<<" %"<<endl;
    return flux;
}
istream& operator>> (istream& flux, Trace& e)
{
    flux>>e.db>>e.soldedb >> e.montantgain>> e.gainP;

    return flux;
}

string  Simulation:: HistroiqueSimulations()
{
    string mon_fichier = "trace.txt";
    ofstream fichier(mon_fichier, ios::app );  // ouverture en écriture avec effacement du fichier ouvert

    if(fichier)
    {
        double s=Montantgain();
        Trace t(dateDebut,nbrJour,soldeInitiale,s,GainEnPourcentage());
        fichier << t;

        fichier.close();
    }
    else
        cerr << "Impossible d'ouvrir le fichier !" << endl;


    return mon_fichier;



}
