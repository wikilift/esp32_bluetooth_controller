#pragma once

void read_buttons(void *pVp);
void read_axis(void *pVp);
void read_hat(void *pVp);
void button_isr_handler(void* arg);