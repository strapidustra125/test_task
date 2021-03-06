
/* ========================== Подключение библиотек ========================== */

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>	// Для функции sleep()
#include <cstdlib> 	// Для функций rand() и srand()
#include <ctime> 	// Для функции time()
#include <algorithm>

#include "../Includes/command.pb.h"
#include "../Includes/zmq_cmd.hpp"

#include "zmq.hpp"


using namespace std;
using namespace zmq;


/* ================================ Константы ================================ */

//#define DETAIL_LOG
//#define DETAIL_EXCHANGE_LOG
//#define DETAIL_CONNECT_LOG
//#define DETAIL_STATISTIC_LOG
//#define DETAIL_ID_LOG

#define CLIENT_CYCLE_USEC       10000

#define STAT_CHECH_COEFF        5

#define ZMQ_MESSAGE_SIZE        10

#define ZMQ_SERVER              1
#define ZMQ_CLIENT              2

// Команды Клиента
#define ZMQ_CMD_IDRequest       10
#define ZMQ_CMD_MoreID          11
#define ZMQ_CMD_KillAll         12
#define ZMQ_CMD_KillCheck       13
#define ZMQ_CMD_UpdateVector    14

// Команды сервера
#define ZMQ_CMD_ID              20
#define ZMQ_CMD_IDVector        21
#define ZMQ_CMD_Kill            22
#define ZMQ_CMD_IDVectorUpdate  23


/* ================================ Переменные ================================ */

    // Создание контекста
    context_t zmqContext;

    // Сокеты для запроса и ответа
    socket_t zmqRequestSocket(zmqContext, socket_type::req);
    socket_t zmqReplaySocket(zmqContext, socket_type::rep);

    // Сообщение для отправки по ZeroMQ
    message_t zmqMessage(ZMQ_MESSAGE_SIZE);

    // Команда для обмена между клиентами и сервером
    ZMQ_cmd ZMQCommand;

    // Адрес сокета для подключения
    string socketAddress = "tcp://127.0.0.1:4444";



    // Флаг первой запущенной программы
    int flag_isServer = 0;

    // ID текущего экземпляра приложения
    int currentApplicationID = 0;

    // ID сервера
    int serverID = 0;

    // Вектор ID запущенных приложений
    vector <int> IDvector;

    vector <int>::iterator it;

    // Флаг выключения всех команд
    bool flag_killAll = false;

    // Флаг выключения всех команд
    bool flag_realTimeID = false;

    vector<pair<int, int>> clientStatistic;





/* ========================= Вспомогательные функции ========================= */

// Новый случайный ID
int genNewID(vector<int> vecID)
{
    int newID;
    while(1)
    {
        newID = 1000 + rand() % 9000;

        if(find(vecID.begin(), vecID.end(), newID) == vecID.end())
        {
            return newID;
        }
    }
}

//
void printIDVector(vector<int> vecID)
{
    cout << endl << "Actieve applications ID:" << endl;

    for(vector<int>::iterator i = vecID.begin(); i < vecID.end(); i++)
    {
        cout << "\t" << *i;
        if(*i == currentApplicationID) cout << " *";

        #ifdef DETAIL_ID_LOG
            if(*i == serverID) cout << " s";
        #endif // DETAIL_ID_LOG


        cout << endl;
    }

    cout << endl;
}


void checkArguments(int argNumber, char *argList[])
{
    #ifdef DETAIL_LOG
        cout << endl << "Application parametrs number: " << argNumber << endl;
        cout << endl << "Application parametrs list: " << endl;

        for(int i = 0; i < argNumber; i++)
        {
            cout << "\t" << i << ": " << argList[i] << endl;
        }

        cout << endl;
    #endif // DETAIL_LOG

    if(argNumber == 1) return;

    // Чтобы гарантировать корректность посимвольного сравнения
    string key(argList[1]);

    // Если указан только один ключ
    if(argNumber <= 2)
    {
        // Проверка значения ключа
        if((key == "-k") || (key == "--kill"))
        {
            // Закрыть все активные приложения

            flag_killAll = true;
        }
        else if(key == "-h" || key == "--help")
        {
            // Вывести справку

            cout << endl << "|----- ZMQ application help -----|" << endl;

            cout << endl << "Application parametrs:" << endl;
            cout << "\t-h --help     : Show this help information" << endl;
            cout << "\t-k --kill     : Kill all active applications" << endl;
            cout << "\t-r --realtime : Showing applications ID in real time" << endl;

            cout << endl << "|----- Closing application ------|" << endl;

            exit(0);
        }
        else if(key == "-r" || key == "--realtime")
        {
            // Вывод в реальном времени

            flag_realTimeID = true;
        }
        else
        {
            cout << endl <<  "Error: Invalid parametr value!" << endl;
            cout << "Exit." << endl << endl;

            exit(0);
        }




    }
    else
    {
        cout << endl << "Error: Too much params!" << endl;
        cout << "Exit." << endl << endl;

        exit(0);
    }
}


void reinitClientStatistic()
{
    pair<int, int> p;
    clientStatistic.clear();

    for(vector<int>::iterator i = IDvector.begin(); i < IDvector.end(); i++)
    {
        p.first = *i;
        p.second = 0;
        clientStatistic.push_back(p);
    }

    #ifdef DETAIL_STATISTIC_LOG
        cout << endl << "Client statistic vector" << endl;
        for(vector<pair<int, int>>::iterator i = clientStatistic.begin(); i < clientStatistic.end(); i++)
        {
            cout << "\t" << "ID: " << i->first << ", ACKs: " << i->second << "." << endl;
        }
        cout << endl;
    #endif // DETAIL_STATISTIC_LOG
}


void checkClientStatistic()
{
    unsigned int clientACKSum = 0;

    if(IDvector.size() > 1)
    {
        for(vector<pair<int, int>>::iterator i = clientStatistic.begin(); i < clientStatistic.end(); i++)
        {
            clientACKSum += i->second;
        }

        // Максимум - каждый клиент ответил по 2 раза
        // Минимум - все клиенты с нулями, а один ответил 2n раз
        // n - число клиентов
        if(clientACKSum > STAT_CHECH_COEFF * (IDvector.size() - 1))
        {
            #ifdef DETAIL_STATISTIC_LOG
                cout << endl << "Client statistic vector" << endl;
                for(vector<pair<int, int>>::iterator i = clientStatistic.begin(); i < clientStatistic.end(); i++)
                {
                    cout << "\t" << "ID: " << i->first << ", ACKs: " << i->second << "." << endl;
                }
                cout << endl;
            #endif // DETAIL_STATISTIC_LOG

            // Удаляем клиенты с нулями
            for(vector<pair<int, int>>::iterator i = clientStatistic.begin(); i < clientStatistic.end(); i++)
            {
                // Если от клиента не приходят запросы
                if(i->first != currentApplicationID && i->second == 0)
                    IDvector.erase(find(IDvector.begin(), IDvector.end(), i->first));
            }

            // Обнуляем статистику клиентов
            reinitClientStatistic();
        }
    }
}



void updateClientStatistic(int clientID)
{
    for(vector<pair<int, int>>::iterator i = clientStatistic.begin(); i < clientStatistic.end(); i++)
    {
        if(i->first == clientID) i->second++;
    }
}


// Обнуление значения переменных для нового сервера
void initNewServer()
{
    // Удаляем ID сервера из вектора
    vector<int>::iterator serverIDIter = find(IDvector.begin(), IDvector.end(), serverID);
    if(serverIDIter != IDvector.end()) IDvector.erase(serverIDIter);

    flag_isServer = ZMQ_SERVER;

    serverID = currentApplicationID;

    reinitClientStatistic();
}


/* ============================= Основная функция ============================ */


int main(int argc, char *argv[])
{
    cout << endl << "Starting ZMQ application..." << endl << endl;

    srand(time(0)); // Стартовое число ГПСЧ

    int temp;

    checkArguments(argc, argv);

    /* ======= Конфигурация обмена сообщениями ======= */

    // Определение роли приложения: клиент или сервер
    try
    {
        // Пытаемся открыть сокет

        #ifdef DETAIL_CONNECT_LOG
            cout << "Try to bind a socket..." << endl;
        #endif // DETAIL_CONNECT_LOG

        zmqReplaySocket.bind(socketAddress);

        #ifdef DETAIL_CONNECT_LOG
            cout << "Success!" << endl;
        #endif // DETAIL_CONNECT_LOG

        flag_isServer = ZMQ_SERVER;
    }
    catch(zmq::error_t err)
    {
        // Если не получилось, значит сервер его уже открыл

        #ifdef DETAIL_CONNECT_LOG
            cout << "Extension: \"" << err.what() << "\"." << endl;
        #endif // DETAIL_CONNECT_LOG

        flag_isServer = ZMQ_CLIENT;

        zmqRequestSocket.connect(socketAddress);

        #ifdef DETAIL_CONNECT_LOG
            cout << "Connection status: " <<
            (zmqReplaySocket.connected() == 1 ? "connected" : "disconnected")
            << " with \"" << socketAddress << "\"" << endl;
            //cout << "Close socket..." << endl << endl;
        #endif // DETAIL_CONNECT_LOG
    }


/* ================== Рабочий цикл приложения ================== */

    while(1)
    {

        /* ======= Выбор фрагмента кода: клиент или сервер ======= */

        switch(flag_isServer)
        {

    /* ===================== Серверная часть ===================== */

        case ZMQ_SERVER:


        /* ======= Инициализация сервера ======= */

            #ifdef DETAIL_LOG
                cout << endl << "====>  It's a server!" << endl << endl;
                cout << "Connection status: " <<
                    (zmqReplaySocket.connected() == 1 ? "connected" : "disconnected")
                    << " with \"" << socketAddress << "\"" << endl << endl;
            #endif // DETAIL_LOG

            // Если первое приложение запущено с ключом выключения всех
            if(flag_killAll)
            {
                cout << endl << "\"Kill All\" command was found." << endl;
                cout << "Closing application." << endl << endl;

                exit(0);
            }

            if(IDvector.empty())
            {
                currentApplicationID = genNewID(IDvector);
                serverID = currentApplicationID;
                IDvector.push_back(serverID);

                cout << "Application ID: " << currentApplicationID << endl;

                printIDVector(IDvector);
            }



            // Обнуляем статистику клиентов
            reinitClientStatistic();


        /* ======= Рабочий цикл сервера ======= */

            while(1)
            {
                #ifdef DETAIL_LOG
                    printIDVector(IDvector);
                #endif // DETAIL_LOG

                zmqReplaySocket.recv(&zmqMessage);

                ZMQCommand.ParseFromArray(zmqMessage.data(), zmqMessage.size());

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf recieved struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG

                switch (ZMQCommand.commandid())
                {


            /* ======= Запрос ID ======= */

                case ZMQ_CMD_IDRequest:

                    #ifdef DETAIL_EXCHANGE_LOG
                        cout << "Got command \"ZMQ_CMD_IDRequest\": " << ZMQ_CMD_IDRequest << endl;
                    #endif // DETAIL_EXCHANGE_LOG

                    IDvector.push_back(genNewID(IDvector));

                    // Обнуляем статистику клиентов
                    reinitClientStatistic();

                    #ifdef DETAIL_LOG
                        cout << endl << "New ID: " << IDvector.back() << endl << endl;
                    #endif // DETAIL_LOG

                    ZMQCommand.set_commandid(ZMQ_CMD_ID);
                    ZMQCommand.set_applicationid(currentApplicationID);
                    ZMQCommand.set_commanddata(IDvector.back());

                    break;


            /* ======= Запрос следующего ID вектора ======= */

                case ZMQ_CMD_MoreID:

                    #ifdef DETAIL_EXCHANGE_LOG
                        cout << "Got command \"ZMQ_CMD_MoreID\": " << ZMQ_CMD_MoreID << endl;
                    #endif // DETAIL_EXCHANGE_LOG

                    if(ZMQCommand.commanddata() < IDvector.size())
                    {
                        ZMQCommand.set_applicationid(IDvector[ZMQCommand.commanddata()]);
                        ZMQCommand.set_commanddata(currentApplicationID);
                    }
                    else ZMQCommand.set_applicationid(0);

                    // Потому что для приема и передачи один экземпляр класса zmq_cmd
                    temp = ZMQCommand.commanddata();
                    ZMQCommand.set_commanddata(ZMQCommand.applicationid());
                    ZMQCommand.set_applicationid(temp);

                    ZMQCommand.set_commandid(ZMQ_CMD_IDVector);

                    break;


            /* ======= Выключить все приложения ======= */

                case ZMQ_CMD_KillAll:

                    #ifdef DETAIL_EXCHANGE_LOG
                        cout << "Got command \"ZMQ_CMD_KillAll\": " << ZMQ_CMD_KillAll << endl;
                    #endif // DETAIL_EXCHANGE_LOG

                    flag_killAll = true;

                    ZMQCommand.set_commandid(ZMQ_CMD_Kill);
                    ZMQCommand.set_applicationid(currentApplicationID);
                    ZMQCommand.set_commanddata(0);

                    break;


            /* ======= Проверка необходимости выключения ======= */

                case ZMQ_CMD_KillCheck:

                    #ifdef DETAIL_EXCHANGE_LOG
                        cout << "Got command \"ZMQ_CMD_KillCheck\": " << ZMQ_CMD_KillCheck << endl;
                    #endif // DETAIL_EXCHANGE_LOG

                    updateClientStatistic(ZMQCommand.applicationid());

                    #ifdef DETAIL_STATISTIC_LOG
                        cout << endl << "Client statistic vector" << endl;
                        for(vector<pair<int, int>>::iterator i = clientStatistic.begin(); i < clientStatistic.end(); i++)
                        {
                            cout << "\t" << "ID: " << i->first << ", ACKs: " << i->second << "." << endl;
                        }
                        cout << endl;
                    #endif // DETAIL_STATISTIC_LOG

                    if(flag_killAll)
                    {
                        ZMQCommand.set_commanddata(1);

                        it = find(IDvector.begin(), IDvector.end(), ZMQCommand.applicationid());

                        if(it != IDvector.end()) IDvector.erase(it);
                        else cout << "Find ERROR!" << endl;
                    }
                    else
                    {
                        ZMQCommand.set_commanddata(0);
                    }

                    ZMQCommand.set_applicationid(currentApplicationID);
                    ZMQCommand.set_commandid(ZMQ_CMD_Kill);

                    break;


            /* ======= Запрос следующего ID вектора ======= */

                case ZMQ_CMD_UpdateVector:

                    #ifdef DETAIL_EXCHANGE_LOG
                        cout << "Got command \"ZMQ_CMD_MoreID\": " << ZMQ_CMD_MoreID << endl;
                    #endif // DETAIL_EXCHANGE_LOG

                    if(ZMQCommand.commanddata() < IDvector.size())
                    {
                        ZMQCommand.set_applicationid(IDvector[ZMQCommand.commanddata()]);
                        ZMQCommand.set_commanddata(currentApplicationID);
                    }
                    else ZMQCommand.set_applicationid(0);

                    // Потому что для приема и передачи один экземпляр класса zmq_cmd
                    temp = ZMQCommand.commanddata();
                    ZMQCommand.set_commanddata(ZMQCommand.applicationid());
                    ZMQCommand.set_applicationid(temp);

                    ZMQCommand.set_commandid(ZMQ_CMD_IDVectorUpdate);

                    break;

                default:
                    break;
                }

                zmqMessage.rebuild(ZMQCommand.SerializeAsString().c_str(), ZMQ_MESSAGE_SIZE);
                zmqReplaySocket.send(zmqMessage);

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf replied struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG

                if(flag_killAll && IDvector.size() == 1)
                {
                    cout << endl << "\"Kill All\" command was found." << endl;
                    cout << "Closing application." << endl << endl;

                    exit(0);
                }

                checkClientStatistic();

                if(flag_realTimeID)
                {
                    printIDVector(IDvector);
                }
            }



            break;




    /* ===================== Клиентская часть ===================== */

        case ZMQ_CLIENT:


        /* ======= Инициализация клиента ======= */

            #ifdef DETAIL_LOG
                cout << endl << "====>  It's a client!" << endl << endl;
            #endif // DETAIL_LOG


        /* ======= Закрыть все приложения ======= */

            if(flag_killAll)
            {
                // Запрос на выключение

                ZMQCommand.set_applicationid(currentApplicationID);
                ZMQCommand.set_commandid(ZMQ_CMD_KillAll);
                ZMQCommand.set_commanddata(0);

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf replied struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG

                zmqMessage.rebuild(ZMQCommand.SerializeAsString().c_str(), ZMQ_MESSAGE_SIZE);
                zmqRequestSocket.send(zmqMessage);


                // Ответ

                zmqRequestSocket.recv(&zmqMessage);

                ZMQCommand.ParseFromArray(zmqMessage.data(), zmqMessage.size());

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf recieved struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG

                if(ZMQCommand.commandid() == ZMQ_CMD_Kill)
                {
                    // Пришла команда выключения

                    cout << endl << "\"Kill All\" command was found." << endl;
                    cout << "Closing application." << endl << endl;

                    exit(0);
                }
            }

        /* ======= Запрос ID ======= */

            // Запрос ID у сервера

            ZMQCommand.set_applicationid(currentApplicationID);
            ZMQCommand.set_commandid(ZMQ_CMD_IDRequest);
            ZMQCommand.set_commanddata(0);

            #ifdef DETAIL_EXCHANGE_LOG
                cout << endl << "Protobuf replied struct:" << endl;
                cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
            #endif // DETAIL_EXCHANGE_LOG

            zmqMessage.rebuild(ZMQCommand.SerializeAsString().c_str(), ZMQ_MESSAGE_SIZE);
            zmqRequestSocket.send(zmqMessage);





            // Получение ID от сервера

            zmqRequestSocket.recv(&zmqMessage);

            ZMQCommand.ParseFromArray(zmqMessage.data(), zmqMessage.size());

            #ifdef DETAIL_EXCHANGE_LOG
                cout << endl << "Protobuf recieved struct:" << endl;
                cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
            #endif // DETAIL_EXCHANGE_LOG

            currentApplicationID = ZMQCommand.commanddata();

            serverID = ZMQCommand.applicationid();

            cout << "Application ID: " << currentApplicationID << endl;

            #ifdef DETAIL_LOG
                cout << "Server ID: " << serverID << endl;
            #endif // DETAIL_LOG




        /* ======= Запрос вектора ID активных приложений ======= */

            // Запрос очередного ID у сервера

            ZMQCommand.set_commandid(ZMQ_CMD_MoreID);
            ZMQCommand.set_applicationid(currentApplicationID);
            ZMQCommand.set_commanddata(IDvector.size());

            zmqMessage.rebuild(ZMQCommand.SerializeAsString().c_str(), ZMQ_MESSAGE_SIZE);
            zmqRequestSocket.send(zmqMessage);

            #ifdef DETAIL_EXCHANGE_LOG
                cout << endl << "Protobuf replied struct:" << endl;
                cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
            #endif // DETAIL_EXCHANGE_LOG

            while(1)
            {
                // Получение очередного ID от сервера

                zmqRequestSocket.recv(&zmqMessage);
                ZMQCommand.ParseFromArray(zmqMessage.data(), zmqMessage.size());

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf recieved struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG

                if(ZMQCommand.commanddata() == 0) break;
                else IDvector.push_back(ZMQCommand.commanddata());


                // Запрос очередного ID у сервера

                ZMQCommand.set_commandid(ZMQ_CMD_MoreID);
                ZMQCommand.set_applicationid(currentApplicationID);
                ZMQCommand.set_commanddata(IDvector.size());

                zmqMessage.rebuild(ZMQCommand.SerializeAsString().c_str(), ZMQ_MESSAGE_SIZE);
                zmqRequestSocket.send(zmqMessage);

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf replied struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG
            }

            printIDVector(IDvector);



        /* ======= Рабочий цикл клиента ======= */

            while (1)
            {
                #ifdef DETAIL_LOG
                    cout << "--------- New client iter ---------" << endl << endl;
                    cout << "Connection status: " <<
                        (zmqReplaySocket.connected() == 1 ? "connected" : "disconnected")
                        << " with \"" << socketAddress << "\"" << endl << endl;
                    printIDVector(IDvector);
                #endif // DETAIL_LOG

                if(flag_realTimeID)
                {
                    printIDVector(IDvector);
                }

                try
                {
                    #ifdef DETAIL_CONNECT_LOG
                        cout << "Try to rebind a socket..." << endl;
                    #endif // DETAIL_CONNECT_LOG

                    zmqReplaySocket.bind(socketAddress);

                    #ifdef DETAIL_CONNECT_LOG
                        cout << "Success..." << "  Server was turned off!" << endl;
                        cout << "Init a new server..." << endl;
                    #endif // DETAIL_CONNECT_LOG

                    initNewServer();

                    break;
                }
                catch(zmq::error_t err)
                {
                    #ifdef DETAIL_CONNECT_LOG
                        cout << "Extension: \"" << err.what() << "\"." << endl;
                        cout << "Server is active!" << endl;
                    #endif // DETAIL_CONNECT_LOG
                }



            /* ======= Отключение по ключу '-k' ======= */

                // Запрос

                ZMQCommand.set_commandid(ZMQ_CMD_KillCheck);
                ZMQCommand.set_applicationid(currentApplicationID);
                ZMQCommand.set_commanddata(0);

                zmqMessage.rebuild(ZMQCommand.SerializeAsString().c_str(), ZMQ_MESSAGE_SIZE);
                zmqRequestSocket.send(zmqMessage);

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf replied struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG


                // Ответ

                zmqRequestSocket.recv(&zmqMessage);
                ZMQCommand.ParseFromArray(zmqMessage.data(), zmqMessage.size());

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf recieved struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG

                serverID = ZMQCommand.applicationid();

                if(ZMQCommand.commanddata() == 1)
                {
                    // Пришла команда выключения

                    cout << endl << "\"Kill All\" command was found." << endl;
                    cout << "Closing application." << endl << endl;

                    exit(0);
                }

            /* ======= Обновление вектора активных приложений ======= */

                IDvector.clear();
                //IDvector.push_back(currentApplicationID);

                // Запрос очередного ID у сервера

                ZMQCommand.set_commandid(ZMQ_CMD_UpdateVector);
                ZMQCommand.set_applicationid(currentApplicationID);
                ZMQCommand.set_commanddata(IDvector.size());

                zmqMessage.rebuild(ZMQCommand.SerializeAsString().c_str(), ZMQ_MESSAGE_SIZE);
                zmqRequestSocket.send(zmqMessage);

                #ifdef DETAIL_EXCHANGE_LOG
                    cout << endl << "Protobuf replied struct:" << endl;
                    cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                    cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                    cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                #endif // DETAIL_EXCHANGE_LOG

                while(1)
                {
                    // Получение очередного ID от сервера

                    zmqRequestSocket.recv(&zmqMessage);
                    ZMQCommand.ParseFromArray(zmqMessage.data(), zmqMessage.size());

                    #ifdef DETAIL_EXCHANGE_LOG
                        cout << endl << "Protobuf recieved struct:" << endl;
                        cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                        cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                        cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                    #endif // DETAIL_EXCHANGE_LOG

                    if(ZMQCommand.commanddata() == 0) break;
                    else IDvector.push_back(ZMQCommand.commanddata());


                    // Запрос очередного ID у сервера

                    ZMQCommand.set_commandid(ZMQ_CMD_UpdateVector);
                    ZMQCommand.set_applicationid(currentApplicationID);
                    ZMQCommand.set_commanddata(IDvector.size());

                    zmqMessage.rebuild(ZMQCommand.SerializeAsString().c_str(), ZMQ_MESSAGE_SIZE);
                    zmqRequestSocket.send(zmqMessage);

                    #ifdef DETAIL_EXCHANGE_LOG
                        cout << endl << "Protobuf replied struct:" << endl;
                        cout << "\tcommandID: " << ZMQCommand.commandid() << endl;
                        cout << "\tcommandData: " << ZMQCommand.commanddata() << endl;
                        cout << "\tapplicationID: " << ZMQCommand.applicationid() << endl << endl;
                    #endif // DETAIL_EXCHANGE_LOG
                }

                usleep(CLIENT_CYCLE_USEC);
            }








            break;

        default:

            cout << "====> Error: Unknown device type!" << endl;

            return 0;

            break;
        }
    }



    return 0;
}
