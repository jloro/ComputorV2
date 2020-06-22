CC = g++
FLAGS = -Wall -Wextra -Werror -std=c++17 -fsanitize=address

EXEC_NAME = computorV2

SRC_FILES = main.cpp \
			Matrix.cpp\
			Historic.cpp \
			Core.cpp \
			Type.cpp \
			Complex.cpp \
			Real.cpp \

INC_FILES = Matrix.hpp \
			Historic.hpp \
			Core.hpp \
			Type.hpp \
			Complex.hpp \
			Real.cpp \
			Regex.hpp \

OBJ_FILES = $(SRC_FILES:.cpp=.o)

SRC_PATH = ./srcs/
INC_PATH = ./incs/
OBJ_PATH = ./.obj/

SRCS = $(addprefix $(SRC_PATH),$(SRC_FILES))
INCS = $(addprefix $(INC_PATH),$(INC_FILES))
OBJS = $(addprefix $(OBJ_PATH),$(OBJ_FILES))

LIBS = -lncurses

.PHONY : all clean fclean re

all : $(EXEC_NAME)

$(EXEC_NAME) : $(OBJS)
	@$(CC) $(FLAGS) -o $(EXEC_NAME) $(OBJS) $(LIBS)
	@echo "$(EXEC_NAME) compiled ✓"

$(OBJ_PATH)%.o : $(SRC_PATH)%.cpp
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(FLAGS) -o $@ -c $< -I $(INC_PATH) 
	@echo "$@ created ✓"

clean :
	@/bin/rm -rf $(OBJ_PATH)
	@echo "Objects cleaned ✓"

fclean : clean
	@/bin/rm -f $(EXEC_NAME)
	@echo "$(EXEC_NAME) deleted ✓"
re : fclean all
