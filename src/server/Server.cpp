// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Student.hpp"
#include "TemplateCertificate.hpp"
#include "Certificate.hpp"
#include "Batch.hpp"
#include <nlohmann/json.hpp>
#include <cxxopts.hpp>

#include "gen-cpp/CertificateGenerator.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::CertificateGeneratorThrift;
using json = nlohmann::json;

json baseConfiguration;

class CertificateGeneratorHandler : virtual public CertificateGeneratorIf {
public:
	CertificateGeneratorHandler() {
		// Your initialization goes here
	}

	void generateCertificate(std::string& _return, const std::string& n1) {
		
		//Parse received configuration
		json batchConfiguration;
		try{
			batchConfiguration = json::parse(n1);
		}catch(const nlohmann::detail::parse_error& error){
			InvalidConfiguration thriftError;
			thriftError.message = error.what();
			std::cerr << "Invalid json in received batch configuration: " << std::endl;
			throw thriftError;
		}
		
		//Add base configuration
		//TODO see if it wouldn't be smarter to use base configuration and just add the students
		batchConfiguration["outputDirectory"] = baseConfiguration["outputDirectory"];
		batchConfiguration["workingDirectory"] = baseConfiguration["workingDirectory"];
		batchConfiguration["templates"] = baseConfiguration["templates"];
		batchConfiguration["resources"] = baseConfiguration["resources"];
		
		//Create batch
		Batch batch(batchConfiguration);
		
		//Check batch
		std::cout << "Checking Batch" << std::endl;
		if(batch.check()){
			std::cout << "Check succeeded" << std::endl;
		}else{
			std::cerr << "Check failed" << std::endl;
			InvalidConfiguration thriftError;
			thriftError.message = "Failed to check configuration";
			throw thriftError;
		}
		
		//Execute batch
		std::cout << "Executing Batch" << std::endl;
		batch.executeBatch();
		std::cout << "All done" << std::endl;
		
		//Returning result
		std::cout << "Returning result" << std::endl;
		stringstream content;
		ifstream pdfFile(batch.getOutputFiles()[0] , ios::in | ios::binary);
		if(!pdfFile){
			InvalidConfiguration thriftError;
			thriftError.message = "Failed to open output file";
			throw thriftError;
		}
		content << pdfFile.rdbuf();
		pdfFile.close();
		_return = content.str();
	}
};

int main(int argc, char **argv) {

	//Parse options
	string batchConfigurationFile;
	string workingDirectory;
	string outputDirectory;
	int serverPort;
	bool verbose = false;
	try{
		cxxopts::Options options(argv[0], "Certificate generator server");
		options.add_options()
			("c,configuration", "The base configuration file", cxxopts::value<string>(), "FILE")
			//("w,working-dir", "The working directory", cxxopts::value<string>(), "PATH")
			//("o,output-dir", "The output directory", cxxopts::value<string>(), "PATH")
			("p,port", "The port on which the server listens", cxxopts::value<int>())
			("v,verbose", "Enable output", cxxopts::value<bool>(verbose))
			("help", "Print help");
		auto result = options.parse(argc, argv);
		if (result.count("help") || result.arguments().size()==0){
			cout << options.help({""}) << std::endl;
			exit(0);
		}
		if (result.count("configuration")){
			batchConfigurationFile = result["configuration"].as<string>();
		}else{
			throw cxxopts::OptionException("No base configuration file specified");
		}
		if (result.count("port")){
			serverPort = result["port"].as<int>();
			if(serverPort > 65535 || serverPort < 1){
				throw cxxopts::OptionException("Invalid port specified");
			}
		}else{
			throw cxxopts::OptionException("No port specified");
		}
	}catch (const cxxopts::OptionException& e){
		cerr << "Error parsing options: " << e.what() << endl;
		exit(1);
	}
	
	//Disable cout
	if(!verbose){
		cout.rdbuf(NULL);
	}
	
	//Load batch configuration
	std::cout << "Loading base configuration" << std::endl;
	ifstream input;
	input.open(batchConfigurationFile, ios::in);
	if(!input){
		cerr << "Error reading base configuration file" << endl;
		exit(EXIT_FAILURE);
	}
	baseConfiguration;
	try{
		baseConfiguration = json::parse(input);
	}catch(const nlohmann::detail::parse_error& error){
		cerr << "Invalid json in base configuration: " << batchConfigurationFile << endl;
		cerr << error.what() << endl;
		exit(EXIT_FAILURE);
	}
	input.close();
	
	//Check base configuration
	std::cout << "Checking base configuration" << std::endl;
	bool abort = true;
	for(json templateFile : baseConfiguration["templates"]){
		if(templateFile.is_string()){
			abort = false;
		}
	}
	if(abort){
		std::cerr << "Error checking configuration: No templates specified" << std::endl;
	}
	if(!baseConfiguration["outputDirectory"].is_string()){
		std::cerr << "Error checking configuration: No output directory specified" << std::endl;
		abort = true;
	}
	if(!baseConfiguration["workingDirectory"].is_string()){
		std::cerr << "Error checking configuration: No working directory specified" << std::endl;
		abort = true;
	}
	if(abort){
		exit(EXIT_FAILURE);
	}
	
	//Initialize thrift server
	std::cout << "Initializing server" << std::endl;
	int port = serverPort;
	::std::shared_ptr<CertificateGeneratorHandler> handler(new CertificateGeneratorHandler());
	::std::shared_ptr<TProcessor> processor(new CertificateGeneratorProcessor(handler));
	::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	//Open thrift server
	std::cout << "Starting server" << std::endl;
	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	server.serve();
	return 0;
}
