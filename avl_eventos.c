/*
 * Sistema de Gerenciamento de Eventos Críticos - Cidade Inteligente
 * Implementação com Árvore AVL
 * Linguagem: C
 */

/* Comparação de strings ignorando maiúsculas/minúsculas (portável) */
static int strIgnoreCase(const char *a, const char *b) {
    while (*a && *b) {
        char ca = (*a >= 'A' && *a <= 'Z') ? *a + 32 : *a;
        char cb = (*b >= 'A' && *b <= 'Z') ? *b + 32 : *b;
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===================== DEFINIÇÕES E ENUMS ===================== */

/* Tipos de evento possíveis */
typedef enum {
    ACIDENTE_TRANSITO = 1,
    FALHA_SEMAFORO,
    INTERRUPCAO_ENERGIA,
    ALAGAMENTO,
    INCENDIO
} TipoEvento;

/* Status do evento */
typedef enum {
    ATIVO = 1,
    RESOLVIDO
} StatusEvento;

/* Estrutura de data e hora */
typedef struct {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
    int segundo;
} DataHora;

/* ===================== ESTRUTURA DO NÓ DA AVL ===================== */

typedef struct NoAVL {
    int id;                    /* Chave primária */
    TipoEvento tipo;           /* Tipo do evento */
    int severidade;            /* 1 a 5 */
    DataHora timestamp;        /* Data e hora do registro */
    char regiao[50];           /* Região da cidade */
    StatusEvento status;       /* Ativo ou Resolvido */
    int altura;                /* Altura do nó na árvore */
    struct NoAVL *esq;         /* Filho esquerdo */
    struct NoAVL *dir;         /* Filho direito */
} NoAVL;

/* ===================== VARIÁVEIS GLOBAIS ===================== */

int totalRotacoes = 0;         /* Contador de rotações realizadas */
int proximoID = 1;             /* Gerador automático de IDs */

/* ===================== FUNÇÕES AUXILIARES ===================== */

/* Retorna a string do tipo de evento */
const char* tipoEventoStr(TipoEvento tipo) {
    switch (tipo) {
        case ACIDENTE_TRANSITO:   return "Acidente de Trânsito";
        case FALHA_SEMAFORO:      return "Falha em Semáforo";
        case INTERRUPCAO_ENERGIA: return "Interrupção de Energia";
        case ALAGAMENTO:          return "Alagamento";
        case INCENDIO:            return "Incêndio";
        default:                  return "Desconhecido";
    }
}

/* Retorna a string do status */
const char* statusStr(StatusEvento status) {
    return (status == ATIVO) ? "Ativo" : "Resolvido";
}

/* Retorna a altura de um nó (nulo = -1) */
int altura(NoAVL *no) {
    return (no == NULL) ? -1 : no->altura;
}

/* Retorna o maior entre dois inteiros */
int maximo(int a, int b) {
    return (a > b) ? a : b;
}

/* Atualiza a altura de um nó com base nos filhos */
void atualizarAltura(NoAVL *no) {
    if (no != NULL)
        no->altura = 1 + maximo(altura(no->esq), altura(no->dir));
}

/* Calcula o fator de balanceamento de um nó */
int fatorBalanceamento(NoAVL *no) {
    if (no == NULL) return 0;
    return altura(no->esq) - altura(no->dir);
}

/* Exibe os dados de um evento */
void exibirEvento(NoAVL *no) {
    if (no == NULL) return;
    printf("  ┌─────────────────────────────────────────┐\n");
    printf("  │ ID: %-5d  Severidade: %d/5              │\n", no->id, no->severidade);
    printf("  │ Tipo: %-35s│\n", tipoEventoStr(no->tipo));
    printf("  │ Região: %-33s│\n", no->regiao);
    printf("  │ Status: %-33s│\n", statusStr(no->status));
    printf("  │ Data: %02d/%02d/%04d %02d:%02d:%02d              │\n",
           no->timestamp.dia, no->timestamp.mes, no->timestamp.ano,
           no->timestamp.hora, no->timestamp.minuto, no->timestamp.segundo);
    printf("  │ Altura na AVL: %-26d│\n", no->altura);
    printf("  │ Fator de Balanceamento: %-17d│\n", fatorBalanceamento(no));
    printf("  └─────────────────────────────────────────┘\n");
}

/* ===================== ROTAÇÕES DA AVL ===================== */

/* Rotação simples à direita */
NoAVL* rotacaoDireita(NoAVL *y) {
    NoAVL *x = y->esq;
    NoAVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    atualizarAltura(y);
    atualizarAltura(x);

    totalRotacoes++;
    return x;
}

/* Rotação simples à esquerda */
NoAVL* rotacaoEsquerda(NoAVL *x) {
    NoAVL *y = x->dir;
    NoAVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    atualizarAltura(x);
    atualizarAltura(y);

    totalRotacoes++;
    return y;
}

/* Balanceia o nó após inserção ou remoção */
NoAVL* balancear(NoAVL *no) {
    atualizarAltura(no);
    int fb = fatorBalanceamento(no);

    /* Caso Esquerda-Esquerda: rotação simples direita */
    if (fb > 1 && fatorBalanceamento(no->esq) >= 0)
        return rotacaoDireita(no);

    /* Caso Esquerda-Direita: rotação dupla */
    if (fb > 1 && fatorBalanceamento(no->esq) < 0) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }

    /* Caso Direita-Direita: rotação simples esquerda */
    if (fb < -1 && fatorBalanceamento(no->dir) <= 0)
        return rotacaoEsquerda(no);

    /* Caso Direita-Esquerda: rotação dupla */
    if (fb < -1 && fatorBalanceamento(no->dir) > 0) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;
}

/* ===================== OPERAÇÕES BÁSICAS DA AVL ===================== */

/* Cria um novo nó com os dados fornecidos */
NoAVL* criarNo(int id, TipoEvento tipo, int severidade,
                DataHora ts, const char *regiao) {
    NoAVL *novo = (NoAVL*) malloc(sizeof(NoAVL));
    if (novo == NULL) {
        printf("Erro: falha na alocação de memória!\n");
        return NULL;
    }
    novo->id = id;
    novo->tipo = tipo;
    novo->severidade = severidade;
    novo->timestamp = ts;
    strncpy(novo->regiao, regiao, 49);
    novo->regiao[49] = '\0';
    novo->status = ATIVO;
    novo->altura = 0;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

/* Insere um evento na AVL */
NoAVL* inserir(NoAVL *raiz, NoAVL *novo) {
    if (raiz == NULL) return novo;

    if (novo->id < raiz->id)
        raiz->esq = inserir(raiz->esq, novo);
    else if (novo->id > raiz->id)
        raiz->dir = inserir(raiz->dir, novo);
    else {
        printf("  [!] ID %d já existe na árvore.\n", novo->id);
        free(novo);
        return raiz;
    }

    return balancear(raiz);
}

/* Busca o nó de menor ID (mais à esquerda) */
NoAVL* menorNo(NoAVL *no) {
    while (no->esq != NULL) no = no->esq;
    return no;
}

/* Remove um evento pelo ID (somente se Resolvido) */
NoAVL* remover(NoAVL *raiz, int id, int *removido) {
    if (raiz == NULL) {
        printf("  [!] Evento ID %d não encontrado.\n", id);
        return NULL;
    }

    if (id < raiz->id)
        raiz->esq = remover(raiz->esq, id, removido);
    else if (id > raiz->id)
        raiz->dir = remover(raiz->dir, id, removido);
    else {
        /* Nó encontrado */
        if (raiz->status == ATIVO) {
            printf("  [!] Evento ID %d está ATIVO. Resolva antes de remover.\n", id);
            *removido = 0;
            return raiz;
        }

        *removido = 1;

        /* Nó com 0 ou 1 filho */
        if (raiz->esq == NULL || raiz->dir == NULL) {
            NoAVL *temp = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            free(raiz);
            return temp;
        }

        /* Nó com 2 filhos: substitui pelo sucessor em-ordem */
        NoAVL *sucessor = menorNo(raiz->dir);
        raiz->id        = sucessor->id;
        raiz->tipo      = sucessor->tipo;
        raiz->severidade= sucessor->severidade;
        raiz->timestamp = sucessor->timestamp;
        strncpy(raiz->regiao, sucessor->regiao, 49);
        raiz->status    = sucessor->status;

        int aux = 0;
        raiz->dir = remover(raiz->dir, sucessor->id, &aux);
    }

    return balancear(raiz);
}

/* Busca um evento por ID */
NoAVL* buscarPorID(NoAVL *raiz, int id) {
    if (raiz == NULL) return NULL;
    if (id == raiz->id) return raiz;
    if (id < raiz->id)  return buscarPorID(raiz->esq, id);
    return buscarPorID(raiz->dir, id);
}

/* ===================== CONSULTAS AVANÇADAS ===================== */

/* Lista eventos ativos por intervalo de severidade */
void listarPorSeveridade(NoAVL *raiz, int minSev, int maxSev, int *encontrou) {
    if (raiz == NULL) return;
    listarPorSeveridade(raiz->esq, minSev, maxSev, encontrou);
    if (raiz->status == ATIVO &&
        raiz->severidade >= minSev &&
        raiz->severidade <= maxSev) {
        exibirEvento(raiz);
        *encontrou = 1;
    }
    listarPorSeveridade(raiz->dir, minSev, maxSev, encontrou);
}

/* Lista eventos ativos de uma região (em-ordem por ID) */
void relatorioPorRegiao(NoAVL *raiz, const char *regiao, int *encontrou) {
    if (raiz == NULL) return;
    relatorioPorRegiao(raiz->esq, regiao, encontrou);
    if (raiz->status == ATIVO &&
        strIgnoreCase(raiz->regiao, regiao) == 0) {
        exibirEvento(raiz);
        *encontrou = 1;
    }
    relatorioPorRegiao(raiz->dir, regiao, encontrou);
}

/* Lista eventos por intervalo de ID */
void buscarIntervaloID(NoAVL *raiz, int idMin, int idMax, int *encontrou) {
    if (raiz == NULL) return;
    if (raiz->id > idMin)
        buscarIntervaloID(raiz->esq, idMin, idMax, encontrou);
    if (raiz->id >= idMin && raiz->id <= idMax) {
        exibirEvento(raiz);
        *encontrou = 1;
    }
    if (raiz->id < idMax)
        buscarIntervaloID(raiz->dir, idMin, idMax, encontrou);
}

/* ===================== MÉTRICAS ===================== */

/* Conta nós totais */
int contarNos(NoAVL *raiz) {
    if (raiz == NULL) return 0;
    return 1 + contarNos(raiz->esq) + contarNos(raiz->dir);
}

/* Conta eventos ativos */
int contarAtivos(NoAVL *raiz) {
    if (raiz == NULL) return 0;
    int ativo = (raiz->status == ATIVO) ? 1 : 0;
    return ativo + contarAtivos(raiz->esq) + contarAtivos(raiz->dir);
}

/* Soma dos fatores de balanceamento (para calcular média) */
void somaFatorBalanceamento(NoAVL *raiz, float *soma, int *count) {
    if (raiz == NULL) return;
    int fb = fatorBalanceamento(raiz);
    *soma += (fb < 0) ? -fb : fb; /* Valor absoluto */
    (*count)++;
    somaFatorBalanceamento(raiz->esq, soma, count);
    somaFatorBalanceamento(raiz->dir, soma, count);
}

/* Exibe métricas da árvore */
void exibirMetricas(NoAVL *raiz) {
    int total  = contarNos(raiz);
    int ativos = contarAtivos(raiz);
    int alt    = altura(raiz);
    float somaFB = 0.0f;
    int countFB  = 0;
    somaFatorBalanceamento(raiz, &somaFB, &countFB);
    float mediaFB = (countFB > 0) ? somaFB / countFB : 0.0f;

    printf("\n  ╔══════════════════════════════════════╗\n");
    printf("  ║         MÉTRICAS DA ÁRVORE AVL       ║\n");
    printf("  ╠══════════════════════════════════════╣\n");
    printf("  ║ Altura total            : %-11d║\n", alt);
    printf("  ║ Total de nós            : %-11d║\n", total);
    printf("  ║ Eventos ativos          : %-11d║\n", ativos);
    printf("  ║ Eventos resolvidos      : %-11d║\n", total - ativos);
    printf("  ║ FB médio (|valor|)      : %-11.2f║\n", mediaFB);
    printf("  ║ Total de rotações       : %-11d║\n", totalRotacoes);
    printf("  ╚══════════════════════════════════════╝\n\n");
}

/* ===================== PERCURSO EM-ORDEM ===================== */

void emOrdem(NoAVL *raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    exibirEvento(raiz);
    emOrdem(raiz->dir);
}

/* ===================== LIBERAÇÃO DE MEMÓRIA ===================== */

void liberarArvore(NoAVL *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

/* ===================== FUNÇÕES DE ENTRADA ===================== */

/* Lê e valida um inteiro dentro de um intervalo */
int lerInteiro(const char *prompt, int min, int max) {
    int val;
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin)) {
            if (sscanf(buf, "%d", &val) == 1 && val >= min && val <= max)
                return val;
        }
        printf("  [!] Valor inválido. Digite um número entre %d e %d.\n", min, max);
    }
}

/* Lê uma string não-vazia */
void lerString(const char *prompt, char *dest, int maxLen) {
    while (1) {
        printf("%s", prompt);
        if (fgets(dest, maxLen, stdin)) {
            dest[strcspn(dest, "\n")] = '\0';
            if (strlen(dest) > 0) return;
        }
        printf("  [!] Entrada inválida. Tente novamente.\n");
    }
}

/* Obtém data/hora atual do sistema */
DataHora obterDataHoraAtual() {
    DataHora dt;
    /* Solicita ao usuário pois ambientes sem time.h podem variar */
    printf("  Data de registro (dd mm aaaa hh mm ss): ");
    char buf[80];
    fgets(buf, sizeof(buf), stdin);
    if (sscanf(buf, "%d %d %d %d %d %d",
               &dt.dia, &dt.mes, &dt.ano,
               &dt.hora, &dt.minuto, &dt.segundo) != 6) {
        dt.dia = 1; dt.mes = 1; dt.ano = 2025;
        dt.hora = 0; dt.minuto = 0; dt.segundo = 0;
    }
    return dt;
}

/* ===================== MENU E SUBMENUS ===================== */

void cabecalho() {
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║   SISTEMA DE EVENTOS CRÍTICOS - CIDADE       ║\n");
    printf("║              INTELIGENTE  v1.0               ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
}

void menuCadastros(NoAVL **raiz) {
    int op;
    do {
        printf("\n  ┌─── CADASTROS ───────────────────────────┐\n");
        printf("  │ 1. Registrar novo evento                │\n");
        printf("  │ 2. Remover evento (somente Resolvido)   │\n");
        printf("  │ 0. Voltar                               │\n");
        printf("  └─────────────────────────────────────────┘\n");
        op = lerInteiro("  Opção: ", 0, 2);

        if (op == 1) {
            printf("\n  === NOVO EVENTO ===\n");
            printf("  Tipos: 1-Acidente  2-Semáforo  3-Energia  4-Alagamento  5-Incêndio\n");
            int tipo  = lerInteiro("  Tipo do evento: ", 1, 5);
            int sev   = lerInteiro("  Severidade (1-5): ", 1, 5);
            char reg[50];
            lerString("  Região: ", reg, 50);
            DataHora ts = obterDataHoraAtual();

            int id = proximoID++;
            NoAVL *novo = criarNo(id, (TipoEvento)tipo, sev, ts, reg);
            if (novo) {
                *raiz = inserir(*raiz, novo);
                printf("  [✓] Evento registrado com ID %d.\n", id);
            }

        } else if (op == 2) {
            int id = lerInteiro("  ID do evento a remover: ", 1, 999999);
            int removido = 0;
            *raiz = remover(*raiz, id, &removido);
            if (removido)
                printf("  [✓] Evento ID %d removido com sucesso.\n", id);
        }
    } while (op != 0);
}

void menuConsultas(NoAVL *raiz) {
    int op;
    do {
        printf("\n  ┌─── CONSULTAS ───────────────────────────┐\n");
        printf("  │ 1. Buscar evento por ID                 │\n");
        printf("  │ 2. Listar por intervalo de severidade   │\n");
        printf("  │ 3. Relatório por região                 │\n");
        printf("  │ 4. Busca por intervalo de ID            │\n");
        printf("  │ 5. Listar todos (em-ordem por ID)       │\n");
        printf("  │ 0. Voltar                               │\n");
        printf("  └─────────────────────────────────────────┘\n");
        op = lerInteiro("  Opção: ", 0, 5);

        if (op == 1) {
            int id = lerInteiro("  ID do evento: ", 1, 999999);
            NoAVL *no = buscarPorID(raiz, id);
            if (no) exibirEvento(no);
            else printf("  [!] Evento ID %d não encontrado.\n", id);

        } else if (op == 2) {
            int minS = lerInteiro("  Severidade mínima (1-5): ", 1, 5);
            int maxS = lerInteiro("  Severidade máxima (1-5): ", minS, 5);
            int enc = 0;
            printf("\n  Eventos ativos com severidade entre %d e %d:\n", minS, maxS);
            listarPorSeveridade(raiz, minS, maxS, &enc);
            if (!enc) printf("  Nenhum evento encontrado.\n");

        } else if (op == 3) {
            char reg[50];
            lerString("  Região: ", reg, 50);
            int enc = 0;
            printf("\n  Eventos ativos na região \"%s\":\n", reg);
            relatorioPorRegiao(raiz, reg, &enc);
            if (!enc) printf("  Nenhum evento encontrado.\n");

        } else if (op == 4) {
            int idMin = lerInteiro("  ID mínimo: ", 1, 999999);
            int idMax = lerInteiro("  ID máximo: ", idMin, 999999);
            int enc = 0;
            printf("\n  Eventos com ID entre %d e %d:\n", idMin, idMax);
            buscarIntervaloID(raiz, idMin, idMax, &enc);
            if (!enc) printf("  Nenhum evento encontrado.\n");

        } else if (op == 5) {
            printf("\n  === TODOS OS EVENTOS (em-ordem por ID) ===\n");
            if (raiz == NULL) printf("  Árvore vazia.\n");
            else emOrdem(raiz);
        }
    } while (op != 0);
}

void menuAtualizacoes(NoAVL *raiz) {
    int op;
    do {
        printf("\n  ┌─── ATUALIZAÇÕES ────────────────────────┐\n");
        printf("  │ 1. Marcar evento como Resolvido         │\n");
        printf("  │ 2. Atualizar severidade de evento ativo │\n");
        printf("  │ 0. Voltar                               │\n");
        printf("  └─────────────────────────────────────────┘\n");
        op = lerInteiro("  Opção: ", 0, 2);

        if (op == 1) {
            int id = lerInteiro("  ID do evento: ", 1, 999999);
            NoAVL *no = buscarPorID(raiz, id);
            if (no == NULL)
                printf("  [!] Evento não encontrado.\n");
            else if (no->status == RESOLVIDO)
                printf("  [!] Evento já está Resolvido.\n");
            else {
                no->status = RESOLVIDO;
                printf("  [✓] Evento ID %d marcado como Resolvido.\n", id);
            }

        } else if (op == 2) {
            int id = lerInteiro("  ID do evento ativo: ", 1, 999999);
            NoAVL *no = buscarPorID(raiz, id);
            if (no == NULL)
                printf("  [!] Evento não encontrado.\n");
            else if (no->status == RESOLVIDO)
                printf("  [!] Evento está Resolvido. Só é possível alterar eventos ativos.\n");
            else {
                int novaSev = lerInteiro("  Nova severidade (1-5): ", 1, 5);
                no->severidade = novaSev;
                printf("  [✓] Severidade do evento ID %d atualizada para %d.\n", id, novaSev);
            }
        }
    } while (op != 0);
}

/* Percurso em-ordem listando apenas eventos resolvidos */
void emOrdemResolvidos(NoAVL *raiz) {
    if (raiz == NULL) return;
    emOrdemResolvidos(raiz->esq);
    if (raiz->status == RESOLVIDO) exibirEvento(raiz);
    emOrdemResolvidos(raiz->dir);
}

void menuRelatorios(NoAVL *raiz) {
    int op;
    do {
        printf("\n  ┌─── RELATÓRIOS E MÉTRICAS ───────────────┐\n");
        printf("  │ 1. Métricas da Árvore AVL               │\n");
        printf("  │ 2. Todos os eventos ativos              │\n");
        printf("  │ 3. Todos os eventos resolvidos          │\n");
        printf("  │ 0. Voltar                               │\n");
        printf("  └─────────────────────────────────────────┘\n");
        op = lerInteiro("  Opção: ", 0, 3);

        if (op == 1) {
            exibirMetricas(raiz);

        } else if (op == 2) {
            printf("\n  === EVENTOS ATIVOS ===\n");
            int enc = 0;
            listarPorSeveridade(raiz, 1, 5, &enc);
            if (!enc) printf("  Nenhum evento ativo.\n");

        } else if (op == 3) {
            printf("\n  === EVENTOS RESOLVIDOS ===\n");
            emOrdemResolvidos(raiz);
        }
    } while (op != 0);
}

/* ===================== MAIN ===================== */

int main() {
    NoAVL *raiz = NULL;
    int op;

    cabecalho();

    do {
        printf("\n╔══════════════════════════════════════════╗\n");
        printf("║              MENU PRINCIPAL              ║\n");
        printf("╠══════════════════════════════════════════╣\n");
        printf("║  1. Cadastros (Inserir / Remover)        ║\n");
        printf("║  2. Consultas                            ║\n");
        printf("║  3. Atualizações                         ║\n");
        printf("║  4. Relatórios e Métricas                ║\n");
        printf("║  0. Sair                                 ║\n");
        printf("╚══════════════════════════════════════════╝\n");
        op = lerInteiro("  Opção: ", 0, 4);

        switch (op) {
            case 1: menuCadastros(&raiz);    break;
            case 2: menuConsultas(raiz);     break;
            case 3: menuAtualizacoes(raiz);  break;
            case 4: menuRelatorios(raiz);    break;
            case 0:
                printf("\n  Encerrando sistema. Liberando memória...\n");
                liberarArvore(raiz);
                printf("  Memória liberada. Até logo!\n\n");
                break;
        }
    } while (op != 0);

    return 0;
}
