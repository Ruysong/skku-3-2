/*
 * k DNA Sequence Alignment
 * Junhyuk Song, 2024315329
 *
 * The fully matched columns form a common subsequence of all input
 * sequences. We first compute its maximum length with dynamic programming,
 * and then insert gaps while tracing a solution back through the table.
 * Compile: gcc -ansi -pedantic -Wall -Wextra -O2 msa.c -o msa
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_K 5
#define MAX_N 75
#define MAX_ALIGNMENT (MAX_K * MAX_N)

typedef struct {
    int k;
    char sequence[MAX_K][MAX_N + 1];
    int length[MAX_K];
    size_t stride[MAX_K];
    size_t total;
    size_t diagonal_step;
    unsigned char *dp;
} Problem;

/* Read the specified input format and check the assignment's limits. */
static int read_input(Problem *p)
{
    FILE *in;
    char token[MAX_N + 2];
    int i;
    int j;

    in = fopen("msa.in", "r");
    if (in == NULL) {
        fprintf(stderr, "Cannot open msa.in.\n");
        return 0;
    }
    if (fscanf(in, "%d", &p->k) != 1 || p->k < 2 || p->k > MAX_K) {
        fprintf(stderr, "Invalid number of sequences.\n");
        fclose(in);
        return 0;
    }
    if (fscanf(in, "%76s", token) != 1 || strcmp(token, "$") != 0) {
        fprintf(stderr, "Missing $ separator.\n");
        fclose(in);
        return 0;
    }
    for (i = 0; i < p->k; i++) {
        if (fscanf(in, "%76s", token) != 1) {
            fprintf(stderr, "Missing DNA sequence.\n");
            fclose(in);
            return 0;
        }
        p->length[i] = (int)strlen(token);
        if (p->length[i] < 1 || p->length[i] > MAX_N) {
            fprintf(stderr, "Invalid sequence length.\n");
            fclose(in);
            return 0;
        }
        for (j = 0; j < p->length[i]; j++) {
            if (strchr("ATGC", token[j]) == NULL) {
                fprintf(stderr, "Invalid DNA character.\n");
                fclose(in);
                return 0;
            }
        }
        strcpy(p->sequence[i], token);
    }
    fclose(in);
    return 1;
}

/*
 * Flatten the k-dimensional table. The last coordinate varies fastest.
 * Every score is at most 75, so one unsigned char per state is enough.
 * size_t is used because the largest table has more than 2^31 states.
 */
static int allocate_table(Problem *p)
{
    int i;
    size_t width;

    p->total = 1;
    p->diagonal_step = 0;
    for (i = p->k - 1; i >= 0; i--) {
        p->stride[i] = p->total;
        width = (size_t)p->length[i] + 1;
        if (p->total > (size_t)-1 / width) {
            fprintf(stderr, "Table size is too large.\n");
            return 0;
        }
        p->total *= width;
        p->diagonal_step += p->stride[i];
    }
    /* States with any zero coordinate are boundary states with score 0. */
    p->dp = (unsigned char *)calloc(p->total, sizeof(unsigned char));
    if (p->dp == NULL) {
        fprintf(stderr, "Cannot allocate the DP table.\n");
        return 0;
    }
    return 1;
}

/*
 * Recursion here only supplies k-1 nested loops (depth at most 5).
 * It does not recursively solve DP subproblems. Each positive-coordinate
 * state is filled exactly once, in increasing lexicographic order.
 */
static void fill_table(Problem *p, int level, size_t offset,
                       int position[MAX_K])
{
    int i;
    int j;
    int last;
    int same;
    char base;
    unsigned char best;
    unsigned char candidate;
    size_t index;

    last = p->k - 1;
    if (level < last) {
        for (i = 1; i <= p->length[level]; i++) {
            position[level] = i;
            fill_table(p, level + 1,
                       offset + (size_t)i * p->stride[level], position);
        }
        return;
    }

    /* The first k-1 final characters are fixed throughout this row. */
    base = p->sequence[0][position[0] - 1];
    same = 1;
    for (i = 1; i < last; i++) {
        if (p->sequence[i][position[i] - 1] != base) {
            same = 0;
            break;
        }
    }
    for (j = 1; j <= p->length[last]; j++) {
        index = offset + (size_t)j;
        if (same && p->sequence[last][j - 1] == base) {
            /* All final bases match: extend the shortened prefixes. */
            p->dp[index] = (unsigned char)
                (p->dp[index - p->diagonal_step] + 1);
        } else {
            /* Otherwise, leave one sequence's final base unmatched. */
            best = p->dp[index - 1];
            for (i = 0; i < last; i++) {
                candidate = p->dp[index - p->stride[i]];
                if (candidate > best) {
                    best = candidate;
                }
            }
            p->dp[index] = best;
        }
    }
}

static void reverse_string(char *text, int length)
{
    int left;
    int right;
    char temp;

    left = 0;
    right = length - 1;
    while (left < right) {
        temp = text[left];
        text[left] = text[right];
        text[right] = temp;
        left++;
        right--;
    }
    text[length] = '\0';
}

/*
 * Recover an optimal alignment without a separate predecessor table.
 * Diagonal move: output the common base in every row and a '*'.
 * Single-coordinate move: output that row's base and gaps in other rows.
 * Ties between skip moves are resolved by the input sequence order.
 */
static int recover_alignment(const Problem *p,
                             char aligned[MAX_K][MAX_ALIGNMENT + 1],
                             char marker[MAX_ALIGNMENT + 1])
{
    int position[MAX_K];
    int i;
    int chosen;
    int same;
    int columns;
    char base;
    size_t index;

    for (i = 0; i < p->k; i++) {
        position[i] = p->length[i];
    }
    index = p->total - 1;
    columns = 0;
    while (index != 0) {
        same = 1;
        base = '\0';
        for (i = 0; i < p->k; i++) {
            if (position[i] == 0) {
                same = 0;
                break;
            }
            if (i == 0) {
                base = p->sequence[i][position[i] - 1];
            } else if (p->sequence[i][position[i] - 1] != base) {
                same = 0;
                break;
            }
        }
        if (same) {
            for (i = 0; i < p->k; i++) {
                aligned[i][columns] = base;
                position[i]--;
            }
            marker[columns] = '*';
            index -= p->diagonal_step;
        } else {
            chosen = -1;
            for (i = 0; i < p->k; i++) {
                if (position[i] > 0 &&
                    p->dp[index - p->stride[i]] == p->dp[index]) {
                    chosen = i;
                    break;
                }
            }
            if (chosen == -1) {
                fprintf(stderr, "Cannot recover an alignment.\n");
                return 0;
            }
            for (i = 0; i < p->k; i++) {
                aligned[i][columns] = (i == chosen)
                    ? p->sequence[i][position[i] - 1] : '-';
            }
            marker[columns] = ' ';
            position[chosen]--;
            index -= p->stride[chosen];
        }
        columns++;
    }
    for (i = 0; i < p->k; i++) {
        reverse_string(aligned[i], columns);
    }
    reverse_string(marker, columns);
    return 1;
}

static int write_output(const Problem *p,
                        char aligned[MAX_K][MAX_ALIGNMENT + 1],
                        const char *marker)
{
    FILE *out;
    int i;
    int ok;

    out = fopen("msa.out", "w");
    if (out == NULL) {
        fprintf(stderr, "Cannot create msa.out.\n");
        return 0;
    }
    ok = 1;
    for (i = 0; i < p->k; i++) {
        if (fprintf(out, "%s\n", aligned[i]) < 0) {
            ok = 0;
        }
    }
    if (fprintf(out, "%s\n", marker) < 0) {
        ok = 0;
    }
    if (fclose(out) != 0) {
        ok = 0;
    }
    if (!ok) {
        fprintf(stderr, "Cannot write msa.out.\n");
    }
    return ok;
}

int main(void)
{
    Problem problem;
    int position[MAX_K];
    char aligned[MAX_K][MAX_ALIGNMENT + 1];
    char marker[MAX_ALIGNMENT + 1];
    int ok;

    if (!read_input(&problem) || !allocate_table(&problem)) {
        return EXIT_FAILURE;
    }
    fill_table(&problem, 0, 0, position);
    ok = recover_alignment(&problem, aligned, marker);
    if (ok) {
        ok = write_output(&problem, aligned, marker);
    }
    free(problem.dp);
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
