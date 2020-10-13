#include<iostream>
#include<string>
#include<stdio.h>
#include<cfloat>
#include <ctime>

using namespace std;
typedef struct arvore {
    int codigo, idade, altura;
    double divida;
    string nome, CPF, data_pagamento;
    struct arvore* esq;
    struct arvore* dir;
    
    void exibir_no() {
        cout <<"Codigo: "<< this->codigo <<endl;
        cout <<"Nome: "<< this->nome <<endl;
        cout <<"Idade: "<< this->idade <<endl;
        cout <<"CPF: "<< this->CPF <<endl;   
        printf("Divida: R$ %.2f \n",this->divida);        
		    cout <<"Data de pagamento: "<< this->data_pagamento.substr(8,2) << "/" << this->data_pagamento.substr(5,2) << "/" << this->data_pagamento.substr(0,4) <<endl; 
        if ((stoi(data_pagamento.substr(0,4)) == 2020
		    && stoi(data_pagamento.substr(5,2)) >= 9 
        &&stoi(data_pagamento.substr(8,2)) > 1 ) || 
		    (stoi(data_pagamento.substr(0,4)) > 2020)) {
		          cout<<"Este registro está com dívida."<<endl;
		}

        cout<<endl;
    }
    
} arvore;


class Arvore {

	arvore* raiz;
    arvore* maior_divida;
    double acumulador_de_dividas;
    int numero_de_dividas;

    arvore* inserir(int codigo,double divida,string nome,string CPF, int idade, string data_pagamento, arvore* A){
        if(A == NULL) {
            A = new arvore;
            A->codigo = codigo;
            A->CPF=CPF;
            A->divida=divida;
            A->idade=idade;
            A->data_pagamento=data_pagamento;
            A->nome=nome;
            A->altura = 0;
            A->esq = A->dir = NULL;


            if((A->divida)>this->maior_divida->divida)
                this->maior_divida = A;

            acumulador_de_dividas+=A->divida;
            this->numero_de_dividas++;
        } else if(codigo < A->codigo) {
            A->esq = inserir(codigo,divida,nome,CPF,idade,data_pagamento,A->esq);
            if(altura(A->esq) - altura(A->dir) == 2) {
                if(codigo < A->esq->codigo)
                    A = rotacao_simples_a_direita(A);
                else
                    A = rotacao_dupla_a_direita(A);
            }
        } else if(codigo > A->codigo) {
            A->dir = inserir(codigo,divida,nome,CPF,idade,data_pagamento,A->dir);
            if(altura(A->dir) - altura(A->esq) == 2) {
                if(codigo > A->dir->codigo)
                    A = rotacao_simples_a_esquerda(A);
                else
                    A = rotacao_dupla_a_esquerda(A);
            }
        
        
        
        
        }
        A->altura = max(altura(A->esq), altura(A->dir))+1;
        
        return A;
    }


    arvore* rotacao_simples_a_direita(arvore* &A) {
        arvore* B = A->esq;
        A->esq = B->dir;
        B->dir = A;
        A->altura = max(altura(A->esq), altura(A->dir))+1;
        B->altura = max(altura(B->esq), A->altura)+1;
        return B;
    }

    arvore* rotacao_simples_a_esquerda(arvore* &A) {
        arvore* B = A->dir;
        A->dir = B->esq;
        B->esq = A;
        A->altura = max(altura(A->esq), altura(A->dir))+1;
        B->altura = max(altura(A->dir), A->altura)+1 ;
        return B;
    }

    arvore* rotacao_dupla_a_esquerda(arvore* &A) {
        A->dir = rotacao_simples_a_direita(A->dir);
        return rotacao_simples_a_esquerda(A);
    }

    arvore* rotacao_dupla_a_direita(arvore* &A) {
        A->esq = rotacao_simples_a_esquerda(A->esq);
        return rotacao_simples_a_direita(A);
    }


    arvore* para_encontrar_o_minimo_codigo(arvore* A) {
        if(A!=NULL)
            while(A->esq!=NULL)
                A = A->esq;
        return A;
    }

    arvore* para_encontrar_o_maximo_codigo(arvore* A) {
        if(A == NULL)
            return NULL;
        else if(A->dir == NULL)
            return A;
        else
            return para_encontrar_o_maximo_codigo(A->dir);
    }



    int altura(arvore* A) {
        return (A == NULL ? -1 : A->altura);
    }

    int retorna_fator_de_balanceameno(arvore* A) {
        if(A == NULL)
            return 0;
        else
            return altura(A->dir) - altura(A->esq);
    }

    void inorder(arvore* A) {
        if(A == NULL)
            return;
        inorder(A->esq);
        A->exibir_no();
        cout<<endl;
        inorder(A->dir);
    }

    arvore* busca_recursiva(int codigo,arvore* raiz_Sub_Arvore) {

        if(codigo==raiz_Sub_Arvore->codigo)
            return raiz_Sub_Arvore;

        else if(codigo>raiz_Sub_Arvore->codigo)
            return busca_recursiva(codigo,raiz_Sub_Arvore->dir);
        else return busca_recursiva(codigo,raiz_Sub_Arvore->esq);

    }

    arvore* remover_no( int codigo,arvore* A) {
        if (A == NULL)
            return A;


        if ( codigo < A->codigo )
            A->esq = remover_no(codigo,A->esq);


        else if(codigo > A->codigo )
            A->dir = remover_no(codigo,A->dir);


        else {
            if( (A->esq == NULL) || (A->dir == NULL) ) {
                arvore *temp = A->esq ? A->esq : A->dir;

                if (temp == NULL) {
                    temp = A;
                    A = NULL;
                } else
                    *A = *temp;
                delete(temp);
            } else {

                arvore* temp = para_encontrar_o_minimo_codigo(A->dir);

                A->codigo = temp->codigo;

                A->dir = remover_no( temp->codigo,A->dir);
            }
        }

        if (A == NULL)
            return A;

        A->altura = 1 + max(altura(A->esq),altura(A->dir));


        int balance = retorna_fator_de_balanceameno(A);


        if (balance > 1 && retorna_fator_de_balanceameno(A->esq) >= 0)
            return rotacao_simples_a_esquerda(A);

        if (balance > 1 && retorna_fator_de_balanceameno(A->esq) < 0) {
            return rotacao_dupla_a_esquerda(A);
        }

        if (balance < -1 && retorna_fator_de_balanceameno(A->dir) <= 0)
            return rotacao_simples_a_direita(A);

        if (balance < -1 && retorna_fator_de_balanceameno(A->dir) > 0) {
            return rotacao_dupla_a_direita(A);
        }
        return A;
    }

public:

    void remover(int codigo) {


        raiz=remover_no(codigo,raiz);

    }
    
    void exibe_atrasado(arvore* A) {
		if(A == NULL)
		return;

		
		exibe_atrasado(A->esq);
		if ((stoi(A->data_pagamento.substr(0,4)) == 2020
		&& stoi(A->data_pagamento.substr(5,2)) >= 9 
    &&stoi(A->data_pagamento.substr(8,2)) > 1 ) || 
		(stoi(A->data_pagamento.substr(0,4)) > 2020
		)) {
		A->exibir_no();
		cout<<endl;
		}
		exibe_atrasado(A->dir);

	}		


    void exibe_intervalo(arvore* A,double divida1,double divida2) {
        if(A == NULL)
            return;
        exibe_intervalo(A->esq,divida1,divida2);
        if( divida1<=A->divida &&  A->divida<=divida2 ) {
            A->exibir_no();
            cout<<endl;
        }
        exibe_intervalo(A->dir,divida1,divida2);
    }

    Arvore() {
        raiz = NULL;
        maior_divida=new arvore;
        maior_divida->divida=DBL_MIN;
        this->acumulador_de_dividas=0.0;
        this->numero_de_dividas=0;
    }

    double get_valor_medio_das_dividas() {

        return this->acumulador_de_dividas/(double)this->numero_de_dividas;

    }

    void get_maior_divida() {

        cout <<"CPF: "<< this->maior_divida->CPF <<endl;
        cout <<"Divida: "<< this->maior_divida->divida <<endl;

        cout << endl;

    }
    
    void exibir_dividas_em_atraso() {
		exibe_atrasado(raiz);
		cout << endl;
	}

    int get_numero_de_devedores() {

        return this->numero_de_dividas;

    }

    void inserir_interface(int codigo,double divida,string nome,string CPF,int idade,string data_pagamento) {
        raiz = inserir(codigo,divida,nome,CPF,idade,data_pagamento,raiz);
    }

    void exibir() {
        inorder(raiz);
        cout << endl;
    }
    
    void exibir_no_intervalo(double divida1,double divida2) {
        if(divida1<=0||divida2<=0) {

            cout<<endl<<"dividas invalidas";
            return;

        }
        if(divida1>divida2)
            swap(divida1,divida2);

        exibe_intervalo(raiz,divida1,divida2);
        cout << endl;
    }


    arvore* busca(int codigo) {
        arvore* i = this->raiz;
        i=busca_recursiva(codigo,i);
        return i;

    }



};

int main() {
	
  Arvore* arvore =new Arvore();
	int codigo = 0, idade=0, op=0, dia = 0, ano = 0;
 	string nome = "", CPF = "", data_pagamento;
 	double divida = 0;
         

	do {
        cout << "\n1 - Incluir";
        cout << "\n2 - Consulta";
        cout << "\n3 - Excluir";
        cout << "\n4 - Relatorio";
        cout << "\n5 - Sair";
        cout << "\n\nDigite sua opcao: ";
        cin >> op;
        switch (op){
            case 1: 
            	cout<<"Codigo: ";
		        cin>>codigo;
		        cout<<"Nome: ";
		        cin>>nome;
		        cout<<"Idade: ";
		        cin>>idade;
		        cout<<"CPF: ";
		        cin>>CPF;
		        cout<<"Divida: ";
		        cin>>divida;
		        cout<<"Data de Pagamento(YYYY/MM/DD): ";
				cin>>data_pagamento;
		        
		        arvore->inserir_interface(codigo,divida,nome,CPF,idade,data_pagamento);
		        cout<<"\nInserido com Sucesso!!"<<endl;
        		break;
            case 2: 
            	arvore->exibir();
            	break;
            case 3: 
            	cout<<"Digite o Codigo para Exclusao: ";
            	cin>>codigo;
            	arvore->remover(codigo);
            	cout<<"\nExcluido com Sucesso!!\n"<<endl;
            	cout<<"\nDados Atualizados: "<<endl;
            	arvore->exibir();  
                break;
            case 4: 
	            cout << "\n1 - Maior Divida";
        		cout << "\n2 - Dividas em Atraso";
        		cout << "\n\nDigite sua opcao: ";
        		cin >> op;
        		switch (op){
        			case 1:
        				arvore->get_maior_divida();
              			break;
        			case 2:
        				arvore->exibir_dividas_em_atraso();
        				break;
				}
                break;
            case 5: 
                break;
            default: cout << "Opcao invalida . . .\n\n";
           		     break;
        }
    } while (op != 5);


}
