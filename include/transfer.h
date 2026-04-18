// 스레드가 실행할 워커(Worker) 함수의 선언부

// 헤더가드. 다수의 '*.c' 파일에서의 중복 호출을 방지한다. `#pragma ones`로 대체 가능
#ifndef TRANSFER_H
#define TRANSFER_H

void* transfer_worker(void* arg);

#endif