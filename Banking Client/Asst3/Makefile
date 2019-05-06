all: bankingClient bankingServer

bankingServer: BankServer.c
	gcc -g -o bankingServer BankServer.c -pthread
bankingClient: BankingClient.c
	gcc -g -o bankingClient BankingClient.c  -pthread
clean:
	rm -rf bankingServer bankingClient
