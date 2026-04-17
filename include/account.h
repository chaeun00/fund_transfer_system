// 헤더가드. 다수의 '*.c' 파일에서의 중복 호출을 방지한다. `#pragma ones`로 대체 가능
#ifndef ACCOUNT_H 
#define ACCOUNT_H

#include <pthread.h>

// 계좌 구조체 정의
typedef struct
{
    int account_id;         // 계좌 번호
    int balance;            // 잔액
    pthread_mutex_t lock;   // 이 Row(계좌) 전용 자물쇠(뮤텍스)
} Account;

#endif