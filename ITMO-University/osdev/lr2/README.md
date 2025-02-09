# Fundamentals of System Programming
# LR2: Server/client communication

client.c - client app  
server.c - server app  
test.sh - opens client 10 times in the background with ADD command and 1 2 3 ... 10 as arguments  
test2.sh - opens client 5 times in the background with GET command

## Build:
```make all```

## for tests:
```make test```
launches test.sh and test2.sh

# Notable: 
## additional server options:
-o option allows to output to terminal instead of log file.  
-s makes it so set is separate for users with different ip. 
## additional client options:
-d - launch client in test mode. makes "ADD" request to the server with the argument value.  
-g - launch client in test mode. makes "GET" request to the server.

## additional requests:
(client only) EXIT - exits client
CLOSE - closes server