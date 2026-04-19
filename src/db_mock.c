// 실제 메모리(배열)에 데이터를 저장하고 수정하는 로직.
// [2단계] 뮤텍스 초기화 로직 추가

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
        pthread_mutex_init(&accounts[i].lock, NULL); // 각 계좌(Row)마다 고유한 Mutex 생성
    }
}

int db_get_balance(int account_id)
{
    return accounts[account_id].balance;
}

void db_update_balance(int account_id, int amount)
{
    int current_balance = accounts[account_id].balance;
    for (volatile int i = 0; i < 10000; i++); // 지연 시간 유지해도 Race Condition을 락이 막아줄 것
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

pthread_mutex_t* db_get_lock(int account_id)
{
    // 외부 스레드가 특정 계좌의 자물쇠를 얻을 수 있도록 포인터 반환
    return &accounts[account_id].lock;
}