#pragma once

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>
#include <grpc++/support/byte_buffer.h>

namespace paddle {
namespace operators {
namespace detail {

enum class GrpcMethod {
  kSayHello,
};

static const int kGrpcNumMethods =
    static_cast<int>(GrpcMethod::kSayHello) + 1;

inline const char* GrpcMethodName(GrpcMethod id) {
  switch (id) {
    case GrpcMethod::kSayHello:
      return "/helloworld.Greeter/SayHello";
  }

  // Shouldn't be reached.
  std::cout << "Invalid id: not found valid method name";
  return "invalid id";
}

namespace grpc {

class GrpcService final {
 public:
  class AsyncService : public ::grpc::Service {
   public:
    AsyncService(){
        for (int i = 0; i < kGrpcNumMethods; ++i) {
            AddMethod(new ::grpc::internal::RpcServiceMethod(
                        GrpcMethodName(static_cast<GrpcMethod>(i)),
                        ::grpc::internal::RpcMethod::NORMAL_RPC, nullptr));
            ::grpc::Service::MarkMethodAsync(i);
        }
    }
    virtual ~AsyncService(){}

    // Make RequestAsyncUnary public for grpc_call.h
    using ::grpc::Service::RequestAsyncUnary;
  };
};

}  // namespace grpc
}  // namespace detail
}  // namespace operator
}  // namespace paddle

