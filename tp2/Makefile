# VARIABLES
CC		=	gcc
CFLAGS	=	-Wall -ansi -pedantic
EXEC	=	mon_ls
SRC		=	tp2.c
OBJ		=	$(SRC:.c=.o)

# RULES
all		:	$(EXEC)

clean 	:
	rm -rf $(OBJ)

mrproper:	clean
	rm -rf $(EXEC)

re		:	mrproper all

# COMPILATION
$(EXEC)	:	$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o		:	%.c
	$(CC) -o $@ -c $< $(CFLAGS)
