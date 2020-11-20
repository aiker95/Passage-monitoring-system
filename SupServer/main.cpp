#include "SServer.h"


#define UNUSED(x) (void)(x)

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


    bool doExchange(TcpServer::Client client)
    {
        rapidjson::StringBuffer jsoneBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> jsoneWriter(jsoneBuffer);
        doc.Accept(jsoneWriter);
        LOG_TRACE << "JSONE data prepared for send:\n" << jsoneBuffer.GetString() << END;


        bool status = client.sendData(jsoneBuffer.GetString(), jsoneBuffer.GetLength());
        if (!status)
        {
            LOG_ERROR << "Connect to client error: " << status << END;
            return false;
        }
        else
        {
            LOG_TRACE << "Recived " << END;
            return true;
        }
    }
};

static PGconn* conn = NULL;
static PGresult* res = NULL;

static void
terminate(int code)
{
    if (code != 0)
        fprintf(stderr, "%s\n", PQerrorMessage(conn));

    if (res != NULL)
        PQclear(res);

    if (conn != NULL)
        PQfinish(conn);

  //  exit(code);
}

static void
clearRes()
{
    PQclear(res);
    res = NULL;
}

static void
processNotice(void* arg, const char* message)
{
    UNUSED(arg);
    UNUSED(message);

    // do nothing
}

//Парсер ip в std::string
std::string getHostStr(const TcpServer::Client& client) {
    uint32_t ip = client.getHost();
    return std::string() + std::to_string(int(reinterpret_cast<char*>(&ip)[0])) + '.' +
        std::to_string(int(reinterpret_cast<char*>(&ip)[1])) + '.' +
        std::to_string(int(reinterpret_cast<char*>(&ip)[2])) + '.' +
        std::to_string(int(reinterpret_cast<char*>(&ip)[3])) + ':' +
        std::to_string(client.getPort());
}

std::string msgs;

void printInMsg(std::string msg) {
    std::system("cls");
    msgs +=  msg ;
    std::cout << msgs;
}

bool manageDB() {
    const char* conninfo = "host=10.77.1.57 dbname=manager2 user=manager2 password=ppUkBOTi";
    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database - failed: %s\n", PQerrorMessage(conn));
        terminate(1);
    }
    else
        fprintf(stderr, "Connection to database - success!\n");
    // Don't output notices like:
           // NOTICE:  relation "phonebook" already exists, skipping
           // see http://stackoverflow.com/a/12504406/1565238
    PQsetNoticeProcessor(conn, processNotice, NULL);

    int server_ver = PQserverVersion(conn);
    char* user = PQuser(conn);
    char* db_name = PQdb(conn);

    printf("Server version: %d\n", server_ver);
    printf("User: %s\n", user);
    printf("Database name: %s\n", db_name);
    res = PQexec(conn, "SELECT * "
        "FROM pass_cards");


    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        terminate(1);
    }

    int ncols = PQnfields(res);
    printf("There are %d columns:", ncols);
    for (int i = 0; i < ncols; i++)
    {
        char* name = PQfname(res, i);
        printf(" %s", name);
    }
    printf("\n");

    int nrows = PQntuples(res);
    for (int i = 0; i < nrows; i++)
    {
        char* id = PQgetvalue(res, i, 0);
        char* type_id = PQgetvalue(res, i, 1);
        char* carrier_id = PQgetvalue(res, i, 2);
        char* start_time = PQgetvalue(res, i, 3);
        printf("Id: %s, type_id: %s, carrier_id: %s, start_time: %s\n",
            id, type_id, carrier_id, start_time);
    }

    printf("Total: %d rows\n", nrows);

    PQclear(res);
    //Int paramValues
    //const int LEN = sizeof(static_cast<char>(inputStruct->Test1));
    const char* paramValues[1];

   // mesOut.Status = inputStruct->Test2;
    //std::string tmp = std::to_string(inputStruct->Test1);
    //char const* num_char = tmp.c_str();
    //String paramValues
    //num_char = inputStruct->start_time;
   // paramValues[0] = num_char;

    //const char* stm = "SELECT * FROM pass_cards WHERE id=$1";
    const char* stm = "SELECT * FROM pass_cards WHERE start_time=$1";

    int paramCount = sizeof(paramValues) / sizeof(paramValues[0]);

    PGresult* res = PQexecParams(conn, stm, paramCount, NULL, paramValues,
        NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        terminate(1);
    }

    ncols = PQnfields(res);
    printf("There are %d columns:", ncols);
    for (int i = 0; i < ncols; i++)
    {
        char* name = PQfname(res, i);
        printf(" %s", name);
    }
    printf("\n");

    nrows = PQntuples(res);
    for (int i = 0; i < nrows; i++)
    {
        char* id = PQgetvalue(res, i, 0);
        char* type_id = PQgetvalue(res, i, 1);
        char* carrier_id = PQgetvalue(res, i, 2);
        char* start_time = PQgetvalue(res, i, 3);
        printf("Id: %s, type_id: %s, carrier_id: %s, start_time: %s\n",
            id, type_id, carrier_id, start_time);
    }

    printf("Total: %d rows\n", nrows);

    clearRes();
    return true;
}


void checkJson(char* clientJson) {
    
}



int main() {
    //Создание объекта TcpServer с передачей аргументами порта и лябда-фунции для обработк клиента
    TcpServer server(8080,

        [](TcpServer::Client client) {
            //Вывод адреса подключившего клиента в консоль
            std::cout << "Connected host:" << getHostStr(client) << std::endl;

            //Ожидание данных от клиента
            int size = 0;
            while (!(size = client.loadData()));
            //Вывод размера данных и самих данных в консоль  


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
                if (!m_answerDoc.HasMember("id") )
                {
                    LOG_ERROR << "The [id]  element dont't find \r\n" << END;

                    std::cout << "The [id] element dont't find \r\n" << getHostStr(client) << std::endl;
                    // return false;
                }
                else
                    std::cout << "Ok [id] " << std::endl;

                if ( !m_answerDoc.HasMember("method") )
                {
                    LOG_ERROR << "The [method] element dont't find \r\n" << END;

                    std::cout << "The [method] element dont't find \r\n" << getHostStr(client) << std::endl;
                    // return false;
                }
                else {
                    std::cout << "Ok [method] " << std::endl;
                    std::string method = m_answerDoc["method"].GetString();
                    std::cout
                        << "method: " << method << std::endl;
                }
                if ( !m_answerDoc.HasMember("params"))
                {
                    LOG_ERROR << "The [params] element dont't find \r\n" << END;

                    std::cout << "The [params] element dont't find \r\n" << getHostStr(client) << std::endl;
                    // return false;
                }
                else
                    std::cout << "Ok  [params] " << std::endl;

                if (!m_answerDoc["params"].HasMember("epc"))
                {
                    LOG_ERROR << "The [epc] element dont't find \r\n" << END;

                    std::cout << "The [epc] element dont't find \r\n" << getHostStr(client) << std::endl;
                   // return false;
                }
                else
                {
                    std::string epc = m_answerDoc["params"]["epc"].GetString();
                    std::cout
                        << "epc: " << epc << std::endl;
                }
            }


                //<< client.getData()->Test1 << std::endl
                //<< client.getData()->Test2 << std::endl;

            //printInMsg(*client.getData()->Para);
           
           // client.outism =  manageDB(client.getData(), client.outism );

            //Отправка ответа клиенту
           // client.outism.Status = 200;

          //Отправка ответа клиенту
            CProfITExchanger exchanger("answer", g_teremId.c_str());
            rapidjson::Document::AllocatorType& allocator = exchanger.allocator;

            exchanger.paramsObj->AddMember("status", "200", allocator);

            exchanger.doExchange(client);
            std::cout << "Disconnected host:" << getHostStr(client) << std::endl;
        }
    );
    
    //Запуск серевера
    if (server.start() == TcpServer::status::up) {
        //Если сервер запущен вывести сообщение и войти в поток ожиданий клиентов
        std::cout << "Server is up!" << std::endl;
        server.joinLoop();
    }
    else {
        //Если сервер не запущен вывод кода ошибки и заверешение программы
        std::cout << "Server start error! Error code:" << int(server.getStatus()) << std::endl;
        return -1;
    }

}