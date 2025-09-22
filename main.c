#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>

int *create_magic_square(int n);
void rotate90(int *A, int n);
void reflect_horizontal(int *A, int n);
void random_transform(int *A, int n);
void print_matrix(int *A, int n);

void on_entry1_insert_text(GtkEditable *editable,
                           const gchar *text,
                           gint length,
                           gint *position,
                           gpointer data);

void on_entry1_changed(GtkEntry *e);


GtkWidget *window1;
GtkWidget *window2;
GtkWidget *window3;

GtkWidget *label1;

GtkWidget *grid;

GtkWidget *entry_number;
GtkBuilder *builder;




GtkWidget *but02;
char tmp[10];
char temp[1024];
int n;


int main(int argc, char *argv[])
{
    srand((unsigned) time(NULL));
	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "Proyecto001.glade", NULL);
	
	window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    window2 = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    window3 = GTK_WIDGET(gtk_builder_get_object(builder, "window3"));
    grid = GTK_WIDGET(gtk_builder_get_object(builder, "grid1"));
    

    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));


    entry_number = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
    g_signal_connect(entry_number, "insert-text", G_CALLBACK(on_entry1_insert_text), NULL);
    but02 = GTK_WIDGET(gtk_builder_get_object(builder, "but02"));


    //g_signal_connect(but02, "on_but01_clicked", G_CALLBACK(on_but02_clicked), entry_number);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);


	gtk_widget_show_all(window1);
	gtk_main();

	return 0;
}


GtkWidget *label[100][100]; //global

void on_but02_clicked() {
    gtk_widget_hide(window2);
    char label_text[10];
    int row, col;

    // Mostrar la ventana dinámica
    gtk_widget_show(GTK_WIDGET(window3));

    int n = atoi(tmp);
    int *sq = create_magic_square(n);
    random_transform(sq, n);

    // Crear y poner labels en la grilla
    for (row = 0; row < n; row++) {
        for (col = 0; col < n; col++) {
            label[row][col] = gtk_label_new(NULL);
            sprintf(label_text, "%d", sq[row * n + col]);
            gtk_label_set_text(GTK_LABEL(label[row][col]), label_text);
            gtk_label_set_justify(GTK_LABEL(label[row][col]), GTK_JUSTIFY_LEFT);

            gtk_grid_attach(GTK_GRID(grid), label[row][col], col, row, 1, 1);
            gtk_widget_show(label[row][col]);
        }
    }

    free(sq);
}


void on_but01_clicked(){
    gtk_widget_hide(window1);
    gtk_widget_show_all(window2);
}


void quit(){

    gtk_main_quit();
}

// Filtro para que solo acepte dígitos en el GtkEntry
void on_entry1_insert_text(GtkEditable *editable,
                           const gchar *text,
                           gint length,
                           gint *position,
                           gpointer data) {
    for (int i = 0; i < length; i++) {
        if (!g_ascii_isdigit(text[i])) {
            g_signal_stop_emission_by_name(editable, "insert-text");
            return; // Si no es dígito, no se inserta
        }
    }
}

// Cuando cambia el texto en el entry
void on_entry1_changed(GtkEntry *e) {
    const char *text = gtk_entry_get_text(e);
    if (text[0] == '\0') {
        gtk_label_set_text(GTK_LABEL(label1), ""); // vacío
        gtk_widget_set_sensitive(but02, FALSE);
        return;
    }

    int val = atoi(text);

    if (val >= 3 && val <= 21 && (val % 2 == 1)) {
        sprintf(tmp, "%d", val); 
        gtk_label_set_text(GTK_LABEL(label1), tmp);
        gtk_widget_set_sensitive(but02, TRUE); // habilita botón
    } else {
        gtk_label_set_text(GTK_LABEL(label1), "Debe ser impar, entre 3 y 21");
        gtk_widget_set_sensitive(but02, FALSE); // desactiva botón
    }
}

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

int main1(void) {
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
