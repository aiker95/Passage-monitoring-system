#include "../tcp/hdr/SClient.h"

using namespace std;


struct Iso8601_Time
{
	char		strTimeMark[32];

	Iso8601_Time()
	{
		SYSTEMTIME				tm;

		GetLocalTime(&tm);
		serialize(tm);
	}

	Iso8601_Time(SYSTEMTIME& tm)
	{
		serialize(tm);
	}

	void serialize(SYSTEMTIME& tm)
	{
		strTimeMark[0] = '\0';

		_TIME_ZONE_INFORMATION  utcZone;
		GetTimeZoneInformation(&utcZone);

		int hShift = abs(utcZone.Bias) / 60;
		int mShift = abs(utcZone.Bias) % 60;
		int n = _snprintf(strTimeMark, sizeof(strTimeMark), "%02d-%02d-%dT%02d:%02d:%02d+%02d:%02d",
			tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, hShift, mShift);
	}

	operator const char* ()
	{
		return strTimeMark;
	}

	const char* c_str()
	{
		return strTimeMark;
	}
};


struct AmmountToStr
{
	char		str[32];

	AmmountToStr(double _amount)
	{
		_snprintf(str, sizeof(str), "%.2f", _amount);
	}

	operator const char* ()
	{
		return str;
	}

	const char* c_str()
	{
		return str;
	}
};


std::string g_adress_url("");
std::string g_teremId("00000000");

void SetParams(const std::string& _adress_url, const std::string& _teremId)
{
	g_adress_url = _adress_url;
	g_teremId = _teremId;
}

// ********************************************************************************************************************** //
// -------------------------     CProfITExchanger implementation begin       -------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //

struct CProfITExchanger
{
	rapidjson::Document			doc;
	rapidjson::Document::AllocatorType& allocator;
	rapidjson::Document::ValueIterator  paramsObj;

	CProfITExchanger(const char* _methodName, const char* _parkomatId)
		: paramsObj(0)
		, allocator(doc.GetAllocator())
	{
		doc.SetObject();

		doc.AddMember("id", rapidjson::Value(int(GetTickCount() & 0xffff)), allocator);
		doc.AddMember("method", rapidjson::Value(_methodName, allocator), allocator);
		rapidjson::Value objParams(rapidjson::kObjectType);
		doc.AddMember("params", objParams, allocator);
		paramsObj = &doc["params"];
	}


	bool doExchange(TcpClient client)
	{
		rapidjson::StringBuffer jsoneBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> jsoneWriter(jsoneBuffer);
		doc.Accept(jsoneWriter);
		LOG_TRACE << "JSONE data prepared for send:\n" << jsoneBuffer.GetString() << END;
		

		bool status = client.sendData(jsoneBuffer.GetString(), jsoneBuffer.GetLength());
		if (!status)
		{
			LOG_ERROR << "Connect to server error: " << status << END;
			return false;
		}
		else
		{
			LOG_TRACE << "Recived " << END;
			return true;
		}
	}
};


std::string getHostStr(uint32_t ip, uint16_t port) {
	return std::string() + std::to_string(int(reinterpret_cast<char*>(&ip)[0])) + '.' +
		std::to_string(int(reinterpret_cast<char*>(&ip)[1])) + '.' +
		std::to_string(int(reinterpret_cast<char*>(&ip)[2])) + '.' +
		std::to_string(int(reinterpret_cast<char*>(&ip)[3])) + ':' +
		std::to_string(port);
}

std::string msgs;

void printInMsg(std::string msg) {
	std::system("cls");
	msgs += "<<" + msg + '\n';
	std::cout << msgs;
}

void printOutMsg(std::string& msg) {
	std::system("cls");
	msgs += ">> Status: " + msg + '\n';
	std::cout << msgs;
}


int main() {
	TcpClient client;


	const char *adr = "127.0.0.1";
	unsigned short adr_port = 8080;

	uint32_t host = inet_addr(adr);
	uint16_t port = adr_port;

	std::string host_str;
	std::cout << "Enter host:";
	//std::cin >> host_str;
	//host = inet_addr(host_str.c_str());
	std::cout << adr << ':';
	std::cout << adr_port << std::endl;
	//std::cin >> port;

	std::cout << "Try connect to: " << adr << ':' << to_string(port) << std::endl;

	if (client.connectTo(host, port) == TcpClient::status::connected) {
		std::cout << "Connected to: " << getHostStr(localhost, 8080) << std::endl;

		std::thread distant_msg([&client]() {
			while (true) {
				//Ожидание данных от сервера
				int size = 0;
				while (!(size = client.loadData()));
				if (size != 0) {
					std::cout
						<< "size: " << size << " bytes" << std::endl;

					rapidjson::Document			m_answerDoc;
					rapidjson::ParseResult parsRes = m_answerDoc.Parse(client.getData());
					if (parsRes.IsError()) {
						LOG_ERROR << "Cannot parse data! Error " << parsRes.Code() << " in pos " << parsRes.Offset() << END;

						std::cout << "Cannot parse data! Error" << parsRes.Code() << " in pos " << parsRes.Offset() << std::endl;
						//return false;
					}
					else {
						if (!m_answerDoc.HasMember("id"))
						{
							LOG_ERROR << "The [id]  element dont't find \r\n" << END;

							std::cout << "The [id] element dont't find \r\n"  << std::endl;
							// return false;
						}
						else
							std::cout << "Ok [id] " << std::endl;

						if (!m_answerDoc.HasMember("method"))
						{
							LOG_ERROR << "The [method] element dont't find \r\n" << END;

							std::cout << "The [method] element dont't find \r\n" << std::endl;
							// return false;
						}
						else {
							std::cout << "Ok [method] " << std::endl;
							std::string method = m_answerDoc["method"].GetString();
							std::cout
								<< "method: " << method << std::endl;
						}
						if (!m_answerDoc.HasMember("params"))
						{
							LOG_ERROR << "The [params] element dont't find \r\n" << END;

							std::cout << "The [params] element dont't find \r\n"  << std::endl;
							// return false;
						}
						else
							std::cout << "Ok  [params] " << std::endl;

						if (!m_answerDoc["params"].HasMember("epc"))
						{
							LOG_ERROR << "The [epc] element dont't find \r\n" << END;

							std::cout << "The [epc] element dont't find \r\n" << std::endl;
							// return false;
						}
						else
						{
							std::string epc = m_answerDoc["params"]["status"].GetString();
							std::cout
								<< "status: " << epc << std::endl;
						}
					}

					cout << "Disconnected";
					std::exit(0);
				}
			}
			});

		CProfITExchanger exchanger("rfidUhf", g_teremId.c_str());
		rapidjson::Document::AllocatorType& allocator = exchanger.allocator;

		exchanger.paramsObj->AddMember("epc", "759C917809A4001092000250", allocator);

		exchanger.doExchange(client);
		//while (true) {
		//	//std::string message = "Hell 1\n";
		//	//printOutMsg(message);
		//	//getline(cin, message, '\n');

		//	CProfITExchanger exchanger("measurements", g_teremId.c_str());
		//	rapidjson::Document::AllocatorType& allocator = exchanger.allocator;

		//	exchanger.paramsObj->AddMember("epc", "759C917809A4001092000250", allocator);

		//	exchanger.doExchange(client);

		//	//client.sendData((char*)&message, sizeof(message));
		//}
	}
	else {
		std::cout << "Error! Client isn't connected! Error code: " << int(client.getStatus()) << std::endl;
	}
}