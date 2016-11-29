#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace std;

boost::asio::io_service io_service;
boost::asio::ip::tcp::socket sock(io_service);
boost::asio::ip::tcp::endpoint vlcEndpoint(boost::asio::ip::address::from_string("192.168.1.118"), 4212);

void connect()
{
    //define an error varable
    boost::system::error_code ec;

    //open connection to endpoint
    sock.connect(vlcEndpoint, ec);
    if (ec)     //error occured
        {cout << "Could not connect to the server" << endl;}
    else        //no error
        {cout << "Connection Successful" << endl;}
}

void disconnect()
{
    //close the socket connection
    sock.close();
}

void write()
{
     boost::asio::write(sock, boost::asio::buffer("vlc\n"));
     sleep(0.5);
     boost::asio::write(sock, boost::asio::buffer("new mic broadcast enabled\n"));
     sleep(0.5);
     boost::asio::write(sock, boost::asio::buffer("setup mic input alsa://hw:0,0\n"));
     sleep(0.5);
     boost::asio::write(sock, boost::asio::buffer("setup mic output #standard{access=file,mux=ogg,dst=mic}\n"));
     sleep(0.5);
     boost::asio::write(sock, boost::asio::buffer("control mic play\n"));

}

void read()
{
      boost::array<char, 1000> buf;
      boost::system::error_code error;
      sock.read_some(boost::asio::buffer(buf));
      cout << buf.data() << endl;
}

int main()
{
    connect();
    write();
    read();


    return 0;
}
