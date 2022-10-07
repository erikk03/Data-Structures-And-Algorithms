# Compile options.
CFLAGS = -Wall -Werror -g

# Αρχεία .o
OBJS = welcome.o

# Το εκτελέσιμο πρόγραμμα
EXEC = welcome

$(EXEC): $(OBJS)
	gcc $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)