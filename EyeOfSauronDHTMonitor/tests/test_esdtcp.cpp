//
//  test_esdtcp.cpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 4/19/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#include "test_esdtcp.hpp"
#include "ESDTCP.hpp"
#include "ThreadPool.hpp"
#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

namespace test_esdtcp {
    
    int main1();
    
    void test_esdtcp(){
        
        
        
        threadPool::Thread thread;
        thread.startThread([]{
            esdht::ESDTcp tcp;
            tcp.receive("10.12.112.55", 6882, [&tcp](std::string msg, uv_stream_t *stream){
                printf("I am Server:%s\n", msg.c_str());
                tcp.response("ni hao woshi wang da chui", stream, nullptr);
            });
        });
        
        for(int i = 0 ; i < 1; i++){
            esdht::ESDTcp sender;
            sender.send("10.12.112.55", 6882, "ss", nullptr, nullptr);
            usleep(1000*500);
        }
//         main1();
    }
    
    
    
    
    
    
    
//#include <stdio.h>
//#include <stdlib.h>
//#include <uv.h>
//    
//    // uv_handle_t > uv_stream_t > uv_tcp_t
//    
//    const int backlog = 128;
//    const int buffer_size = 1024;
//    uv_fs_t open_req;
//    uv_fs_t read_req;
//    uv_tcp_t *client; // 接続してきたクライアント(取り回しを楽にするため
//    
//    void on_new_connection(uv_stream_t *server, int status);
//    void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
//    void on_client_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
//    void on_client_write(uv_write_t *req, int status);
//    void on_file_open(uv_fs_t *req);
//    void on_file_read(uv_fs_t *req);
//    
//    // クライアントと接続後, クライアントからのデータを読み込む
//    void on_new_connection(uv_stream_t *server, int status) {
//        if (status == -1) {
//            fprintf(stderr, "error on_new_connection");
//            uv_close((uv_handle_t*) client, NULL);
//            return;
//        }
//        
//        // クライアントを保持するためのメモリを確保
//        client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
//        // loop への登録
//        uv_tcp_init(uv_default_loop(), client);
//        
//        // accept
//        int result = uv_accept(server, (uv_stream_t*) client);
//        
//        if (result == 0) { // success
//            // クライアントからのデータを読みだして、alloc_buffer で確保した領域に書き込み、callback を呼ぶ
//            uv_read_start((uv_stream_t*) client, alloc_buffer, on_client_read);
//        } else { // error
//            uv_close((uv_handle_t*) client, NULL);
//        }
//    }
//    
//    // suggeseted_size で渡された領域を確保
//    void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
//        (*buf) = uv_buf_init((char*) malloc(suggested_size), (unsigned int)suggested_size);
//    }
//    
//    // tcp クライアントから受け取ったファイル名を開く
//    void on_client_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
//        if (nread == -1) {
//            fprintf(stderr, "error on_client_read");
//            uv_close((uv_handle_t*) client, NULL);
//            return;
//        }
//        
//        // クライアントから受け取ったデータ
//        // データは buf.base に格納されている。
//        char *filename = buf->base;
//        int mode = 0;
//        
//        // ファイルを開く
//        uv_fs_open(uv_default_loop(), &open_req, filename, O_RDONLY, mode, on_file_open);
//    }
//    
//    // client に返したあとの後片付け
//    void on_client_write(uv_write_t *req, int status) {
//        if (status == -1) {
//            fprintf(stderr, "error on_client_write");
//            uv_close((uv_handle_t*) client, NULL);
//            return;
//        }
//        
//        // 開放
//        free(req);
//
//        // ここで開放するためにつけておいた。
//        char *buffer = (char*) req->data;
//        free(buffer);
//        
//        // tcp connection を閉じる
//        uv_close((uv_handle_t*) client, NULL);
//    }
//    
//    // 開いたファイルの内容を読む
//    void on_file_open(uv_fs_t *req) {
//        if (req->result == -1) {
//            fprintf(stderr, "error on_file_read");
//            uv_close((uv_handle_t*) client, NULL);
//            return;
//        }
//        
//        // ファイルから読み込んだデータを入れるバッファ
//        uv_buf_t *buffer = (uv_buf_t *) malloc(sizeof(uv_buf_t) * buffer_size);
//        
//        // 読み込みを登録,書き込みのバッファを指定
//        int offset = -1;
//        // data フィールドに入れてコールバック関数に渡す
//        read_req.data = (void*) buffer;
//        // ここは非同期コールバック
////        uv_fs_read(uv_loop_t* loop,
////                   uv_fs_t* req,
////                   uv_file file,
////                   const uv_buf_t bufs[],
////                   unsigned int nbufs,
////                   int64_t offset,
////                   uv_fs_cb cb);
//        
//        uv_fs_read(uv_default_loop(), (uv_fs_t*)&read_req, req->file, buffer, sizeof(uv_buf_t) * buffer_size, offset, on_file_read);
//        // read を登録したら開放
//        uv_fs_req_cleanup(req);
//    }
//    
//    // ファイルの内容をクライアントに返す
//    void on_file_read(uv_fs_t* req) {
//        if (req->result < 0) {
//            fprintf(stderr, "error on_file_read");
//            uv_close((uv_handle_t*) client, NULL);
//        } else if (req->result == 0) { // 読み終わったら閉じる
//            uv_fs_t close_req;
//            uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
//            uv_close((uv_handle_t*) client, NULL);
//        } else { // 読み込んだ内容をクライアントに返す
//            // 書き込み用の領域確保
//            uv_write_t *write_req = (uv_write_t *) malloc(sizeof(uv_write_t));
//            
//            // クライアントに返す内容
//            // req->data に入っているバッファのポインタより読み込む
//            char *message = (char*) req->data;
//            
//            // uv_write に使う uv_buf_t の準備
//            uv_buf_t buf = uv_buf_init(message, sizeof(message));
//            buf.len = req->result;
//            buf.base = message;
//            int buf_count = 1;
//            
//            // on_client_write 内で開放できるように、
//            // ポインタを格納して渡す。
//            write_req->data = (void*) message;
//            
//            // client に buf を書き込む
//            uv_write(write_req, (uv_stream_t*) client, &buf, buf_count, on_client_write);
//        }
//        // 同期で開放
//        uv_fs_req_cleanup(req);
//    }
//    
//    int main1(void) {
//        // Network I/O の構造体
//        uv_tcp_t server;
//        // loop への登録
//        uv_tcp_init(uv_default_loop(), &server);
//        // アドレスの取得
//        struct sockaddr_in bind_addr;
//        uv_ip4_addr("0.0.0.0", 6882, &bind_addr);
//        // bind
//        uv_tcp_bind(&server, (const struct sockaddr*)&bind_addr, 0);
//        
//        // uv_stream_t: uv_handle_t のサブクラス uv_tcp_t の親
//        // listen
//        int r = uv_listen((uv_stream_t*) &server, backlog, on_new_connection);
//        if (r) {
//            // エラー処理
//            fprintf(stderr, "error uv_listen");
//            return 1;
//        }
//        
//        // ループ開始
//        uv_run(uv_default_loop(), UV_RUN_DEFAULT);
//        
//        return 0;
//    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <uv.h>
//    
//#define log(x) printf("%s\n", x);
//    
//    uv_loop_t *loop;
//    
//    void on_connect(uv_connect_t *req, int status);
//    void on_write_end(uv_write_t *req, int status);
//    uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size);
//    void echo_read(uv_stream_t *server, ssize_t nread, uv_buf_t buf);
//    
//    // サーバからのレスポンスを表示
//    void echo_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
//        if (nread == -1) {
//            fprintf(stderr, "error echo_read");
//            return;
//        }
//        
//        // 結果を buf から取得して表示
//        printf("result: %s\n", buf->base);
//    }
//    
//    // suggeseted_size で渡された領域を確保
//    void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
//        (*buf) = uv_buf_init((char*) malloc(suggested_size), (unsigned int)suggested_size);
//    }
//    
//    // サーバへデータ送信後, サーバからのレスポンスを読み込む
//    void on_write_end(uv_write_t *req, int status) {
//        if (status == -1) {
//            fprintf(stderr, "error on_write_end");
//            return;
//        }
//        
//        // 書き込みが終わったら、すぐに読み込みを開始
//        uv_read_start(req->handle, alloc_buffer, echo_read);
//    }
//    
//    // サーバとの接続を確立後, サーバに文字列を送信
//    void on_connect(uv_connect_t *req, int status) {
//        if (status == -1) {
//            fprintf(stderr, "error on_write_end");
//            return;
//        }
//        
//        // 送信メッセージを登録
//        char *message = "hello.txt";
//        int len = strlen(message);
//        
//        /** これだとセグフォ
//         * uv_buf_t buf[1];
//         * buf[0].len = len;
//         * buf[0].base = message;
//         */
//        
//        // 送信データ用のバッファ
//        char buffer[100];
//        uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));
//        
//        // 送信データを載せる
//        buf.len = len;
//        buf.base = message;
//        
//        // ハンドルを取得
//        uv_stream_t* tcp = req->handle;
//        
//        // 書き込み用構造体
//        uv_write_t write_req;
//        
//        int buf_count = 1;
//        // 書き込み
//        uv_write(&write_req, tcp, &buf, buf_count, on_write_end);
//    }
//    
//    int main(void) {
//        // loop 生成
//        loop = uv_default_loop();
//        
//        // Network I/O の構造体
//        uv_tcp_t client;
//        
//        // loop への登録
//        uv_tcp_init(loop, &client);
//        
//        // アドレスの取得
//        struct sockaddr_in req_addr;
//        uv_ip4_addr("127.0.0.1", 6882, &req_addr);
//        
//        // TCP コネクション用の構造体
//        uv_connect_t connect_req;
//        
//        // 接続
//        uv_tcp_connect(&connect_req, &client, req_addr, on_connect);
//        
//        // ループを開始
//        return uv_run(loop, UV_RUN_DEFAULT);
//    }
}