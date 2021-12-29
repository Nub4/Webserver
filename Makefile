NAME := Webserv

SRCS_DIR := Networking/Servers

SRCS := main.cpp \
		 Server.cpp

OBJ_DIR := objs

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

CXX := clang++

RM := rm -f

CXXFLAGS := -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJECTS)
	@echo "\033[1;33mBuilding Webserv..\033[0m"
	@${CXX} ${CXXFLAGS} -o server $(OBJECTS)
	@echo "\033[1;32mDONE!\033[0m"

clean:
	@echo "\033[1;31mRemoving object files..\033[0m"
	@${RM} $(OBJECTS)
	@rm -rf ${OBJ_DIR}

fclean: clean
	@echo "\033[1;31mRemoving launch file..\033[0m"
	@${RM} server

re: fclean all

.PHONY: all clean fclean re