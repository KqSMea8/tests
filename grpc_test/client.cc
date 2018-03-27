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

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>
#include <thread>
#include "threadpool.h"

#include "helloworld.grpc.pb.h"

#include "grpc_util.h"
#include "elapsed_time.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

int g_payload_size = 0;
int loop_times = 200;

void GenRequest(std::string user, HelloRequest* request){
    char* payload_alloc = (char*)malloc(g_payload_size);

    request->set_name(user);
    request->set_payload(payload_alloc, g_payload_size);

    free(payload_alloc);
}

class GreeterClient {
  public:
    explicit GreeterClient(std::shared_ptr<Channel> channel)
            : stub_(Greeter::NewStub(channel)) {}

    // Assembles the client's payload and sends it to the server.
    void SayHello(const HelloRequest& request) {
        //const int size = 3 * 1024 * 1024;
        //char* payload_alloc = (char*)GenPayload(size);
        //double ts = GetTimestamp();
        // Call object to store rpc data
        AsyncClientCall* call = new AsyncClientCall;

        // Data we are sending to the server.
        //HelloRequest request;
        //request.set_name(user);
        //request.set_payload(payload_alloc, size);
        //printf("time is %.2f ms\n", GetTimestamp() - ts);
        //free(payload_alloc);

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

            if (cq_count == loop_times) {
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

void loop(int i){
}

int main(int argc, char** argv) {
    if (argc != 4){
          printf("cmd format:client ip port size");
          exit(1);
      }

      g_payload_size = atoi(argv[3]);

      std::string ip = std::string(argv[1]);
      int port = atoi(argv[2]);
      char end_point[128];
      snprintf(end_point, sizeof(end_point) -1, "%s:%d", ip.c_str(), port);
      printf("server end_point:%s\n", end_point);


    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    grpc::ChannelArguments args;
    args.SetMaxSendMessageSize(std::numeric_limits<int>::max());
    args.SetMaxReceiveMessageSize(std::numeric_limits<int>::max());

    auto ch = std::shared_ptr<grpc::Channel>( 
            grpc::CreateCustomChannel(end_point, grpc::InsecureChannelCredentials(), args));

    GreeterClient greeter(ch);

    // Spawn reader thread that loops indefinitely
    std::thread thread_ = std::thread(&GreeterClient::AsyncCompleteRpc, &greeter);

    //const int max = 1 * 1024 * 1024;
    //for(int m=1024; m < max; m *= 2){
    ElapsedTime time(false);
    //g_payload_size = 2 * 1024 * 1024;
    HelloRequest request;
    GenRequest("hello", &request);
    for (int i = 0; i < loop_times; i++) {
        greeter.SayHello(request);  // The actual RPC call!
    }
    thread_.join();  //blocks forever

    double total_time = time.GetElapsed();
    //std::cout << "total " << GetTimestamp() - ts << std::endl;
    printf("total:%d size:%.2f KB time:%.2f speed:%.2f MB/s\n",
            loop_times, (g_payload_size / 1024.0), total_time,
            g_payload_size * 1.0 * (loop_times) / (1024.0 * total_time));

    //}

    // std::cout << "Press control-c to quit" << std::endl << std::endl;
    return 0;
}
