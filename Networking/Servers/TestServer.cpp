#include "TestServer.hpp"

TestServer::TestServer()
: Server(AF_INET, SOCK_STREAM, 0, 3490, INADDR_ANY, 10) { launch(); }

void    TestServer::_accept()
{
    struct sockaddr_in address = getSocket()->getAddress();
    int addrlen = sizeof(address);
    _newSocket = accept(getSocket()->getSock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    testConnection(_newSocket);
    read(_newSocket, _buffer, 30000);
}

void    TestServer::_response()
{
    std::cout << _buffer << std::endl;
    std::istringstream iss(_buffer);
    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

    std::string content;
    int errorCode = 200;

    if (parsed.size() >= 3 && parsed[0] == "GET")
    {
        if (parsed[1].size() != 1)
        {
            std::ifstream f("./Pages/" + parsed[1]);
            if (f.good())
            {
                std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                content = str;
                errorCode = 200;
            }
            f.close();
        }
        else
        {
            std::ifstream f("./Pages/default.html");
            if (f.good())
            {
                std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                content = str;
                errorCode = 200;
            }
            f.close();
        }
    }

    std::ifstream f("./Pages/" + parsed[1]);
    if (!f.good())
    {
        std::ifstream f2("./Pages/error.html");
        if (f2.good())
        {
            std::string str((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());
            content = str;
            errorCode = 404;
        }
        f2.close();
    }
    f.close();

    std::ostringstream oss;
    oss << "HTTP/1.1 " << errorCode << " OK\r\n";
    oss << "Cache-Control: no-cache, private\r\n";
    oss << "Content-type: text/html\r\n";
    oss << "Content-Length: " << content.size() << "\r\n";
    oss << "\r\n";
    oss << content;

    std::string output = oss.str();
    int size = output.size();

    sendToClient(output.c_str(), size);
}

void    TestServer::sendToClient(const char *msg, int len)
{
    int bytes_sending;
 //   size_t bytes_sent;

    bytes_sending = send(_newSocket, msg, len, 0);
    testConnection(bytes_sending);
  //  bytes_sent = recv(_newSocket, (void *)msg, len, 0);
  //  testConnection((int)bytes_sent);
    close(_newSocket);
}

template<typename T>
void    TestServer::testConnection(T item)
{
    if (item < 0)
    {
        perror("Connection failed...");
        exit(EXIT_FAILURE);
    }
}

void    TestServer::launch()
{
    while (1)
    {
        std::cout << "======= WAITING ======\n";
        _accept();
        _response();
        std::cout << "======= DONE ======\n";
    }
    close(getSocket()->getSock());
}