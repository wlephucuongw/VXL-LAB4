/*
 * scheduler.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Administrator
 */

#include "scheduler.h"
sTasks SCH_task_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_Init(void){
	unsigned char i;
	for(i = 0; i < SCH_MAX_TASKS; i++){
		SCH_Delete_Tasks(i);
	}
}
void SCH_Add_Task(void (*pFuntion)(), uint32_t DELAY, uint32_t PERIOD){
	if(current_index_task < SCH_MAX_TASKS){
		SCH_task_G[current_index_task].pTask = pFuntion;
		SCH_task_G[current_index_task].Delay = DELAY;
		SCH_task_G[current_index_task].Period = PERIOD;
		SCH_task_G[current_index_task].RunMe = 0;
		SCH_task_G[current_index_task].TaskID = current_index_task;
		current_index_task++;
	}
}
void SCH_Update(void){
	for(int i = 0; i < current_index_task; i++){
		if(SCH_task_G[i].Delay > 0){
			SCH_task_G[i].Delay--;
		}
		else {
			SCH_task_G[i].Delay = SCH_task_G[i].Period;
			SCH_task_G[i].RunMe += 1;
		}
	}
}
void SCH_Dispatch_Tasks(void){
	for(int i=0; i < current_index_task; i++){
		if(SCH_task_G[i].RunMe > 0){
			SCH_task_G[i].RunMe--;
			(*SCH_task_G[i].pTask)();
			if(SCH_task_G[i].Period == 0){
				SCH_Delete_Tasks(i);
			}
		}

	}
}
uint8_t SCH_Delete_Tasks(uint32_t taskID){
	if(current_index_task == 0){
		return;
	}
	if(taskID > current_index_task - 1 || taskID < 0){
		return -1;
	}
	if(current_index_task > 1){
	for(int i=taskID; i < current_index_task - 1; i++){
		SCH_task_G[i].pTask = SCH_task_G[i+1].pTask;
		SCH_task_G[i].Delay = SCH_task_G[i+1].Delay;
		SCH_task_G[i].Period = SCH_task_G[i+1].Period;
		SCH_task_G[i].RunMe = SCH_task_G[i+1].RunMe;
		SCH_task_G[i].TaskID = SCH_task_G[i+1].TaskID;
		}
	}
    SCH_task_G[current_index_task - 1].pTask = nullptr;
    SCH_task_G[current_index_task - 1].Delay = 0;
    SCH_task_G[current_index_task - 1].Period = 0;
    SCH_task_G[current_index_task - 1].RunMe = 0;
    SCH_task_G[current_index_task - 1].TaskID = 0;
	current_index_task--;
	return 1;
}
