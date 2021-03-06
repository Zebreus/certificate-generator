// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Batch.hpp"
#include "Certificate.hpp"
#include "Exceptions.hpp"
#include "Student.hpp"
#include "TemplateCertificate.hpp"
#include <ctime>
#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

#include <thrift/TToString.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/CertificateGenerator.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TServerSocket.h>

#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace ::CertificateGeneratorThrift;

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

	void addResourceFiles(const std::vector<File>& resourceFiles);

	void addTemplateFiles(const std::vector<File>& templateFiles);

	bool checkJob();

	void generateCertificates(std::vector<File>& _return);

	bool sanitizeFilename(string& filename);
};

class CertificateGeneratorCloneFactory : virtual public CertificateGeneratorIfFactory {
private:
	int count = 0;

public:
	~CertificateGeneratorCloneFactory() override = default;

	CertificateGeneratorIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) override;

	void releaseHandler(CertificateGeneratorIf* handler) override;
};

int main(int argc, char** argv);
