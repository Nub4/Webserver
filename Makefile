NAME := Webserv

SRCS_DIR := Networking/Sockets

SRCS_DIR2 := Networking/Servers

SRCS := Socket.cpp \
		Bind.cpp \
		Listen.cpp

SRCS2 := Server_test.cpp \
		 Server.cpp \
		 TestServer.cpp

OBJ_DIR := objs
OBJ_DIR2 := objs2

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
OBJECTS2 := $(addprefix $(OBJ_DIR2)/, $(SRCS2:.cpp=.o))

CXX := clang++

RM := rm -f

CXXFLAGS := #-Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR2):
	@mkdir -p $(OBJ_DIR2)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR2)/%.o: $(SRCS_DIR2)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJECTS) $(OBJ_DIR2) $(OBJECTS2)
	@echo "\033[1;33mBuilding Webserv..\033[0m"
	@${CXX} ${CXXFLAGS} -o ${NAME} ${OBJECTS} $(OBJECTS2)
	@echo "\033[1;32mDONE!\033[0m"

clean:
	@echo "\033[1;31mRemoving object files..\033[0m"
	@${RM} ${OBJECTS} $(OBJECTS2)
	@rm -rf ${OBJ_DIR} ${OBJ_DIR2}

fclean: clean
	@echo "\033[1;31mRemoving launch file..\033[0m"
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re