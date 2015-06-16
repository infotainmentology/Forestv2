#------------------------------------------------------------------------------

SOURCE = Forest.c
MYPROGRAM = forest
CC = mpicc

#------------------------------------------------------------------------------



all: $(MYPROGRAM)


$(MYPROGRAM): $(SOURCE)
	$(CC) -lpthread $(SOURCE) -o$(MYPROGRAM)

clean:
	rm -f $(MYPROGRAM)

