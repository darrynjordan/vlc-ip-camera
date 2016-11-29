#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <ctime>

//namespaces
using namespace boost::asio;
using namespace std;

//declare functions
void enterPassword(string text);
void setupMic();
void start();
void stop();
void connect();

//global variables
char buff[300];
char option;
io_service service;
ip::tcp::socket sock(service);

//functions
void clearBuffer()
{
    for (int i = 0; i < 301; i++)
    {
        buff[i] = ' '; //clear all elements of the array
    }
}

void connect()
{
    system("clear\n"); //clear console
    cout << "Attempting to establish connection..." << endl << endl;
    ip::tcp::endpoint ep(ip::address::from_string("192.168.1.143"), 4212); //define the endpoint at the known server address & port

    try
        {
            sock.connect(ep); //attempt to connect to the endpoint, if no exception is thrown the connection is successful
            cout << "Connection Active!" << endl;
            enterPassword("vlc\n"); //enter the vlc server password
            cout << "Password has been entered" << endl << endl;

            cout << "Set-up Microphone? [y/n]" << endl << endl;
            while(true) //wait for response
            {
                cin >> option;
                if (option == 'y')
                    {setupMic();}
                else if (option == 'n')
                    {exit(0);} //close program
                else
                    {cout << "Invalid Response. Please use 'y' or 'n'" << endl;}
            }

        }
    catch (boost::system::system_error const& e) //exception was thrown, connection failed
        {
            cout << "Warning: could not " << e.what() << endl << endl;

            cout << "Retry Connection? [y/n]" << endl << endl;
            while(true)
            {
                cin >> option;
                if (option == 'y')
                    {connect();} //restart connection
                else if (option == 'n')
                    {exit(0);}
                else
                    {cout << "Invalid Response. Please use 'y' or 'n'" << endl;}
            }
        }
}

void write(string text)
{
    sock.write_some(buffer(text)); //write to the terminal
    cout << text; //echo to console
}

void read()
{
    clearBuffer();
    sock.read_some(buffer(buff)); //read terminal response to the buffer array
    cout << buff << endl; //echo to console
}

void setupMic()
{
    system("clear\n"); //clear console
    cout << "Configuring Microphone..." << endl << endl;

    write("del mic\n"); //delete any previous instances of 'mic' if they exist
    write("new mic broadcast enabled\n"); //create a new instance of 'mic' and enable it
    write("setup mic input alsa://hw:0,0\n"); //set the input to hardware microphone

    time_t rawtime;                 //
    struct tm * timeinfo;           //
    char buffer[80];                //
                                    //code to get date & time as a atring - not sure how this works
    time (&rawtime);                //
    timeinfo = localtime(&rawtime); //

    strftime(buffer,80,"%d.%m.%Y-%I:%M:%S",timeinfo); //set date & time format

    string dateTime(buffer); //define date & time string

    write("setup mic output #standard{access=file,mux=wma,dst=" + dateTime + ".wav}\n"); //setup the output type, encoding format and filename

    cout << endl << "Microphone Configured!" << endl << endl;

    cout << "Start Recoding? [y/n]" << endl << endl;
    while(true)
    {
        cin >> option;
        if (option == 'y')
            {start();}
        else if (option == 'n')
            {exit(0);}
        else
            {cout << "Invalid Response. Please use 'y' or 'n'" << endl;}
    }

}

void start()
{
    system("clear\n"); //clear console
    cout << "Recording..." << endl << endl;
    write("control mic play\n"); //start recoring

    cout << endl << "Stop Recoding? [y/n]" << endl << endl;
    while(true)
    {
        cin >> option;
        if (option == 'y')
            {stop();}
        else if (option == 'n')
            {exit(0);}
        else
            {cout << "Invalid Response. Please use 'y' or 'n'" << endl;}
    }

}

void stop()
{
    system("clear\n"); //clear console
    cout << "Recording Stoped!" << endl << endl;
    write("control mic stop\n"); //stop recording

    cout << endl << "Setup New Recoding? [y/n]" << endl << endl;
    while(true)
    {
        cin >> option;
        if (option == 'y')
            {setupMic();}
        else if (option == 'n')
            {exit(0);}
        else
            {cout << "Invalid Response. Please use 'y' or 'n'" << endl;}
    }
}

void enterPassword(string text)
{
    sock.write_some(buffer(text)); //this will not be shown on the console
    read();
}

void welcome()
{
    cout << "UCT NeXtRad VLC Control Client" << endl;
    cout << "--------------------------------" << endl << endl;
    cout << "Steps:" << endl;
    cout << "1 - Establish Connection" << endl;
    cout << "2 - Set-up Microphone" << endl;
    cout << "3 - Start Recording" << endl;
    cout << "4 - Stop Recording" << endl << endl;
}

int main()
{
    welcome(); //display welcome screen

    cout << "Establish Connection? [y/n]" << endl << endl;
    while(true) //wait for response
    {
        cin >> option;
        if (option == 'y')
            {connect();} //chose yes - attempt connection
        else if (option == 'n')
            {break;} //chose no - close program
        else
            {cout << "Invalid Response. Please use 'y' or 'n'" << endl;} //request new response
    }
    return 0;
}
