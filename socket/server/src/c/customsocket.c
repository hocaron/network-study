#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <uuid/uuid.h>
#include "com_study_network_CustomSocketServer.h"

// 클라이언트 연결 처리를 위한 구조체 정의
typedef struct {
    int clientFd;
    char clientId[37];  // UUID를 담을 공간 (36자 + null 종결)
} ClientInfo;

// 클라이언트와의 통신을 위한 스레드 함수
void *handleClient(void *arg) {
    ClientInfo *clientInfo = (ClientInfo *)arg;
    int clientFd = clientInfo->clientFd;
    char buffer[256];
    char response[300];  // 에코 응답을 저장할 버퍼

    printf("클라이언트 연결 ID: %s, FD: %d\n", clientInfo->clientId, clientFd);

    // 데이터 수신 및 에코 응답
    int bytesRead;
    while ((bytesRead = read(clientFd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        printf("클라이언트 %s로부터 수신된 메시지: %s\n", clientInfo->clientId, buffer);

        // 에코 응답
        snprintf(response, sizeof(response), "Echo: %s", buffer);
        write(clientFd, response, strlen(response));
    }

    close(clientFd);
    free(clientInfo);  // 동적 할당된 메모리 해제
    return NULL;
}

// 서버 소켓 생성 및 바인딩
JNIEXPORT jint JNICALL Java_com_study_network_CustomSocketServer_createServerSocket(JNIEnv *env, jobject obj, jint port) {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        perror("서버 소켓 생성 실패");
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("소켓 바인딩 실패");
        close(serverFd);
        return -1;
    }

    if (listen(serverFd, 10) == -1) {
        perror("소켓 리슨 실패");
        close(serverFd);
        return -1;
    }

    return serverFd;
}

// 클라이언트 연결 수락 및 고유 ID 생성
JNIEXPORT jint JNICALL Java_com_study_network_CustomSocketServer_acceptConnection(JNIEnv *env, jobject obj, jint serverFd) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientFd == -1) {
        perror("클라이언트 연결 수락 실패");
        return -1;
    }

    // 고유한 UUID 생성
    uuid_t uuid;
    uuid_generate(uuid);
    char clientId[37];
    uuid_unparse(uuid, clientId);

    printf("클라이언트 연결 수락됨. FD: %d, ID: %s\n", clientFd, clientId);

    // 클라이언트 정보를 구조체에 저장 후 스레드에 전달
    ClientInfo *clientInfo = malloc(sizeof(ClientInfo));
    clientInfo->clientFd = clientFd;
    strncpy(clientInfo->clientId, clientId, sizeof(clientInfo->clientId));

    pthread_t thread;
    pthread_create(&thread, NULL, handleClient, clientInfo);
    pthread_detach(thread);  // 스레드를 분리하여 독립적으로 실행

    return clientFd;
}