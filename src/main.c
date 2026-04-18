// 메인 함수 (스레드 생성, 세마포어 초기화 등 시스템 오케스트레이션)
// 1단계에서는 여러 스레드 동시 수행만

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "db_layer.h"
#include "transfer.h"

#define NUM_THREADS 5000 // 동시에 수행할 이체 요청 수

int main(void)
{
    srand(time(NULL)); // 랜덤 시드 초기화
    pthread_t threads[NUM_THREADS];

    printf("[1단계] 멀티스레드 이체 시스템 시작\n");
    printf("DB 초기화 중...\n\n");
    db_init(); // DB 초기화

    printf("=== 이체 전 상태 ===\n");
    db_print_total_balance(); // 초기 잔액 출력

    printf("\n%d개의 이체 스레드 생성 및 실행중...\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) // 스레드 생성
    {
        pthread_create(&threads[i], NULL, transfer_worker, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) // 모든 스레드가 끝날 때까지 대기
    {
        pthread_join(threads[i], NULL);
    }

    printf("\n=== 이체 완료 후 상태 ===\n");
    db_print_total_balance(); // 시스템 총 잔액이 초기 총 잔액과 일치하는가 확인
    // 일치하지 않을 확률이 매우 높다. 동기화 과정이 없었기 때문.

    return 0;
}