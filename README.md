# 📚 Estrutura de Dados — Contagem/Plotagem (MATLAB), Huffman, SAT Solver e Seminário

**Repositório:** [Estrutura-de-dados-25.1](https://github.com/maisabb/Estrutura-de-dados-25.1.git)

Este repositório reúne projetos desenvolvidos na disciplina **Estrutura de Dados (2025.1)**, abordando temas fundamentais como contagem e análise de desempenho, compressão de dados (Huffman), resolução de fórmulas lógicas (SAT Solver) e apresentações teóricas em seminário.

---

## 🧩 Projetos Incluídos

1. **Contagem & Plotagem (MATLAB)** — Geração de dados com C e visualização com MATLAB.  
2. **Compressor Huffman** — Compactação e descompactação de arquivos com codificação binária.  
3. **SAT Solver (CNF/DIMACS)** — Resolução de fórmulas booleanas via backtracking.  
4. **Seminário** — Materiais de apoio, slides e roteiro de apresentação.

---

# 📊 Contagem e Plotagem (MATLAB)

## 🎯 Objetivo

Este projeto tem como objetivo **analisar o desempenho de estruturas de dados** (como listas encadeadas e árvores binárias de busca) medindo o **número de comparações** realizadas durante as operações e, em seguida, **visualizar graficamente os resultados** no **MATLAB**.

A abordagem envolve duas etapas principais:

1. **Geração de dados em C** — o programa `contagem.c` executa operações nas estruturas e grava os resultados (tempo, número de comparações, tamanho da entrada etc.) em um arquivo CSV.  
2. **Plotagem em MATLAB** — o script `plot.m` lê o arquivo gerado (`resultados.csv`) e exibe os gráficos de comparação.

---

## ⚙️ Como Rodar

### 🧱 1. Compilar o código em C

No terminal, acesse a pasta do projeto e compile o programa: gcc contagem.c -o contagem

# 🗜️ Compressor Huffman em C

## 🎯 Objetivo

Este projeto implementa o **algoritmo de compressão e descompressão Huffman**, um método clássico de **codificação sem perda**, que reduz o tamanho de arquivos ao atribuir **códigos binários menores a caracteres mais frequentes**.

O programa lê um arquivo de entrada, constrói a árvore de Huffman, gera a tabela de códigos e escreve um **arquivo compactado `.huff`**.  
Também é possível realizar o processo inverso (descompressão), reconstruindo o arquivo original.

---

# 🧩 SAT Solver (CNF/DIMACS) em C

## 🎯 Objetivo
Implementar um **resolvedor de SAT** (Satisfatibilidade Booleana) que:
- **Lê fórmulas** no formato **DIMACS CNF**;
- **Verifica** se a fórmula é **SAT** (satisfatível) ou **UNSAT** (insatisfatível);
- Em caso **SAT**, imprime uma **interpretação** para as variáveis.

---

# 🎤 Seminário — Árvore de Segmentos (Segment Tree) para Soma e Média

Este seminário demonstra **Árvore de Segmentos** aplicada ao problema de **consultas de soma em intervalos** e **atualizações pontuais** em um vetor de notas. O exemplo está 100% em **C** e usa índice base **0**.

---

## 🎯 Objetivo

- Construir uma estrutura que responda **somas em intervalos [l..r]** em **O(log n)**.  
- Permitir **atualizações de um elemento** (mudança de nota) também em **O(log n)**.  
- Comparar com o custo de fazer a soma “ingênua” (**O(r−l+1)** por consulta).

---

# ▶️ Como Rodar o Seminário — Árvore de Segmentos em C

- Abra o terminal na pasta e execute

## ⚙️ Passos para Compilar e Executar

### 1️⃣ Salve o código
Salve o código do seminário em um arquivo, por exemplo:

