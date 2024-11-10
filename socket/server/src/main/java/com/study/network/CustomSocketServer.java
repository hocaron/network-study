package com.study.network;

public class CustomSocketServer {

    static {
        System.loadLibrary("customsocket");  // 네이티브 라이브러리 로드
    }

    private int serverFd;  // 서버 소켓의 파일 디스크립터

    // 서버 소켓 생성 및 바인딩
    public native int createServerSocket(int port);

    // 클라이언트 연결 수락 및 고유 ID 생성, 스레드 생성
    public native int acceptConnection(int serverFd);

    public void startServer(int port) {
        serverFd = createServerSocket(port);  // 소켓 생성 및 바인딩
        if (serverFd == -1) {
            System.err.println("서버 소켓 생성에 실패했습니다.");
            return;
        }
        System.out.println("서버가 포트 " + port + "에서 시작되었습니다.");

        while (true) {
            int clientFd = acceptConnection(serverFd);  // 클라이언트 연결 수락
            if (clientFd != -1) {
                System.out.println("클라이언트 연결 수락됨. FD: " + clientFd);
                // `acceptConnection`에서 이미 스레드를 생성하므로 별도 통신 메서드 호출 불필요
            }
        }
    }

    public static void main(String[] args) {
        CustomSocketServer server = new CustomSocketServer();
        server.startServer(8080);  // 서버 시작
    }
}