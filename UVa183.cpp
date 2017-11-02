#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

void inline printer(int& numChar){
    numChar++;
    if(numChar==50){
        numChar=0;
        printf("\n");
    }
}

void inline addToStack(vector<vector<int>>& toTranslate, int xx, int yy, int num_row, int num_col){
    if(num_row>1 && num_col>1)
        toTranslate.push_back(vector<int>{xx+(1+num_row)/2,yy+(1+num_col)/2,
                                          num_row/2,num_col/2});
    if(num_row>1)
        toTranslate.push_back(vector<int>{xx+(1+num_row)/2,yy,
                                          num_row/2,(1+num_col)/2});
    if(num_col>1)//need to do something
        toTranslate.push_back(vector<int>{xx,yy+(1+num_col)/2,
                                          (1+num_row)/2,num_col/2});
    toTranslate.push_back(vector<int>{xx,yy,(1+num_row)/2,(1+num_col)/2});
}

void translate(bool bitMap[200][200],int row,int col){
    vector<vector<int>> toTranslate{{0,0,row,col}};
    int numChar=0; //used to keep track of characters printed at this row
    while(!toTranslate.empty()){
        vector<int> cur=toTranslate.back(); 
        toTranslate.pop_back();
        int xx=cur[0],yy=cur[1],num_row=cur[2],num_col=cur[3];
        bool zeroOrOne=bitMap[xx][yy], success=true;
        
        for(int i=xx;i<xx+num_row;i++){
            for(int j=yy;j<yy+num_col;j++){
                if(bitMap[i][j]!=zeroOrOne){
                    success=false;
                    i=200; //break the double for loop
                    break;
                }
            }
        }
        
        if(success){
            printf(zeroOrOne?"1":"0");
            printer(numChar);
        }else{
            printf("D");
            printer(numChar);
            addToStack(toTranslate,xx,yy,num_row,num_col);
        }
    }
    if(numChar!=0)
        printf("\n");
}

void DtoB(bool bitMap[200][200],int row,int col){
    vector<vector<int>> toTranslate{{0,0,row,col}};
    char curChar;
    while(toTranslate.size()!=0){
        cin>>curChar;
        if(curChar!='D'&&curChar!='0'&&curChar!='1')
            cin>>curChar;
        vector<int> cur=toTranslate.back();
        toTranslate.pop_back();
        if(curChar=='D'){
            addToStack(toTranslate,cur[0],cur[1],cur[2],cur[3]);
        }else{
            for(int i=cur[0];i<cur[0]+cur[2];i++){
                for(int j=cur[1];j<cur[1]+cur[3];j++){
                    bitMap[i][j]=(curChar=='1'?true:false);
                }
            }
        }
    }
    
    int numChar=0;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            printf(bitMap[i][j]?"1":"0");
            printer(numChar);
        }
    }
    if(numChar!=0)
        printf("\n");
}

int main()
{
    bool bitMap[200][200];
    char type;
    int row,col;
    while(cin>>type>>row>>col){
        if(type=='#') break;
        else if(type=='B'){
            for(int i=0;i<row;i++){
                for(int j=0;j<col;j++){
                    cin>>type;
                    while(type!='1'&&type!='0') 
                        cin>>type;
                    if(type=='1') bitMap[i][j]=true;
                    else          bitMap[i][j]=false;
                }
            }
            printf("D%4d%4d\n",row,col);
            translate(bitMap,row,col);
        }else{
            printf("B%4d%4d\n",row,col);
            DtoB(bitMap,row,col);
        }
    }
    return 0;
}