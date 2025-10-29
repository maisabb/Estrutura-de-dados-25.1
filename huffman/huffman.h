#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "pqueue_heap.h"

#define BUFFER_SIZE 1024

// Conta a frequência de cada caractere e preenche duas filas de prioridade.
void create_huff_queue(FILE *input_file, PRIORITY_QUEUE** pq1, PRIORITY_QUEUE** pq2) {
    int freq[256] = {0};
    unsigned char c; 

    while (fread(&c, 1, 1, input_file) == 1) { 
        freq[c]++; 
    }

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) { 
            NODE* node = create_node(i, freq[i], NULL, NULL); 
            insert(*pq1, node); 
            insert(*pq2, node); 
        }
    }
}

// Constrói a árvore de Huffman a partir da fila de prioridade.
NODE* build_huffman_tree(PRIORITY_QUEUE* pq) {
    while (pq->size > 1) {
        NODE* left = remove_lower(pq);
        NODE* right = remove_lower(pq); 

        NODE* parent = create_node('\0', left->frequency + right->frequency, left, right);
        insert(pq, parent);
    }

    return remove_lower(pq);
}

// Struct para armazenar o código de Huffman e seu comprimento.
typedef struct {
    uint32_t code; 
    int length;
} HuffmanCode;

// Gera a tabela de códigos de Huffman percorrendo a árvore.
void create_huffman_table(NODE* root, uint32_t code, int depth, HuffmanCode huff_table[256]) {
    if (!root) return;

    if (!root->left && !root->right) {
        huff_table[root->character].code = code;
        huff_table[root->character].length = depth;
        return;
    }

    create_huffman_table(root->left, (code << 1), depth + 1, huff_table);
    create_huffman_table(root->right, (code << 1) | 1, depth + 1, huff_table);
}

// Calcula o total de bits necessários para o arquivo compactado.
int calculate_bits_trashed(PRIORITY_QUEUE* pq, HuffmanCode huff_table[256]) {
    int bit_amount = 0;
    while (pq->size > 0) {
        NODE* node = remove_lower(pq); 
        if (node && huff_table[node->character].length > 0) {
            bit_amount += node->frequency * huff_table[node->character].length;
        }
    }

    return bit_amount;
}

// Conta o número total de nós na árvore.
int count_tree_size(NODE* root) {
    if (!root) return 0;
    return count_tree_size(root->left) + count_tree_size(root->right) + 1;
}

// Verifica se um nó é uma folha.
int is_leaf(NODE* node) {
    return node && !node->left && !node->right;
} 

// Escreve a estrutura da árvore no arquivo de saída (formato pré-ordem).
void write_tree(NODE* root, FILE* output_file) {
    if (is_leaf(root)) {
        fputc('1', output_file); 

        if (root->character == '*' || root->character == '\\') {
            fputc('\\', output_file);
        }
        fputc(root->character, output_file);
    } else {
        fputc('0', output_file);
        write_tree(root->left, output_file);
        write_tree(root->right, output_file);
    }
}

// Escreve o cabeçalho do arquivo compactado (lixo, tamanho da árvore e a própria árvore).
void write_header(PRIORITY_QUEUE* pq, HuffmanCode huff_table[256], FILE *output_file, NODE* root) {

    int total_bits = calculate_bits_trashed(pq, huff_table);
    int trash = ((8 - (total_bits % 8)) % 8);
    int tree_size = count_tree_size(root);

    unsigned short header = (trash << 13) | tree_size;

    unsigned char byte1 = header >> 8;
    unsigned char byte2 = header & 0xFF;

    fwrite(&byte1, 1, 1, output_file);
    fwrite(&byte2, 1, 1, output_file);

    write_tree(root, output_file);
}

// Struct para acumular bits até formar um byte completo.
typedef struct {
    unsigned char byte;
    int bits_used;
} BitBuffer;

// Adiciona um único bit ao buffer.
void bit_buffer_add(BitBuffer *bit_buffer, int bit) {
    bit_buffer->byte <<= 1;
    bit_buffer->byte |= (bit & 1);
    bit_buffer->bits_used++;
}

// Escreve o byte acumulado no buffer para o arquivo.
void write_buffer(FILE *f, BitBuffer *bit_buffer) { 
    if (bit_buffer->bits_used == 0) return;

    unsigned char temp_byte = bit_buffer->byte << (8 - bit_buffer->bits_used);
    fwrite(&temp_byte, 1, 1, f);

    bit_buffer->byte = 0;   
    bit_buffer->bits_used = 0;
}

// Lê o arquivo original e o compacta bit a bit.
void compactor(FILE *input_file, FILE *output_file, HuffmanCode huff_table[256]) {
    unsigned char c;
    BitBuffer bit_buffer = {0, 0}; 

    while (fread(&c, 1, 1, input_file) == 1) {
        HuffmanCode code = huff_table[c];

        for (int i = code.length - 1; i >= 0; i--) {
            bit_buffer_add(&bit_buffer, (code.code >> i) & 1);

            if (bit_buffer.bits_used == 8) { 
                write_buffer(output_file, &bit_buffer);
            }
        }
    }

    write_buffer(output_file, &bit_buffer);
}

// Libera toda a memória alocada para a árvore de Huffman.
void free_huffman_tree(NODE* root) {
    if (root == NULL) return;

    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

// Imprime a tabela de Huffman para depuração.
void print_huff_table(HuffmanCode huff_table[256]) {
    for (int i = 0; i < 256; i++) {
        if (huff_table[i].length > 0) {
            printf("'%c' (%d): Code: ", i, i);
            for (int j = huff_table[i].length - 1; j >= 0; j--) {
                printf("%d", (huff_table[i].code >> j) & 1);
            }
            printf(", Length: %d\n", huff_table[i].length);
        }
    }
}

// Imprime a árvore de Huffman para depuração.
void print_huffman_tree(NODE* root, int level) {
    if (!root) return;

    print_huffman_tree(root->right, level + 1);

    for (int i = 0; i < level; i++) {
        printf("    ");
    }

    if (is_leaf(root)) {
        if (root->character >= 32 && root->character <= 126) {
            printf("'%c' (%d)\n", root->character, root->frequency);
        } else {
            printf("0x%02X (%d)\n", root->character, root->frequency);
        }
    } else {
        printf("* (%d)\n", root->frequency);
    }

    print_huffman_tree(root->left, level + 1);
}

/*
    FUNÇÕES PARA DESCOMPACTAR O ARQUIVO
*/

// Lê os dois bytes de cabeçalho e extrai o lixo e o tamanho da árvore.
void read_header(FILE *file, int *trash, int *tree_size) {
    unsigned char byte1, byte2;
    fread(&byte1, 1, 1, file);
    fread(&byte2, 1, 1, file);

    unsigned short header = (byte1 << 8) | byte2;

    *trash = header >> 13;
    *tree_size = header & 0x1FFF;
}

// Lê a representação da árvore do arquivo e a reconstrói.
NODE* read_tree(FILE *file, int *bytes_read) { 
    int c = fgetc(file);
    (*bytes_read)++;

    if (c == '1') {
        int next = fgetc(file);
        (*bytes_read)++;

        if (next == '\\') {
            next = fgetc(file);
            (*bytes_read)++;
        }

        return create_node((unsigned char)next, 0, NULL, NULL);

    } else if (c == '0') {
        NODE *left = read_tree(file, bytes_read);
        NODE *right = read_tree(file, bytes_read);
        return create_node('\0', 0, left, right);
    }

    return NULL;
}

// Decodifica os bits do arquivo compactado usando a árvore e escreve o resultado.
void decompress(FILE *input, FILE *output, NODE* root, int trash_size, int header_bytes) {
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    long data_size = file_size - header_bytes;
    fseek(input, header_bytes, SEEK_SET);

    NODE* current = root;
    unsigned char byte;
    for (long i = 0; i < data_size; i++) {
        fread(&byte, 1, 1, input);

        for (int bit = 7; bit >= 0; bit--) {
            int current_bit = (byte >> bit) & 1;
            if (current_bit == 0)
                current = current->left;
            else
                current = current->right;

            if (is_leaf(current)) {
                fputc(current->character, output);
                current = root;
            }

            if (i == data_size - 1 && bit == trash_size - 1) {
                return;
            }
        }
    }
}

// Função principal de descompactação.
void decompact(const char* compressed_filename, char final_format[]) {
    FILE *input_file = fopen(compressed_filename, "rb"); 
    if (!input_file) {
        perror("Erro ao abrir o arquivo compactado");
        return;
    }

    char output_filename[BUFFER_SIZE]; 
    strcpy(output_filename, compressed_filename); 
    char* dot = strrchr(output_filename, '.');  
    if (dot) *dot = '\0';  

   snprintf(output_filename, sizeof(output_filename), "%.*s_descompactado.%s", 
        (int)(sizeof(output_filename) - strlen("_descompactado.") - strlen(final_format) - 1), 
        output_filename, 
        final_format);

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Erro ao criar arquivo de saída");
        fclose(input_file);
        return;
    }

    int trash_size = 0, tree_size = 0, bytes_read = 0;
    read_header(input_file, &trash_size, &tree_size);
    NODE* root = read_tree(input_file, &bytes_read);
    decompress(input_file, output_file, root, trash_size, 2 + bytes_read);

    printf("Arquivo descompactado com sucesso: %s\n", output_filename);

    free_huffman_tree(root);
    fclose(input_file);
    fclose(output_file);
}

#endif // HUFFMAN_H