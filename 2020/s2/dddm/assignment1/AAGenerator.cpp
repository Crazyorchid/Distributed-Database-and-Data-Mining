#include<iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstdio>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

class toAA{
    public:
    int queryS[30][30];
    int wqueryS = 0; //width of the query;
    int hqueryS = 0; // height of the query;

    string bufftmp[2];
    int queryA[30][30];
    int wqueryA = 0;
    int hqueryA = 0;

    int aa[30][30];

void readattAndQuery ()
{

    std::ifstream aa("att_2.txt",std::ios_base::binary);
    string first_line;
    vector<string> read;
    try{
        if(aa){
            while(getline(aa, first_line)){
                read.push_back(first_line);
            }
        }
    } catch (const char* msg) {
        cout << msg << endl;
    }
    // cout the number stored in the txt to vector


    ifstream qu("query_2.txt",std::ios_base::binary);
    string second_line;
    vector<string> read2;
    try{
        if(qu){
            while(getline(qu, second_line)){
                read2.push_back(second_line);
            }
        }
    } catch (const char* msg) {
        cout << msg << endl;
    }
    hqueryA = read2.size();
    wqueryA = read2.size()-1;

    //the matrix of aa and query and put into query2[][];

    for (int i = 0; i < read2.size(); i++) {
        for (int j = 0; j < read.size(); j++) {
            string buff;
            stringstream ss(read[j]);
            for (int k = 0; k < 2; k++) {
                if (ss >> buff){
                    bufftmp[k] = buff;
                }
            }
            bool found = findATT(read2[i],bufftmp[1]);
            if(found){
                queryA[i][j-1] = 1;
            }else {
                queryA[i][j - 1] = 0;
            }

        }
    }
    //cout << queryA[10][10]<<endl;

    }
    bool findATT(string a, string b) {
        int alength = a.length();
        int blength = b.length();
        bool is = false;
        for (int i = 8; i < alength; i++) {
            if (a[i] == b[0] && alength - i >= blength) {
                if (a.substr(i, blength) == b) {
                    if (a[i - 1] - '0' < 17 || a[i - 1] - '0' > 42) {
                        if (alength - i == blength) {
                            is = true;
                        } else {
                            if (a[i + blength] - '0' < 17 || a[i+blength] - '0' > 42) {
                                is = true;
                            }
                        }
                    }
                }
            }
        }
        return is;
    }

void readacc(){
    ifstream in("acc_2.txt",std::ios_base::binary);
    string line;
    vector<string> t;
    try{
        if(in){
            while(getline(in, line)){
                t.push_back(line);
            }
        }
    } catch (const char* msg) {
        cout << msg << endl;
    }
    hqueryS = t.size() - 1;
//write the data from the file
    for(int i=1; i < t.size();i++){
        string temp = t[i];
        string buf;
        stringstream ss(temp);
        int num = 0;
        while(ss >> buf){
            if (num>0){
                stringstream convert(buf);
                convert>> queryS[i][num];
            }num++;

            if(num-1>wqueryS){
                wqueryS=num-1;
            }
        }

    }


}

void makeAA(){
    for (int i = 0; i < wqueryA; i++) {
        for (int j = 0; j < wqueryA; j++) {
            if(i<=j){
                doaff(i,j);
            }else{
                aa[i][j]=aa[j][i];
            }


        }

    }
     //cout << aa[11][11] << endl;
}

void doaff(int i, int j){

    float a = 0;
    float b0 = 0;
    float b1 = 0;
    for(int k=0; k < hqueryA;k++){
        float Aik;
        float Ajk;
        float QkSj = 0;
        for (int s = 0; s < wqueryS; s++) {
            QkSj+=(float)queryS[k][s];
        }
        Aik = ((float)queryA[k][i])*QkSj;
        Ajk = ((float)queryA[k][j])*QkSj;

        a+=Aik*Ajk;
        b0 += Aik;
        b1 += Ajk;
    }
    float bfinal = pow(b0*b1, 0.5);
    if(bfinal==0){
        aa[i][j] = 0;

    }else{
        float result = a/bfinal;
        int resultInt = (int)result;
        if(result>resultInt){
            resultInt++;
        }
        aa[i][j]=resultInt;
    }
   // cout<<aa[2][1]<<endl;
}
void printaa(){
    for (int i = 0; i < wqueryA; i++) {
        for (int j = 0; j < wqueryA; j++) {
            if(j ==wqueryA-1 ){
                cout<<aa[i][j]<<endl;
            }else{
                cout<<aa[i][j]<<" ";
            }
        }
    }
}
void printqa(){
    for (int i = 0; i < wqueryA; i++) {
        for (int j = 0; j < wqueryA; j++) {
            if(j ==wqueryA-1 )
                cout<<queryA[i][j]<<endl;
            else
                cout<<queryA[i][j]<<" ";
            }
        }
    }
};


int main()
{

    toAA myobj;
    myobj.readattAndQuery();
    myobj.readacc();
    //myobj.doaff(0,18);
    myobj.makeAA();
    myobj.printaa();
    //myobj.printqa();

    return 0;


}
