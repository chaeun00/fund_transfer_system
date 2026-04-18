// 실제 메모리(배열)에 데이터를 저장하고 수정하는 로직.
// 의도적으로 동기화를 하지 않아 문제가 발생하도록 둠.

#include <stdio.h>
#include "db_layer.h"

// 전역 계좌 배열 (공유 자원)
Account accounts[NUM_ACCOUNTS];

void db_init()
{
    for (int i = 0; i < NUM_ACCOUNTS; i++)
    {
        accounts[i].account_id = i;
        accounts[i].balance = INITIAL_BALANCE;
        // 1단계에서는 뮤텍스(lock) 초기화 생략.
    }
}

int db_get_balance(int account_id)
{
    return accounts[account_id].balance;
}

void db_update_balance(int account_id, int amount)
{
    // [Race Condition 발생 지점]
    // 여러 스레드가 동시에 이 코드를 실행하면 잔액이 유실.

    int current_balance = accounts[account_id].balance;

    // CPU가 다른 스레드로 컨텍스트 스위칭(Context Switching)을 하도록
    // 고의로 약간의 지연 시간을 줌. (DB 통신 지연 모사)
    for (volatile int i = 0; i < 10000; i++);

    accounts[account_id].balance = current_balance + amount;
}

void db_print_total_balance()
{
    long long total = 0;
    for (int i = 0; i < NUM_ACCOUNTS; i++)
    {
        printf("계좌 [%d] 잔액: %d원\n", accounts[i].account_id, accounts[i].balance);
        total += accounts[i].balance;
    }
    printf("=== 시스템 총 잔액: %lld원 ===\n", total);
}