#include "Syncronization.h"

/**
 * @brief Initialize the frame queue.
 * @param queue The frame queue to initialize.
 */
void FFmpegFrameQueue_create(FFmpegFrameQueue* queue, int maxSize)
{
    // Create a new FFmpegContext struct.
    queue->size = 0;
    queue->readIndex = 0;
    queue->writeIndex = 0;
    queue->maxSize = maxSize;

    // Allocate space for the frame pointers.
    queue->queue = new AVFrame*[maxSize];

    // Do NOT initialize mutex and cond.
}

/**
 * @brief Push a frame onto the queue.
 *        This is done in a thread-safe manner.
 * 
 * @param queue The queue to push the frame onto.
 * @param frame The frame to push onto the queue.
 */
void FFmpegFrameQueue_push(FFmpegFrameQueue* queue, AVFrame *frame)
{
    // Wait until we have access to the queue.
    queue->mutex.lock();

    // Wait until the queue has space for a new frame (free the mutex while we wait).
    std::unique_lock<std::mutex> lck(queue->mutex);
    queue->cond.wait(lck, [queue] { return queue->size < queue->maxSize; });

    // The mutex is locked again, and the queue is ready for a new frame.

    // Push the frame onto the queue.
    queue->queue[queue->writeIndex] = frame;
    queue->writeIndex = (queue->writeIndex + 1) % queue->maxSize;
    queue->size++;

    // Signal that the queue is not empty.
    queue->cond.notify_one();

    // Release the mutex.
    queue->mutex.unlock();
}

/**
 * @brief Pop a frame from the queue.
 *        This is done in a thread-safe manner.
 * 
 * @param queue The queue to pop from.
 * @return The popped frame.
 */
AVFrame* FFmpegFrameQueue_pop(FFmpegFrameQueue* queue)
{
    // Wait until we have access to the queue.
    queue->mutex.lock();

    // Wait until the queue has a frame to read (free the mutex while we wait).
    std::unique_lock<std::mutex> lck(queue->mutex);
    queue->cond.wait(lck, [queue] { return queue->size > 0; });

    // The mutex is locked again, and the queue is ready for a new frame.
    AVFrame *frame = queue->queue[queue->readIndex];
    queue->readIndex = (queue->readIndex + 1) % queue->maxSize;
    queue->size--;

    // Signal that the queue is not empty.
    queue->cond.notify_one();
    queue->mutex.unlock();
    return frame;
}

/**
 * @brief Retrieve the size of the queue.
 *        This is done in a thread-safe manner.
 * 
 * @param queue The FFmpegFrameQueue to query.
 * @return The size of the queue.
 */
int FFmpegFrameQueue_size(FFmpegFrameQueue* queue)
{
    // Wait until we have access to the queue.
    queue->mutex.lock();
    int size = queue->size;
    queue->mutex.unlock();
    return size;
}