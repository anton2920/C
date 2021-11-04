#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <process.h>
#include <string.h>
#include <spawn.h>


#define handle_error(_msg)  \
    do {                    \
        perror(_msg);       \
        exit(EXIT_FAILURE); \
    } while (0)


#define STATUS_OK     200
#define STATUS_OK_STR "OK"


int main(int argc, const char *argv[])
{
    char message_buf[BUFSIZ], reply_buf[BUFSIZ], *endptr;
    int pid, chid, connid, client_id;

    assert(argc == 4);

    pid = (int) strtoul(argv[1], &endptr, 10);
    assert((endptr == NULL) || (*endptr == '\0'));

    chid = (int) strtoul(argv[2], &endptr, 10);
    assert((endptr == NULL) || (*endptr == '\0'));

    client_id = (int) strtoul(argv[3], &endptr, 10);
    assert((endptr == NULL) || (*endptr == '\0'));

    connid = ConnectAttach(ND_LOCAL_NODE, pid, chid, _NTO_SIDE_CHANNEL, 0);
    if (connid < 0) {
        handle_error("ConnectAttach");
    }

    for (;;) {
        snprintf(message_buf, sizeof(message_buf), "%d%d", client_id, (rand() % (99 - 10 + 1)) + 10);
        if (MsgSend(connid, message_buf, sizeof(message_buf), reply_buf, sizeof(reply_buf)) != STATUS_OK) {
            printf("[client #%d]: server returned error status (it's most likely terminated)\n", client_id);
            break;
        }
        usleep(rand % 100);
    }

    return 0;
}
