#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int burstTime;
    int remainingTime;
} Process;

void rrobin() {
    int totalRemainingTime = 0, currentTime = 0, completed = 0;
    int count = 0, timeQuantum = 0;
    printf("roundrobin에 scheduling 할 process 개수를 입력하세요 : ");
    scanf("%d", &count);
    printf("process에 할당할 최대 시간을 입력하세요 : ");
    scanf("%d", &timeQuantum);

    Process processes[count];

    for (int i = 0; i < count; i++) {
        printf("process %d 가 명령을 완수하는데 필요한 실행 시간을 입력하세요 : ", i + 1);
        scanf("%d", &processes[i].burstTime);
    }
    for (int i = 0; i < count; i++)
    {
        processes[i].remainingTime = processes[i].burstTime;
        totalRemainingTime += processes[i].burstTime;
    }
    while (totalRemainingTime > 0)
    {
        for (int i = 0; i < count; i++)
        {
            if (processes[i].remainingTime > 0 )    {
                if (processes[i].remainingTime > timeQuantum)   {
                    currentTime += timeQuantum;
                    processes[i].remainingTime -= timeQuantum;
                    totalRemainingTime -= timeQuantum;
                }
                else {
                    currentTime += processes[i].remainingTime;
                    totalRemainingTime -= processes[i].remainingTime;
                    processes[i].remainingTime = 0;
                    completed++;
                    printf("Process %d is completed at %d time\n", processes[i].id, currentTime);
                }
            }
        }
        
    }
    
    
}
