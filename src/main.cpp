#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>

#define arrayCount(x) ((sizeof(x) / sizeof(x[0])))
#define THREAD_COUNT 10
#define N_ITEMS 15

struct List
{
	uint32_t currentIndex = 0;
	uint32_t data[N_ITEMS] = {};
};

void* threadFunc(void* userData)
{
	List* sharedList = (List*) userData;

	for(;;) {
		if (sharedList->currentIndex >= N_ITEMS) {
			break;
		}		
		printf("Processing item with index %d: %d\n", sharedList->currentIndex, sharedList->data[sharedList->currentIndex]);
		sharedList->currentIndex++;
	}
	pthread_exit(nullptr);
}

int main()
{
	pthread_t threadHandles[THREAD_COUNT] = {};
	List list = {};
	for(uint32_t i = 0; i < arrayCount(list.data); ++i)
	{
		list.data[i] = i;
	}

	pthread_attr_t attributes;
	int32_t result = pthread_attr_init(&attributes);
	assert(result == 0);

	for (uint32_t i = 0; i < THREAD_COUNT; ++i)
	{
		result = pthread_create(&threadHandles[i], &attributes, threadFunc, &list);
		assert(result == 0);
	}

	result = pthread_attr_destroy(&attributes);
	assert(result == 0);

	for (uint32_t i = 0; i < THREAD_COUNT; ++i)
	{
		result = pthread_join(threadHandles[i], nullptr);
		assert(result == 0);
	}

	return 0;
}

// Example race condition output:
/*
Processing item with index 0: 0
Processing item with index 1: 1
Processing item with index 2: 2
Processing item with index 3: 3
Processing item with index 4: 4
Processing item with index 5: 5
Processing item with index 6: 6
Processing item with index 4: 4
Processing item with index 7: 7
Processing item with index 8: 8
Processing item with index 9: 9
Processing item with index 10: 10
Processing item with index 11: 11
Processing item with index 12: 12
Processing item with index 13: 13
Processing item with index 14: 14
Processing item with index 6: 6
*/