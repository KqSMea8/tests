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

#include <memory>
#include <iostream>
#include <string>
#include <thread>


#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include "helloworld.grpc.pb.h"
#include "grpc_service.h"
#include "threadpool.h"
#include "util.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

using namespace paddle::operators;

int g_thread_num=0;

std::unique_ptr<::grpc::ByteBuffer> SerializeToByteBuffer(
    ::grpc::protobuf::Message* message) {
  ::grpc::string buf;
  message->SerializeToString(&buf);
  ::grpc::Slice slice(buf);
  return std::unique_ptr<::grpc::ByteBuffer>(new ::grpc::ByteBuffer(&slice, 1));
}

  class CallData {
   public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    // CallData(Greeter::AsyncService* service, ServerCompletionQueue* cq)
    CallData(detail::grpc::GrpcService::AsyncService* service, ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
      // Invoke the serving logic right away.
      Proceed();
    }

    void Proceed() {
      if (status_ == CREATE) {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;

        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
        //printf("CREATE");
        //service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_,
        //                         this);
        int method_id = static_cast<int>(detail::GrpcMethod::kSayHello);
        service_->RequestAsyncUnary(method_id, 
                &ctx_, &request_, &responder_, cq_, cq_, this);
      } else if (status_ == PROCESS) {
            // Spawn a new CallData instance to serve new clients while we process
            // the one for this CallData. The instance will deallocate itself as
            // part of its FINISH state.
            new CallData(service_, cq_);

            std::cout << "[server] payload: " << request_.ByteSizeLong() << std::endl;

            // The actual processing.
            std::string prefix("Hello 123");
            HelloReply reply;
            reply.set_message(prefix + request_.name());

            // And we are done! Let the gRPC runtime know we've finished, using the
            // memory address of this instance as the uniquely identifying tag for
            // the event.
            status_ = FINISH;
            std::unique_ptr<::grpc::ByteBuffer> send_buffer = SerializeToByteBuffer(&reply);
            responder_.Finish(*(send_buffer.get()), Status::OK, this);
      } else {
        GPR_ASSERT(status_ == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }

   private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    // Greeter::AsyncService* service_;
    detail::grpc::GrpcService::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    // What we get from the client.
    HelloRequest request_;
    // What we send back to the client.
    //HelloReply reply_;
    // ::grpc::ByteBuffer reply_;

    // The means to get back to the client.
    // ServerAsyncResponseWriter<HelloReply> responder_;
    ServerAsyncResponseWriter<::grpc::ByteBuffer> responder_;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
  };

class WorkThread {
    public:
};

class ServerImpl final {
 public:
  ~ServerImpl() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    // cq_->Shutdown();
  }

  // There is no shutdown handling in this code.
  void Run(int port) {
    //std::string server_address("0.0.0.0:50051");
    char tmp[128];
    snprintf(tmp, sizeof(tmp), "0.0.0.0:%d", port);
    std::string server_address = tmp;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.SetMaxSendMessageSize(std::numeric_limits<int>::max());
    builder.SetMaxReceiveMessageSize(std::numeric_limits<int>::max());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&service_);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    for(int i=0;i<g_thread_num;i++){
        cqs_.push_back(builder.AddCompletionQueue());
    }

    server_ = builder.BuildAndStart();

    for(int i=0;i<g_thread_num;i++){
        threads_.push_back(new std::thread(std::bind(&ServerImpl::HandleRpcs, this,cqs_[i].get())));
    }

    // Finally assemble the server.
    std::cout << "Server listening on " << server_address << std::endl;
    server_->Wait();
    //thread0_->join();
  }

 private:
  // Class encompasing the state and logic needed to serve a request.
  //
  void RegistNew(::grpc::ServerCompletionQueue* cq){
      std::unique_lock<std::mutex> lock(cq_mutex_);
      new CallData(&service_, cq);
  }

  // This can be run in multiple threads if needed.
  void HandleRpcs(::grpc::ServerCompletionQueue* cq) {
        RegistNew(cq);
        void* tag;  // uniquely identifies a request.
        bool ok;
        while (cq->Next(&tag, &ok)) {
          // Block waiting to read the next event from the completion queue. The
          // event is uniquely identified by its tag, which in this case is the
          // memory address of a CallData instance.
          // The return value of Next should always be checked. This return value
          // tells us whether there is any kind of event or cq is shutting down.
          //GPR_ASSERT(cq_[i].get()->Next(&tag, &ok));
          
          //printf("cq:%ld thread_id:%ld\n", (int64_t)cq, std::this_thread::get_id());
            std::cout << (int64_t)cq << "," << std::this_thread::get_id() << std::endl;
          GPR_ASSERT(ok);
          static_cast<CallData*>(tag)->Proceed();
     }
  }

  std::vector<std::thread*> threads_;
  std::mutex cq_mutex_;

  std::vector<std::unique_ptr<ServerCompletionQueue>> cqs_;
  detail::grpc::GrpcService::AsyncService service_;
  std::unique_ptr<Server> server_;
};

void start_server(int port){
  ServerImpl server;
  server.Run(port);
}

int main(int argc, char** argv) {
    if(argc != 3){
        printf("format:exe <port> thread_num_of_one_server\n");
        exit(0);
    }

  g_thread_num=atoi(argv[2]);
  printf("thread_num:%d\n", g_thread_num);

  std::vector<std::string> ports = split(argv[1]);
  std::vector<std::thread*> threads;
  for(int i = 0;i < (int)ports.size();i++){
      printf("port:%s\n", ports[i].c_str());
      threads.push_back(new std::thread(start_server, atoi(ports[i].c_str())));
  }

  for(int i = 0;i < (int)ports.size();i++){
      threads[i]->join();
  }

  return 0;
}
