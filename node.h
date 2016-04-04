/* Sun Mar 27 09:58:24 JST 2016
  kk@ise.chuo-u.ac.jp
*/
#ifndef DEFINE_MAXMEM
const int MAXMEM = 32768*256;
int my_memory_on;
void* my_memory_pointer;
void* my_memory_pointer_end;
char my_memory[MAXMEM];
int size_of_my_memory = MAXMEM;

#define DEFINE_MAXMEM 1
#endif

template<typename T>
class FTD;

template<typename T>
class Node {
protected:
	int op;
	T val;
	T d_val;
	FTD<T> hd_val;
	Node<T> *dfsp;
static void dfs_body(Node<T>*p, int sw);
static void rdfs_body(Node<T>*p, void(*spread_func)(Node<T>*));
static void fdfs_body(Node<T>*p, void(*accum_func)(Node<T>*));
public:
	Node(void):op(OP_NOP),val(T()),d_val(T()),hd_val(FTD<T>()),dfsp(NULL){ }
	void set_op(int op){this->op=op;}
	void set_val(T val){this->val=val;}
	void set_d_val(T d_val){this->d_val=d_val;}
	void set_hd_val(FTD<T> hd_val){this->hd_val=hd_val;}
	void set_dfsp(Node<T>* p){this->dfsp=p;}
	T get_val() const {return val;}
	T get_d_val() const {return d_val;}
static void dfs(Node<T>*p);
static void h_dfs(Node<T>*p);
static void memfree_dfs(Node<T>*p);
static void rdfs(Node<T>*p);
static void h_rdfs(Node<T>*p);
static void fdfs(Node<T>*p);
static void h_fdfs(Node<T>*p);

static void h_spread_derivatives(Node<T>*p);
static void spread_derivatives(Node<T>*p);

static void push_fabs_nodes(Node<T>*p);
static void push_fabs_arg_nodes(Node<T>*p);
static void push_input_nodes(Node<T>*p);
static std::vector<Node<T>*> fabs_nodes;
static std::vector<Node<T>*> fabs_arg_nodes;
static std::vector<Node<T>*> input_nodes;
static void reset_fabs_d_val(void){
			//for(Node<T>* p : fabs_nodes) {
			for(size_t i=0;i<fabs_nodes.size();i++){
				Node<T>*p = fabs_nodes[i];
				//std::cout<<"  reset_fabs_d_val: p="<<p<<std::endl;
				p->set_d_val(T());
			}
		}
static void reset_input_d_val(void){
			//for(Node<T>* p : input_nodes) {
			for(size_t i=0;i<input_nodes.size();i++){
				Node<T>*p = input_nodes[i];
				//std::cout<<"  reset_input_d_val: p="<<p<<std::endl;
				p->set_d_val(T());
			}
		}
static std::vector<T> getJ(){
		std::vector<T> J;
		for(int j=0;j<input_nodes.size();j++)
			J.push_back(input_nodes[j]->get_d_val());
		return J;
	}
static std::vector<T> getV(){
		std::vector<T> V;
		for(int j=0;j<fabs_nodes.size();j++)
			V.push_back(fabs_nodes[j]->get_d_val());
		return V;
	}
static std::vector<std::vector<T>> getU(){
		std::vector<std::vector<T>> U;
		for(int i=0;i<fabs_arg_nodes.size();i++){
			Node<T>* ui = fabs_arg_nodes[i];
			dfs(ui);
			ui->set_d_val(1.0);
			rdfs(ui);
			std::vector<double> uu;
			for(int j=0;j<input_nodes.size();j++)
				uu.push_back(input_nodes[j]->get_d_val());
			U.push_back(uu);
		}
		return U;
	}
static std::vector<std::vector<T>> getL(){
		std::vector<std::vector<T>> L;
		for(int i=0;i<fabs_arg_nodes.size();i++){
			Node<T>* ui = fabs_arg_nodes[i];
			dfs(ui);
			ui->set_d_val(1.0);
			rdfs(ui);
			std::vector<double> ll;
			for(int j=0;j<fabs_nodes.size();j++)
				ll.push_back(fabs_nodes[j]->get_d_val());
			L.push_back(ll);
		}
		return L;
	}
};

template<typename T>
std::vector<Node<T>*> Node<T>::fabs_nodes;
template<typename T>
std::vector<Node<T>*> Node<T>::fabs_arg_nodes;
template<typename T>
std::vector<Node<T>*> Node<T>::input_nodes;

template<typename T>
void Node<T>::push_fabs_nodes(Node<T>*p){
	fabs_nodes.push_back(p);
}
template<typename T>
void Node<T>::push_fabs_arg_nodes(Node<T>*p){
	fabs_arg_nodes.push_back(p);
}
template<typename T>
void Node<T>::push_input_nodes(Node<T>*p){
	input_nodes.push_back(p);
}

void* my_new(size_t s){
	void* p;
	p = my_memory_pointer;
	my_memory_pointer = ((char*)my_memory_pointer) + ((s+3)/4)*4;
	if(my_memory_pointer < my_memory_pointer_end) return p;
	else {
		std::cerr<<"my_new: private memory is exausted\n";
		return NULL;
	}
}

template<typename T>
void history_on(void){
	my_memory_pointer = &Node<T>::my_memory[0];
	my_memory_pointer_end = &Node<T>::my_memory[Node<T>::size_of_my_memory];
	my_memory_on = 1;
}
template<typename T>
void history_off(void){
	my_memory_on = 0;
}

//template<typename T>
//int my_memory_on = 0;

template<typename T>
class UNode : public Node<T> {
	friend Node<T>;
protected:
	Node<T> *arg1;
public:
	UNode(void):arg1(NULL){ }
	UNode(int op, T val, Node<T>* arg1){
		this->op = op;
		this->val = val;
		this->d_val = T();
		this->arg1 = arg1;
	}
	void set_arg1(Node<T>* arg1){this->arg1 = arg1;}
	Node<T>* get_arg1(void) const { return this->arg1; }

};

template<typename T>
UNode<T>* newUNode(int op, T val, Node<T>* arg1){
	UNode<T>* p;
	if(my_memory_on == 1) p = (UNode<T>*)my_new(sizeof(UNode<T>));
	else p = new UNode<T>;
	p->set_op(op); //p->op = op;
	p->set_val(val); //p->val = val;
	p->set_d_val(T()); //p->d_val = T();
	p->set_arg1(arg1); //p->arg1 = arg1;
	return p;
}

template<typename T>
class BUNode : public UNode<T> {
	friend Node<T>;
protected:
	T argval;
public:
	BUNode(void):argval(T()){ }
	BUNode(int op, T val, Node<T>* arg1, T v2) {
		this->op = op;
		this->val = val;
		this->d_val = T();
		this->arg1 = arg1;
		this->argval = v2;
	}
	void set_argval(T v2){ this->argval = v2; }
};

template<typename T>
BUNode<T>* newBUNode(int op, T val, Node<T>* arg1, T v2){
	BUNode<T>* p;
	if(my_memory_on == 1) p = (BUNode<T>*)my_new(sizeof(BUNode<T>));
	else p = new BUNode<T>;
	p->set_op(op); //p->op = op;
	p->set_val(val); //p->val = val;
	p->set_d_val(T()); //p->d_val = T();
	p->set_arg1(arg1); //p->arg1 = arg1;
	p->set_argval(v2); //p->argval = v2;
	return p;
}

template<typename T>
class BNode : public UNode<T> {
	friend Node<T>;
protected:
	Node<T> *arg2;
public:
	BNode(void):arg2(NULL){ }
	BNode(int op, T val, Node<T>* arg1, Node<T>* arg2) {
		this->op = op;
		this->val = val;
		this->d_val = T();
		this->arg1 = arg1;
		this->arg2 = arg2;
	}
	void set_arg2(Node<T>* arg2){this->arg2 = arg2;}
	Node<T>* get_arg2(void) const { return this->arg2; }
};

template<typename T>
BNode<T>* newBNode(int op, T val, Node<T>*arg1, Node<T>*arg2){
	BNode<T>* p;
	if(my_memory_on == 1) p = (BNode<T>*)my_new(sizeof(BNode<T>));
	else p = new BNode<T>();
	p->set_op(op); //p->op = op;
	p->set_val(val); //p->val = val;
	p->set_d_val(T()); //p->d_val = T();
	p->set_arg1(arg1); //p->arg1 = arg1;
	p->set_arg2(arg2); //p->arg2 = arg2;
	return p;
}

template<typename T>
Node<T>* newNode(){
	Node<T>* p;
	if(my_memory_on == 1) p = (Node<T>*)my_new(sizeof(Node<T>));
	else p = new Node<T>();
	return p;
}

