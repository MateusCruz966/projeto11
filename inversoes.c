/*
 * Projeto 11 - Medidor do Grau de Desordem
 * Contagem de inversoes (lacos aninhados, O(n^2)) x Insertion Sort.
 *
 * Compilar: gcc -O2 -Wall -o inversoes inversoes.c
 * Executar: ./inversoes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef long long ll;

/* Conta pares (i, j) com i < j e v[i] > v[j]. */
ll contar_inversoes(const int *v, int n) {
    ll inv = 0;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (v[i] > v[j])
                inv++;
    return inv;
}

/* Insertion Sort que conta deslocamentos (v[j+1] = v[j]) e comparacoes. */
void insertion_sort(int *v, int n, ll *desloc, ll *comp) {
    *desloc = 0;
    *comp = 0;
    for (int i = 1; i < n; i++) {
        int chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            (*comp)++;
            if (v[j] > chave) {
                v[j + 1] = v[j];
                (*desloc)++;
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave;
    }
}

/* ---------- geradores de entrada ---------- */

/* Quase ordenado: vetor crescente com 2% de trocas locais (distancia <= 5). */
void gerar_quase_ordenado(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = i;
    int trocas = n / 50;
    for (int t = 0; t < trocas; t++) {
        int i = rand() % n;
        int d = 1 + rand() % 5;
        if (i + d < n) {
            int tmp = v[i]; v[i] = v[i + d]; v[i + d] = tmp;
        }
    }
}

void gerar_aleatorio(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = rand() % 100000;
}

void gerar_invertido(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = n - i;
}

int esta_ordenado(const int *v, int n) {
    for (int i = 0; i + 1 < n; i++)
        if (v[i] > v[i + 1]) return 0;
    return 1;
}

static double ms(clock_t a, clock_t b) {
    return 1000.0 * (double)(b - a) / CLOCKS_PER_SEC;
}

int main(void) {
    srand(12345); /* semente fixa: resultados reproduziveis */

    /* ---- Demonstracao pequena, para conferir a mao ---- */
    int demo[] = {5, 2, 4, 1, 3};
    int nd = 5;
    ll dd, cd;
    ll inv_demo = contar_inversoes(demo, nd);
    insertion_sort(demo, nd, &dd, &cd);
    printf("Demo [5 2 4 1 3]: inversoes = %lld, deslocamentos = %lld\n\n",
           inv_demo, dd);

    /* ---- Experimentos ---- */
    const int tamanhos[] = {1000, 5000, 10000, 20000};
    const int ntam = sizeof(tamanhos) / sizeof(tamanhos[0]);
    const char *nomes[] = {"quase ordenado", "aleatorio", "invertido"};
    void (*geradores[])(int *, int) = {gerar_quase_ordenado, gerar_aleatorio,
                                       gerar_invertido};

    printf("%-15s %6s %14s %14s %14s %5s %10s %10s\n", "tipo", "n",
           "inversoes", "deslocamentos", "comparacoes", "i==d", "t_inv(ms)",
           "t_ins(ms)");
    printf("---------------------------------------------------------------"
           "---------------------------------\n");

    for (int k = 0; k < 3; k++) {
        for (int t = 0; t < ntam; t++) {
            int n = tamanhos[t];
            int *v = malloc(n * sizeof(int));
            if (!v) { fprintf(stderr, "sem memoria\n"); return 1; }
            geradores[k](v, n);

            clock_t a = clock();
            ll inv = contar_inversoes(v, n);
            clock_t b = clock();

            ll desloc, comp;
            clock_t c = clock();
            insertion_sort(v, n, &desloc, &comp);
            clock_t d = clock();

            printf("%-15s %6d %14lld %14lld %14lld %5s %10.3f %10.3f%s\n",
                   nomes[k], n, inv, desloc, comp,
                   inv == desloc ? "sim" : "NAO", ms(a, b), ms(c, d),
                   esta_ordenado(v, n) ? "" : "  (ERRO: nao ordenou)");
            free(v);
        }
        printf("\n");
    }
    return 0;
}
