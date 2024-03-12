#ifndef SEMLED_H_
#define SEMLED_H_

static void SemLedTaskRed(void *pv);

static void SemLedTaskGreen(void *pv);

static void SemLedTaskBlue(void *pv);

void SemLed_Init(void);

void SemLed_Deinit(void);

#endif