#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace std;

boost::asio::io_service io_service;
boost::asio::ip::tcp::socket sock(io_service);
boost::array<char, 4096> buffer;
boost::asio::ip::tcp::endpoint vlcEndpoint(boost::asio::ip::address::from_string("192.168.1.118"), 4212);

void connect()
{
    boost::system::error_code ec;

    sock.connect(vlcEndpoint, ec);
    if (ec)
        {cout << "Could not connect to the server" << endl;}
    else
        {cout << "Connection Successful" << endl;}
}

void disconnect()
{
    sock.close();
}

void status()
{
    if (sock.is_open())
        {cout << "Connection Active" << endl;}
    else
        {cout << "Connection Inactive" << endl;}
}

void write()
{
     boost::asio::write(sock, boost::asio::buffer("Test", 60));
}

int main(int argc, char* argv[])
{
  try
  {
    connect();
    status();

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = sock.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
