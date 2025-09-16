#include <stdio.h>
#include <stdlib.h>

int *create_magic_square(int n) {
    if (n < 3 || (n % 2) == 0) return NULL; /* sólo impar y >=3 */

    int *A = malloc(n * n * sizeof(int));
    if (!A) return NULL;
    for (int i = 0; i < n * n; ++i) A[i] = 0;

    int row = 0;
    int col = (n - 1) / 2;   /* columna inicial central */
    A[row * n + col] = 1;

    for (int val = 2; val <= n * n; ++val) {
        /* movimiento noreste */
        row = row - 1;
        col = col + 1;

        /* ajuste por "wrap"*/
        if (row < 0) row += n;
        if (col >= n) col -= n;

        if (A[row * n + col] == 0) {
            /* celda vacía: colocar valor */
            A[row * n + col] = val;
        } else {
            /* celda ocupada: RETROCEDER según Kurosaka:
            */
            row = row + 2;
            col = col - 1;
            if (row >= n) row -= n;
            if (col < 0) col += n;
            A[row * n + col] = val;
        }
    }

    return A;
}

void print_matrix(int *A, int n) {
    if (!A) return;
    printf("Cuadrado mágico de orden %d (cada fila):\n", n);
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            printf("%4d", A[r * n + c]);
        }
        printf("\n");
    }
    long magic_constant = (long)n * (n * (long)n + 1) / 2;
    printf("\nConstante mágica: %ld (suma por fila/col/diagonal)\n\n", magic_constant);
}


int main(void) {
    int side;
    while (1) {
        printf("Ingrese el orden del cuadrado (impar, 3 o mayor): ");
        if (scanf("%d", &side) != 1) {
            /* limpiar entrada no válida */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Entrada inválida. Intente de nuevo.\n");
            continue;
        }
        if (side < 3) {
            printf("El número debe ser 3 o mayor.\n");
            continue;
        }
        if (side % 2 == 0) {
            printf("El número debe ser impar.\n");
            continue;
        }
        break;
    }

    int *sq = create_magic_square(side);
    if (!sq) {
        fprintf(stderr, "Error creando el cuadrado mágico (memoria).\n");
        return 1;
    }

    print_matrix(sq, side);

    free(sq);
    return 0;
}
