#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

int sensor[4]={10,24,15,24};

void printObject(vector<vector<vector<int> > >&objects){
    //printf("%d %d",objects[3][19][0],objects[3][19][1]);
}

void update(vector<vector<vector<int> > >&objects,int i,int j,int removeUnknown){
    objects[0][i][removeUnknown]+=(removeUnknown?-1:1);
    objects[2][j][removeUnknown]+=(removeUnknown?-1:1);
    objects[1][i+j][removeUnknown]+=(removeUnknown?-1:1);
    objects[3][j+(9-i)][removeUnknown]+=(removeUnknown?-1:1);
}

void replace(vector<vector<char> >&slice, vector<vector<vector<int> > >& objects, 
            int sensorType, int sensorNum, char to){
    if(sensorType==0){
        for(int i=0;i<slice[sensorNum].size();i++){
            if(slice[sensorNum][i]=='?'){
                slice[sensorNum][i]=to;
                update(objects,sensorNum,i,1);
                if(to=='#')
                    update(objects,sensorNum,i,0);
            }
        }
    }else if(sensorType==2){
        for(int i=0;i<slice.size();i++){
            if(slice[i][sensorNum]=='?'){
                slice[i][sensorNum]=to;
                update(objects,i,sensorNum,1);
                if(to=='#')
                    update(objects,i,sensorNum,0);
            }
        }
    }else if(sensorType==1){
        int xx=min(sensorNum,(int)slice.size()-1);
        int yy=max(0,sensorNum-xx);
        for(;xx>=0 && yy<slice[0].size();xx--,yy++){
            if(slice[xx][yy]=='?'){
                slice[xx][yy]=to;
                update(objects,xx,yy,1);
                if(to=='#')
                    update(objects,xx,yy,0);
            }
        }
    }else{
        int xx=min((int)slice.size()-1,(int)(slice.size()+slice[0].size()-2-sensorNum));
        int yy=min(sensorNum,(int)slice[0].size()-1);
        //printf("xx:%d yy:%d\n",xx,yy);
        for(;xx>=0 && yy>=0;xx--,yy--){
            if(slice[xx][yy]=='?'){
                slice[xx][yy]=to;
                update(objects,xx,yy,1);
                if(to=='#')
                    update(objects,xx,yy,0);
            }
        }
    }
}

void image(vector<vector<char> >&slice, int unsolved){
    //printf("%d:\n",unsolved);
    for(int i=0;i<slice.size();i++){
        for(int j=0;j<slice[i].size();j++){
            printf(unsolved==0?"%c":".",slice[i][j]);
        }printf("\n");
    }
    //printf("--------------------------------\n");
}

void analyze(const int s[4][24]){
    vector<vector<bool> > verified;
    vector<vector<vector<int> > > objects; //{objects,unknown} for last vector
    vector<vector<char> > slice(10,vector<char>(15,'?'));
    int unsolved=10+24+15+24,prev;
    
    for(int i=0;i<sizeof(sensor)/sizeof(sensor[0]);i++)
        verified.push_back(vector<bool>(sensor[i],false));
    objects.push_back(vector<vector<int> >(sensor[0],vector<int>{0,15}));
    objects.push_back(vector<vector<int> >(sensor[1],vector<int>{0,0}));
    objects.push_back(vector<vector<int> >(sensor[2],vector<int>{0,10}));
    objects.push_back(vector<vector<int> >(sensor[3],vector<int>{0,0}));
    for(int i=0;i<10;i++){
        objects[1][i][1]=i+1;
        objects[3][i][1]=i+1;
    }
    for(int i=10;i<14;i++){
        objects[1][i][1]=10;
        objects[3][i][1]=10;
    }
    for(int i=14;i<24;i++){
        objects[1][i][1]=24-i;
        objects[3][i][1]=24-i;
    }
    
    do{
        prev=unsolved;
        for(int i=0;i<verified.size();i++){//sensor type
            for(int j=0;j<sensor[i];j++){//sensor number
                if(!verified[i][j]){
                    if(s[i][j]==objects[i][j][0]){
                        //if(unsolved==63){printf("%d %d %d\n",objects[2][13][0],objects[2][13][1],s[2][13]);}
                        replace(slice,objects,i,j,'.');
                        //image(slice,unsolved);
                        verified[i][j]=true;
                        unsolved--;
                    }else if(s[i][j]==objects[i][j][0]+objects[i][j][1]){
                        //if(unsolved==63){printf("%d %d %d\n",objects[2][13][0],objects[2][13][1],s[2][13]);}
                        replace(slice,objects,i,j,'#');
                        //image(slice,unsolved);
                        verified[i][j]=true;
                        unsolved--;
                    }
                }
            }
        }
    }while(unsolved!=0 && prev!=unsolved);
    
    image(slice,unsolved);
}

int main()
{
    int numImages;
    cin>>numImages;
    int s[4][24];
    for(int i=0;i<numImages;i++){
        for(int j=0;j<sensor[0];j++)
            cin>>s[0][j];
        for(int j=0;j<sensor[1];j++)
            cin>>s[1][j];
        for(int j=0;j<sensor[2];j++)
            cin>>s[2][j];
        for(int j=0;j<sensor[3];j++)
            cin>>s[3][j];
        analyze(s);
        if(i!=numImages-1)
            printf("\n");
    }
}