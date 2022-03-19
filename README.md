# lastResort
a simple tcp server to upload files

story:
I have a virtual server in cloud. the story began when I tried to upgrade ssl library, and deleted some library without backup in advance. Suddenly I was in the worst situation: couldn’t login using ssh, curl, wget ,scp , all clients depending on SSL stopped working. Lucky enough, the terminal session is still on, and I still could type. So just before I decided to email the cloud providers to retore the image( I hated to do so, because there were many things to reconfig),  a thought came out: maybe I can implement a server to receive from other machine the library deleted


Compile:

gcc -o server server.c 

Run :
./server
 
It listen on port 9999 and save files sent from a client in the current dir, name as: out.so

In client machine, we can just send file using nc, like this:

cat so.gz |nc xxx.xxx.xxx.xxx 9999

