// 워커 스레드 로직
// 무작위 계좌 간 1,000원씩 이체하는 로직

#include <stdlib.h>
#include <unistd.h>
#include "transfer.h"
#include "db_layer.h"

#define TRANSFER_AMOUNT 1000

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

    // 💡 [3단계: 데드락 예방 핵심 로직 - 자원 순서 정렬] 💡
    // 입출금 여부에 상관없이, 무조건 ID가 작은 계좌 번호와 큰 계좌 번호를 먼저 분류
    int first_lock = (from_acc < to_acc) ? from_acc : to_acc;
    int second_lock = (from_acc > to_acc) ? from_acc : to_acc;

    // 1. 번호가 작은 계좌부터 잠금 (순서 강제)
    pthread_mutex_lock(db_get_lock(first_lock));

    // 데드라 유발용 딜레이를 그대로 둬도, 이제 멈추지 않음
    usleep(1000); // 1밀리초

    // 2. 번호가 큰 계좌 잠금
    pthread_mutex_lock(db_get_lock(second_lock));

    // --- 임계 구역(Critical Section) 진입 ---
    // 주의: 실제 잔액 업데이트는 first/second가 아닌, 입출금 목적에 맞게 수행
    db_update_balance(from_acc, -TRANSFER_AMOUNT); // 출금
    db_update_balance(to_acc, TRANSFER_AMOUNT);    // 입금

    // 3. 잠금 해제 (역순이 일반적: 논리적 대칭성과 구조적 안정성)
    pthread_mutex_unlock(db_get_lock(second_lock));
    pthread_mutex_unlock(db_get_lock(first_lock));

    return NULL;
}