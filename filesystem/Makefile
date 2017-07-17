# Name of your program:
NAME=assign3

# List of all .cpp source code files included in your program (separated by spaces):
SRC= block.cpp file.cpp tree.cpp str2vec.cpp fsFunctions.cpp main.cpp



SRCPATH=./src/
OBJ=$(addprefix $(SRCPATH), $(SRC:.cpp=.o))

RM=rm -f
INCPATH=include
CPPFLAGS+= -g -I $(INCPATH)


all: $(OBJ)
	g++ $(CPPFLAGS) $(OBJ) -o $(NAME) 
	
clean:
	-$(RM) *~
	-$(RM) *#*
	-$(RM) *swp
	-$(RM) *.core
	-$(RM) *.stackdump
	-$(RM) $(SRCPATH)*.o
	-$(RM) $(SRCPATH)*.obj
	-$(RM) $(SRCPATH)*~
	-$(RM) $(SRCPATH)*#*
	-$(RM) $(SRCPATH)*swp
	-$(RM) $(SRCPATH)*.core
	-$(RM) $(SRCPATH)*.stackdump

fclean: clean
	-$(RM) $(NAME)

re: fclean all

