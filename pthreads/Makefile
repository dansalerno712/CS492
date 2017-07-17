# Name of your program:
NAME=assign1

# List of all .cpp source code files included in your program (separated by spaces):
SRC=src/main.cpp src/fn.cpp src/consumer.cpp src/producer.cpp



SRCPATH=./
OBJ=$(addprefix $(SRCPATH), $(SRC:.cpp=.o))

RM=rm -f
INCPATH=include
CPPFLAGS+= -I $(INCPATH)


all: $(OBJ)
	g++ $(CPPFLAGS) $(OBJ) -o $(NAME) -lpthread -lrt

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

