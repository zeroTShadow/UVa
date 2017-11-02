#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

struct Pos{
    int myX, myY, v_x, v_y;
};

typedef vector<vector<int> > curV;

void generateChoices(const Pos& myS, const pair<int,int> end, vector<Pos>& choices){
    // used to generate the list of possible next steps and arrange them to generate 
    // optimal pathing
    int dir_x=end.first-myS.myX, dir_y=end.second-myS.myY;
    vector<vector<int> > possibleSpeed;
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(myS.v_x+i>=-3 && myS.v_x+i<=3 &&
               myS.v_y+j>=-3 && myS.v_y+j<=3){
                possibleSpeed.push_back(vector<int>{myS.v_x+i,myS.v_y+j,dir_x,dir_y});
            }
        }
    }sort(possibleSpeed.begin(),possibleSpeed.end(),[](vector<int>& a, vector<int>& b){
        if(a[0]!=b[0]){
            if(a[2]==0)
                return abs(a[0])>abs(b[0]);
            else if(a[2]<0)
                return a[0]>b[0];
            else 
                return a[0]<b[0];
        }else{
            if(a[3]==0)
                return abs(a[1])>abs(b[1]);
            else if(a[3]<0)
                return a[1]>b[1];
            else 
                return a[1]<b[1];
        }
    });
    for(int i=0;i<possibleSpeed.size();i++){
        Pos tmp;
        tmp.myX=myS.myX+possibleSpeed[i][0];
        tmp.myY=myS.myY+possibleSpeed[i][1];
        tmp.v_x=possibleSpeed[i][0];
        tmp.v_y=possibleSpeed[i][1];
        choices.push_back(tmp);
    }
}

void findRoute(const int x_len, const int y_len, 
               const pair<int,int> start, const pair<int,int> end,
               const vector<vector<int> >& blocks){
    vector<vector<bool> > isBlocked(x_len,vector<bool>(y_len,false));
    curV tmp(7,vector<int>(7,INT_MAX));
    vector<vector<curV> > bestRecord(x_len,vector<curV>(y_len,tmp));
    for(int i=0;i<blocks.size();i++){
        for(int xx=blocks[i][0];xx<=blocks[i][1];xx++){
            for(int yy=blocks[i][2];yy<=blocks[i][3];yy++){
                isBlocked[xx][yy]=true;
            }
        }
    }isBlocked[start.first][start.second]=false;
    //check if the end point is occupied
    if(isBlocked[end.first][end.second]){
        cout<<"No solution."<<endl;
        return;
    }
    
    int re=-1;
    Pos init;
    init.myX=start.first; init.myY=start.second; init.v_x=0; init.v_y=0;
    vector<pair<Pos,int> > toBeVisited{pair<Pos,int>(init,0)};
    while(toBeVisited.size()!=0){
        pair<Pos,int> cur=toBeVisited.back();
        Pos myS=cur.first;
        toBeVisited.pop_back();
        if(myS.myX==end.first && myS.myY==end.second){
            if(re==-1 || re>cur.second) re=cur.second;
        } //reach destination
        else if(bestRecord[myS.myX][myS.myY][myS.v_x+3][myS.v_y+3]>cur.second){
            bestRecord[myS.myX][myS.myY][myS.v_x+3][myS.v_y+3]=cur.second;
            if(re==-1 || re>1+cur.second){
                vector<Pos> choices;
                generateChoices(myS,end,choices);
                for(int i=0;i<choices.size();i++){
                    Pos next=choices[i];
                    if(next.myX>=0 && next.myX<x_len &&
                       next.myY>=0 && next.myY<y_len &&
                       !isBlocked[next.myX][next.myY]){
                        toBeVisited.push_back(pair<Pos,int>(next,cur.second+1));
                    }
                }
            } //check if it is possible to minimize the number of hops
        } //update information and add new queries to the list of visiting
    }
    
    if(re==-1)
        cout<<"No solution."<<endl;
    else
        cout<<"Optimal solution takes "<<re<<" hops."<<endl;
}

int main()
{
    int numCases, numBlocks, x_len, y_len;
    pair<int,int> start,end;
    vector<vector<int> > blocks;
    cin>>numCases;
    while(numCases--){
        cin>>x_len>>y_len;
        cin>>start.first>>start.second>>end.first>>end.second;
        cin>>numBlocks;
        while(numBlocks--){
            vector<int> t(4);
            for(int i=0;i<t.size();i++){
                cin>>t[i];
            }
            blocks.push_back(t);
        }findRoute(x_len,y_len,start,end,blocks);
        blocks=vector<vector<int> >();
    }
}