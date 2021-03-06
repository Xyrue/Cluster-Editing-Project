/*Queue has five properties. capacity stands for the maximum number of elements Queue can hold.
  Size stands for the current size of the Queue and elements is the array of elements. front is the
 index of first element (the index at which we remove the element) and rear is the index of last element
 (the index at which we insert the element) */
typedef struct Queue{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements;
}Queue;


/* crateQueue function takes argument the maximum number of elements the Queue can hold, creates
   a Queue according to it and returns a pointer to the Queue. */
Queue * CreateQueue(int);
void Dequeue(Queue *);
int Front(Queue *Q);
void Enqueue(Queue *,int);