import socket

# 서버 IP 주소와 포트 번호
server_ip = "127.0.0.1"  # 로컬 호스트 (Java 서버가 같은 컴퓨터에서 실행 중이라고 가정)
server_port = 8080       # Java 서버가 대기 중인 포트 번호

# 소켓 객체 생성
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 서버에 연결 요청
client_socket.connect((server_ip, server_port))
print("서버에 연결되었습니다.")

try:
    while True:
        # 사용자로부터 메시지 입력
        message = input("서버에 보낼 메시지를 입력하세요 (종료하려면 'EXIT' 입력): ")

        # EXIT를 입력하면 연결 종료
        if message.upper() == "EXIT":
            print("서버 연결을 종료합니다.")
            break

        # 서버에 메시지 전송
        client_socket.sendall(message.encode('utf-8'))
        print(f"서버로 '{message}' 메시지를 전송했습니다.")

        # 서버로부터 에코 메시지 수신
        response = client_socket.recv(1024).decode('utf-8')
        print(f"서버로부터 수신한 응답: {response}")

finally:
    # 연결 종료
    client_socket.close()
    print("클라이언트 연결이 종료되었습니다.")