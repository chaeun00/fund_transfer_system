// 워커 스레드 로직
// 무작위 계좌 간 1,000원씩 이체하는 로직

#include <stdlib.h>
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

    // 이체 로직
    db_update_balance(from_acc, -TRANSFER_AMOUNT); // 출금
    db_update_balance(to_acc, TRANSFER_AMOUNT);    // 입금

    return NULL;
}