#include "Syncronization.h"

/**
 * @brief Initialize the frame queue.
 * @param queue The frame queue to initialize.
 */
FFmpegFrameQueue *FFmpegFrameQueue_create(int maxSize)
{
    // printf("Initializing frame queue with size %d\n", maxSize);

    FFmpegFrameQueue *queue = new FFmpegFrameQueue;

    // Create a new FFmpegContext struct.
    queue->size = 0;
    queue->readIndex = 0;
    queue->writeIndex = 0;
    queue->maxSize = maxSize;

    // printf("Allocating frame pointer array... %d\n", maxSize);
    // Allocate space for the frame pointers.
    queue->queue = new AVFrame *[maxSize];
    // printf("Allocated frame pointer array.");

    // Do NOT initialize mutex and cond.

    return queue;
}

/**
 * @brief Push a frame onto the queue.
 *        This is done in a thread-safe manner.
 *
 * @param queue The queue to push the frame onto.
 * @param frame The frame to push onto the queue.
 * @param force Whether to force the frame onto the queue, even if it is full.
 */
void FFmpegFrameQueue_push(FFmpegFrameQueue *queue, AVFrame *frame, bool force)
{
    // Wait until we have access to the queue.
    // printf("Attempting frame queue push...\n");

    // Wait until the queue has space for a new frame.
    // printf("WAITING to push frame to queue.\n");

    // Rather than calling mutex.lock directly, use a unique_lock.
    // This allows us to use the RAII idiom to automatically unlock the mutex.
    // RAII means construction will lock the mutex, and unlock it when it leaves scope.
    // (lock_guard is not used because it is incompatible with condition variables.)
    std::unique_lock<std::mutex> lck(queue->mutex);

    // printf("WAITING to push frame to queue.\n");

    // We use a condition variable to wait until the queue has space for a new frame,
    // unlocking the mutex while we wait.
    queue->cond.wait(lck, [queue, force]
                     { return queue->size < queue->maxSize || force; });

    // printf("READY to push frame to queue.\n");

    // At this point, there is space in the queue AND the mutex is locked,
    // so we can safely push the frame.

    if (force && queue->size == queue->maxSize) {
        // Forcibly push the frame onto the queue, destroying the first frame.
        printf("Forcibly pushing frame onto queue.\n");
        queue->queue[queue->writeIndex] = frame;
        queue->writeIndex = (queue->writeIndex + 1) % queue->maxSize;
        // DON'T increment the size.
    } else {
        // Push the frame onto the queue normally.
        queue->queue[queue->writeIndex] = frame;
        queue->writeIndex = (queue->writeIndex + 1) % queue->maxSize;
        queue->size++;
    }

    // Signal that the queue is not empty.
    queue->cond.notify_one();
}

/**
 * @brief Pop a frame from the queue. If no data is available, wait until data is available.
 *        This is done in a thread-safe manner.
 *
 * @param queue The queue to pop from.
 * @return The popped frame.
 */
AVFrame *FFmpegFrameQueue_pop(FFmpegFrameQueue *queue)
{
    // Wait until we have access to the queue.
    // printf("Attempting frame queue pop...\n");

    // printf("WAITING to pop frame from queue.\n");

    // Create a unique_lock to lock the mutex within the current scope.
    // Wait until the queue has a frame to read (free the mutex while we wait).
    std::unique_lock<std::mutex> lck(queue->mutex);
    queue->cond.wait(lck, [queue]
                     { return queue->size > 0; });

    // printf("READY to pop frame from queue.\n");

    // The mutex is locked again, and the queue is ready for a new frame.
    AVFrame *frame = queue->queue[queue->readIndex];
    queue->readIndex = (queue->readIndex + 1) % queue->maxSize;
    queue->size--;

    // Signal that the queue is not full.
    queue->cond.notify_one();
    queue->mutex.unlock();
    return frame;
}

/**
 * @brief Pop a frame from the queue. If no data is available, return NULL.
 *        This is done in a thread-safe manner.
 *
 * @param queue The queue to pop from.
 * @return The popped frame.
 */
AVFrame *FFmpegFrameQueue_tryPop(FFmpegFrameQueue *queue)
{
    // Wait until we have access to the queue.
    queue->mutex.lock();

    AVFrame *frame = NULL;

    // If the queue is empty, return NULL.
    if (queue->size > 0)
    {
        // Otherwise, pop the frame.
        frame = queue->queue[queue->readIndex];
        queue->readIndex = (queue->readIndex + 1) % queue->maxSize;
        queue->size--;
    }

    // Signal that the queue is not full.
    queue->cond.notify_one();
    queue->mutex.unlock();
    return frame;
}

/**
 * @brief Fetch the current frame of the queue without popping it.
 *        This is done in a thread-safe manner.
 */
AVFrame *FFmpegFrameQueue_peek(FFmpegFrameQueue *queue)
{
    // Create a unique_lock to lock the mutex within the current scope.
    // Wait until the queue has a frame to read (free the mutex while we wait).
    std::unique_lock<std::mutex> lck(queue->mutex);
    queue->cond.wait(lck, [queue]
                     { return queue->size > 0; });

    // The mutex is locked again, and the queue is ready for a new frame.
    AVFrame *frame = queue->queue[queue->readIndex];
    // DON'T update the read index or size.

    // Signal that the queue is not empty.
    queue->cond.notify_one();
    queue->mutex.unlock();
    return frame;
}
