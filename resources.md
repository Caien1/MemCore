Socket Programming 
https://www.youtube.com/watch?v=x7obJLN7FcQ

to get local and remote address if you let os to setermine it 
int getsockname(int fd, struct sockaddr *addr, socklen_t *addrlen); // local
int getpeername(int fd, struct sockaddr *addr, socklen_t *addrlen); // remote
