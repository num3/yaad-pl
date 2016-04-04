/* Sun Mar 27 09:54:53 JST 2016 
 kk@ise.chuo-u.ac.jp
*/
#include <iostream>
#include <vector>
#include "ftd.h"

using namespace std;

typedef vector<double> vec;
typedef vector<vec> mat;

ostream&operator<<(ostream&s,vector<double> x){
    if(x.size()>0){s<<x[0]; for(int i=1;i<x.size();i++) s<<" "<<x[i];}
    return s; }
ostream&operator<<(ostream&s,vector<vector<double>> x){
    if(x.size()>0){s<<x[0]; for(int i=1;i<x.size();i++) s<<endl<<x[i];}
    return s; }

typedef FTD<double> advar;

advar f(advar x, advar y){
    advar v1=fabs(x+y);
    advar v2=fabs(x-y);
    advar v3=(v1+v2-fabs(v1-v2))*0.5;
    return -v3;
}

void ex1(){
    advar x=0;
    advar y=0;
    advar v=0;
    x.set_indep();
    y.set_indep();
    Node<double>::push_input_nodes(x.get_p());
    Node<double>::push_input_nodes(y.get_p());
    v=f(x,y);
    v.abs_normal_form();
    cout<<v.J.size()<<" "<<v.V.size()<<endl;
    cout<<"J"<<endl<<v.J<<std::endl;
    cout<<"V"<<endl<<v.V<<std::endl;
    cout<<"U"<<endl<<v.U<<std::endl;
    cout<<"L"<<endl<<v.L<<std::endl;
}

int main(int ac,char**av){
    ex1();
    return 0;
}
