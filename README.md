# Sistema de Gerenciamento de Eventos Críticos — Cidade Inteligente

## Descrição Geral

Este sistema simula o monitoramento e gerenciamento de eventos críticos urbanos em uma cidade inteligente. Eventos como acidentes de trânsito, falhas em semáforos, interrupções de energia, alagamentos e incêndios são registrados, consultados, priorizados e encerrados por meio de uma interface textual interativa.

O núcleo do sistema é implementado com uma **Árvore AVL** (Árvore Binária de Busca Auto-Balanceada), garantindo operações de inserção, remoção e busca em tempo **O(log n)** mesmo com grande volume de dados.

---

## Funcionalidades

### Gerenciamento da Árvore AVL
- Inserção de eventos com cálculo automático do fator de balanceamento e aplicação de rotações simples e duplas (LL, RR, LR, RL)
- Remoção de eventos — somente eventos com status **Resolvido** podem ser removidos; rebalanceamento automático após a remoção
- Busca eficiente de evento por ID

### Consultas Avançadas
- Listagem de eventos ativos por **intervalo de severidade** (ex.: severidade entre 3 e 5)
- **Relatório por região** — exibe todos os eventos ativos de uma região, ordenados por ID via percurso em-ordem
- **Busca por intervalo de ID** — lista eventos cujo ID esteja entre X e Y

### Atualizações
- Alterar o status de um evento de **Ativo** para **Resolvido**
- Atualizar a **severidade** de um evento ativo

### Métricas da Árvore
- Altura total da árvore
- Número total de nós
- Número de eventos ativos e resolvidos
- Fator de balanceamento médio (valor absoluto)
- Total de rotações realizadas desde o início da execução

---

## Estrutura do Evento (Nó da AVL)

| Campo | Tipo | Descrição |
|---|---|---|
| `id` | `int` | Chave primária, gerado automaticamente |
| `tipo` | `enum TipoEvento` | Acidente, Semáforo, Energia, Alagamento, Incêndio |
| `severidade` | `int` (1–5) | Nível de criticidade do evento |
| `timestamp` | `struct DataHora` | Data e hora do registro |
| `regiao` | `char[50]` | Região da cidade |
| `status` | `enum StatusEvento` | Ativo ou Resolvido |
| `altura` | `int` | Altura do nó na AVL |
| `esq` / `dir` | `NoAVL*` | Ponteiros para filhos esquerdo e direito |

---

## Instruções de Compilação

### Requisitos
- Compilador C com suporte ao padrão **C11** (GCC recomendado)
- Sistema operacional: Linux, macOS ou Windows (com MinGW/WSL)

### Compilação (terminal / linha de comando)

```bash
gcc -std=c11 -o avl_eventos avl_eventos.c
```

### Execução

```bash
./avl_eventos
```

> **Windows (MinGW):**
> ```bash
> gcc -std=c11 -o avl_eventos.exe avl_eventos.c
> avl_eventos.exe
> ```

### Compilador Online (alternativa)

Acesse [programiz.com/c-programming/online-compiler](https://www.programiz.com/c-programming/online-compiler), cole o conteúdo de `avl_eventos.c` e clique em **Run**.

---

## Organização do Menu

```
MENU PRINCIPAL
├── 1. Cadastros
│   ├── Registrar novo evento
│   └── Remover evento (somente Resolvido)
├── 2. Consultas
│   ├── Buscar por ID
│   ├── Listar por intervalo de severidade
│   ├── Relatório por região
│   ├── Busca por intervalo de ID
│   └── Listar todos (em-ordem)
├── 3. Atualizações
│   ├── Marcar evento como Resolvido
│   └── Atualizar severidade
└── 4. Relatórios e Métricas
    ├── Métricas da Árvore AVL
    ├── Todos os eventos ativos
    └── Todos os eventos resolvidos
```

---

## Requisitos Técnicos Atendidos

- Linguagem: **C**
- Alocação dinâmica de memória (`malloc` / `free`)
- Uso de `struct`, `enum` e ponteiros
- Sem uso de bibliotecas prontas de árvores
- Sem substituição da árvore por listas ou vetores

---

## Arquivos

| Arquivo | Descrição |
|---|---|
| `avl_eventos.c` | Código-fonte completo do sistema |
| `README.md` | Este arquivo |

## Aluna

- Rebecca Beccari Fagundes
