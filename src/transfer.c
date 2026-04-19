// 워커 스레드 로직
// 무작위 계좌 간 1,000원씩 이체하는 로직

#include <stdlib.h>
#include <unistd.h>
#include "transfer.h"
#include "db_layer.h"

#define TRANSFER_AMOUNT 1000

void* transfer_worker(void* arg)
{
    // 무작위로 입춝금 계좌 선택
    int from_acc = rand() % NUM_ACCOUNTS;
    int to_acc = rand() % NUM_ACCOUNTS;

    // 입출금 계좌가 같으면 다른 계좌 선택
    while (from_acc == to_acc)
    {
        to_acc = rand() % NUM_ACCOUNTS;
    }

    // 💣 [데드락 유발 지점] 💣
    // 순진하게 "출금 계좌 먼저 잠그고, 입금 계좌 잠그자"라는 로직.

    // 1. 출금 계좌 잠금
    pthread_mutex_lock(db_get_lock(from_acc));

    // 🚨 데드락 확률을 극대화하기 위한 의도적 딜레이
    // 출금 계좌를 잠근 상태에서 다른 스레드에게 CPU를 넘겨줌.
    usleep(1000); // 1밀리초

    // 2. 입금 계좌 잠금
    pthread_mutex_lock(db_get_lock(to_acc));

    // --- 임계 구역(Critical Section) 진입 ---
    db_update_balance(from_acc, -TRANSFER_AMOUNT); // 출금
    db_update_balance(to_acc, TRANSFER_AMOUNT);    // 입금

    // 3. 잠금 해제 (역순이 일반적: 논리적 대칭성과 구조적 안정성)
    pthread_mutex_unlock(db_get_lock(to_acc));
    pthread_mutex_unlock(db_get_lock(from_acc));

    return NULL;
}