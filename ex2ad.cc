/*
  Mon Apr  4 19:13:14 JST 2016
  kk@ise.chuo-u.ac.jp
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "ftd.h"

std::ostream&operator<<(std::ostream&s, std::vector<double> x){
    if(x.size()>0){ s<<x[0]; for(int i=1;i<x.size();i++) s<<" "<<x[i]; }
    return s;
}
std::ostream&operator<<(std::ostream&s, std::vector<std::vector<double>> x){
    if(x.size()>0){ 
        s<<x[0]; for(int i=1;i<x.size();i++) s<<std::endl<<x[i]; }
    return s;
}

typedef Node<double> Nodetype;
typedef FTD<double> Ftd;

template<typename T>
FTD<T> min(const FTD<T>&x, const FTD<T>&y){
    return (x+y-fabs(x-y))*FTD<T>(0.5);
}
template<typename T>
FTD<T> max(const FTD<T>&x, const FTD<T>&y){
    std::cout<<"max: x="<<x.get_val()<<"  y="<<y.get_val()<<std::endl;
    return (x+y+fabs(x-y))*FTD<T>(0.5);
}

void ex1(){
    Ftd x,y,u1,u2,v1,v2,f;
    x=0.0;
    y=0.0;
    x.set_indep();
    y.set_indep();
    Nodetype::push_input_nodes(x.get_p());
    Nodetype::push_input_nodes(y.get_p());

    std::cout<<"x.p="<<x.get_p()<<std::endl;
    std::cout<<"y.p="<<y.get_p()<<std::endl;
    
    typedef FTD<double> ADvariable;
    ADvariable epsilon=0.1;
    ADvariable zero=0.0;
    ADvariable bigval=-1e10;
    ADvariable z1,z2,z3,z4,z5,z6,z7,z8;
    ADvariable f0,f1,f2,f3,f4,f5,f6,f7,f8,f9;
    ADvariable f10,f11,f12,f13,f14,f15,f16,f17,f18,f19;
    ADvariable f20,f21,f22,f23,f24,f25,f26,f27,f28,f29;
    ADvariable f30,f31,f32,f33,f34,f35,f36,f37,f38,f39;
    ADvariable f40,f41,f42,f43,f44,f45,f46,f47,f48,f49;
    ADvariable f50,f51,f52,f53,f54,f55,f56,f57,f58,f59;
    ADvariable f60,f61,f62,f63;
    ADvariable g0,g1,g2,g3,g4,g5,g6,g7,g8,g9;
    ADvariable g10,g11,g12,g13,g14,g15,g16,g17,g18,g19;
    ADvariable g20,g21,g22,g23,g24,g25,g26,g27,g28,g29;
    ADvariable g30,g31;

f0=-0.051506623982651787708*x+7.1624462910800907878*y;
f1=2.0298596439122134072*x+-6.8689853421693669588*y;
f2=-1.4478408919954870449*x+7.0147734501113250261*y;
f3=3.3309290223615288795*x+-6.340993743326885479*y;
f4=-2.788535446487464764*x+6.5975267993850588155*y;
f5=4.5039926664880045593*x+-5.5693213109986077214*y;
f6=-4.0220681475996222787*x+5.9267408936940748632*y;
f7=5.5039703983095327189*x+-4.583622984000028211*y;
f8=-5.101035025401338352*x+5.0281936592130191954*y;
f9=6.2924336343861799747*x+-3.4217785962517113241*y;
f10=-5.9839719878702712919*x+3.936415762251888939*y;
f11=6.8390821747308185863*x+-2.128437176004994047*y;
f12=-6.6369482626955003113*x+2.6933636151148112603*y;
f13=7.1229086225020132872*x+-0.75330110872571987013*y;
f14=-7.0348703378288117705*x+1.3468070147045907436*y;
f15=7.1330056864836315356*x+0.6507838977057547325*y;
f16=-7.1624462910800987814*x+-0.051506623982652120775*y;
f17=6.8689853421693598534*x+2.0298596439122116308*y;
f18=-7.0147734501113321315*x+-1.4478408919954888212*y;
f19=6.3409937433268961371*x+3.3309290223615319881*y;
f20=-6.5975267993850437165*x+-2.7885354464874554381*y;
f21=5.5693213109986077214*x+4.5039926664880045593*y;
f22=-5.9267408936940659814*x+-4.0220681475996160614*y;
f23=4.58362298400005308*x+5.5039703983095558115*y;
f24=-5.0281936592130058727*x+-5.1010350254013205884*y;
f25=3.4217785962517175413*x+6.2924336343861888565*y;
f26=-3.936415762251878725*x+-5.9839719878702526401*y;
f27=2.1284371760050069255*x+6.8390821747308487843*y;
f28=-2.6933636151148081517*x+-6.6369482626954843241*y;
f29=0.75330110872571998115*x+7.1229086225020115108*y;
f30=-1.3468070147045889673*x+-7.0348703378287975596*y;
f31=-0.65078389770575428841*x+7.1330056864836643982*y;
f32=0.051506623982649268889*x+-7.1624462910800827942*y;
f33=-2.0298596439122116308*x+6.8689853421693598534*y;
f34=1.4478408919954848244*x+-7.0147734501113170325*y;
f35=-3.3309290223615373172*x+6.3409937433269085716*y;
f36=2.7885354464874567704*x+-6.5975267993850472692*y;
f37=-4.5039926664880036711*x+5.5693213109986077214*y;
f38=4.0220681475996142851*x+-5.9267408936940650932*y;
f39=-5.5039703983095558115*x+4.58362298400005308*y;
f40=5.101035025401293943*x+-5.0281936592129827801*y;
f41=-6.2924336343862146137*x+3.4217785962517326404*y;
f42=5.9839719878702837264*x+-3.9364157622518973767*y;
f43=-6.8390821747307883882*x+2.1284371760049825006*y;
f44=6.6369482626955162985*x+-2.6933636151148161453*y;
f45=-7.1229086225020434853*x+0.75330110872572841885*y;
f46=7.0348703378287638088*x+-1.346807014704587635*y;
f47=-7.1330056864836643982*x+-0.65078389770575384432*y;
f48=7.1624462910800517079*x+0.051506623982647298243*y;
f49=-6.8689853421693900515*x+-2.0298596439122187363*y;
f50=7.0147734501113498951*x+1.4478408919954923739*y;
f51=-6.3409937433268526163*x+-3.3309290223615137805*y;
f52=6.5975267993850721382*x+2.7885354464874714253*y;
f53=-5.5693213109986325904*x+-4.5039926664880178819*y;
f54=5.9267408936940446651*x+4.0220681475995956333*y;
f55=-4.5836229840000495273*x+-5.5039703983095522588*y;
f56=5.028193659212987221*x+5.1010350254012983839*y;
f57=-3.4217785962517330844*x+-6.2924336343862137255*y;
f58=3.9364157622518960444*x+5.9839719878702801736*y;
f59=-2.1284371760049829447*x+-6.8390821747307883882*y;
f60=2.6933636151148165894*x+6.6369482626955162985*y;
f61=-0.75330110872572930703*x+-7.1229086225020452616*y;
f62=1.3468070147045880791*x+7.0348703378287629207*y;
f63=0.65078389770575484352*x+-7.1330056864836342001*y;
g0=min(f0,f1);
g1=min(f2,f3);
g2=min(f4,f5);
g3=min(f6,f7);
g4=min(f8,f9);
g5=min(f10,f11);
g6=min(f12,f13);
g7=min(f14,f15);
g8=min(f16,f17);
g9=min(f18,f19);
g10=min(f20,f21);
g11=min(f22,f23);
g12=min(f24,f25);
g13=min(f26,f27);
g14=min(f28,f29);
g15=min(f30,f31);
g16=min(f32,f33);
g17=min(f34,f35);
g18=min(f36,f37);
g19=min(f38,f39);
g20=min(f40,f41);
g21=min(f42,f43);
g22=min(f44,f45);
g23=min(f46,f47);
g24=min(f48,f49);
g25=min(f50,f51);
g26=min(f52,f53);
g27=min(f54,f55);
g28=min(f56,f57);
g29=min(f58,f59);
g30=min(f60,f61);
g31=min(f62,f63);
f=max(max(max(max(max(g0,g1),max(g2,g3)),max(max(g4,g5),max(g6,g7))),max(max(max(g8,g9),max(g10,g11)),max(max(g12,g13),max(g14,g15)))),max(max(max(max(g16,g17),max(g18,g19)),max(max(g20,g21),max(g22,g23))),max(max(max(g24,g25),max(g26,g27)),max(max(g28,g29),max(g30,g31)))));

    std::cout<<"dfdx="<<x.get_d_val()<<std::endl;
    std::cout<<"dfdy="<<y.get_d_val()<<std::endl;
    std::cout<<"z1="<<z1.get_val()<<std::endl;
    std::cout<<"z2="<<z2.get_val()<<std::endl;
    std::cout<<"z3="<<z3.get_val()<<std::endl;
    std::cout<<"z4="<<z4.get_val()<<std::endl;
    std::cout<<"z5="<<z5.get_val()<<std::endl;
    std::cout<<"z6="<<z6.get_val()<<std::endl;
    std::cout<<"z7="<<z7.get_val()<<std::endl;
    std::cout<<"z8="<<z8.get_val()<<std::endl;

    Nodetype::reset_input_d_val();
    f.diff();

    std::cout<<"f="<<f.get_val()<<std::endl;
    std::cout<<"dfdx="<<x.get_d_val()<<std::endl;
    std::cout<<"dfdy="<<y.get_d_val()<<std::endl;

    std::cout<<"-----"<<std::endl;

    std::cout<<"# of fabs_nodes="<< (Nodetype::fabs_nodes.size())<<std::endl;
    std::cout<<"# of fabs_arg_nodes="<< (Nodetype::fabs_arg_nodes.size())<<std::endl;
    std::cout<<"# of input_nodes="<< (Nodetype::input_nodes.size())<<std::endl;
    int ns=Nodetype::fabs_nodes.size();
    int ni=Nodetype::input_nodes.size();
    std::reverse(&Nodetype::fabs_nodes[0],&Nodetype::fabs_nodes[ns]);
    std::reverse(&Nodetype::fabs_arg_nodes[0],&Nodetype::fabs_arg_nodes[ns]);

    for(Nodetype* p : Nodetype::input_nodes) {
        std::cout<<"  "<<p<<std::endl;
    }
    /*change OP_FABS nodes to OP_INP */
    for(Nodetype* p : Nodetype::fabs_nodes) {
        p->set_op(OP_INP);
    }
    Nodetype::reset_input_d_val();
    f.diff();
    std::cout<<"f="<<f.get_val()<<std::endl;
    std::cout<<"dfdx="<<x.get_d_val()<<std::endl;
    std::cout<<"dfdy="<<y.get_d_val()<<std::endl;

    std::cout<<"J : V "<<std::endl;

    std::vector<double> J;
    std::vector<double> V;
    std::vector<std::vector<double>> U;
    std::vector<std::vector<double>> L;

    /* Matrix J */
    for(int j=0;j<ni;j++) std::cout<<Nodetype::input_nodes[j]->get_d_val()<<" ";
    std::cout<<": ";
    for(int j=0;j<ni;j++) J.push_back(Nodetype::input_nodes[j]->get_d_val());

    /* Matrix V */
    for(int i=0;i<ns;i++) std::cout<<Nodetype::fabs_nodes[i]->get_d_val()<<" ";
    std::cout<<std::endl;
    for(int i=0;i<ns;i++) V.push_back(Nodetype::fabs_nodes[i]->get_d_val());

    std::cout<<"   ---   "<<std::endl;
    std::cout<<"U : L "<<std::endl;

    for(int i=0;i<ns;i++) {

        /* reset dydx=0 */
        Nodetype::reset_input_d_val();
        Nodetype::reset_fabs_d_val();
        
        /* dfs from fabs_arg_nodes[i] */
        Nodetype* ui = Nodetype::fabs_arg_nodes[i] ;
        //std::cout<<" ui="<<ui<<std::endl;
        Nodetype::dfs( ui );
        ui->set_d_val(1.0);
        Nodetype::rdfs( ui );

        std::vector<double> uu;
        /* Matrix U */
        for(int j=0;j<ni;j++) std::cout<<Nodetype::input_nodes[j]->get_d_val()<<" ";
        std::cout<<": ";
        for(int j=0;j<ni;j++) uu.push_back(Nodetype::input_nodes[j]->get_d_val());
        U.push_back(uu);


        std::vector<double> ll;
        /* Matrix L */
        for(int j=0;j<ns;j++) std::cout<<Nodetype::fabs_nodes[j]->get_d_val()<<" ";
        std::cout<<std::endl;
        for(int j=0;j<ns;j++) ll.push_back(Nodetype::fabs_nodes[j]->get_d_val());
        L.push_back(ll);
    }

    std::cout<<"----------------------------"<<std::endl;
    for(int i=0;i<L.size();i++) for(int j=i;j<L.size();j++) std::cout<<L[i][j]<<" ";
    std::cout<<std::endl;
    std::cout<<"----------------------------"<<std::endl;
    std::ofstream fout("ex2-output.m");
    std::ofstream gout("ex2-output.dat");
    gout<<ni<<" "<<ns<<std::endl;
    fout<<"J=["<<J<<"]"<<std::endl;
    gout<<"J"<<std::endl<<J<<std::endl;
    fout<<"V=["<<V<<"]"<<std::endl;
    gout<<"V"<<std::endl<<V<<std::endl;
    fout<<"U=["<<U<<"]"<<std::endl;
    gout<<"U"<<std::endl<<U<<std::endl;
    fout<<"L=["<<L<<"]"<<std::endl;
    gout<<"L"<<std::endl<<L<<std::endl;
}

int main(){
    ex1();
}
