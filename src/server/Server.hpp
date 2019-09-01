// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <string>
#include <sstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <ctime>
#include "Student.hpp"
#include "TemplateCertificate.hpp"
#include "Certificate.hpp"
#include "Batch.hpp"
#include "Exceptions.hpp"
#include <nlohmann/json.hpp>
#include <cxxopts.hpp>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include "gen-cpp/CertificateGenerator.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async_logger.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::CertificateGeneratorThrift;

json baseConfiguration;
bool keepGeneratedFiles;
bool dontCrash;

class CertificateGeneratorHandler : virtual public CertificateGeneratorIf {
 private:
  string id;
  string peerAddress;
  json batchConfiguration;
 
 public:
  CertificateGeneratorHandler(const string& id, const string& peerAddress);
  
  ~CertificateGeneratorHandler();
  
  void setConfigurationData(const std::string& configuration);
  
  void addResourceFile(const File& resourceFile);

  void addTemplateFile(const File& templateFile);
  
  void addResourceFiles(const std::vector<File> & resourceFiles);

  void addTemplateFiles(const std::vector<File> & templateFiles);

  bool checkJob();
  
  void generateCertificates(std::vector<File> & _return);
  
  bool sanitizeFilename(string& filename);
};

class CertificateGeneratorCloneFactory : virtual public CertificateGeneratorIfFactory {
 private:
  int count = 0;
 public:
  ~CertificateGeneratorCloneFactory() override = default;
  
  CertificateGeneratorIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) override;
  
  void releaseHandler( CertificateGeneratorIf* handler) override;
};

int main(int argc, char **argv);
