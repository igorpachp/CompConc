CFLAGS = -Wall
PTHREAD = -lpthread
MATH = -lm

LDFLAGS = $(MATH)

# SOURCES
SRC_INTEGRAL = integral.c
SRC_SEQUENCIAL = sequencial.c

# OBJECTS
OBJ_INTEGRAL = integral.o
OBJ_SEQUENCIAL = sequencial.o
OBJS = $(OBJ_INTEGRAL) $(OBJ_SEQUENCIAL)

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
