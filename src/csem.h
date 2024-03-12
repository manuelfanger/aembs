
#ifndef CSEM_H_
#define CSEM_H_

void BossTask(void *pv);

void WorkerTask(void *pv);

void Csem_Init(void);

void Csem_Deinit(void);

#endif