NAME := Webserv

SRCS_DIR := Networking/Servers

SRCS_DIR2 := Networking/Parsing

SRCS_DIR3 := Networking/Utils

SRCS := main.cpp \
		 Server.cpp \
		 Response.cpp \
		 CGI.cpp

SRCS2 := Parse.cpp

SRCS3 := Utils.cpp

OBJ_DIR := objs

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

OBJECTS2 := $(addprefix $(OBJ_DIR)/, $(SRCS2:.cpp=.o))

OBJECTS3 := $(addprefix $(OBJ_DIR)/, $(SRCS3:.cpp=.o))

CXX := clang++

RM := rm -f

CXXFLAGS := -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@echo "\033[1;33mCompiling $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRCS_DIR2)/%.cpp
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRCS_DIR3)/%.cpp
	@echo "Compiling $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJECTS) $(OBJECTS2) $(OBJECTS3)
	@echo "\033[1;32mDONE!\033[0m"
	@${CXX} ${CXXFLAGS} -o server $(OBJECTS) $(OBJECTS2) $(OBJECTS3)

clean:
	@echo "\033[1;31mRemoving object files..\033[0m"
	@${RM} $(OBJECTS) $(OBJECTS2) $(OBJECTS3)
	@rm -rf ${OBJ_DIR}

fclean: clean
	@echo "\033[1;31mRemoving launch file..\033[0m"
	@${RM} server

re: fclean all

.PHONY: all clean fclean re