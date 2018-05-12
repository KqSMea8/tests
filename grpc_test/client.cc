/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <sys/time.h>

#include <sstream>
#include <iostream>
#include <vector>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>
#include <thread>
#include "threadpool.h"

#include "helloworld.grpc.pb.h"

#include "grpc_util.h"
#include "elapsed_time.h"

#include "util.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

int g_payload_size = 0;
int g_loop_times = 1000;
int g_thread_num = 1; // send threads

void GenRequest(std::string user, HelloRequest* request){
    char* payload_alloc = (char*)malloc(g_payload_size);

    request->set_name(user);
    request->set_payload(payload_alloc, g_payload_size);

    free(payload_alloc);
}

class GreeterClient {
  public:
    explicit GreeterClient()
            //: stub_(Greeter::NewStub(channel)) 
            {}

    // Assembles the client's payload and sends it to the server.
    void SayHello(const HelloRequest& request, std::unique_ptr<Greeter::Stub> stub) {
        stub_ = std::move(stub);
        // paddle::framework::Async([&request, this](){
            AsyncClientCall* call = new AsyncClientCall;

            // stub_->PrepareAsyncSayHello() creates an RPC object, returning
            // an instance to store in "call" but does not actually start the RPC
            // Because we are using the asynchronous API, we need to hold on to
            // the "call" instance in order to get updates on the ongoing RPC.
            call->response_reader =
                stub_->PrepareAsyncSayHello(&call->context, request, &cq_);

            // StartCall initiates the RPC call
            call->response_reader->StartCall();

            // Request that, upon completion of the RPC, "reply" be updated with the
            // server's response; "status" with the indication of whether the operation
            // was successful. Tag the request with the memory address of the call object.
            call->response_reader->Finish(&call->reply, &call->status, (void*)call);
        // });
    }

    // Loop while listening for completed responses.
    // Prints out the response from the server.
    void AsyncCompleteRpc() {
        void* got_tag;
        bool ok = false;
        int cq_count = 0;
        //double total_time = 0.0;

        // Block until the next result is available in the completion queue "cq".
        while (cq_.Next(&got_tag, &ok)) {
            // The tag in this example is the memory location of the call object
            AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

            // Verify that the request was completed successfully. Note that "ok"
            // corresponds solely to the request for updates introduced by Finish().
            GPR_ASSERT(ok);

            cq_count++;

            if (call->status.ok()){
                //std::cout << "Greeter received: " << call->reply.message() << std::endl;
            }
            else{
                std::cout << "RPC failed" << std::endl;
                exit(1);
            }

            //total_time += call->time.GetElapsed();
            // Once we're complete, deallocate the call object.
            delete call;

            if (cq_count == g_loop_times) {
                //std::cout << "total " << GetTimestamp() - ts << std::endl;
                /*
                printf("total:%d size:%.2f KB time:%.2f speed:%.2f MB/s\n",
                        cq_count, g_payload_size / 1024.0, total_time,
                        g_payload_size * 1.0 * (cq_count+1) / (1024.0 * total_time));
                        */
                return;
            }
        }
    }

  private:

    // struct for keeping state and data information
    struct AsyncClientCall {
        // Container for the data we expect from the server.
        HelloReply reply;

        //std::unique_ptr<ElapsedTime> time;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // Storage for the status of the RPC upon completion.
        Status status;


        std::unique_ptr<ClientAsyncResponseReader<HelloReply>> response_reader;
    };

    // Out of the passed in Channel comes the stub, stored here, our view of the
    // server's exposed services.
    std::unique_ptr<Greeter::Stub> stub_;

    // The producer-consumer queue we use to communicate asynchronously with the
    // gRPC runtime.
    CompletionQueue cq_;
};

void send(const std::string& ep, const HelloRequest& req, int max){
    grpc::ChannelArguments args;
    args.SetMaxSendMessageSize(std::numeric_limits<int>::max());
    args.SetMaxReceiveMessageSize(std::numeric_limits<int>::max());
    args.SetInt(GRPC_ARG_HTTP2_STREAM_LOOKAHEAD_BYTES, 1024*1024*1024);
    args.SetInt(GRPC_ARG_MAX_CONCURRENT_STREAMS, max);

    auto ch = grpc::CreateCustomChannel(ep, grpc::InsecureChannelCredentials(), args);
    GreeterClient c;
 
    for (int i = 0; i < g_loop_times; i++) {
        c.SayHello(req, Greeter::NewStub(ch));
    }

    std::thread t = std::thread(&GreeterClient::AsyncCompleteRpc, &c);
    t.join();  //blocks forever
}

void run(const std::string& ep, const HelloRequest& req){
    std::vector<std::thread*> threads;
    for(int i=0;i<g_thread_num;i++){
        threads.push_back(new std::thread(send, ep, req, 256+i));
    }

    for(int i=0;i<g_thread_num;i++){
        threads[i]->join();
    }
}

int main(int argc, char** argv) {
    if (argc != 5){
          printf("cmd format:client <ip:port> loop_times size thread_num\n");
          exit(1);
      }

      g_loop_times = atoi(argv[2]);
      g_payload_size = atoi(argv[3]);
      g_thread_num = atoi(argv[4]);

      std::vector<std::string> endpoints = split(argv[1]);
      for(int i=0;i<(int)endpoints.size();i++){
        printf("server end_point:%s\n", endpoints[i].c_str());
      }

      printf("g_loop_times:%d payload:%d  thread_num:%d\n", 
              g_loop_times, g_payload_size, g_thread_num);

    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).

    HelloRequest request;
    GenRequest("hello", &request);

    ElapsedTime time(false);
    struct timeval s;
    gettimeofday(&s,0);

    std::vector<std::thread*> threads;
    for(int m=0;m<(int)endpoints.size();m++){
        threads.push_back(new std::thread(run, endpoints[m], request));
    } 

    for(int m=0;m<(int)endpoints.size();m++){
        threads[m]->join();
    }

    struct timeval e;
    gettimeofday(&e,0);
    int64_t time_s = s.tv_sec * 1000 + s.tv_usec / 1000;
    int64_t time_e = e.tv_sec * 1000 + e.tv_usec / 1000;
    printf("begin :%ld, end:%ld, used:%ld\n", time_s, time_e, time_e - time_s);

    double total_time = time.GetElapsed();
    printf("g_loop_times:%d size:%.2f KB time:%.2f speed:%.2f MB/s\n",
            g_loop_times, (g_payload_size / 1024.0), total_time,
            endpoints.size() * g_payload_size * 1.0 * (g_loop_times) / (1024.0 * total_time));

    return 0;
}
