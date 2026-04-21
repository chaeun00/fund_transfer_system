// 워커 스레드 로직
// 무작위 계좌 간 1,000원씩 이체하는 로직

#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h> // [4단계]: 세마포어 헤더 추가
#include "transfer.h"
#include "db_layer.h"

#define TRANSFER_AMOUNT 1000

extern sem_t db_pool; // main.c에서 만든 세마포어 가져오기

void* transfer_worker(void* arg)
{
    // 무작위로 입출금 계좌 선택
    int from_acc = rand() % NUM_ACCOUNTS;
    int to_acc = rand() % NUM_ACCOUNTS;

    // 입출금 계좌가 같으면 다른 계좌 선택
    while (from_acc == to_acc)
    {
        to_acc = rand() % NUM_ACCOUNTS;
    }

    int first_lock = (from_acc < to_acc) ? from_acc : to_acc;
    int second_lock = (from_acc > to_acc) ? from_acc : to_acc;

    // 💡 DB 커넥션 풀 진입 대기 (세마포어 획득)
    // 빈자리가 10개 미만이면 바로 들어가고, 꽉 찼으면 여기서 대기.
    sem_wait(&db_pool);

    pthread_mutex_lock(db_get_lock(first_lock));
    pthread_mutex_lock(db_get_lock(second_lock));

    // --- 임계 구역(Critical Section) 진입 ---
    // 주의: 실제 잔액 업데이트는 first/second가 아닌, 입출금 목적에 맞게 수행
    db_update_balance(from_acc, -TRANSFER_AMOUNT); // 출금
    db_update_balance(to_acc, TRANSFER_AMOUNT);    // 입금

    // 잠금 해제 (역순이 일반적: 논리적 대칭성과 구조적 안정성)
    pthread_mutex_unlock(db_get_lock(second_lock));
    pthread_mutex_unlock(db_get_lock(first_lock));

    // 💡 DB 커넥션 반납 (세마포어 해제)
    // 작업이 끝났으니, 밖에서 대기하던 다른 스레드가 들어올 수 있게 함.
    sem_post(&db_pool);

    return NULL;
}