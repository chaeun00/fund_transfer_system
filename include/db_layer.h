// DB 접근 함수 선언 (get_balance, update_balance 등)

// 헤더가드. 다수의 '*.c' 파일에서의 중복 호출을 방지한다. `#pragma ones`로 대체 가능
#ifndef DB_LAYER_H
#define DB_LAYER_H

#include "account.h"

#define NUM_ACCOUNTS 5    // 테스트용 계좌 개수
#define INITIAL_BALANCE 10000 // 초기 잔액

void db_init();
int db_get_balance(int account_id);
void db_update_balance(int account_id, int new_balance);
void db_print_total_balance();
pthread_mutex_t* db_get_lock(int account_id); // 각 계좌의 뮤텍스 락을 얻는 함수

#endif