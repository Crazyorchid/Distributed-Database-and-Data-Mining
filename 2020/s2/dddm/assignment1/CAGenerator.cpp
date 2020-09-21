#include<iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <tgmath.h>

using namespace std;

class AAtoCA{
public:
    int aa[50][50];
    int ca[50][50];
    int insertcolumn[50];
    int sequence[50];
    int length;
    int cont = 0;
    int bond = 0;
    ofstream outf;

    void readaa(vector<string>x){
        length = x.size();

        for (int i = 0; i < length; i++) {
            string temp = x[i];
            string buf;
            stringstream ss(temp);
            for (int j = 0; j < length; j++) {
                if(ss>>buf){
                    stringstream convert(buf);
                    convert >> aa[i][j];
                }

            }

        }
    }
    void toca(){
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < 2; j++) {
                ca[i][j]=aa[i][j];

            }

        }
        for (int i = 0; i < 2; i++) {
            sequence[i]=i;
            }
        for (int j = 0; j < length; j++) {
            for (int i = 0; i < length; i++) {
                insertcolumn[i] = aa[i][j];
            }
            int maxcont = 0;
            int loc = 0;
            for (int num = 0; num < j; num++) {
                docont(num,j);
                if(cont > maxcont){
                    maxcont = cont;
                    loc = num;

            }

        }
            for (int x = j; x >=loc; x--) {
                for (int i = 0; i < length; i++) {
                    ca[i][x+i]=ca[i][x];
                }
                sequence[x+1]=sequence[x];
            }
            for (int i = 0; i < length; i++) {
                ca[i][loc] = insertcolumn[i];
            }
            sequence[loc]=j;

        }
    }
    void docont(int num, int j){
        if(num==0){
            dobond(0, true);
            cont = bond;
        }
        if(num==j){
            dobond(j-1,true);
            cont = bond;
        }
        if(num!=0&&num!=j){
            dobond(num-1,true);
            int bond0 = bond;
            dobond(num, true);
            int bond1 = bond;
            dobond(num,false);
            int bond2 = bond;
            cont = bond0+bond1-bond2;
        }
    }
    void dobond(int a, bool b){
        bond=0;
        if(b== true){
            for (int i = 0; i < length; i++) {
                bond+=ca[i][a]*insertcolumn[i];
            }
        }else{
            for (int i = 0; i < length; i++) {
                bond+=ca[i][a-1]*ca[i][a];

            }
        }

    }

    void turnover(){
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < length; j++) {
                aa[j][i]=ca[i][j];
            }
        }
    }
    void printsequence(){
        outf.open("ca_2.txt");
        cout<<" ";
        for (int i = 0; i < length; i++) {
            //cout << "A" <<sequence[i] +1<<" ";
            cout << "A" <<sequence[i] +1<<" ";
        }
        outf<<endl;
        cout<<endl;
    }
    void printca(){
        for (int i = 0; i < length; i++) {
            outf << "A" <<sequence[i] + 1 << " ";
            cout << "A" <<sequence[i] + 1 << " ";
            for (int j = 0; j < length; j++) {
                outf<<ca[i][j];
                cout<<ca[i][j];
                if(j==length-1){
                    outf<<endl;
                    cout<<endl;
                }else{
                    outf<<" ";
                    cout<<" ";
                }

            }

        }
        outf.close();
    }
};

int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    string line;
    vector<string>aa;

    try{
        if(in){
            while(getline(in,line)){
                aa.push_back(line);
            }
        }
    }catch(const char* msg){
        cout <<msg <<endl;
    }
    AAtoCA myobj;
    myobj.readaa(aa);
    myobj.toca();
    myobj.printsequence();
    myobj.turnover();
    //myobj.printsequence();
    myobj.toca();
    myobj.printca();

    return 0;

}