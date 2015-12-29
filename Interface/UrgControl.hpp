#ifndef _URGCONTROL_HPP
#define _URGCONTROL_HPP
#include "stdafx.h"
#include "Point2D.hpp"
#include <string>
#include <windows.h>


using namespace std;

typedef struct {
	enum {
		MODL = 0,		//!< Sensor Model
		DMIN,			//!< Min detection range [mm]
		DMAX,			//!< Man detection range [mm]
		ARES,			//!< Angular resolution (division of 360degree)
		AMIN,			//!< Min Measurement step
		AMAX,			//!< Max Measurement step
		AFRT,			//!< Front Step 
		SCAN,			//!< Standard scan speed
	};
	std::string model;		//!< Obtained Sensor Model,  MODL
	long distance_min;		//!< Obtained DMIN 
	long distance_max;		//!< Obtained DMAX 
	int area_total;		//!< Obtained ARES 
	int area_min;			//!< Obtained AMIN 
	int area_max;			//!< Obtained AMAX 
	int area_front;		//!< Obtained AFRT 
	int scan_rpm;			//!< Obtained SCAN 

	int first;			//!< Scan Start position
	int last;			//!< Scan end position
	int max_size;			//!< Max. size of data
	long last_timestamp; //!< Time stamp of the last obtained data
} urg_state_t;

class UrgControl
{
public:
	UrgControl(string port_name, int baud_rate);
	bool openPort(string port_name, int baud_rate);
	void closePort(){}
	void getScan682();
	void Scan796(int *range769);
	void deleteNoiseData(double max_p2p_dist);
	Point2DSet getValidPoints(double max_p2p_dist);
	Point2DSet UrgControl::get682centervalues(int point_numbers);
	HANDLE _hComm;
	vector<int> _vData;
	int _buffer[769];
	string _errMsg;
	static const int TIMEOUT = 800;
	static const int LINE_LENGTH = 16 + 64 + 1 + 1 + 1;

// private:
	urg_state_t urg_state;
	int com_send(const char* data, int size);
	int com_recv(char* data, int max_size, int timeout);
	int urg_sendTag(const char* tag);
	int urg_readLine(char *buffer);
	int urg_sendMessage(const char* command, int timeout, int* recv_n);
	int urg_getParameters(urg_state_t* state);
	//int urg_connect(urg_state_t* state, const char* port, const long baudrate);
	void urg_disconnect(void);
	int urg_captureByGD(const urg_state_t* state);
	int urg_captureByMD(const urg_state_t* state, int capture_times);
	long urg_decode(const char* data, int data_byte);
	int urg_addRecvData(const char buffer[], long data[], int* filled);
	int urg_receiveData(urg_state_t* state, long data[], size_t max_size);
	int Robot_size;
};

#endif