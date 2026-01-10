#ifndef GESTAO_ESTOQUE_H
#define GESTAO_ESTOQUE_H

#define TIPO_TAM 16
#define NOME_TAM 32

#define PRODUTO_EDITAR 0
#define Produto_EXCLUIR 1

#define ERRO_NADA_ESCRITO -1
#define ERRO_CODIGO_EXISTENTE 0
#define SEM_ERRO 2
#define ERRO_PRODUTO_NAO_EXISTE -2
#define ERRO_QUANTIDADE_INSUFICIENTE -3
#define PRODUTO_VALIDO 1

struct Produto {
    int id;
    int Quantidade;
    char dia[3];
    char mes[3];
    char ano[5];
    float precoTotal;
    float preco;
    bool disponivel;
    char categoria[TIPO_TAM+1];
    char TipoUnidade[TIPO_TAM+1];
    char nome[NOME_TAM+1];
};

struct TabelaProduto {
    int qtd;
    Produto *dados;
};

void carregarProdutos(TabelaProduto &tabela);
void ArmazenarDados(TabelaProduto &tabela, Produto &f);
bool verificarIdExistente(TabelaProduto &tabela, int codigo);
int checarEstoque(int id, int quantidade, int &indice, TabelaProduto &tabela);
void removerEstoqueId(TabelaProduto &tabela, int id, int qtd);
void adicionarEstoqueId(TabelaProduto &tabela, int id, int qtd);
void efetuarCadastro(char **dados, TabelaProduto &tabela);               // Criado pelo módulo Interface
void efetuarExclusao(TabelaProduto &tabela, int codigo);            // Criado pelo módulo interface
void efetuarPesquisa(TabelaProduto &tabela, char *filtro);               // Criado pelo módulo interface
void efetuarEdicao(TabelaProduto &tabela, Produto &f, int indice);   // Criado pelo módulo interface
void efetuarEdicao(TabelaProduto &tabela);                              // Criado pelo módulo interface
int verificarCad(char **dados, TabelaProduto &tabela);                  // Criado pelo módulo interface
int verificarEdicaoP(char **p);                                        // Criado pelo módulo interface
void corrigirTabelaInterface(TabelaProduto &tabela, int codigo);         // Criado pelo módulo interface
void LiberarMemoria(TabelaProduto &tabela);

#endif // GESTAO_ESTOQUE_H
