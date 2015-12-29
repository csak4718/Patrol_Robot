#include "stdafx.h"
#include "UrgControl.hpp"
#include "Point2D.hpp"
#include <iostream>
#include <cmath>

using namespace std;

UrgControl::UrgControl(string port_name, int baud_rate)
{
	openPort(port_name, baud_rate);
	urg_getParameters(&urg_state);
	Robot_size=150;//150 mm
}
bool UrgControl::openPort(string port_name, int baud_rate)
{
	DCB dcb;
	_hComm = CreateFile(port_name.c_str(),
						GENERIC_READ | GENERIC_WRITE, 
						0,		// exclusive access
						0,		// no security
						OPEN_EXISTING,
						0,		// no overlapped I/O
						NULL);	// null template

	if(_hComm == INVALID_HANDLE_VALUE){
		_errMsg = string("openPort: _hComm == INVALID_HANDLE_VALUE");
		return false;
	}

	if(!GetCommState(_hComm, &dcb)){
		_errMsg = string("openPort: GetCommState error!");
		return false;
	}

	// Baud Rate
	dcb.BaudRate = baud_rate;
	// 8N1
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fErrorChar = FALSE;
	dcb.fNull = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fAbortOnError = FALSE;
	dcb.fDummy2 = 0;
	dcb.wReserved = 0;
	dcb.XonLim = 0;
	dcb.XoffLim = 0;
	dcb.XonChar = 0;
	dcb.XoffChar = 0;
	dcb.ErrorChar = 0;
	dcb.EofChar = 0;
	dcb.EvtChar = 0;
	dcb.wReserved1 =0;
	
	if(!SetCommState(_hComm, &dcb)){ 
		_errMsg = string("openPort: SetCommState error!");
		return false;
	}
	if(!SetCommMask(_hComm, EV_RXCHAR)){ 
		_errMsg = string("openPort: SetCommMask error!");
		return false;
	}
/*
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTIMEOUT = MAXDWORD;
	timeouts.ReadTotalTIMEOUTMultiplier = 0;
	timeouts.ReadTotalTIMEOUTConstant = 0;
	timeouts.WriteTotalTIMEOUTMultiplier = 10;
	timeouts.WriteTotalTIMEOUTConstant = 100;
	if (!SetCommTIMEOUTs(_hComm, &timeouts)){
		// Error setting time-outs.
		_errMsg = string("Error setting time-outs");
		return false;
	}
*/ // copy from "MotorPortControl.cpp" 
	return true;
}

void UrgControl::getScan682()
{
	urg_captureByMD(&urg_state, 1);
	//int recv_n = 0;
	//urg_sendMessage("BM", TIMEOUT, &recv_n);
	//urg_captureByGD(&urg_state);
	urg_receiveData(&urg_state, (long*)_buffer, urg_state.max_size);
	_vData.clear();
	for(int i=0; i<682; i++){
		_vData.push_back(_buffer[i+44]);
	}
}
void UrgControl::Scan796(int *range769){
	urg_captureByMD(&urg_state, 1);
	urg_receiveData(&urg_state, (long*)range769, urg_state.max_size);
}
void UrgControl::deleteNoiseData(double max_p2p_dist)
{
	vector<int> noise_index;
	for(int i=1; i<(int)_vData.size()-1; i++){
		/* general case
		if(_vData[i] < 19 || (abs(_vData[i] - _vData[i-1]) > max_p2p_dist*10 && abs(_vData[i] - _vData[i+1]) > max_p2p_dist*10)){
			noise_index.push_back(i);
		}
		*/
		// just for RenBot
		if(_vData[i] < Robot_size || (abs(_vData[i] - _vData[i-1]) > max_p2p_dist*10 && abs(_vData[i] - _vData[i+1]) > max_p2p_dist*10)){
			noise_index.push_back(i);
		}//robot size
	}
	// just for RenBot
	for(int i=0; i<45; i++){
		noise_index.push_back(i);
		noise_index.push_back(681-i);
	}
	// end just for RenBot
	for(int i=0; i<(int)noise_index.size(); i++){
		_vData[noise_index[i]] = 0;
	}
}
Point2DSet UrgControl::getValidPoints(double max_p2p_dist = 0)
{
	Point2DSet ps;
	deleteNoiseData(max_p2p_dist);
	for(int i=0; i<(int)_vData.size(); i++){
		if(_vData[i] != 0){
			ps.addPoint(new Point2D(_vData[i]/10,(-30+i*0.3515625)*3.1415926/180,true));
		}
	}
	return ps;
}
int UrgControl::com_send(const char *data, int size)
{
	DWORD n;
	WriteFile(this->_hComm, data, size, &n, NULL);
	return n;
}

int UrgControl::com_recv(char *data, int max_size, int timeout)
{
	int filled = 0;
	int readable_size = 0;

	// Reading of Concerned data.
	do {
		DWORD dwErrors;
		COMSTAT ComStat;
		ClearCommError(_hComm, &dwErrors, &ComStat);
		readable_size = (int)ComStat.cbInQue;
		int read_n = (readable_size > max_size) ? max_size : readable_size;

		DWORD n;
		ReadFile(_hComm, &data[filled], read_n, &n, NULL);
		filled += n;
		readable_size -= n;

		if (filled >= max_size) {
			return filled;
		}
	}while(readable_size > 0);

	if(timeout > 0){
		// Read data using time out
		COMMTIMEOUTS pcto;
		GetCommTimeouts(_hComm, &pcto);
		pcto.ReadIntervalTimeout = 0;
		pcto.ReadTotalTimeoutMultiplier = 0;
		pcto.ReadTotalTimeoutConstant = timeout;
		SetCommTimeouts(_hComm, &pcto);

		//Read data one charaters each
		DWORD n;
		while (1) {
			ReadFile(_hComm, &data[filled], 1, &n, NULL);
			if (n < 1) {
				return filled;
			}
			filled += n;
			if (filled >= max_size) {
				return filled;
			}
		}
	}
	return filled;
}

int UrgControl::urg_sendTag(const char* tag) 
{
	char send_message[LINE_LENGTH];
	sprintf_s(send_message, "%s\n", tag);
	int send_size = strlen(send_message);
	com_send(send_message, send_size);
	return send_size;
}



int UrgControl::urg_readLine(char *buffer)
{
	int i;
	for (i = 0; i < LINE_LENGTH -1; ++i) {
		char recv_ch;
		int n = com_recv(&recv_ch, 1, TIMEOUT);
		if (n <= 0) {
			if (i == 0) {
				return -1;		// timeout
			}
			break;
		}
		if ((recv_ch == '\r') || (recv_ch == '\n')) {
			break;
		}
		buffer[i] = recv_ch;
	}
	buffer[i] = '\0';
	return i;
}
int UrgControl::urg_sendMessage(const char *command, int timeout, int *recv_n)
{
	int send_size = urg_sendTag(command);
	int recv_size = send_size + 2 + 1 + 2;
	char buffer[LINE_LENGTH];
	int n = com_recv(buffer, recv_size, timeout);
	
	*recv_n = n;
	
	if (n < recv_size) {
		// When received size not matched
		return -1;
	}

	if (strncmp(buffer, command, send_size -1)) {
		// When command not matched
		return -1;
	}

	// !!! If possible do calculate check-sum to verify data
	// Convert the received data to Hex and return data.
	char reply_str[3] = "00";
	reply_str[0] = buffer[send_size];
	reply_str[1] = buffer[send_size + 1];
	return strtol(reply_str, NULL, 16);
}

int UrgControl::urg_getParameters(urg_state_t *state)
{
	// Parameter read and prcessing (use)
	urg_sendTag("PP");
	char buffer[LINE_LENGTH];
	int line_index = 0;
	enum {
		TagReply = 0,
		DataReply,
		Other,
	};
	int line_length;
	for (; (line_length = urg_readLine(buffer)) > 0; ++line_index) {
		if (line_index == Other + urg_state_t::MODL) {
			buffer[line_length - 2] = '\0';
			state->model = &buffer[5];
		}else if(line_index == Other + urg_state_t::DMIN){
			state->distance_min = atoi(&buffer[5]);
		}else if(line_index == Other + urg_state_t::DMAX) {
			state->distance_max = atoi(&buffer[5]);
		}else if(line_index == Other + urg_state_t::ARES) {
			state->area_total = atoi(&buffer[5]);
		}else if(line_index == Other + urg_state_t::AMIN) {
			state->area_min = atoi(&buffer[5]);
			state->first = state->area_min;
		}else if(line_index == Other + urg_state_t::AMAX) {
			state->area_max = atoi(&buffer[5]);
			state->last = state->area_max;
		}else if(line_index == Other + urg_state_t::AFRT) {
			state->area_front = atoi(&buffer[5]);
		}else if(line_index == Other + urg_state_t::SCAN) {
			state->scan_rpm = atoi(&buffer[5]);
		}
	}
	if (line_index <= Other + urg_state_t::SCAN) {
		return -1;
	}
	// Caluclate size of the data if necessary
	state->max_size = state->area_max +1;
	return 0;
}
void UrgControl::urg_disconnect()
{
	closePort();
}

int UrgControl::urg_captureByGD(const urg_state_t *state)
{
	char send_message[LINE_LENGTH];
	sprintf_s(send_message, "GD%04d%04d%02d", state->first, state->last, 0);
	return urg_sendTag(send_message);
}

int UrgControl::urg_captureByMD(const urg_state_t *state, int capture_times)
{
	char send_message[LINE_LENGTH];
	sprintf_s(send_message, "MD%04d%04d%02d%01d%02d", state->first, state->last, 0, 0, capture_times);
	return urg_sendTag(send_message);
}
long UrgControl::urg_decode(const char *data, int data_byte)
{
	long value = 0;
	for (int i = 0; i < data_byte; ++i) {
		value <<= 6;
		value &= ~0x3f;
		value |= data[i] - 0x30;
	}
	return value;
}
int UrgControl::urg_addRecvData(const char buffer[], long data[], int *filled)
{
	static int remain_byte = 0;
	static char remain_data[3];
	const int data_byte = 3;
	const char* pre_p = buffer;
	const char* p = pre_p;
	if (remain_byte > 0) {
		memmove(&remain_data[remain_byte], buffer, data_byte - remain_byte);
		data[*filled] = urg_decode(remain_data, data_byte);
		++(*filled);
		pre_p = &buffer[data_byte - remain_byte];
		p = pre_p;
		remain_byte = 0;
	}
	do{
		++p;
		if ((p - pre_p) >= static_cast<int>(data_byte)) {
			data[*filled] = urg_decode(pre_p, data_byte);
			++(*filled);
			pre_p = p;
		}
	}while(*p != '\0');
	remain_byte = p - pre_p;
	memmove(remain_data, pre_p, remain_byte);
	return 0;
}
Point2DSet UrgControl::get682centervalues(int point_numbers){
	Point2DSet ps;
	for(int i=341-point_numbers/2;i<341+point_numbers/2;i++)
		ps.addPoint(new Point2D(_vData[i]/10,(-30+i*0.3515625)*3.1415926/180,true));
	return ps;

}
int UrgControl::urg_receiveData(urg_state_t *state, long data[], size_t max_size)
{
	int filled = 0;
	// Fill the positions upto first or min by 19 (non-measurement range)
	for (int i = state->first -1; i >= 0; --i) {
		data[filled++] = 19;
	}
	char message_type = 'M';
	char buffer[LINE_LENGTH];
	int line_length;
	for (int i = 0; (line_length = urg_readLine(buffer)) >= 0; ++i) {
		// Verify the checksum
		if ((i >= 6) && (line_length == 0)) {
			// End of data receive
			for (size_t i = filled; i < max_size; ++i) {
			// Fill the position upto data end by 19 (non-measurement range)
				data[filled++] = 19;
			}
			return filled;
		}else if(i == 0){
			// Judge the message (Command) by first letter of receive data
			if ((buffer[0] != 'M') && (buffer[0] != 'G')) {
				return -1;
			}
			message_type = buffer[0];
		}else if(! strncmp(buffer, "99b", 3)) {
			// Detect "99b" and assume [time-stamp] and [data] to follow
			i = 4;
		}else if((i == 1) && (message_type == 'G')) {
			i = 4;
		}else if(i == 4) {
			// "99b" Fixed
			if (strncmp(buffer, "99b", 3)) {
				return -1;
			}
		}else if(i >= 6) {
			// Received Data
			if (line_length > (64 + 1)) {
				line_length = (64 + 1);
			}
			buffer[line_length -1] = '\0';
			int ret = urg_addRecvData(buffer, data, &filled);
			if (ret < 0) {
				return ret;
			}
		}
	}
	return -1;
}

