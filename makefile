# Nombre del ejecutable
TARGET = Proyecto1

# Archivos fuente
SRC = main.c

# Flags de compilación
CFLAGS = -Wall
# Incluir flags y librerías de GTK
GTK_FLAGS = `pkg-config --cflags --libs gtk+-3.0`

# Regla principal
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(GTK_FLAGS) -export-dynamic

# Limpieza de binarios y archivos temporales
clean:
	rm -f $(TARGET) *.o
