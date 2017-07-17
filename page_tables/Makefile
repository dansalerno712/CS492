# Name of your program:
NAME=assign2

# List of all .cpp source code files included in your program (separated by spaces):
SRC= src/Page.cpp src/PageTable.cpp src/main.cpp



SRCPATH=./
OBJ=$(addprefix $(SRCPATH), $(SRC:.cpp=.o))

RM=rm -f
INCPATH=include
CPPFLAGS+= -I $(INCPATH)


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

