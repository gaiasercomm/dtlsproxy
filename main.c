
#include <string.h>
#include <getopt.h>
#include <signal.h>

#include "proxy.h"
#include "utils.h"
#include "tinydtls/dtls_debug.h"

static proxy_context_t context;

static void usage(const char *program)
{
    const char *p = strrchr( program, '/' );
    if ( p )
        program = ++p;

    printf("\nDTLS reverse proxy server (c) 2017 yus\n\n"
        "usage: %s -l <host:port> -b <hosts:ports> -k <key maps>\n"
        "\t-l listen    listen on specified host and port\n"
        "\t-b backends  backend servers (host1:port1,host2:port2,...)\n"
        "\t-k keys      psk identities (id1:key1,id2:key2,...)\n", program);
    exit(1);
}

static void handle_sigint(int signum)
{
    static int done = 0;
    //DBG("%s done=%d", __func__, done);
    if (done) {
        return;
    }
    proxy_exit(&context);
    done = 1;
}

/* this array has the same order as the type log_t */
static char *loglevels[] = {
  "EMRG", "ALRT", "CRIT", "WARN", "NOTE", "INFO", "DEBG" 
};

static void set_tinydtls_log_level(char *loglevel)
{
    for (int i = 0; loglevel[0] && i < sizeof(loglevels); i++) {
        if(!strcasecmp(loglevel, loglevels[i], sizeof(loglevels[i]))) {
            dtls_set_log_level(i);
            break;
        }
    }
}

int main(int argc, char **argv)
{
    char listen_addr_buf[128];
    char backends_addr_buf[512];
    char psk_buf[512];
    char log_level_buf[32];

    DBG("%s started", argv[0]);

    memset(&context, 0, sizeof(proxy_context_t));
    memset(listen_addr_buf, 0, sizeof(listen_addr_buf));
    memset(backends_addr_buf, 0, sizeof(backends_addr_buf));
    memset(psk_buf, 0, sizeof(psk_buf));
    memset(log_level_buf, 0, sizeof(log_level_buf));

    static const struct option lopts[] = {
        {"listen",   required_argument, 0, 'l'},
        {"backends", required_argument, 0, 'b'},
        {"key",      required_argument, 0, 'k'},
        {"log",      optional_argument, 0, 'g'},
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "l:b:k:g:", lopts, NULL)) != -1) {
        switch (opt) {
        case 'l' :
            strncpy(listen_addr_buf, optarg, sizeof(listen_addr_buf)-1);
            break;
        case 'b' :
            strncpy(backends_addr_buf, optarg, sizeof(backends_addr_buf)-1);
            break;
        case 'k' :
            strncpy(psk_buf, optarg, sizeof(psk_buf)-1);
            break;
        case 'g' :
            strncpy(log_level_buf, optarg, sizeof(log_level_buf)-1);
            break;
        default:
            usage(argv[0]);
        }
    }

    set_tinydtls_log_level(log_level_buf);

    if (0!=proxy_init(&context,
                      listen_addr_buf,
                      backends_addr_buf,
                      psk_buf)) {
        ERR("proxy init failed");
        proxy_deinit(&context);
        usage(argv[0]);
    }

    signal(SIGINT, handle_sigint);
    proxy_run(&context);

    proxy_deinit(&context);
    DBG("%s exit", argv[0]);
    return 0;
}
