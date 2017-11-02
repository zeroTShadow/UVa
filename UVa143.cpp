#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cstdio>
#define ROUNDING 1e-9
using namespace std;

//UVa143 runtime O(N), where N equals to the length of the field

void operation(vector<double>& a,double value){
    if(a.size()==0)
        a.push_back(value);
    else if(a.size()==1 && abs(a[0]-value)>ROUNDING)
        a.push_back(value);
}

int main() {
	vector<vector<double>> pts(3,vector<double>(2,0));
	while(true){
	    bool allZero=true;
	    for(int i=0;i<pts.size();i++){
	        for(int j=0;j<pts[i].size();j++){
	            cin>>pts[i][j];
	            if(pts[i][j]>ROUNDING)
	                allZero=false;
	        }
	    }if(allZero)
	        break;
	       
	   int re=0;
	   vector<vector<double>> xAxis(101);
	   for(int i=0;i<pts.size();i++){
	       for(int j=i+1;j<pts.size();j++){
	           if(abs(pts[i][0]-pts[j][0])<ROUNDING && 
	              abs(round(pts[i][0])-pts[i][0])<ROUNDING){
	               int x=(int)round(pts[i][0]);
	               operation(xAxis[x],pts[i][1]);
	               operation(xAxis[x],pts[j][1]);
	           }else{
	               double m=(pts[i][1]-pts[j][1])/(pts[i][0]-pts[j][0]);
	               double yinter=pts[i][1]-m*pts[i][0];
	               for(int k=(int)ceil(min(pts[i][0],pts[j][0]));
	                   k<=(int)floor(max(pts[i][0],pts[j][0]));
	                   k++){
	                   operation(xAxis[k],m*k+yinter);
	               }
	           }
	       }
	   }
	   for(int i=1;i<=99;i++){
	       if(xAxis[i].size()==0) continue;
	       else if(xAxis[i].size()==1){
	           int yvalue=(int)round(xAxis[i][0]);
    	       if(abs(yvalue-xAxis[i][0])<ROUNDING &&
    	          yvalue>=1 && yvalue<=99){
    	           re++;
    	       }
	       }
	       else{
	           if(xAxis[i][0]>xAxis[i][1])
	               swap(xAxis[i][0],xAxis[i][1]);
	           int lower,upper;
	           if(abs(round(xAxis[i][0])-xAxis[i][0])<ROUNDING)
	               lower=(int)round(xAxis[i][0]);
	           else
	               lower=(int)ceil(xAxis[i][0]);
	           lower=max(1,lower);
	               
	           if(abs(round(xAxis[i][1])-xAxis[i][1])<ROUNDING)
	               upper=(int)round(xAxis[i][1]);
	           else
	               upper=(int)floor(xAxis[i][1]);
	           upper=min(99,upper);
	           
	           re+=upper-lower+1;
	       }
	   }printf("%4d\n",re);
	}
	return 0;
}
