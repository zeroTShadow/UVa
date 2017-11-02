#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
#include <vector>

using namespace std;

class Kingdom{
    public: 
        bool hit=false;
        int xmin=INT_MAX,xmax=INT_MIN,ymin=INT_MAX,ymax=INT_MIN;
        vector<pair<int,int>> sites;
    int crossProduct(pair<int,int> O, pair<int,int> A, pair<int,int> B){
        return (A.first-O.first)*(B.second-O.second)-
               (A.second-O.second)*(B.first-O.first);
        //return 0 if OA and OB are co-linear
        //return negative num for clockwise 
        //return postive num for ccw
    }
    void convexHull(){
        sort(sites.begin(),sites.end());
        //andrew algorithm, monotone chain, using resources from wikibooks.org
        int k=0;
        vector<pair<int,int>> tmp(sites.size()+1);
        for(int i=0;i<sites.size();i++){
            while(k>=2 && crossProduct(tmp[k-2],tmp[k-1],sites[i])>0) k--;
            tmp[k++]=sites[i];
        }//building clockwise, top half
        for(int i=sites.size()-2,t=k+1;i>=0;i--){
            while(k>=t && crossProduct(tmp[k-2],tmp[k-1],sites[i])>0) k--;
            tmp[k++]=sites[i];
        }//building clockwise, bottom half
        tmp.resize(k);//first and last should be the same point
        sites=tmp;
    }
    double hitted(int x,int y){
        if(hit)
            return 0;
        if(x<xmin || x>xmax || y<ymin || y>ymax)
            return 0;//simple box check
        int numTimesHit=0;
        for(int i=0;i<sites.size()-1;i++){
            if(sites[i].second==sites[i+1].second){
                if(sites[i].second!=y)
                    continue;//two parallel lines
                else if((x>=sites[i].first && x<=sites[i+1].first)||
                        (x<=sites[i].first && x>=sites[i+1].first))
                    return getMyArea();//missile hits on the border of a horizontal line
                else
                    continue;
            }
            double tx=(double)(y-sites[i].second)*(sites[i+1].first-sites[i].first)/
                      (sites[i+1].second-sites[i].second)+sites[i].first;
            if((tx>=sites[i].first && tx<=sites[i+1].first)||
               (tx<=sites[i].first && tx>=sites[i+1].first)){
                if(tx>x)
                    numTimesHit++;
                else if(tx==(double)x)
                    return getMyArea();
            }
        }return numTimesHit&1?getMyArea():0;
    }
    double getMyArea(){
        hit=true;
        double re=0;
        for(int i=1;i<sites.size();i++)
            re+=(sites[i-1].first*sites[i].second)-(sites[i].first*sites[i-1].second);
        re/=2;
        return -re;
    }
};

int main()
{
    Kingdom kingdoms[20];
    string input;
    int entries, numKingdoms=0;
    while(cin>>entries){
        //get entries about kingdoms
        if(entries==-1)
            break;
        else{
            numKingdoms++;
            kingdoms[numKingdoms-1].sites=vector<pair<int,int>>(entries);
        }
        for(int i=0;i<entries;i++){
            int x,y;
            cin>>x>>y;
            kingdoms[numKingdoms-1].sites[i].first=x;
            kingdoms[numKingdoms-1].sites[i].second=y;
            kingdoms[numKingdoms-1].xmin=min(kingdoms[numKingdoms-1].xmin,x);
            kingdoms[numKingdoms-1].xmax=max(kingdoms[numKingdoms-1].xmax,x);
            kingdoms[numKingdoms-1].ymin=min(kingdoms[numKingdoms-1].ymin,y);
            kingdoms[numKingdoms-1].ymax=max(kingdoms[numKingdoms-1].ymax,y);
            
        }
    }
    
    //generate data of each kingdom
    for(int i=0;i<numKingdoms;i++){
        kingdoms[i].convexHull();
    }
    
    double affectedArea=0;
    int x,y;
    while(cin>>x>>y){
        for(int i=0;i<numKingdoms;i++){
            affectedArea+=kingdoms[i].hitted(x,y);
        }
    }
    printf("%.2f\n",affectedArea);
    return 0;
}
