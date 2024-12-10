// Copyright (C) 2024 wwhai
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <pthread.h>
#include <stdio.h>

#define QUEUE_SIZE 10

typedef struct
{
    AVFrame *frame;
} FrameQueueData;

typedef struct
{
    FrameQueueData data[QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} FrameQueue;

// 初始化队列
void queue_init(FrameQueue *q)
{
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
}

// 销毁队列
void queue_destroy(FrameQueue *q)
{
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_full);
    pthread_cond_t not_empty;
}

// 入队操作
void queue_push(FrameQueue *q, FrameQueueData item)
{
    pthread_mutex_lock(&q->mutex);
    while (q->count == QUEUE_SIZE)
    {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }
    q->data[q->rear] = item;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->count++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}

// 出队操作
FrameQueueData queue_pop(FrameQueue *q)
{
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0)
    {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    FrameQueueData item = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return item;
}

// 拉流和解码线程函数
void *rtsp_streaming_thread(void *arg)
{
    AVFormatContext *format_ctx = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVCodec *codec = NULL;
    AVPacket packet;
    AVFrame *frame = NULL;
    FrameQueue *queue = (FrameQueue *)arg;

    // 打开 RTSP 流
    avformat_open_input(&format_ctx, "rtsp://your_rtsp_url", NULL, NULL);
    avformat_find_stream_info(format_ctx, NULL);

    // 找到视频流
    int video_stream_index = -1;
    for (int i = 0; i < format_ctx->nb_streams; ++i)
    {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            break;
        }
    }

    codec = avcodec_find_decoder(format_ctx->streams[video_stream_index]->codecpar->codec_id);
    codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, format_ctx->streams[video_stream_index]->codecpar);
    avcodec_open2(codec_ctx, codec, NULL);

    av_init_packet(&packet);

    while (av_read_frame(format_ctx, &packet) >= 0)
    {
        if (packet.stream_index == video_stream_index)
        {
            avcodec_send_packet(codec_ctx, &packet);
            if (frame == NULL)
            {
                frame = av_frame_alloc();
            }
            if (avcodec_receive_frame(codec_ctx, frame) == 0)
            {
                FrameQueueData data;
                data.frame = av_frame_clone(frame);
                queue_push(queue, data);
            }
        }
        av_packet_unref(&packet);
    }

    // 清理资源
    av_frame_free(&frame);
    avcodec_close(codec_ctx);
    avformat_close_input(&format_ctx);

    return NULL;
}

// 识别线程函数
void *recognition_thread(void *arg)
{
    FrameQueue *input_queue = (FrameQueue *)arg;
    FrameQueue *output_queue; // 假设另一个队列
    FrameQueueData data;

    while (1)
    {
        data = queue_pop(input_queue);
        // 进行识别处理，例如调用 OpenCV 或其他识别库
        // 这里需要将识别结果添加到帧上，可以是绘制框或添加文本等
        queue_push(output_queue, data);
    }

    return NULL;
}

// 编码和播放线程函数
void *encoding_playback_thread(void *arg)
{
    FrameQueue *queue = (FrameQueue *)arg;
    FrameQueueData data;

    while (1)
    {
        data = queue_pop(queue);
        // 进行编码和播放操作，例如使用 libavcodec 和 SDL 进行播放
        av_frame_free(&data.frame);
    }

    return NULL;
}

int main()
{
    FrameQueue queue;
    queue_init(&queue);

    pthread_t rtsp_thread, recognition_thread, encoding_thread;

    pthread_create(&rtsp_thread, NULL, rtsp_streaming_thread, &queue);
    pthread_create(&recognition_thread, NULL, recognition_thread, &queue);
    pthread_create(&encoding_thread, NULL, encoding_playback_thread, &queue);

    pthread_join(rtsp_thread, NULL);
    pthread_join(recognition_thread, NULL);
    pthread_join(encoding_thread, NULL);

    queue_destroy(&queue);

    return 0;
}