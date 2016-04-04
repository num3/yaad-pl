/* Sun Mar 27 09:57:49 JST 2016 
 kk@ise.chuo-u.ac.jp
*/
#include <iostream>
#include <stdlib.h>
#include <cmath>

int endpoint_ops(int op) {
	if( op == OP_INP ) return 1; 
	//if( op == OP_FABS ) return 1; 
	return 0;
}

int binary_ops(int op)
{       return (op&0xf) == 0x3; }

int unary_ops(int op)
{       return (op&0xf) == 0x0 || (op&0xf) == 0x1 || (op&0xf) == 0x2; 
}

/* DFS_1 */
template<typename T>
void    Node<T>::dfs_body(Node<T>* p, int sw)
/*  sw: 0  --- diff();
 *      1  --- h_diff();
 *      2  --- memfree();
 */
{       //if(sw==0) p->d_val = 0.0;         /* Initialize */
        //else if(sw == 1) p->hd_val = 0.0; /* Initialize */
		//std::cout<<"op="<<std::hex<<(p->op)<<std::endl;
		if(sw==0) p->set_d_val( T(0.0) );
		else if(sw==1) p->set_hd_val( T(0.0) );

        if(endpoint_ops(p->op)) {
                /* */
			if(sw==2) delete p;
        } else if(binary_ops(p->op)) {
                BNode<T>* q = (BNode<T>*)p;
                if(q->arg1->dfsp == NULL) {
                        q->arg1->dfsp = p;
                        dfs_body(q->arg1,sw);
                }
                if(q->arg2->dfsp == NULL) {
                        q->arg2->dfsp = p;
                        dfs_body(q->arg2,sw);
                }
        } else if(unary_ops(p->op)) {
                UNode<T>* q = (UNode<T>*)p;
                if(q->arg1->dfsp == NULL) {
                        q->arg1->dfsp = p;
                        dfs_body(q->arg1,sw);
                }
        }
}
/* DFS_2 */

/*
template<typename T>
void Node<T>::h_spread_derivatives(Node<T>* p);

template<typename T>
void Node<T>::spread_derivatives(Node<T>* p);

template<typename T>
void    Node<T>::rdfs_body(Node<T>* p, void(*spread_func)(Node<T>*));
*/

/* RDFS_1 */
template<typename T>
void    Node<T>::rdfs(Node<T>* p)
{       rdfs_body(p, spread_derivatives); }

template<typename T>
void    Node<T>::h_rdfs(Node<T>* p)
{       rdfs_body(p, h_spread_derivatives); }

template<typename T>
void    Node<T>::rdfs_body(Node<T>* p, void(*spread_func)(Node<T>*))
{       p->dfsp=NULL;
        spread_func(p);

        if(endpoint_ops(p->op)) {
                /* */
        } else if(binary_ops(p->op)) {
                BNode<T>* q = (BNode<T>*)p;
                if(q->arg2->dfsp == p) {
                        rdfs_body(q->arg2,spread_func);
                }
                if(q->arg1->dfsp == p) {
                        rdfs_body(q->arg1,spread_func);
                }
        } else if(unary_ops(p->op)) {
                UNode<T>* q = (UNode<T>*)p;
                if(q->arg1->dfsp == p) {
                        rdfs_body(q->arg1,spread_func);
                }
        }
}
/* RDFS_2 */

/* SPREAD_DRV_1 */
template<typename T>
void Node<T>::spread_derivatives(Node<T>* p)
{       int op;
        op = p->op;
        switch(op){
        case OP_NOP:
                break;
        case OP_INP:
                break;
        case OP_ADD:
                { BNode<T>* q = (BNode<T>*)p;
                  q->arg1->d_val += p->d_val;
                  q->arg2->d_val += p->d_val;
                }
                break;
        case OP_ADDL:
        case OP_ADDR:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->d_val += p->d_val;
                }
                break;
/* SPREAD_DRV_2 */
        case OP_SUB:
                { BNode<T>* q = (BNode<T>*)p;
                  q->arg1->d_val += p->d_val;
                  q->arg2->d_val += (-1.0)* p->d_val;
                }
                break;
        case OP_SUBL:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->d_val += p->d_val;
                }
                break;
        case OP_SUBR:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->d_val += (-1.0)* p->d_val;
                }
                break;
        case OP_MUL:
                { BNode<T>* q = (BNode<T>*)p;
                  q->arg1->d_val += p->d_val * q->arg2->val;
                  q->arg2->d_val += p->d_val * q->arg1->val;
                }
                break;
        case OP_MULL:
        case OP_MULR:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->d_val += p->d_val * q->argval;
                }
                break;
        case OP_DIV:
                { BNode<T>* q = (BNode<T>*)p;
                  T tmp = p->get_d_val() / q->get_arg2()->get_val();
                  q->arg1->d_val += tmp;
                  q->arg2->d_val += (-1.0)* tmp * q->val ;
                }
                break;
        case OP_DIVL:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->d_val += p->d_val / q->argval;
                }
                break;
        case OP_DIVR:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->d_val += (-1.0)* p->d_val * p->val / q->arg1->val;
                }
                break;
        case OP_UPLUS:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->d_val += p->d_val;
                }
                break;
        case OP_UMINUS:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->d_val += (-1.0)* p->d_val;
                }
                break;
        case OP_SQRT:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->d_val += p->d_val / (2.0 * p->val);
                }
                break;
        case OP_EXP:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->d_val += p->d_val * p->val;
                }
                break;
        case OP_LOG:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->d_val += p->d_val / q->arg1->val;
                }
                break;
        case OP_FABS:
				{ UNode<T>* q = (UNode<T>*)p;
                  double eps=1.0e-8;
				  std::cout<<"OP_FABS q->val="<<q->val<<"  ";
				  if(q->val > eps) q->arg1->d_val += p->d_val;
				  if(q->val < -eps) q->arg1->d_val += - p->d_val;
				  if(fabs(q->val) <= eps) {
				  //if(fabs(q->val) == 0.0 ) {
					std::cout<<" critical point q->val="<<q->val;
					Node<T>::push_fabs_nodes(q);
					Node<T>::push_fabs_arg_nodes(q->get_arg1());
				  }
				  std::cout<<std::endl;
				}
                break;
        default:
                std::cerr<<"spread_derivatives: Unknown op\n";
        }
}

/* SPREAD_H_DRV_1 */
template<typename T>
void Node<T>::h_spread_derivatives(Node<T>* p)
{       int op;
        op = p->op;
        switch(op){
        case OP_NOP:
                break;
        case OP_INP:
                break;
        case OP_ADD:
                { BNode<T>* q = (BNode<T>*)p;
                  q->arg1->hd_val += p->hd_val;
                  q->arg2->hd_val += p->hd_val; }
                break;
        case OP_ADDL:
        case OP_ADDR:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->hd_val += p->hd_val; } break;
/* SPREAD_H_DRV_2 */
        case OP_SUB:
                { BNode<T>* q = (BNode<T>*)p;
                  q->arg1->hd_val += p->hd_val;
                  q->arg2->hd_val += (-1.0)* p->hd_val; } break;
        case OP_SUBL:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->hd_val += p->hd_val; } break;
        case OP_SUBR:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->hd_val += (-1.0)* p->hd_val; } break;
        case OP_MUL:
                { BNode<T>* q = (BNode<T>*)p;
                  q->arg1->hd_val += p->hd_val * q->arg2;
                  q->arg2->hd_val += p->hd_val * q->arg1; } break;
        case OP_MULL:
        case OP_MULR:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->hd_val += p->hd_val * q->argval; } break;
        case OP_DIV:
                { BNode<T>* q = (BNode<T>*)p;
                  FTD<T> tmp = p->hd_val / q->arg2; 
                  q->arg1->hd_val += tmp;
                  q->arg2->hd_val += (-1.0)* tmp * FTD<T>(p) ; } break;
        case OP_DIVL:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->hd_val += p->hd_val / q->argval; } break;
        case OP_DIVR:
                { BUNode<T>* q = (BUNode<T>*)p;
                  q->arg1->hd_val += (-1.0)* p->hd_val * FTD<T>(p) / q->arg1;
                }
                break;
        case OP_UPLUS:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->hd_val += p->hd_val; } break;
        case OP_UMINUS:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->hd_val += (-1.0)* p->hd_val; } break;
        case OP_SQRT:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->hd_val += p->hd_val / (2 * FTD<T>(p)); } break;
        case OP_EXP:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->hd_val += p->hd_val * FTD<T>(p); } break;
        case OP_LOG:
                { UNode<T>* q = (UNode<T>*)p;
                  q->arg1->hd_val += p->hd_val / q->arg1; } break;
        default:
                std::cerr<<"h_spread_derivatives: Unknown op\n";
        }
}


template<typename T>
void    Node<T>::dfs(Node<T>* p) {   Node<T>::dfs_body(p, 0); }

template<typename T>
void    Node<T>::h_dfs(Node<T>* p) {   Node<T>::dfs_body(p, 1); }

template<typename T>
void    Node<T>::memfree_dfs(Node<T>* p) {       Node<T>::dfs_body(p, 2); }

