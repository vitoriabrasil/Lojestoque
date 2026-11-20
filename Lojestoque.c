#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lojestoque.h"

#define TAMANHO 10            
#define CATEGORIA 11

#define CAPACIDADE 100

//===============================================================================================================

struct no_Hash {
    Produto *produto;  // guarda o endereço de um produto da lista estoque principal -> não duplica, referencia
    struct no_Hash *prox; // aponta para o próximo nó da lista de colisões
};

struct tabela_hash
{
    NoHash **baldes; // vetor de ponteiros para as listas de colisões
    int capacidade;  // número de baldes
    int quantidade;  // igual ao n_itens
};

struct produto
{
    int id;
    char nome_prod[20];
    char tamanho[3];
    char categoria[30];
    float preco;
    int qtd;
    struct produto *prox;
    struct produto *ant;
};

struct estoque
{
    Produto *head;
	Produto *tail;
	int n_itens;
    int prox_id;
    Tab_Hash *hash;
};

//=============================================================================================================

Estoque *criar_estoque(void){
    Estoque *e = malloc(sizeof(Estoque));
    if(!e) return NULL;

    e->head = NULL;
    e->tail = NULL;
    e->n_itens = 0;
    e->prox_id = 1;

    e->hash = inicializarHash(CAPACIDADE);

    if (!e->hash) { // se  não for possível criar a tabela hash, também libera o estoque
        free(e);
        return NULL;
    }

    return e;
}

void destruir_estoque(Estoque **e){
    if(!e ||!(*e)) return; 

    destruirHash(&(*e)->hash);

    Produto *rem = NULL;
    Produto *temp = (*e)->head;
    while(temp!= NULL){
        rem = temp;
        temp = temp->prox;
        free(rem);
    }

    (*e)->head = NULL;
    (*e)->tail = NULL;
    (*e)->n_itens = 0;

    free(*e);
    *e = NULL;
}

// Gerenciamento de produtos
int cadastrarProduto(Estoque *e){
    if(!e || !e->hash) return 0;

    // Criando novo nó
    Produto *novo = malloc(sizeof(Produto));
    if(!novo) return 0;
    
    novo->prox = NULL;
    novo->ant = NULL;

    // ----Início do cadastro-----

    // Determinando ID
    novo->id = e->prox_id;

    limpa_buffer();

    // Definindo nome do produto
    printf("Nome do produto para cadastro: ");
    fgets(novo->nome_prod, sizeof(novo->nome_prod), stdin);
    novo->nome_prod[strcspn(novo->nome_prod, "\n")] = '\0'; // Remove a quebra de linha '\n'

    // Definindo tamanho do produto
    do{
        printf("Tamanho (PP, P, M, G, GG, XG): ");
        scanf("%2s",novo->tamanho);

        if(!validar(novo->tamanho,TAMANHO)) printf("Tamanho inválido! Tente novamente.\n");

    }while(!validar(novo->tamanho,TAMANHO));

    // Definindo categoria de roupa
    do{
        printf("Categoria (Calça, Camiseta, Saia, Short, Blusa, Íntimo): ");
        scanf("%8s",novo->categoria);

        if(!validar(novo->categoria,CATEGORIA)) printf("Categoria inválida! Tente novamente.\n");

    }while(!validar(novo->categoria,CATEGORIA));

    // Registrando preço
    do{
        printf("Preço: R$");
        scanf("%f", &novo->preco);

        if(novo->preco < 0) printf("Preço inválido! Tente novamente.\n");

    }while(novo->preco < 0);

    // Definindo tamanho do lote
    do{
        printf("Quantidade em estoque: ");
        scanf("%d", &novo->qtd);

        if(novo->qtd < 0) printf("Preço inválido! Tente novamente.\n");

    }while(novo->qtd < 0);

    // Passando o novo produto para a hash
    if (!inserirHash(e->hash, novo)) {
        free(novo);
        return 0;
    }

    // Inserindo na lista
     if(e->head == NULL){ // Caso seja a primeira inserção
        e->head = novo;
        e->tail = novo;
    } else{
        novo->ant = e->tail;
        e->tail->prox = novo;
        e->tail = novo;
    }

    e->n_itens++;  // Lote adicionado com sucesso
    e->prox_id++;

    return 1;
}

int removerProduto(Estoque *e, int id){
    if(!e || id < 1) return 0;

    Produto *removivel = buscarHash(e->hash, id);
    if (removivel == NULL) return 0;

    if(!removerHash(e->hash, id)) return 0; //caso não dê certo de remover o nó na hash

    //como ele já foi localizado pela hash, não precisa percorrer!

    if (removivel->ant != NULL) { // se ele não for o primeiro da lista
        removivel->ant->prox = removivel->prox;  // pulando o elemento removivel
    } else {
        e->head = removivel->prox;  // caso seja o primeiro
    }

    if (removivel->prox != NULL) {  //se ele não for o único da lista
        removivel->prox->ant = removivel->ant;  // pulando o elemento removivel
    } else {
        e->tail = removivel->ant; //caso seja o ultimo
    }

    free(removivel);

    e->n_itens--;
    return 1;
}

int alterarProduto(Estoque *e, int id){
    if(!e || id < 1) return 0;

    Produto *alt = buscarHash(e->hash, id);
    if (alt == NULL) return 0;

    printf("Deseja alterar qual informação?\n(0) Nome do cadastro do produto\n(1) Tamanho\n(2) Categoria\n(3) Preço\n(4) Quantidade\nDigite o número correspondente: ");
    int op, leitura;
    
    do{
        scanf("%d",&op);
        switch(op){
            case 0: // Nome do cadastro do produto
                limpa_buffer();

                printf("Nome do produto para cadastro: ");
                fgets(alt->nome_prod, sizeof(alt->nome_prod), stdin);
                alt->nome_prod[strcspn(alt->nome_prod, "\n")] = '\0'; // Remove a quebra de linha '\n'

                break;

            case 1: // Tamanho
                do{
                    printf("Tamanho (PP, P, M, G, GG, XG): ");
                    scanf("%2s",alt->tamanho);

                    if(!validar(alt->tamanho,TAMANHO)) printf("Tamanho inválido! Tente novamente.\n");

                }while(!validar(alt->tamanho,TAMANHO));

                break;

            case 2: // Categoria
                do{
                    printf("Categoria (Calça, Camiseta, Saia, Short, Blusa, Íntimo): ");
                    scanf("%8s",alt->categoria);

                    if(!validar(alt->categoria,CATEGORIA)) printf("Categoria inválida! Tente novamente.\n");

                }while(!validar(alt->categoria,CATEGORIA));

                break;

            case 3: // Preço
                do{
                    printf("Preço: R$");
                    leitura = scanf("%f", &alt->preco);
                    
                    if((leitura != 1) || (alt->preco < 0)) printf("Preço inválido! Tente novamente.\n");

                }while(alt->preco < 0 || leitura != 1);

                break;

            case 4: // Quantidade
                do{
                    printf("Quantidade em estoque: ");
                    leitura = scanf("%f", &alt->qtd);

                    if((leitura != 1) || (alt->qtd < 0)) printf("Quantidade inválida! Tente novamente.\n");

                }while(alt->qtd < 0 || leitura != 1);

                break;

            default:
                printf("Opção Inválida, tente novamente!\nDigite o número correspondente: ");
        }
    }while((op < 0) || (op > 4));

    return 1;
}

// Auxiliar do cadastro

int validar(char *t, int op){
    switch(op){
        case TAMANHO:{ //tamanho
            char tamanhos[6][3] = {"PP", "P", "M", "G", "GG", "XG"};
            for (int i = 0; i < 6; i++) {
                if (strcmp(t, tamanhos[i]) == 0) {
                    return 1;
                }
            }
            break;
        }

        case CATEGORIA:{
            char categorias[6][9] = {"Calça", "Camiseta", "Saia", "Short", "Blusa", "Íntimo"};
            for (int i = 0; i < 6; i++) {
                if (strcmp(t, categorias[i]) == 0) {
                    return 1;
                }
            }
            break;
        }
    }
    
    return 0;
}

void limpa_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
}


//========================================Implementações Hash======================================

Tab_Hash *inicializarHash(int cap){
    if(cap < 1) return NULL;  // capacidade inválida
    
    Tab_Hash *h = malloc(sizeof(Tab_Hash));
    if(!h) return NULL;

    h->baldes = calloc(cap, sizeof(NoHash *));

    if(!h->baldes){  // caso a alocação dos baldes dê errado, destruir tudo
        free(h);
        return NULL;
    }

    h->capacidade = cap;
    h->quantidade = 0;

    return h;
}

int inserirHash(Tab_Hash *h, Produto *p){
    if(!h || !p || p->id < 1 || h->capacidade < 1) return 0; // também verifica se o ID é válido

    // Calculando a posição
    int pos = p->id % h->capacidade;  //h[id] = id mod capacidade

    NoHash *atual = h->baldes[pos]; // é como se usasse o head

    while(atual != NULL) {
        if(atual->produto->id == p->id) return 0; // verificando se já existe um elemento cadastrado no hash com esse ID

        atual = atual->prox;
    }

    // depois dessa verificação, podemos adicionar o nó a tabela

    NoHash *novo = malloc(sizeof(NoHash));
    if(!novo) return 0;

    novo->produto = p; // nó aponta para o produto p

    // inserindo no início da lista do balde
    novo->prox = h->baldes[pos];
    h->baldes[pos] = novo;

    h->quantidade++;

    return 1;
}

Produto *buscarHash(Tab_Hash *h, int id){
    if(!h || id < 1 || h->capacidade < 1) return NULL; // também verifica se o ID é válido

    // Calculando a posição
    int pos = id % h->capacidade;  //h[id] = id mod capacidade

    NoHash *tmp = h->baldes[pos]; // nó temporário para percorrer a lista

    while(tmp != NULL){
        if(tmp->produto->id == id) return tmp->produto;

        tmp = tmp->prox;
    }

    return NULL;
}

int removerHash(Tab_Hash *h, int id){
    if(!h || id < 1 || h->capacidade < 1) return 0; // também verifica se o ID é válido

    // Calculando a posição
    int pos = id % h->capacidade;  //h[id] = id mod capacidade

    NoHash *tmp = h->baldes[pos]; // percorre a lista até encontrar o produto a ser removido
    NoHash *anterior = NULL; // guarda a posiçao do produto anterior

    while(tmp != NULL && tmp->produto->id != id){ // para quando tmp == NULL e/ou o ID é encontrado
        anterior = tmp;
        tmp = tmp->prox;
    }

    if(tmp == NULL) return 0;

    if(anterior == NULL){ //caso o elemento a ser removido seja o primeiro
         h->baldes[pos] = tmp->prox;
    } else{
        anterior->prox = tmp->prox; // pula o elemento atual
    }

    free(tmp);
    h->quantidade--;

    return 1;
}

void destruirHash(Tab_Hash **h){
    if (!h || !*h) return;

    for (int i = 0; i < (*h)->capacidade; i++) {
        NoHash *tmp = (*h)->baldes[i];

        while (tmp != NULL) {
            NoHash *proximo = tmp->prox;

            free(tmp);

            tmp = proximo;
        }
    }

    free((*h)->baldes);
    free(*h);

    *h = NULL;
}

//=====================================Ordenação e Busca=========================================

Produto **criarVetorEstoque(Estoque *e)
{
    if(!e) return NULL;

    Produto **v = malloc(e->n_itens * sizeof(Produto *));

    if (v == NULL)
        return NULL;

    Produto *atual = e->head;

    for (int i = 0; i < e->n_itens; i++)
    {
        v[i] = atual;
        atual = atual->prox;
    }

    return v;
}

// Ordenação -> a emissão do relatório tbm precisa permitir escolher a ordenação
void insertionSort(Produto **vetor, int n);
void mergeSort(Produto **vetor, int n);
void heapSort(Produto **vetor, int n);

// Buscas -> no main.c, deve existir uma interface que permita que o usuário escolha entre as buscas
Produto *buscaSequencial(Produto **v, int n, int id);
Produto *buscaBinaria(Produto **v, int n, int id);
Produto *buscaPorSalto(Produto **v, int n, int id);

