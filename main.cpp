#include"Simaulation.h"
#include <iostream>
#include<vector>
#include<fstream>
#include<string>
#include <string.h>
#include<cstdlib> //aleatoire
#include<time.h>
#include<list>

using namespace std;


int main(){

     cout<<" \n   /////////////////////////////////////////////////////////////////////////\n "<<endl;

    cout<<" \n               ///////////////////////////////////                                                       \n "<<endl;
    cout<<" \n                         TRADING SIMULATOR                                \n "<<endl;
   cout<<" \n                ///////////////////////////////////                                              \n "<<endl;

    cout<<" \n   ////////////////////////////////////////////////////////////////////////////\n "<<endl;
    system("PAUSE");
    system("cls");



int test=1;

while(test==1){
        cout<<"\n                                     \n"<<endl;
cout<<"voulez-vous lancer une simulation?  \n tapez  (1) oui \n        (2) non "<<endl;
cin>>test;
if (test!=1) break;
 vector<PrixJournalier> PrixJournaliers;
    GestionHistorique imp;
    PrixJournaliers=imp.import("prices_simple.csv");

            date Deb;

            double soldInit;
            int nbreJr;
             Portefeuille P(0);
 cout<<"\n                                     \n"<<endl;
    cout<<"donnez la date de debut de la simulation"<<endl;
        Deb.saisir();

        cout<<"donnez le nombre de jour de la simulation"<<endl;
        cin>>nbreJr;
        cout<<"donnez votre solde initiale"<<endl;
        cin>>soldInit;
        system("PAUSE");
        system("cls");
         cout<<"\n                                     \n"<<endl;
cout<<"donnez le type de Bourse voulu: \n 1:Vector \n 2:Liste \n "<<endl;
Bourse* b;
int z;
cin>>z;

 switch(z)
        {

		case 1:{
		    b=new Vector(PrixJournaliers,Deb);
		    //double s =b->getPrixAction(Deb,"SS")->prix;
		   // cout<<s;


		}break;
		case 2:{
		    b=new ListVersion(Deb,PrixJournaliers);
		    //b=new Set(PrixJournaliers,dateAuj);
		}break;
		default:
            exit(-1);
        }
        system("PAUSE");
system("cls");
 cout<<"\n                                     \n"<<endl;
cout<<"donner le type de trader voulu: \n 1:humain \n 2:aleatoire \n 3:Algo1"<<endl;


  Trader* trd;

int x;
cin>>x;
 switch(x)
        {

		case 1:
            {cout<<"donner l'identifiant de trader"<<endl;
                string c;
                cin>>c;
                trd=new TraderHumain(c);



            }
             break;




        case 2:
            {  cout<<"donner l'identifiant du trader"<<endl;
                string c;
                cin>>c;
                trd =new TraderAleatoire(c);
                }break;


        case 3:
           {
                 cout<<"donner l'identifiant du trader"<<endl;
                string c;
                cin>>c;
            trd =new Algo1(c);}




        default:
            break;


        }
Simulation s(soldInit,nbreJr,b,trd,P,Deb);
s.lancerSimulation();
system("PAUSE");
system("cls");
cout<<"\n                                                                                                                     \n"<<endl;
cout<<"      *******                                   HISTORIQUE des simulations                  ********                  "<<endl;
string trace=s.HistroiqueSimulations();

    ifstream fi(trace, ios::in);  // on ouvre le fichier en lecture

        if(fi)
       {string ligne;
        while(getline(fi, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
                cout << ligne << endl;  // on l'affiche
        }}
        else  // sinon
                cerr << "Impossible d'ouvrir le fichier !" << endl;

system("PAUSE");
cout<<"\n                                     \n"<<endl;
system("cls");
}

return 0;
}



