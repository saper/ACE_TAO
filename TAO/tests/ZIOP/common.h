#define CLIENT_COMPRESSION_LEVEL 7
#define SERVER_COMPRESSION_LEVEL 5
#define LEAST_COMPRESSION_LEVEL ((CLIENT_COMPRESSION_LEVEL < SERVER_COMPRESSION_LEVEL) ?\
                                  CLIENT_COMPRESSION_LEVEL : SERVER_COMPRESSION_LEVEL)

#define DEFAULT_IOR_FILENAME ACE_TEXT("test.ior");
static int test = 1;
