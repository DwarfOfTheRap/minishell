NAME = minishell


##### COLORS #####
BLUE = \033[1;35m
GREY = \033[3;31m
GREEN = \033[1;37m
RED = \033[1;31m
END = \033[0m



##### SRC & OBJ PATH #####
LIBFTPATH = libft
SRCPATH = ./srcs
TESTPATH = ./test
OBJPATH = $(SRCPATH)/obj

##### INCLUDE #####
INC_MINISHELL = includes
INC_LIBFT = $(LIBFTPATH)/includes

##### INCLUDES DEPENDENCIES  ---  any modif in any .h will alert makefile to compile again
INC_DEP = $(INC_MINISHELL)/*.h

##### LIB #####
LIBFT = $(LIBFTPATH)/libft.a

##### FLAG #####
CC = clang
FLAGS = -Wall -Wextra -Werror -g# -fsanitize=address

##### OSTYPE #####
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# mac
	FLAGS += -D DARWIN
else
	#Linus and others...
	FLAGS += -D LINUX
endif

#SRC & OBJ PATH
SRCPATH = ./srcs
OBJPATH = $(SRCPATH)/obj

#SRCS
SRCS = $(addprefix $(SRCPATH)/, builtins_1.c builtins_2.c builtins_3.c \
	   environment.c environment_2.c main.c main_loop.c signal_handler.c \
	   executable_external.c executable_external_path.c exit_error_manager.c \
	   tokenizer.c token_spliters.c token_expand.c token_quotes.c \
	   token_utils.c token_find_redirections.c token_redirections.c \
	   clear_lists.c file_descriptor.c utils.c executable_builtin.c)

##### OBJS #####
OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)


### RULES ###

all : mk_objdir mk_libft $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi

mk_libft :
	@echo "\n$(END)$(BLUE)# Checking libft #$(END)$(GREY)"
	@make -C $(LIBFTPATH)


$(NAME) : message $(OBJ) $(INC_DEP)
	@echo "\n$(END)$(BLUE)# Making $(NAME) #$(END)$(GREY)"
	$(CC) -o $@ $(OBJ) $(LIBFT)
	@echo "\n$(END)$(GREEN)# $(NAME) is built #$(END)"

message :
	@echo "\n$(END)$(BLUE)# Making $(NAME) objects #$(END)$(GREY)"


$(OBJPATH)/%.o : $(SRCPATH)/%.c $(INC_DEP)
	$(CC) $(FLAGS) -I $(INC_LIBFT) -I $(INC_MINISHELL) -c $< -o $@

testit :
	@$(MAKE) -C $(TESTPATH)


### CLEAN ###
.PHONY : clean fclean re

clean :
	@echo "$(END)$(RED)# removing $(NAME) objects #$(END)$(GREY)"
	rm -rf $(OBJ)
	@echo "$(END)$(RED)# removing libft objects #$(END)"
	@make clean -C $(LIBFTPATH)

fclean : clean
	@echo "$(END)$(RED)\n# removing $(NAME) #$(END)$(GREY)"
	@rm -f $(NAME)
	@echo "$(END)$(RED)\n# removing libft.a #$(END)"
	@make fclean -C $(LIBFTPATH)

re : fclean all

tclean :
	@make clean -C $(TESTPATH)

tfclean :
	@make fclean -C $(TESTPATH)

tre :
	@make re -C $(TESTPATH)
