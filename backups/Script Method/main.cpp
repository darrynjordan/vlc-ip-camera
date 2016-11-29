#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void welcome();

void connect()
{
    system("clear\n");
    welcome();
    cout << "Establishing Connection..." << endl << endl;
    system("./Connect");
    cout << endl << endl;
}

void start()
{
    system("clear\n");
    welcome();
    cout << "Recording Started..." << endl << endl;
    system("./Start");
    cout << endl << endl;
}

void stop()
{
    system("clear\n");
    welcome();
    cout << "Recording Stopped..." << endl << endl;
    system("./Stop");
    cout << endl << endl;
}

void welcome()
{
    cout << "UCT NeXtRad Video Control Client" << endl;
    cout << "--------------------------------" << endl << endl;
    cout << "OPTIONS:" << endl;
    cout << "1 - Establish Connection" << endl;
    cout << "2 - Start Recording" << endl;
    cout << "3 - Stop Recording" << endl;
    cout << "4 - Exit" << endl << endl;
}

int main()
{
    welcome();

    int option;

    for(;;)
    {
        cin >> option;
        if (option == 1)
            {connect();}
        if (option == 2)
            {start();}
        if (option == 3)
            {stop();}
        if (option == 4)
            {break;}
    }

    return 0;
}
