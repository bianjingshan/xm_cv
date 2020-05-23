#pragma once

void* XMMsgCreate(void);
bool XMMsgSend(void *p_msg, void *p_buffer, int msg_size);
bool XMMsgRecv(void *p_msg, void *p_buffer, int msg_size);



