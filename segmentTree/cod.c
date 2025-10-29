#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float *dados;  
    float *segmento; 
    int n;         
} ArvoreSegmentos;

ArvoreSegmentos* inicializar(float *valores, int n) {
    ArvoreSegmentos *a = malloc(sizeof(ArvoreSegmentos));
    a->n = n;
    a->dados = malloc(n * sizeof(float));
    a->segmento = malloc(4 * n * sizeof(float));

    for (int i = 0; i < n; i++)
        a->dados[i] = valores[i];

    return a;
}

void montar(ArvoreSegmentos *a, int indice, int inicio, int fim) {
    if (inicio == fim) {
        a->segmento[indice] = a->dados[inicio];
        return;
    }

    int meio = (inicio + fim) / 2;
    montar(a, 2 * indice + 1, inicio, meio);
    montar(a, 2 * indice + 2, meio + 1, fim);

    a->segmento[indice] = a->segmento[2 * indice + 1] + a->segmento[2 * indice + 2];
}

float soma_intervalo(ArvoreSegmentos *a, int indice, int inicio, int fim, int l, int r) {
    if (r < inicio || l > fim)
        return 0;
    if (l <= inicio && fim <= r)
        return a->segmento[indice];

    int meio = (inicio + fim) / 2;
    float esquerda = soma_intervalo(a, 2 * indice + 1, inicio, meio, l, r);
    float direita = soma_intervalo(a, 2 * indice + 2, meio + 1, fim, l, r);

    return esquerda + direita;
}

void modificar(ArvoreSegmentos *a, int indice, int inicio, int fim, int pos, float novo_valor) {
    if (inicio == fim) {
        a->segmento[indice] = novo_valor;
        a->dados[pos] = novo_valor;
        return;
    }

    int meio = (inicio + fim) / 2;
    if (pos <= meio)
        modificar(a, 2 * indice + 1, inicio, meio, pos, novo_valor);
    else
        modificar(a, 2 * indice + 2, meio + 1, fim, pos, novo_valor);

    a->segmento[indice] = a->segmento[2 * indice + 1] + a->segmento[2 * indice + 2];
}

int main() {
    
    float notas[] = {7.5, 8.0, 6.0, 9.0, 5.5, 8.5};
    int n = 6;

    ArvoreSegmentos *a = inicializar(notas, n);
    montar(a, 0, 0, n - 1);

    float soma = soma_intervalo(a, 0, 0, n - 1, 1, 3);
    printf("Soma das notas dos alunos 1 a 3: %.1f\n", soma);
    printf("Media das notas [1..3]: %.2f\n", soma / 3.0);

    modificar(a, 0, 0, n - 1, 2, 9.5);
    printf("\nApos atualizar a nota do aluno 3:\n");
    soma = soma_intervalo(a, 0, 0, n - 1, 1, 3);
    printf("Soma das notas [1..3]: %.1f\n", soma);
    printf("Media das notas [1..3]: %.2f\n", soma / 3.0);

    free(a->dados);
    free(a->segmento);
    free(a);


    return 0;
}
