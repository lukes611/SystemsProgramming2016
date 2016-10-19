#ifndef LQ_H
#define LQ_H

typedef struct _LQNode
{
	struct _LQNode * next;
	void * data;
} LQNode;

typedef struct
{
	int size;
	LQNode * head;
} LQ;

LQ newLQ()
{
	LQ ret;
	ret.size = 0;
	ret.head = NULL;
	return ret;
}

void destroyLQ(LQ * self)
{
	LQNode * ptr = self->head;
	while(ptr != NULL)
	{
		LQNode * temp = ptr;
		ptr = ptr->next;
		free(temp);
	}
}

int LQIsEmpty(LQ * self)
{
	return self->size <= 0;
}

void LQPush(LQ * self, void * data)
{
	LQNode * node = (LQNode*)malloc(sizeof(LQNode));
	node->data = data;
	node->next = NULL;
	
	

	if(LQIsEmpty(self)){
		self->head = node;
	}else{
		LQNode * ptr = self->head;
		while(ptr->next != NULL) ptr = ptr->next;
		ptr->next = node;
	}
	self->size++;
}

void * LQPop(LQ * self)
{
	if(LQIsEmpty(self)) return NULL;
	LQNode * front = self->head;

	self->head = self->head->next;
	self->size--;
	return front->data;
}




#endif