NAME = fractol
WFLAGS = -Wall -Werror -Wextra
SRC = ./src/main.c ./src/mandelbar.c ./src/julia_mandelbrot.c ./src/image_calc.c ./src/hooks.c
OBJ = $(SRC:.c=.o)
INC = -I ./libft/includes -I ./includes
LIBFT = -L ./libft -lft
LIBS = -lmlx -lm -framework OpenGL -framework AppKit

all : make_libft $(NAME)

$(NAME) : $(OBJ)
	gcc $(WFLAGS) $(INC) $(OBJ) -o $(NAME) $(LIBS) $(LIBFT)

make_libft :
	make -C ./libft

%.o : %.c
	gcc -g $(WFLAGS) $(INC) -o $@ -c $<

clean :
	make -C ./libft clean
	rm $(OBJ)

fclean : clean
	make -C ./libft fclean
	rm $(NAME)

re : fclean all
