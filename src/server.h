#ifndef SERVER_H
#define SERVER_H

#include <grpcpp/grpcpp.h>

#include "proto/rbench.grpc.pb.h"
#include "proto/rbench.pb.h"

class RBenchImpl final : public RBench::Service
{
  ::grpc::Status ProcessFile(::grpc::ServerContext* context, const ::InputFile* request,
                             ::BenchmarkResults* response) override;
};

#endif