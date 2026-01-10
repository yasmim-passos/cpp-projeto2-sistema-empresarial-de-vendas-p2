#include <iostream>
#include "utilitarios_re.h"

using namespace std;

// Funcao para calcular o valor total de um produto
float calcular_valor_total(const mercadorias *produtos, int total)
{
    if (total == 0)
        return 0.0;
    return (produtos->qtd_atual * produtos->preco_custo) + calcular_valor_total(produtos + 1, total - 1); // Chamada recursiva
}

// Funcao para determinar o status de reabastecimento
string determinar_status_reabastecimento(int qtd_atual, int estoque_minimo, int estoque_normal)
{
    if (qtd_atual <= estoque_minimo)
    {
        return "RC";
    }
    else if (qtd_atual <= estoque_normal)
    {
        return "RN";
    }
    else
    {
        return "NR";
    }
}

// Mapa global para armazenar os dados anteriores de id e quantidade
// Mapa para armazenar os dados anteriores
std::unordered_map<int, int> dados_anteriores;

// Função para carregar os dados anteriores
void carregarDadosAnteriores(const char *nomeArquivo)
{
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo)
    {
        //cout << "Arquivo de dados anteriores não encontrado. Criando novo arquivo...\n";
        return;
    }

    int id, quantidade;
    while (arquivo.read(reinterpret_cast<char *>(&id), sizeof(int)))
    {
        arquivo.read(reinterpret_cast<char *>(&quantidade), sizeof(int));
        dados_anteriores[id] = quantidade;
    }

    arquivo.close();
    //cout << "Dados anteriores carregados com sucesso!\n";
}

// Função para salvar os dados anteriores
void salvarDadosAnteriores(const char *nomeArquivo)
{
    ofstream arquivo(nomeArquivo, ios::binary | ios::trunc);
    if (!arquivo)
    {
        //cerr << "Erro ao abrir o arquivo de dados anteriores para escrita.\n";
        //return;
        exit(1);
    }

    for (const auto &par : dados_anteriores)
    {
        //cout << "Código (ID): " << par.first << ", Quantidade: " << par.second << endl;
        arquivo.write(reinterpret_cast<const char *>(&par.first), sizeof(int));
        arquivo.write(reinterpret_cast<const char *>(&par.second), sizeof(int));
    }

    arquivo.close();
}

// Função para verificar e atualizar a quantidade anterior
void verificarQtdAnterior(mercadorias &objeto)
{
    if (dados_anteriores.find(objeto.codigo) != dados_anteriores.end())
    {
        objeto.qtd_anterior = dados_anteriores[objeto.codigo];
        //cout << "Dados anteriores encontrados para o código " << objeto.codigo
          //   << ": Quantidade Anterior = " << objeto.qtd_anterior << endl;
    }
    else
    {
        dados_anteriores[objeto.codigo] = objeto.qtd_atual;
        objeto.qtd_anterior = objeto.qtd_atual; // Atualiza a quantidade inicial corretamente
        //cout << "Dados anteriores não encontrados para o código " << objeto.codigo
        //     << ". Definindo quantidade anterior como quantidade atual.\n";
    }
}
