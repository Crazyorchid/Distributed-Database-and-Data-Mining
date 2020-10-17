/*
*   Distributed Databases and Data Mining
*   2020, Semester 02
*   Assignment 2
*
*   student (id): a1778478
*   student (name): Runtao Zhuge
*
*/
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <iomanip>
#include <cmath>
using namespace std;
//Create the cluseter paket for later implementation.
class packet{
public:
    string flowKey;
    int arrivalTime;
    int packetLength;
};

//Create the class cluster for later implementation
class cluster{
public:
    int modiodIdx;
    vector<int> nesrestPoints;
    void addPoint(int pointIdx)
    {
        nesrestPoints.push_back(pointIdx);
    }
    void clearPoints()
    {
        nesrestPoints.clear();
    }

};

//Calculate the distance with manhatan distance return the sum of the two results
double Distance(vector<vector<double>> flowResult, int point1, int point2)
{
    double r1 = abs(flowResult[point1][0]-flowResult[point2][0]);
    double r2 = abs(flowResult[point1][1]-flowResult[point2][1]);
    return r1 + r2;
    //return abs(flowResult[point1][0]-flowResult[point2][0])+abs(flowResult[point1][1]-flowResult[point2][1]);
}

/*float distance (int b, int a){
    float r1 = abs(flow[b][0]-flow[a][0]);
    float r2 = abs(flow[b][1]-flow[a][1]);
    return r1+r2;*/

//Find the nearest points
int DetermineNearest(int pointIdx, vector<vector<double>> map, vector<int> modoidIdx)
{
    int minIdx;
    double min_dis=9999999.0;
    double dis;
    for(int k=0;k<modoidIdx.size();k++)
    {
        dis=map[pointIdx][modoidIdx[k]];
        if (dis<min_dis)
        {
            min_dis=dis;
            minIdx=modoidIdx[k];
        }
    }
    return minIdx;
}
//determine wether it is a medoid or not
bool isMedoid(vector<int> medoid_idx, int key)
{
    for (int i=0; i<medoid_idx.size(); i++) {
        if(key==medoid_idx[i])
        {
            return true;
        }
    }
    return false;
}
//build the mao by adding the results
vector<vector<double>>  buildMap(vector<vector<double>> flowResult)
{
    vector<vector<double>> map;
    for (int x=0; x<flowResult.size(); x++) {
        vector<double> entity;
        for (int y=0; y<flowResult.size(); y++) {
            entity.push_back(Distance(flowResult, x, y));
        }
        map.push_back(entity);
    }
    return map;
}

void assignPoints(vector<vector<double>>& flowResult, vector<int>& modoidIdx, vector<cluster>& clusters)
{
    int l = 0;
    while (l<clusters.size()) {
        clusters[l].clearPoints();
        clusters[l].modiodIdx=modoidIdx[l];
        l++;
    }

    for (int i=0; i<flowResult.size(); i++) {
        int minIdx;
        minIdx= DetermineNearest(i, flowResult, modoidIdx);
        if (minIdx!=-1)
        {
            for(int h=0;h<clusters.size();h++)
            {
                if(clusters[h].modiodIdx==minIdx)
                {
                    clusters[h].addPoint(i);
                    break;
                }
            }
        }
    }
}
//adding the flow with push_bakc to the result
vector<int> findFlow(string key,vector<packet> packets)
{
    vector<int> resultIdx;
    for (int i=0; i<packets.size(); i++) {
        if(packets[i].flowKey == key)
        {
            resultIdx.push_back(i);
        }
    }
    return resultIdx;
}
//remove the flow fron the vector
void removeFlow(vector<int> resultIdx, vector<packet>& packets)
{
    vector <packet>::iterator Iter;
    //remove from end to first
    for (int i=resultIdx.size()-1; i>=0; i--) {
        Iter = packets.begin()+resultIdx[i];
        packets.erase(Iter);
        Iter = packets.begin();
    }
}

//Calculate the total cost(Manhaton Distance)
double calculateTC(int previousModoidIdx, int potientalModoidIdx, vector<cluster>& clusters,  vector<vector<double>>& map, vector<int>& newModiodIdx,vector<int>& oldModiodIdx)
{

    double TCih=0;
    for (int j=0; j<map.size(); j++) {
        bool belongPrevious =false;
        bool belongPotential=false;
        int newClusterIdx;
        int previousCluster;
        double cost=0;
        previousCluster= DetermineNearest(j, map, oldModiodIdx);
        if(previousCluster==previousModoidIdx)
        {
            belongPrevious = true;
        }
        newClusterIdx= DetermineNearest(j, map, newModiodIdx);
        if(newClusterIdx==potientalModoidIdx)
        {
            belongPotential=true;
        }


        if (belongPrevious==true&&belongPotential==true)
        {
            cost=map[j][potientalModoidIdx]-map[j][previousModoidIdx];
        }
        else if (belongPrevious==true&&belongPotential==false)
        {
            cost=map[j] [newClusterIdx]-map[j][previousModoidIdx];
        }
        else if (belongPrevious==false&&belongPotential==true)
        {
            cost=map[j][potientalModoidIdx]-map[j][previousCluster];
        }
        else
        {
            cost=0;
        }
        TCih+=cost;
    }
    return TCih;
}

//calculate the error
double calculateError(vector<cluster>& clusters, vector<vector<double>>& map)
{
    double error=0;
    for (int i=0;i<clusters.size();i++)
    {
        for (int j=0; j<clusters[i].nesrestPoints.size(); j++) {
            error=error+map[clusters[i].modiodIdx][ clusters[i].nesrestPoints[j]];
        }
    }
    return error;
}

int main(int argc, const char * argv[]) {

    vector<packet> packets;
    vector<vector<double>> flow_result;
    vector<int> flow_idx;
    int medoid_num;
    vector<int> medoid_idx;
    ifstream infile;
    string data;
    //Read the first file and do the calculation
    infile.open(argv[1],ios::in);
    getline(infile,data);
    while (getline(infile, data)) {
        string tmp;
        vector<string> vec;
        stringstream ss(data);
        while (ss>>tmp) {
            vec.push_back(tmp);
        }
        packet pac;
        pac.flowKey= vec[0] + vec[1] + vec[2] + vec[3] + vec[4];
        pac.arrivalTime=std::stoi(vec[5]);
        pac.packetLength=std::stoi(vec[6]);
        packets.push_back(pac);
    }
    infile.close();

    while (packets.size()!=0) {

        flow_idx=findFlow(packets[0].flowKey, packets);
        if (flow_idx.size()<=1) {
            removeFlow(flow_idx, packets);
        }
        else
        {
            double time=0;
            double length=0;
            vector<double> result;
            for (int j=flow_idx.size()-1; j>0; j--) {
                length=length+packets[flow_idx[j]].packetLength;
                time= time + packets[flow_idx[j]].arrivalTime - packets[flow_idx[j - 1]].arrivalTime;
            }
            length=length+packets[0].packetLength;
            time=time/(flow_idx.size()-1);
            length=length/flow_idx.size();
            result.push_back(time);
            result.push_back(length);
            flow_result.push_back(result);
            removeFlow(flow_idx, packets);

        }
    }
    //output the result to "Flow.txt"
    ofstream fout;
    fout.open ("Flow.txt", ios::out | ios::trunc);
    fout.flags(ios::fixed);


    for(int k=0;k<flow_result.size();k++)
    {
        fout<<k<<" ";
        fout<<setprecision(2)<<flow_result[k][0];
        fout<<" ";
        fout<<setprecision(2)<<flow_result[k][1]<<endl;
    }

    fout.close();

    ifstream infile1;
//open the second file
    infile1.open(argv[2],ios::in);
    getline(infile1,data);
    medoid_num=stoi(data);
    while (getline(infile1, data)) {
        int num;
        stringstream ss(data);
        while (ss>>num) {
            medoid_idx.push_back(num);
        }
    }
    infile1.close();

    vector<cluster> clusters;
    for(int i=0; i<medoid_num; i++)
    {
        cluster clust;
        clusters.push_back(clust);
    }

    vector<vector<double>> map=buildMap(flow_result);

    bool change=true;
    double error;

    while(change)
    {
        assignPoints(map, medoid_idx, clusters);
        bool isChange=false;
        for (int m=0; m<flow_result.size(); m++) {
            if (isMedoid(medoid_idx, m)==false) {
                bool isExchange=false;
                for (int g=0; g<medoid_num; g++) {

                    vector<int> newModiodIdx(medoid_idx);
                    newModiodIdx[g]=m;
                    double TC;
                    TC=calculateTC(medoid_idx[g], newModiodIdx[g], clusters, map,newModiodIdx,medoid_idx);
                    if (TC<0)
                    {
                        isExchange=true;
                        medoid_idx=newModiodIdx;
                        isChange=true;

                        break;
                    }
                }
                if(isExchange)
                {
                    break;
                }
            }

        }
        if(isChange==true)
        {
            change=true;
        }
        else
        {
            change=false;
        }
    }
    error=calculateError(clusters, map);
    ofstream ccc;
    ccc.open ("KMedoidsClusters.txt", ios::out | ios::trunc);
    ccc.flags(ios::fixed);
    ccc << setprecision(2) << error << endl;
    for(int i=0;i<medoid_idx.size();i++)
    {
        ccc << setprecision(2) << medoid_idx[i] << " ";
    }
    ccc << endl;
    for (int i=0; i<clusters.size(); i++) {
        for (int j=0; j<clusters[i].nesrestPoints.size(); j++) {
            ccc << setprecision(2) << clusters[i].nesrestPoints[j] << " ";
        }
        ccc << endl;
    }

    ccc.close();

    return 0;
}
