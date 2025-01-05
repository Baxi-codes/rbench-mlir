#include <filesystem>
#include <fstream>
#include <iostream>

using std::filesystem::path;

#include "config.h"
#include "mlir.h"
#include "server.h"

using ::grpc::Server;
using ::grpc::ServerBuilder;
using ::grpc::ServerContext;
using ::grpc::Status;

Status RBenchImpl::ProcessFile(ServerContext* context, const ::InputFile* request,
                               ::BenchmarkResults* response)
{
  std::string filepath(path(FILE_UPLOAD_PATH) / path(request->filename()));
  std::ofstream outfile(filepath, std::ios::binary);
  if (!outfile)
  {
    return Status(grpc::StatusCode::INTERNAL, "Failed to write file");
  }
  outfile.write(request->file_data().data(), request->file_data().size());
  outfile.close();

  // TODO: Add the benchmarking logic
  // TODO: Add cache functionality

  bool cache = 0;
  if (!cache)
  {
    if (!std::filesystem::remove(filepath))
    {
      return Status(grpc::StatusCode::INTERNAL, "Could not delete file " + request->filename());
    }
  }
  return Status::OK;
}

int main(int argc, char const* argv[])
{
  registerPasses();
  std::string port;
  if (argc == 1)
  {
    port = "127.0.0.1:3000";
  }
  else if (argc == 2)
  {
    port = std::string(argv[1]);
  }
  else
  {
    printf("%s [listening port]\n", argv[0]);
  }
  std::cout << "Upload path: " << FILE_UPLOAD_PATH << std::endl;
  RBenchImpl service;
  ServerBuilder builder;
  builder.AddListeningPort(port, ::grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << port << std::endl;
  server->Wait();
  return 0;
}