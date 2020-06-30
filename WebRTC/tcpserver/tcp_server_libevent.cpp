
#include <iostream>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <arpa/inet.h>

// /usr/libevent/include/ -L/usr/libevent/lib -levent

const int PORT = 8111;

void on_read_cb(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *input = NULL;
    struct evbuffer *output = NULL;
    input = bufferevent_get_input(bev);
    output = bufferevent_get_output(bev);
    evbuffer_add_buffer(output, input); //将输入的数据添加到输出
}

//回调函数
void on_accept_cb(struct evconnlistener *listener,
                  evutil_socket_t fd,
                  struct sockaddr *addr,
                  int socklen,
                  void *ctx)
{
    struct bufferevent *bev = NULL;
    struct event_base *base = NULL;

    base = evconnlistener_get_base(listener);
    bev = bufferevent_socket_new(base, fd, 0);

    bufferevent_enable(bev, EV_READ | EV_WRITE);          //读事件和写事件
    bufferevent_setcb(bev, on_read_cb, NULL, NULL, NULL); //如果有读事件，调用on_read_cb
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serveraddr;
    struct event_base *base = NULL;
    struct evconnlistener *listenner = NULL;

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    base = event_base_new();
    listenner = evconnlistener_new_bind(base,
                                        on_accept_cb,      //回调函数
                                        NULL,              //回调函数的参数
                                        LEV_OPT_REUSEABLE, //可重用
                                        10,                //1毫秒可以有十个并发申请
                                        (struct sockaddr *)&serveraddr,
                                        sizeof(serveraddr));
    if (!listener)
    {
        perror("Couldn't create listener");
        return 1;
    }
    /* 设置Listen错误回调函数 */
    evconnlistener_set_error_cb(listener, accept_error_cb);

    event_base_dispatch(base);

    return 0;
}