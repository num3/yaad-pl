/* Sun Mar 27 09:54:53 JST 2016 
 Revised code of original in 
   K.Kubota and M.Iri:Automatic Differentiation of Algorithms and Applications,
   Corona, Tokyo, 1998, ISBN 4-339-02602-6. (in Japanese) 
 kk@ise.chuo-u.ac.jp
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "ops.h"
#include "node.h"
#include "diff.h"

template<typename T>
class Node;

template<typename T>
class BNode;

template<typename T>
class BUNode;

template<typename T>
class UNode;

void history_on(void);
void history_off(void);

template<typename T>
class FTD {
	union {
		Node<T> *p;
		T val;
	};
	int active;
public:
	int is_active() const { return active == F_Active; }

	FTD(void){ val=T(); active = F_InActive; }
	FTD(const FTD& x){
		if(x.active == F_Active) { active = F_Active; p = x.p; }
		else { active = F_InActive; val = x.val; }
	}
	FTD& operator=(const FTD& x){
		if(x.active == F_Active) { active = F_Active; p = x.p; }
		else { active = F_InActive; val = x.val; }
		return *this;
	}
	~FTD(void){ }
	FTD(T x){ val = x; active = F_InActive; }
	FTD(int x){ val = x; active = F_InActive; }
	FTD(Node<T>* pp){ p = pp; active = F_Active; }

	void set_indep(void){
		T tmp;
		Node<T>* pp = newNode<T>();
		tmp = val;
		pp->set_op(OP_INP); //pp->op = OP_INP;
		pp->set_val(tmp);   //pp->val = tmp;
		pp->set_dfsp(NULL); //pp->dfsp = NULL;
		p = pp;
		active = F_Active;
	}
	void diff(void){
		if(active == F_Active) {
			Node<T>::dfs(p); p->set_d_val(T(1.0)); Node<T>::rdfs(p); }
	}
	void h_diff(void){
		if(active == F_Active) {
			Node<T>::h_dfs(p); p->set_hd_val(T(1.0)); Node<T>::h_rdfs(p); }
	}
	FTD<T> get_hd_val(void){ return this->p->get_hd_val(); }
	T get_d_val(void){ return this->p->get_d_val(); }
	void set_d_val(T v=T()){ this->p->set_d_val(v); }
	T get_val(void) const {
		if(active == F_Active) return this->p->get_val();
		else return this->val;
	}
	Node<T>*get_p(void) const { return p; }

	std::vector<std::vector<T>> U,L;
	std::vector<T> J,V;
	int ni,ns;
	void abs_normal_form(void){
		Node<T>::reset_input_d_val();
		diff();
		ni=Node<T>::input_nodes.size();
		ns=Node<T>::fabs_nodes.size();
		std::cout<<"ni="<<ni<<" ns="<<ns<<std::endl;
		std::reverse(&Node<T>::fabs_nodes[0],&Node<T>::fabs_nodes[ns]);
		std::reverse(&Node<T>::fabs_arg_nodes[0],&Node<T>::fabs_arg_nodes[ns]);
		/* change OP_FABS nodes to OP_INP */
		for(Node<T>* p : Node<T>::fabs_nodes ) { p->set_op(OP_INP); }
		Node<T>::reset_input_d_val();
		diff();
		for(int j=0;j<ni;j++) J.push_back(Node<T>::input_nodes[j]->get_d_val());
		for(int j=0;j<ns;j++) V.push_back(Node<T>::fabs_nodes[j]->get_d_val());
		for(int i=0;i<ns;i++){
			/* reset dydx=0 */
			Node<T>::reset_input_d_val();
			Node<T>::reset_fabs_d_val();

			/* dfs from fabs_arg_nodes[i] */
			Node<T>* ui = Node<T>::fabs_arg_nodes[i];
			Node<T>::dfs(ui);
			ui->set_d_val(1.0);
			Node<T>::rdfs(ui);

			std::vector<double> uu;
			for(int j=0;j<ni;j++) uu.push_back(Node<T>::input_nodes[j]->get_d_val());
			U.push_back(uu);
			std::vector<double> ll;
			for(int j=0;j<ns;j++) ll.push_back(Node<T>::fabs_nodes[j]->get_d_val());
			L.push_back(ll);
		}
	}
};

template<typename T>
FTD<T> operator+(const FTD<T>& x, const FTD<T>& y){
	if( x.is_active() && y.is_active() ) {
		return newBNode<T>(OP_ADD,x.get_val()+y.get_val(),x.get_p(),y.get_p());
	} else if(x.is_active()){
		return newBUNode<T>(OP_ADDL, x.get_val()+ y.get_val(), x.get_p(), y.get_val());
	} else if(y.is_active()){
		return newBUNode<T>(OP_ADDR, x.get_val() + y.get_val(), y.get_p(), x.get_val());
	} else {
		return FTD<T>(x.get_val() + y.get_val());
	}
}
template<typename T>
FTD<T> operator+(const FTD<T>& x, const T& y){
	if(x.is_active()){
		return newBUNode<T>(OP_ADDL, x.get_val()+ y, x.get_p(), y);
	} else {
		return FTD<T>(x.get_val() + y);
	}
}
template<typename T>
FTD<T> operator+(const T& x, const FTD<T>& y){
	if(y.is_active()){
		return newBUNode<T>(OP_ADDR, x + y.get_val(), y.get_p(), x);
	} else {
		return FTD<T>(x + y.get_val());
	}
}

template<typename T>
FTD<T> operator+=(const FTD<T>& x, const FTD<T>& y){
	if( x.is_active() && y.is_active() ) {
		x = newBNode<T>(OP_ADD, x.get_val()+y.get_val(), x.get_p(), y.get_p());
		return x;
	} else if(x.is_acitive() ){
		x = newBUNode<T>(OP_ADDL, x.get_val() + y.get_val(), x.get_p(), y.get_val());
		return x;
	} else if(y.is_acitive() ){
		x = newBUNode<T>(OP_ADDR, x.get_val() + y.get_val(), y.get_p(), x.get_val());
		return x;
	} else {
		x = FTD<T>(x.val + y.val);
		return x;
	}
}

template<typename T>
FTD<T> operator-(const FTD<T>& x, const FTD<T>& y){
	if( x.is_active() && y.is_active() ) {
		return newBNode<T>(OP_SUB, x.get_val() - y.get_val(), x.get_p(), y.get_p());
	} else if(x.is_active() ){
		return newBUNode<T>(OP_SUBL, x.get_val()-y.get_val(),x.get_p(), y.get_val());
	} else if(y.is_active() ){
		return newBUNode<T>(OP_SUBR, x.get_val() - y.get_val(), y.get_p(), x.get_val());
	} else {
		return FTD<T>(x.get_val() - y.get_val());
	}
}
template<typename T>
FTD<T> operator-(const FTD<T>& x, T& y){
	if(x.is_active() ){
		return newBUNode<T>(OP_SUBL, x.get_val()-y,x.get_p(), y);
	} else {
		return FTD<T>(x.get_val() - y);
	}
}
template<typename T>
FTD<T> operator-(const T& x, const FTD<T>& y){
	if(y.is_active() ){
		return newBUNode<T>(OP_SUBR, x - y.get_val(), y.get_p(), x);
	} else {
		return FTD<T>(x - y.get_val());
	}
}

template<typename T>
FTD<T> operator*(const FTD<T>& x, const FTD<T>& y){
	if( x.is_active() && y.is_active() ) {
		return newBNode<T>(OP_MUL, x.get_val()*y.get_val(), x.get_p(), y.get_p());
	} else if(x.is_active() ){
		return newBUNode<T>(OP_MULL, x.get_val() * y.get_val(), x.get_p(), y.get_val());
	} else if(y.is_active() ){
		return newBUNode<T>(OP_MULR, x.get_val() * y.get_val(), y.get_p(), x.get_val());
	} else {
		return FTD<T>(x.get_val() * y.get_val());
	}
}
template<typename T>
FTD<T> operator*(const T& x, const FTD<T>& y){
	if(y.is_active() ){
		return newBUNode<T>(OP_MULR, x * y.get_val(), y.get_p(), x);
	} else {
		return FTD<T>(x * y);
	}
}
template<typename T>
FTD<T> operator*(const FTD<T>& x, const T& y){
	if(x.is_active() ){
		return newBUNode<T>(OP_MULL, x.get_val() * y, x.get_p(), y);
	} else {
		return FTD<T>(x * y);
	}
}

template<typename T>
FTD<T> operator/(const FTD<T>& x, const FTD<T>& y){
	if( x.is_active() && y.is_active() ) {
		return newBNode<T>(OP_DIV, x.get_val() / y.get_val(), x.get_p(), y.get_p());
	} else if(x.is_active() ){
		return newBUNode<T>(OP_DIVL, x.get_val() / y.get_val(), x.get_p(), y.get_val());
	} else if(y.is_active() ){
		return newBUNode<T>(OP_DIVR, x.get_val() / y.get_val(), y.get_p(), x.get_val());
	} else {
		return FTD<T>(x.get_val() / y.get_val());
	}
}

template<typename T>
FTD<T> operator+(const FTD<T>& x) {
	if( x.is_active() ) {
		return newUNode<T>(OP_UPLUS, x.p->val, x.p);
	} else {
		return FTD<T>(+x.val);
	}
}

template<typename T>
FTD<T> operator-(const FTD<T>& x) {
	if( x.is_active() ) {
		return newUNode<T>(OP_UMINUS, -x.get_val(), x.get_p());
	} else {
		return FTD<T>(-x.get_val());
	}
}

template<typename T>
int operator<(const FTD<T>& x, const FTD<T>& y){
	return x.get_val() < y.get_val();
}
template<typename T>
int operator<=(const FTD<T>& x, const FTD<T>& y){
	return x.get_val() <= y.get_val();
}
template<typename T>
int operator>(const FTD<T>& x, const FTD<T>& y){
	return x.get_val() > y.get_val();
}
template<typename T>
int operator>=(const FTD<T>& x, const FTD<T>& y){
	return x.get_val() >= y.get_val();
}

template<typename T>
FTD<T> sqrt(const FTD<T>& x){
	if(x.is_active()) {
		return newUNode<T>(OP_SQRT, sqrt(x.p->val), x.p);
	} else {
		return FTD<T>(sqrt(x.val));
	}
}
template<typename T>
FTD<T> exp(const FTD<T>& x){
	if(x.is_active()) {
		return newUNode<T>(OP_EXP, exp(x.p->val), x.p);
	} else {
		return FTD<T>(exp(x.val));
	}
}
template<typename T>
FTD<T> log(const FTD<T>& x){
	if(x.active == F_Active) {
		return newUNode<T>(OP_LOG, exp(x.p->val), x.p);
	} else {
		return FTD<T>(exp(x.val));
	}
}
template<typename T>
FTD<T> fabs(const FTD<T>& x){
	if(x.is_active() ) {
		return newUNode<T>(OP_FABS, fabs(x.get_val()), x.get_p());
	} else {
		return FTD<T>(abs(x.get_val()));
	}
}

template<typename T>
std::ostream&operator<<(std::ostream&s, const FTD<T>&x){
	if(x.active == F_Active) s<<"("<<x.p->val<<", "<<x.p->d_val<<") ";
	else s<<x.val<<" ";
	return s;
}
