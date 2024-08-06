NAME = fractol
MLX_NAME = libmlx.a
CC = cc
CFLAGS = -Wall -Wextra -Werror 
MLX_FLAGS = -lX11 -lXext -lmlx
MLX_PATH = minilibx-linux/
MLX = $(MLX_PATH)$(MLX_NAME)

INC :=	-I fractol.h\
		-I $(MLX_PATH)

SRCS = fractal.c hooks.c main.c parse.c render.c utils.c

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(INC)

OBJS := $(SRCS:.c=.o)

$(NAME): $(MLX) $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(MLX) $(MLX_FLAGS) $(INC) -g3

$(MLX):
	@make -sC $(MLX_PATH)

all: $(MLX) $(NAME)

bonus: all

clean:
	rm -f $(OBJS)
	@make clean -sC $(MLX_PATH)
	
fclean: clean
	rm -f $(NAME)

re: fclean all