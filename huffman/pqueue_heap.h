#ifndef PQUEUE_HEAP_H
#define PQUEUE_HEAP_H

#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP 256

typedef struct NODE {
    unsigned char character;
    int frequency;
    struct NODE *left, *right;
} NODE;

typedef struct {
    NODE* data[MAX_HEAP];
    int size;
} PRIORITY_QUEUE;

/*
    Create a new node assining the given parameters
*/
NODE* create_node(unsigned char c, int freq, NODE* left, NODE* right) {
    NODE* NODE = malloc(sizeof(*NODE));
    NODE->character = c;
    NODE->frequency = freq;
    NODE->left = left;
    NODE->right = right;
    return NODE;
}


/*
    Create a new priority queue
*/
PRIORITY_QUEUE* create_queue() {
    PRIORITY_QUEUE* pq = malloc(sizeof(PRIORITY_QUEUE));
    pq->size = 0;
    return pq;
}

// Adicione isso antes de qualquer uso da função
void free_huffman_tree(NODE* root);

int is_empty(PRIORITY_QUEUE* pq) {
    return pq->size == 0;
}



/*
    Troca dois nós
*/
void swap(NODE** a, NODE** b) {
    NODE* temp = *a;
    *a = *b;
    *b = temp;
}


/*
    Reorganiza a heap quando um novo no é adicionado 
*/
void heapify_up(PRIORITY_QUEUE* pq, int idx) {
    int dad = (idx - 1) / 2;
    if (idx > 0 && pq->data[idx]->frequency < pq->data[dad]->frequency) {
        swap(&pq->data[idx], &pq->data[dad]);
        heapify_up(pq, dad);
    }
}

/*
    Reorganiza a heap quando um no é removido 
*/

void heapify_down(PRIORITY_QUEUE* pq, int idx) {
    int lower = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < pq->size && pq->data[left]->frequency < pq->data[lower]->frequency)
        lower = left;
    if (right < pq->size && pq->data[right]->frequency < pq->data[lower]->frequency)
        lower = right;

    if (lower != idx) {
        swap(&pq->data[idx], &pq->data[lower]);
        heapify_down(pq, lower);
    }
}

void insert(PRIORITY_QUEUE* pq, NODE* node) {
    if (pq->size >= MAX_HEAP) {
        fprintf(stderr, "Error: Priority queue is full. Cannot insert new element.\n");
        return;
    }
    pq->data[pq->size] = node; // Armazena o nó diretamente
    heapify_up(pq, pq->size);
    pq->size++;
}

NODE* remove_lower(PRIORITY_QUEUE* pq) {
    if (pq->size == 0) return NULL;

    NODE* min = pq->data[0];
    pq->size--;
    pq->data[0] = pq->data[pq->size];
    heapify_down(pq, 0);
    return min;
}


void free_priority_queue(PRIORITY_QUEUE* pq) {
    if (pq == NULL) return;

    for (int i = 0; i < pq->size; i++) {
        free_huffman_tree(pq->data[i]); // Libera cada nó na fila
    }
    free(pq); // FLibera a própria fila de prioridade
}


#endif // PQUEUE_HEAP_H

