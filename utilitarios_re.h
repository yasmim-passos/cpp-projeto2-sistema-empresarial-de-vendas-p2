#ifndef UTILITARIOS_RE_H
#define UTILITARIOS_RE_H

#include <cstdio>
#include <string>
#include <unordered_map>
#include <fstream>
#include "gestao_estoque.h"

using namespace std;

struct mercadorias
{
    int codigo;                    // Código do Produto
    char nome[33];                 // Nome do Produto
    char categoria[17];            // Categoria
    int qtd_anterior;              // Quantidade anterior
    int qtd_atual;                 // Quantidade atual
    float preco_custo;             // Preço unitário
    float valor_total;             // Valor total
    string status_reabastecimento; // Status de reabastecimento
    string data_cadastro;          // Data de cadastro do produto
};

struct Relatorio
{
    string data;               // Data do relat�rio
    string hora;               // Hora do relat�rio
    string responsavel;        // Respons�vel pelo relat�rio
    string localizacao;        // Localiza��o do estoque
    string fonte_dados;        // Fonte dos dados
    mercadorias *objetos;      // Array din�mico de produtos
    int total_produtos;        // Contador de produtos armazenados
    int total_movimentacoes;   // Contador de movimenta��es armazenadas
    float valor_total_estoque; // Valor total do estoque

    // Construtor: inicializa os ponteiros e vari�veis
    Relatorio()
        : objetos(NULL),
          total_produtos(0),
          total_movimentacoes(0),
          valor_total_estoque(0.0) {}

    // Destrutor: libera mem�ria dos arrays din�micos
    ~Relatorio()
    {
        if (objetos)
        {
            delete[] objetos;
            objetos = NULL;
        }
    }
};

extern unordered_map<int, int> dados_anteriores;

// Fun��o para calcular o valor total de um produto
float calcular_valor_total(const mercadorias *produtos, int total);

// Fun��o para determinar o status de reabastecimento
string determinar_status_reabastecimento(int qtd_atual, int estoque_minimo, int estoque_normal);

void carregarDadosAnteriores(const char *nomeArquivo);
void salvarDadosAnteriores(const char *nomeArquivo);
void verificarQtdAnterior(mercadorias &objeto);

#endif // UTILITARIOS_RE_H
