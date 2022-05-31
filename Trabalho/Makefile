CFLAGS = -Wall
PTHREAD = -lpthread
MATH = -lm

LDFLAGS = $(MATH) $(PTHREAD)

# SOURCES
SRC_INTEGRAL = integral.c
SRC_SEQUENCIAL = sequencial.c
SRC_CONCORRENTE = concorrente.c

# OBJECTS
OBJ_INTEGRAL = integral.o
OBJ_SEQUENCIAL = sequencial.o
OBJ_CONCORRENTE = concorrente.o
OBJS = $(OBJ_INTEGRAL) $(OBJ_SEQUENCIAL) $(OBJ_CONCORRENTE)

EXEC = integral

clean:
	rm $(OBJS)
	rm $(EXEC)

integral: $(OBJS)
	gcc $(OBJS) -o integral $(LDFLAGS) $(CFLAGS)

$(OBJ_INTEGRAL): $(SRC_INTEGRAL)
	gcc -c $(SRC_INTEGRAL) -o $(OBJ_INTEGRAL) $(CFLAGS)

$(OBJ_SEQUENCIAL): $(SRC_SEQUENCIAL)
	gcc -c $(SRC_SEQUENCIAL) -o $(OBJ_SEQUENCIAL) $(CFLAGS)

$(OBJ_CONCORRENTE): $(SRC_CONCORRENTE)
	gcc -c $(SRC_CONCORRENTE) -o $(OBJ_CONCORRENTE) $(CFLAGS)
