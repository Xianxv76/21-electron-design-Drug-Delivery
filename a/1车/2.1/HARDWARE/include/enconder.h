#ifndef _ENCONDER_H_
#define _ENCONDER_H_

#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

void Enconder_init(void);
void direction_init(void);
void left_positive(void);
void left_forward(void);
void right_positive(void);
void right_forward(void);

#endif


