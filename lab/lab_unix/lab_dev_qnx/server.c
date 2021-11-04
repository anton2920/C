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


#define NP            4
#define STATUS_OK     200
#define STATUS_OK_STR "OK"


int main()
{
    char buffer[BUFSIZ], pid_str[20], chid_str[20], client_id_str[20], *endptr;
    unsigned long message_number, sum[NP] = {};
    size_t msgs_received, i;
    pid_t pid_arr[NP];
    int chid, rcvid;
    FILE *pidchid;

    chid = ChannelCreate(0);
    if (chid < 0) {
        handle_error("ChannelCreate");
    }

    pidchid = fopen("pidchid", "w");
    assert(pidchid != NULL);

    fprintf(pidchid, "%d %d\n", getpid(), chid);

    fclose(pidchid);

    for (i = 0; i < sizeof(pid_arr) / sizeof(pid_arr[0]); ++i) {
        snprintf(pid_str, sizeof(pid_str), "%d", getpid());
        snprintf(chid_str, sizeof(chid_str), "%d", chid);
        snprintf(client_id_str, sizeof(client_id_str), "%u", i);
        if (spawnl(P_NOWAIT, "./client", "./client", pid_str, chid_str, client_id_str, NULL) == -1) {
            handle_error("posix_spawn()");
        }
    }

    for (msgs_received = 0; msgs_received < 30; ++msgs_received) {
        rcvid = MsgReceive(chid, buffer, sizeof(buffer), NULL);
        if (rcvid < 0) {
            handle_error("MsgReceive");
        }

        printf("Debug: %s\n", buffer);
        message_number = strtoull(buffer, &endptr, 10);
        assert((endptr == NULL) || (*endptr == '\0'));

        printf("From client %lu received %lu\n", message_number / 100, message_number % 100);
        sum[message_number / 100] += message_number % 100;

        if (MsgReply(rcvid, STATUS_OK, STATUS_OK_STR, sizeof(STATUS_OK_STR) - 1) < 0) {
            handle_error("MsgReply");
        }
    }

    if (ChannelDestroy(chid) < 0) {
        handle_error("ChannelDestroy");
    }

    return 0;
}
