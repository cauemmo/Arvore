#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"
//Cauê Mendonça Magela do Ó
//rgm : 43558

struct Arvore
{
    int chave;
    struct Arvore *no_esq;
    struct Arvore *no_dir;
};

void Buscar(struct Arvore*, struct Arvore**, struct Arvore**, int, int*);

void Inserir(struct Arvore**, struct Arvore*, int, int);

void Remover(struct Arvore**, struct Arvore*, struct Arvore*);

void Tela(int, int, int, int, int, int, struct Arvore*);

void Desalocar(struct Arvore*);

int main() {
    int resp, f, num;
    struct Arvore *arvore = NULL;
    struct Arvore *pt = NULL;
    struct Arvore *ant = NULL;
    gfx_init(1200, 720, "Arvore");
    do{
        printf("1-Busca\n2-Incerção\n3-Remoção\n4-sair\n");
        scanf("%d", &resp);
        switch (resp) {
            case 1:
                printf("Informe o número que vai ser buscado: \n");
                scanf("%d", &num);
                f = -1;
                pt = NULL;
                ant = NULL;
                Buscar(arvore, &pt, &ant, num, &f);
                switch (f) {
                    case -1:
                        printf("Ocorreu algum erro ao buscar o número\n");
                        break;
                    case 0:
                        printf("A árvore está vazia\n");
                        break;
                    case 1:
                        printf("O número foi encontrado\n");
                        break;
                    default:
                        printf("O número não foi encontrado na árvore\n");
                        break;
                }
                break;
            case 2:
                printf("Informe o número que vai ser inserido: \n");
                scanf("%d", &num);
                f = -1;
                pt = NULL;
                ant = NULL;
                Buscar(arvore, &pt, &ant, num, &f);
                if(f == -1){
                    printf("Ocorreu algum erro ao buscar o número\n");
                    break;
                }else{
                    Inserir(&arvore, pt, num, f);
                    break;
                }
            case 3:
                printf("Informe o número que vai ser removido: \n");
                scanf("%d", &num);
                f = -1;
                pt = NULL;
                ant = NULL;
                Buscar(arvore, &pt, &ant, num, &f);
                if(f == -1){
                    printf("Ocorreu algum erro ao buscar o número\n");
                    break;
                }

                if(f == 1){
                    Remover(&arvore, pt, ant);
                }else{
                    printf("O número não está na árvore\n");
                }
                break;
            case 4:
                break;
            default:
                printf("Opção inválida\n");
        }
        gfx_clear();
        Tela(0, 1200, 10, 0, 0, 0, arvore);
        gfx_paint();
        
    } while (resp != 4);
    gfx_quit();
    Desalocar(arvore);
    return 0;
}

/*
Busca uma chave na árvore, 
- Se a árvore for vazia então f recebe 0.
- Se a chave buscada for igual a chave de no então f recebe 1 e pt recebe no,
- Se a chave buscada for menor que a chave de no e no possuir um nó esquerdo então ant recebe no Buscar se chama recursivamente passando esse nó esquerdo,
- Se a chave buscada for menor que a chave de no e no não possuir um nó esquerdo então f recebe 2 e pt recebe no,
- Se a chave buscada for maior que a chave de no e no possuir um nó direito então ant recebe no Buscar se chama recursivamente passando esse nó direito,
- Se a chave buscada for maior que a chave de no e no não possuir um nó direito então f recebe 3 e pt recebe no.
*/
void Buscar(struct Arvore *no, struct Arvore **pt, struct Arvore **ant, int chave, int *f){
    if(no == NULL){
        *f = 0;
        return;
    }
    if(no->chave == chave){
        *pt = no;
        *f = 1;
        return;
    } else{
        if(no->chave > chave){
            if(no->no_esq != NULL){
                *ant = no;
                Buscar(no->no_esq, pt, ant, chave, f);
                return;
            }else{
                *pt = no;
                *f = 2;
                return;
            }
        }else{
            if(no->chave < chave){
                if(no->no_dir != NULL){
                    *ant = no;
                    Buscar(no->no_dir, pt, ant, chave, f);
                    return;
                }else{
                    *pt = no;
                    *f = 3;
                    return;
                }
            }
        }
    }
}

/*
Insere uma chave na árvore,
- Se f = 1 então a chave já está na árvore e não precisa ser inserida,
caso a chave não esteja na árvore ainda então se aloca ptr que vai receber a chave,
- Se f = 0 então ptr será o primeiro elemento da árvore,
- Se f = 2 então pt recebe ptr como seu nó esquerdo,
- Se f = 3 então pt recebe ptr como seu ná direito.
*/
void Inserir(struct Arvore **arvore, struct Arvore *pt, int chave, int f){
    if(f == 1){
        printf("A chave já está na árvore\n");
        return;
    }
    struct Arvore *ptr = malloc(sizeof (struct Arvore));
    ptr->chave = chave;
    ptr->no_esq = NULL;
    ptr->no_dir = NULL;
    switch (f) {
        case 0:
            *arvore = ptr;
            return;
        case 2:
            pt->no_esq = ptr;
            return;
        case 3:
            pt->no_dir = ptr;
            return;
    }
}

/*
Remove uma chave da árvore
- Se pt não possuir outros nós então ant recebe NULL no lugar de pt e desaloca pt;
- Se pt possuir apenas um nó então ant recebe esse nó no lugar de pt e desaloca pt;
- Se pt possuir dois nós e o nó direito não possuir nó esquerdo então ant recebe o nó direito no lugar de pt, o nó direito recebe o nó esquerdo e desaloca pt,
- Se pt possuir dois nós e o nó direito possuir nó esquerdo então se procura o sucessor de pt, ao encontrar ant recebe o sucessor no lugar de pt,
o nó anterior do sucessor recebe toda subárvore direita do sucessor como sua subárvore esquerda, o sucessor recebe tanto a subárvore esquerda quanto a direita de pt e desaloca pt.
*/
void Remover(struct Arvore **arvore, struct Arvore *pt, struct Arvore *ant){
    struct Arvore *aux_pt;
    struct Arvore *aux_ant;
    if(*arvore == pt){
        if((pt->no_esq == NULL) && (pt->no_dir == NULL)){
            *arvore = NULL;
            free(pt);
            return;
        }
        if((pt->no_esq != NULL) && (pt->no_dir == NULL)){
            *arvore = pt->no_esq;
            return;
        }
        if((pt->no_esq == NULL) && (pt->no_dir != NULL)){
            *arvore = pt->no_dir;
        }else{
            if((pt->no_esq != NULL) && (pt->no_dir != NULL)){
                aux_pt = pt->no_dir;
                aux_ant = pt;
                if(aux_pt->no_esq == NULL){
                    *arvore = aux_pt;
                    aux_pt->no_esq = pt->no_esq;
                    free(pt);
                    return;
                }else{
                    while(aux_pt->no_esq != NULL){
                        aux_ant = aux_pt;
                        aux_pt = aux_pt->no_esq;
                    }
                    *arvore = aux_pt;
                    aux_pt->no_esq = pt->no_esq;
                    aux_ant->no_esq = aux_pt->no_dir;
                    aux_pt->no_dir = pt->no_dir;
                    free(pt);
                    return;
                }
            }
        }
    }else {
        if ((pt->no_esq == NULL) && (pt->no_dir == NULL)) {
            if (ant->no_esq == pt) {
                ant->no_esq = NULL;
                free(pt);
                return;
            } else {
                ant->no_dir = NULL;
                free(pt);
                return;
            }
        }
        if ((pt->no_esq != NULL) && (pt->no_dir == NULL)) {
            if (ant->no_esq == pt) {
                ant->no_esq = pt->no_esq;
                free(pt);
                return;
            } else {
                ant->no_dir = pt->no_esq;
                free(pt);
                return;
            }
        }
        if ((pt->no_esq == NULL) && (pt->no_dir != NULL)) {
            if (ant->no_esq == pt) {
                ant->no_esq = pt->no_dir;
                free(pt);
                return;
            } else {
                ant->no_dir = pt->no_dir;
                free(pt);
                return;
            }
        }
        if ((pt->no_esq != NULL) && (pt->no_dir != NULL)) {
            aux_pt = pt->no_dir;
            aux_ant = pt;
            if (aux_pt->no_esq == NULL) {
                if (ant->no_esq == pt) {
                    ant->no_esq = aux_pt;
                    aux_pt->no_esq = pt->no_esq;
                    free(pt);
                    return;
                } else {
                    ant->no_dir = aux_pt;
                    aux_pt->no_esq = pt->no_esq;
                    free(pt);
                    return;
                }
            } else {
                while (aux_pt->no_esq != NULL) {
                    aux_ant = aux_pt;
                    aux_pt = aux_pt->no_esq;
                }
                if (ant->no_esq == pt) {
                    ant->no_esq = aux_pt;
                    aux_pt->no_esq = pt->no_esq;
                    aux_ant->no_esq = aux_pt->no_dir;
                    aux_pt->no_dir = pt->no_dir;
                    free(pt);
                    return;
                } else {
                    ant->no_dir = aux_pt;
                    aux_pt->no_esq = pt->no_esq;
                    aux_ant->no_esq = aux_pt->no_dir;
                    aux_pt->no_dir = pt->no_dir;
                    free(pt);
                    return;
                }
            }
        }
    }
}

//Desenha a árvore
void Tela(int iwidth, int width, int height, int x, int y, int b, struct Arvore *pt){
    char string[50];
    int textwid, texthei, wid;
    sprintf(string, "%d", pt->chave);
    wid = width-iwidth;
    gfx_get_text_size(string, &textwid, &texthei);
    gfx_set_color(255, 255, 255);
    gfx_text(iwidth+((wid - textwid)/2), height, string);
    gfx_set_color(0, 0, 255);
    gfx_ellipse(iwidth+(wid/2), height + (texthei/2), 15, 15);
    gfx_set_color(0, 0, b);
    gfx_line(x, y, iwidth+(wid/2), height + (texthei/2) - 15); 
    if(pt->no_esq != NULL){
        Tela(iwidth, wid/2 + iwidth, height+100, iwidth+(wid/2) - 10, height + (texthei/2) + 10, 255,  pt->no_esq);
    }
    if(pt->no_dir != NULL){
        Tela(iwidth + wid/2, width, height+100, iwidth+(wid/2) + 10, height + (texthei/2) + 10, 255, pt->no_dir);
    }
}

//Desaloca a árvore
void Desalocar(struct Arvore *no){
    if(no->no_esq != NULL){
        Desalocar(no->no_esq);
    }
    if(no->no_dir != NULL){
        Desalocar(no->no_dir);
    }
    if((no->no_dir == NULL) && (no->no_esq == NULL)){
        free(no);
        return;
    }
}
