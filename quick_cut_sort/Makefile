GCC_CMD=g++ -g -Wall -std=c++11
main : clean
	@$(GCC_CMD) -c *.cpp
	@$(GCC_CMD) *.o -L/usr/lib64/mysql -lmysqlclient -lpthread -llog -o main
	@./main
	@rm -rf *.o
	@rm -rf main
clean :
	@rm -rf *.o
	@rm -rf main

