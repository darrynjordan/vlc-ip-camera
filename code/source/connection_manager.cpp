#include "connection_manager.hpp"


ConnectionManager::ConnectionManager(void)  : socket(service)
{
	socketEndpoint.address(ip::address::from_string(TELNET_HOST)); 
	socketEndpoint.port(TELNET_PORT);	
	
	string cameraProfile;
    
	switch(CAMERA_RESOLUTION)
	{
		case 1080: 
		{
			cameraOverlayConfig = "sfilter=logo{file='../overlay_images/CameraOverlayNode1_1080p.png'},"; 
			cameraProfile = "profile1";
			break;
		}
		
		case 720: 
		{
			cameraOverlayConfig = "sfilter=logo{file='../overlay_images/CameraOverlayNode1_720p.png'},";  
			cameraProfile = "profile2";
			break;
		}
		
		case 480: 
		{
			cameraOverlayConfig = "sfilter=logo{file='../overlay_images/CameraOverlayNode1_480p.png'},"; 
			cameraProfile = "profile3"; 
			break;
		}
		
		case 240: 
		{
			cameraOverlayConfig = "sfilter=logo{file='../overlay_images/CameraOverlayNode1_240p.png'},"; 
			cameraProfile = "profile3"; 
			break;
		}
	}
	
	ipCameraAddress = (string)"rtsp://" + (string)RTSP_HOST + (string)":" + (string)RTSP_PORT + (string)"/live/video/" + cameraProfile;	
}


void ConnectionManager::clearSocketBuffer(void)
{
	for (int i = 0; i < SOCKET_BUFFER_SIZE; i++)
    {
        socketBuffer[i] = ' '; 
    }
}


void ConnectionManager::connectToSocket(void)
{
    cout << "Attempting to establish connection to VLC server..." << endl;  
    try
    {
		socket.connect(socketEndpoint); //attempt to connect to the endpoint, if no exception is thrown the connection is successful
		printf("Connection Active!\n");
		writeToSocket(TELNET_PASSWORD, PRIVATE); //enter the vlc server password
		//readFromSocket();  //display vlc response
    }
    catch (boost::system::system_error const& e) //exception was thrown, connection failed
    {
		cout << "Exiting: could not " << e.what();		
		exit(0);
		//connectToSocket(); //restart connection

    }
}


void ConnectionManager::writeToSocket(string message, WritePrivacy privacy)
{
	socket.write_some(buffer(message + (string)"\n")); //write to the socket
	
	if (privacy == PUBLIC)
	{
		cout << " ~ " << message << endl;   //echo to console
	}            		
}


void ConnectionManager::readFromSocket(void)
{
	clearSocketBuffer();
    socket.read_some(buffer(socketBuffer)); //read terminal response to the buffer array
    cout << socketBuffer << endl;         	//echo to console
}


string ConnectionManager::getTimeAndDate(const char *format)
{
	time_t rawtime;                 
    struct tm * timeinfo;           
    char buffer[80];                                     
    time (&rawtime);                
    timeinfo = localtime(&rawtime); 
    strftime(buffer,80,format,timeinfo); 
    string dateTime(buffer);            
    return dateTime;
}


void ConnectionManager::configureVideoStream(void)
{
    printf("Configuring RTSP stream...\n");

    writeToSocket("del stream", PUBLIC);        //delete any previous instances of 'stream' if they exist
    writeToSocket("new stream broadcast enabled", PUBLIC); //create a new instance of 'stream' and enable it
    writeToSocket("setup stream input " + ipCameraAddress, PUBLIC); //set stream input    
    //writeToSocket("setup stream output #transcode{" + cameraOverlayConfig + "vcodec=h264,acodec=none}:std{access=file,mux=mp4,dst=" + OUTPUT_DIRECTORY + getTimeAndDate("%d.%m.%y-%I:%M:%S") + ".mp4}", PUBLIC); //setup the output type, encoding format and filename
	writeToSocket("setup stream output #file{mux=mp4,dst=" + (string)OUTPUT_DIRECTORY + getTimeAndDate("%d.%m.%y-%I:%M:%S") + ".mp4,no-overite}", PUBLIC); //setup the output type, encoding format and filename
	
    printf("Stream Configured!\n");
    printf("-----------------------------\n");
}


void ConnectionManager::startRecording(time_t startTime, time_t endTime)
{
    printf("\nWaiting for Recording to Begin...\n\n");
   
    time_t oldTime;
    time_t currentTime = time(NULL);	
    
    while (currentTime != startTime)
    {
		currentTime = time(NULL);
		
		if (currentTime == startTime)
		{
			cout << startTime - currentTime << "s to start\n";	
			break;
		}
		
		if (currentTime != oldTime)
		{
			cout << startTime - currentTime << "s to start\n";			
		}
		
		oldTime = currentTime;		
	}
	
	printf("\nRecording...\n\n");	
	writeToSocket("control stream play", PUBLIC); //start recoring
	cout << endl;

    while (currentTime < endTime)
    {
        currentTime = time(NULL);
        
        if (currentTime != oldTime)
        {     
            cout << "Time Elapsed: " << currentTime - startTime << "s" << endl;
        }
        oldTime = currentTime;
    }  

}


void ConnectionManager::stopRecording(void)
{
    printf("Recording Stoped!\n");
    writeToSocket("control stream stop", PUBLIC); //stop recording
    printf("-----------------------------\n\n");
}






