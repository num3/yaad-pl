/* Sun Mar 27 09:54:53 JST 2016 
 kk@ise.chuo-u.ac.jp

 du = U * dx + L * dv  ==>   du = (I - LS)^(-1) * U * dx
 dv = S * du
 dy = J * dx + V * dv = (J + V * S * (I - LS)^(-1) * U) * dx

 [du1]   [U1 |  0   0   0 ][ dx]
 [du2] = [U2 |L21   0   0 ][dv1]
 [du3]   [U3 |L31 L32   0 ][dv2]
 [ dy]   [ J | V1  V2  V3 ][dv3]

*/
#include <iostream>
#include <vector>
#include <glpk.h>
#include <set>

#define FOR(i,n)	for(int (i)=0;i<(n);(i)++)

typedef std::vector<double> vec;
typedef std::vector<std::vector<double>> mat;
std::ostream&operator<<(std::ostream&s,vec x){
	if(x.size()>0){s<<x[0];for(int i=1;i<x.size();i++)s<<" "<<x[i];}
	return s; }
std::ostream&operator<<(std::ostream&s,mat x){
	if(x.size()>0){s<<x[0];for(int i=1;i<x.size();i++)s<<std::endl<<x[i];}
	return s; }

using namespace std;

struct ex1 {
	int ni,ns;
	std::vector<double> J,V;
	std::vector<std::vector<double>> U,L;
	std::vector<std::vector<double>> A;
	std::vector<double> obj;
	ex1(){ }
	void readfile(){
		double v; string dummy;
		cin>>ni>>ns;
		/* J */
		cin>>dummy; for(int i=0;i<ni;i++) { cin>>v; J.push_back(v); }
		/* V */
		cin>>dummy; for(int i=0;i<ns;i++)  { cin>>v; V.push_back(v); }
		/* U */
		cin>>dummy; for(int i=0;i<ns;i++) { vector<double> ui;
			for(int j=0;j<ni;j++)  { cin>>v; ui.push_back(v); }
			U.push_back(ui); }
		/* L */
		cin>>dummy; for(int i=0;i<ns;i++) { vector<double> li;
			for(int j=0;j<ns;j++)   { cin>>v; li.push_back(v); }
			L.push_back(li); }
	}
	void dump(){
		cout<<ni<<" "<<ns<<endl;
		cout<<"J"<<endl<<J<<endl;
		cout<<"V"<<endl<<V<<endl;
		cout<<"U"<<endl<<U<<endl;
		cout<<"L"<<endl<<L<<endl;
	}
	void top(){
		readfile();
		//dump(); 
		sigma1=vector<double>(ns+1);
		LL=vector<vector<double>>(ns,vector<double>(ns));
		genmatA(ns,ni);
	}
	std::vector<double> sigma1;
	std::vector<std::vector<double>> LL;
	std::set<mat> sm;

	void print_retval(int rv){
		string str;
		switch(rv){
		case GLP_OPT: str="optimal"; break;
		case GLP_FEAS: str="feasible"; break;
		case GLP_INFEAS: str="infeasible"; break;
		case GLP_NOFEAS: str="nofeasible"; break;
		case GLP_UNBND: str="unbound"; break;
		case GLP_UNDEF: str="undef"; break;
		}
		cout<<"print_retval: "<<str<<endl;
	}
	/*
       du_1 = U_1 * dx
       dv_1 = s1 * du_1 
       du_2 = U_2 * dx + L21 * dv_1
       dv_2 = s2 * du_2 
       du_3 = U_3 * dx + L31 * dv_1 + L32 * dv_2
       dv_3 = s3 * du_3 
       du_4 = U_4 * dx + L41 * dv_1 + L42 * dv_2 + L43 * dv_3
       dv_4 = s4 * du_4 
         dy =  J * dx + v1 * dv_1 + v2 * dv_2 + v3 * dv_3 + v4 * dv_4
    */
	void matA(int k,int nr,int nc){
		if(k<nr) {
			cout<<"k="<<k<<" nr="<<nr<<" nc="<<nc<<endl;
			int rv;

			sigma1[k]=1;
			cout<<"sigma1="<<sigma1<<endl;
			FOR(j,nc) A[k][j] = U[k][j];
			FOR(j,nc) FOR(i,k) A[k][j] += L[k][i]*sigma1[i]*A[i][j];

			rv=lpsol(k+1,nc);
			//print_retval(rv);
			cout<<"Aint="<<A<<endl;
			if(0|| rv==GLP_OPT || rv==GLP_FEAS || rv==GLP_UNBND ) {
				matA(k+1,nr,nc);
			} 

			sigma1[k]=-1;
			cout<<"sigma1="<<sigma1<<endl;
			FOR(j,nc) A[k][j] = U[k][j];
			FOR(j,nc) FOR(i,k) A[k][j] += L[k][i]*sigma1[i]*A[i][j];
			rv=lpsol(k+1,nc);
			//print_retval(rv);
			cout<<"Aint="<<A<<endl;
			if(0|| rv==GLP_OPT || rv==GLP_FEAS || rv==GLP_UNBND ) {
				matA(k+1,nr,nc);
			} 
			sigma1[k]=0;
		} else {
			sigma1[k]=-1; // fixed! since  dy < 0 only!
			std::cout<<"sigma1="<<sigma1<<endl;
			//cout<<"J="<<J<<endl;
			//cout<<"V="<<V<<endl;
			//cout<<"VS="<<VS<<endl;
			//cout<<"VSM="<<VSM<<endl;
			//cout<<"VSMU="<<VSMU<<endl;
			//cout<<"obj="<<obj<<endl;
			//cout<<"A="<<A.size()<<" k="<<k<<endl;
			FOR(j,nc) A[k][j] = J[j];
			// dy = (J + V*dv )
			FOR(j,nc) FOR(i,k) A[k][j] += V[i]*sigma1[i]*A[i][j];
			int rv=lpsol(nr+1,nc);
			print_retval(rv);
			if( rv==GLP_OPT) cout<<"A="<<A<<endl;
			//exit(1);
		}
	}
	void genmatA(int nr,int nc){
		A=vector<vector<double>>(ns+1,vector<double>(ni));
		obj=vector<double>(ni);
		//for(int i=0;i<ns;i++)for(int j=0;j<ni;j++)A[i][j]=U[i][j];
		matA(0,nr,nc);
	}
	int lpsol(int nr,int nc){
		cout<<"lpsol: nr="<<nr<<" nc="<<nc<<endl;
		cout<<"A.size()="<<A.size()<<endl;
		double eps=1e-5;
		glp_prob *lp;
		vector<int> ia,ja;
		vector<double> ar;
		ia.push_back(0); ja.push_back(0); ar.push_back(0);
		lp=glp_create_prob();
		glp_set_prob_name(lp, "sample");
		glp_set_obj_dir(lp, GLP_MAX);
		glp_add_rows(lp,nr);
		for(int i=1;i<=nr;i++){
			char b[10]; sprintf(b,"%d",i);
			string sn="row";
			sn = sn + string(b);
			//cout<<"sn="<<sn<<endl;
			glp_set_row_name(lp,i,sn.data());
			if(sigma1[i-1]>0) glp_set_row_bnds(lp,i,GLP_LO,eps,0.0);
			else glp_set_row_bnds(lp,i,GLP_UP,0.0,-eps);
		}
		//glp_set_row_name(lp,nr+1,"obj");
		//glp_set_row_bnds(lp,nr+1,GLP_UP,0.0,-eps);

		glp_add_cols(lp,nc);
		for(int j=1;j<=nc;j++){
			char b[10]; sprintf(b,"%d",j);
			string sn="col";
			sn = sn + string(b);
			//cout<<"sn="<<sn<<endl;
			glp_set_col_name(lp,j,sn.data());
			glp_set_col_bnds(lp,j,GLP_FR,0.0,0.0);
			glp_set_obj_coef(lp,j,0.0);
		}
		int cnt=0;
		for(int i=0;i<nr;i++) for(int j=0;j<nc;j++){
				cnt++;
			    ia.push_back(i+1); // 0 origin -> 1 origin
				ja.push_back(j+1); // 0 origin -> 1 origin
				ar.push_back(A[i][j]);
		}
		cout<<"ia.size()="<<ia.size()<<endl;
		cout<<"cnt="<<cnt<<endl;
		cout<<"ar="<<ar<<endl;
		glp_load_matrix(lp,cnt,ia.data(),ja.data(),ar.data());
		glp_simplex(lp,NULL);
		int rv=glp_get_status(lp);
		glp_delete_prob(lp);
		return rv;
	}
};

int main(int ac,char**av){
	ex1 p;
	p.top();
	return 0;
}
