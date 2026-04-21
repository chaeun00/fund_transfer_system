// 메인 함수 (스레드 생성, 세마포어 초기화 등 시스템 오케스트레이션)
// [4단계]: 세마포어를 통한 DB 풀 관리

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include "db_layer.h"
#include "transfer.h"

#define NUM_THREADS 5000    // 동시에 수행할 이체 요청 수
#define POOL_SIZE 10        // 최대 동시 DB 접근 스레드 수 제한. Hikari CP의 기본값으로 설정.

sem_t db_pool;              // 전역 세마포어 선언

int main(void)
{
    srand(time(NULL));
    pthread_t threads[NUM_THREADS];
    struct timespec start, end; // 시간 측정을 위한 구조체

    printf("[4단계]: %d개의 커넥션 풀, %d개 동시 접속 스레드의 펀드 이체 시스템 시작\n", POOL_SIZE, NUM_THREADS);
    printf("DB 초기화 중...\n");
    db_init();
    printf("DB Pool 초기화 중...\n\n");
    sem_init(&db_pool, 0, POOL_SIZE); // 세마포어 초기화 (두 번째 인자 0은 '현재 프로세스 내 스레드 간 공유'를 의미)

    printf("=== 이체 전 상태 ===\n");
    db_print_total_balance();
    clock_gettime(CLOCK_MONOTONIC, &start); // 타이머 시작

    printf("\n%d개의 이체 스레드 생성 및 실행중...\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&threads[i], NULL, transfer_worker, NULL) != 0)
        {
            perror("에러!!! 스레드 생성 실패");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end); // 타이머 종료
    printf("\n=== 이체 완료 후 상태 ===\n");
    db_print_total_balance();

    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("\n총 소요 시간: %.4f초\n", time_taken);

    sem_destroy(&db_pool); // 세마포어 자원 반환
    return 0;
}