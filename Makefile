#------------------------------------------------------------------------------

SOURCE = Forest.c
MYPROGRAM = forest
CC = mpic

#------------------------------------------------------------------------------



all: $(MYPROGRAM)


$(MYPROGRAM): $(SOURCE)
	$(CC) -lpthread $(SOURCE) -o$(MYPROGRAM)

clean:
	rm -f $(MYPROGRAM)

