#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Genera el cuadrado canonico de Kurosaka (siempre va a ser igual) */
int *create_magic_square(int n) {
    if (n < 3 || (n % 2) == 0) return NULL;

    int *A = malloc(n * n * sizeof(int));
    if (!A) return NULL;
    for (int i = 0; i < n * n; ++i) A[i] = 0;

    int row = 0;
    int col = (n - 1) / 2;
    A[row * n + col] = 1;

    for (int val = 2; val <= n * n; ++val) {
        int newRow = row - 1;
        int newCol = col + 1;

        if (newRow < 0) newRow += n;
        if (newCol >= n) newCol -= n;

        if (A[newRow * n + newCol] == 0) {
            row = newRow;
            col = newCol;
        } else {
            row = (row + 1) % n;
            /* col es igual*/
        }
        A[row * n + col] = val;
    }

    return A;
}

/* Rotar 90 grados en sentio del reloj */
void rotate90(int *A, int n) {
    int *tmp = malloc(n * n * sizeof(int));
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            tmp[c * n + (n - 1 - r)] = A[r * n + c];
        }
    }
    for (int i = 0; i < n * n; i++) A[i] = tmp[i];
    free(tmp);
}

/*Hacer reflect al cuadrado horizontalmente */
void reflect_horizontal(int *A, int n) {
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n / 2; c++) {
            int tmp = A[r * n + c];
            A[r * n + c] = A[r * n + (n - 1 - c)];
            A[r * n + (n - 1 - c)] = tmp;
        }
    }
}

/* Poner transformacion */
void random_transform(int *A, int n) {
    int choice = rand() % 8; // 0..7
    int rotations = choice % 4;
    int reflect = choice / 4;

    for (int i = 0; i < rotations; i++)
        rotate90(A, n);

    if (reflect)
        reflect_horizontal(A, n);
}

/* Imprimir la matriz */
void print_matrix(int *A, int n) {
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            printf("%4d", A[r * n + c]);
        }
        printf("\n");
    }
    long magic_constant = (long)n * (n * (long)n + 1) / 2;
    printf("\nConstante mágica: %ld\n\n", magic_constant);
}

int main(void) {
    srand((unsigned) time(NULL));

    int side;
    while (1) {
        printf("Ingrese el orden del cuadrado (impar, 3 o mayor): ");
        if (scanf("%d", &side) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Entrada inválida.\n");
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
        if (side > 21){
            printf("El numero debe ser menor o igual a 21.\n");
            continue;
        }
        
        break;
    }

    int *sq = create_magic_square(side);
    if (!sq) return 1;

    random_transform(sq, side);
    print_matrix(sq, side);

    free(sq);
    return 0;
}
