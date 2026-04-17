# 전체 프로젝트 빌드 및 pthread 링킹 설정

# 1. 변수(매크로) 선언부
# 사용할 컴파일러를 gcc로 설정
CC = gcc

# 컴파일 옵션
# -Wall 옵션으로 모든 경고를 띄움
# -g 옵션으로 디버깅 정보를 포함
# -I 옵션으로 헤더 파일 위치(./include) 지정
# -pthread로 멀티스레딩을 위한 POSIX 라이브러리 연결
CFLAGS = -Wall -g -I./include -pthread

# 최종적으로 만들어질 실행 파일의 이름을 지정
TARGET = transfer_system

# 소스 파일들 목록
SRCS = src/main.c src/transfer.c src/db_mock.c

# SRCS 변수에 있는 파일 이름들에서 .c를 찾아 전부 .o(Object 파일, 즉 컴파일된 부품)로 바꾼 목록을 만듦
# 결과적으로 OBJS는 src/main.o src/transfer.o src/db_mock.o가 됨
OBJS = $(SRCS:.c=.o)

# 2. 빌드 타겟 규칙부 (의존성과 명령어)
# 기본 타겟. `make`라고만 쳤을 때, 가장 먼저 실행되는 목표. 즉, 최종 실행 파일(TARGET)을 만들어라
all: $(TARGET)

# 타겟 빌드 규칙
# 최종 링크(Link) 단계: 최종 실행 파일(transfer_system)을 만들려면 부품 파일들(main.o, transfer.o, db_mock.o)이 필요하다
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 개별 소스 파일 빌드 규칙
# 모든 .o 파일은 이름이 같은 .c 파일로부터 만들어짐. 소스 코드를 하나하나의 부품(.o)으로 컴파일
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 빌드 결과물 정리 (make clean). 빌드 과정에서 생긴 찌꺼기 파일들을 지움
# 생성되었던 .o 부품 파일들과 최종 실행 파일을 모두 강제로(-f) 삭제하여 초기 상태로 되돌림
clean:
	rm -f $(OBJS) $(TARGET)