#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::string;

int post(const string& host, const string& port, const string& page, const string& data, string& reponse_data)
{
    try
    {
        boost::asio::io_service service;
        //如果io_service存在复用的情况
        if(service.stopped())
            service.reset();

        // 从dns取得域名下的所有ip
        tcp::resolver resolver(service);
        tcp::resolver::query query(host, port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    
        // 尝试连接到其中的某个ip直到成功 
        tcp::socket socket(service);
        boost::asio::connect(socket, endpoint_iterator); 

        /*
        Form the request. We specify the "Connection: close" header so that the
        server will close the socket after transmitting the response. This will
        allow us to treat all data up until the EOF as the content.
        */
        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "POST " << page << " HTTP/1.0\r\n";
        request_stream << "Host: " << host << ":" << port << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Content-Length: " << data.length() << "\r\n";
        request_stream << "Content-Type: application/x-www-form-urlencoded\r\n";
        request_stream << "Connection: close\r\n\r\n";
        request_stream << data;

        // Send the request.
        boost::asio::write(socket, request);

        /*
        Read the response status line. The response streambuf will automatically
        grow to accommodate the entire line. The growth may be limited by passing
        a maximum size to the streambuf constructor.
        */
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        // Check that response is OK.
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            reponse_data = "Invalid response";
            return -2;
        }
        // 如果服务器返回非200都认为有错,不支持301/302等跳转
        if (status_code != 200)
        {
            reponse_data = "Response returned with status code != 200 " ;
            return status_code;
        }

        // 传说中的包头可以读下来了
        std::string header;
        std::vector<string> headers;        
        while (std::getline(response_stream, header) && header != "\r")
            headers.push_back(header);

        // 读取所有剩下的数据作为包体
        boost::system::error_code error;
        while (boost::asio::read(socket, response,
            boost::asio::transfer_at_least(1), error))
        {           
        }

        //响应有数据
        if (response.size())
        {
            std::istream response_stream(&response);
            std::istreambuf_iterator<char> eos;
            reponse_data = string(std::istreambuf_iterator<char>(response_stream), eos);                        
        }

        if (error != boost::asio::error::eof)
        {
            reponse_data = error.message();
            return -3;
        }
    }
    catch(std::exception& e)
    {
        reponse_data = e.what();
        return -4;  
    }
    return 0;
}
