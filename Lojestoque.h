# ifndef LOJESTOQUE_H
# define LOJESTOQUE_H

typedef struct tabela_hash Tab_Hash;
typedef struct no_Hash NoHash;
typedef struct estoque Estoque;
typedef struct produto Produto;


Estoque *criar_estoque(void);
void destruir_estoque(Estoque **e);

// Gerenciamento de produtos
int cadastrarProduto(Estoque *e);
int removerProduto(Estoque *e, int id);
int alterarProduto(Estoque *e, int id);

// Auxiliares do cadastro
int validar(char *t, int op);

// Tabela hash
Tab_Hash *inicializarHash(int cap);  
int inserirHash(Tab_Hash *h, Produto *p);
Produto *buscarHash(Tab_Hash *h, int id);
int removerHash(Tab_Hash *h, int id);
void destruirHash(Tab_Hash **h);  

// Função Auxiliar para Ordenação e Busca
Produto **criarVetorEstoque(Estoque *e);

// Ordenação
void insertionSort(Produto **vetor, int n);
void mergeSort(Produto **vetor, int n);
void heapSort(Produto **vetor, int n);

// Buscas

//precisa criar uma função de busca que reúna os 3 + a busca Hash (parte ja feita pela vitoria)
Produto *buscaSequencial(Produto **v, int n, int id);
Produto *buscaBinaria(Produto **v, int n, int id);
Produto *buscaPorSalto(Produto **v, int n, int id);

#endif