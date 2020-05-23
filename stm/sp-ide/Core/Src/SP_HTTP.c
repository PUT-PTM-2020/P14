#include "SP_HTTP.h"
#include "SP_SD.h"
#include "SP_NET.h"
#include "SP_MENU.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DT_UPDATE_PATTERN 	"date\":["
#define DT_UPDATE_OK		"{\"id\":\"ok\"}"
#define DT_UPDATE_ERR		"{\"id\":\"err\"}"

#define ALL_AMOUNT(arg) 		"{\"amount\":"arg"}"
#define ALL_OFFSET_PATTERN 		"offset\":"

#define MAX_REQUEST_LEN		20
#define MAX_RESPONSE_LEN	400
#define MAX_LINE_LEN		100

#define RSP_NOT_FOUND 	"404 Not Found"
#define RSP_OK			"200 OK"

#define CT_JS 		"text/javascript"
#define CT_CSS 		"text/css"
#define CT_HTML 	"text/html"
#define CT_JSON		"text/json"

#define CN_CLOSE 	"close"
#define CN_KEEP		"keep-alive"

#define EX_CORS 	"Access-Control-Allow-Origin: *\r\n"

char _response[MAX_RESPONSE_LEN];
char _line[MAX_LINE_LEN];
char _request[MAX_REQUEST_LEN];

#define __resetResponse() for(int i=0;i<MAX_RESPONSE_LEN;i++)_response[i]=0
#define __resetLine() for(int i=0;i<MAX_LINE_LEN;i++)_line[i]=0
#define __resetRequest() for(int i=0;i<MAX_REQUEST_LEN;i++)_request[i]=0

#define IF_REQUEST(arg) if(strcmp(request,arg)==0)
#define OR_REQUEST(arg) else IF_REQUEST(arg)

char* _HTTP_ParseHeader(char *response, char *contentType, uint32_t length,
		char *connection) {
	__resetResponse();
	__resetLine();

	sprintf(_line, "HTTP/1.1 %s\r\n", response);
	strcpy(_response, _line);
	__resetLine();

	sprintf(_line, "Content-Type: %s\r\n", contentType);
	strcat(_response, _line);
	__resetLine();

	if (strcmp(contentType, CT_JSON) == 0) {
		strcat(_response, EX_CORS);
	}

	sprintf(_line, "Content-Lenght: %ld\r\n", length);
	strcat(_response, _line);
	__resetLine();

	sprintf(_line, "Connection: %s\r\n\r\n", connection);
	strcat(_response, _line);

	return (char*) _response;
}

char* _HTTP_GetRawRequest(char *request) {
	int index = 0;

	__resetRequest();
	while (request[index] != 'H') {
		_request[index] = request[index];
		index++;
	}

	_request[--index] = 0;
	return (char*) _request;
}

void HTTP_HandleRequest(char *req, char connID) {
	char *request = _HTTP_GetRawRequest(req);
	char *header;
	uint32_t size;

	IF_REQUEST("GET /") {
		char *file = SD_ReadFile("index.htm", &size);
		header = _HTTP_ParseHeader(RSP_OK, CT_HTML, size, CN_CLOSE);

		NET_SendTCPData(connID, header);
		NET_SendTCPData(connID, file);

		NET_CloseConnSignal(connID);

	} OR_REQUEST("GET /about") {
		char *file = SD_ReadFile("about.htm", &size);
		header = _HTTP_ParseHeader(RSP_OK, CT_HTML, size, CN_CLOSE);

		NET_SendTCPData(connID, header);
		NET_SendTCPData(connID, file);

		NET_CloseConnSignal(connID);

	} OR_REQUEST("GET /data") {
		char *file = SD_ReadFile("data.htm", &size);
		header = _HTTP_ParseHeader(RSP_OK, CT_HTML, size, CN_CLOSE);

		NET_SendTCPData(connID, header);
		NET_SendTCPData(connID, file);

		NET_CloseConnSignal(connID);

	} OR_REQUEST("GET /now") {
		MENU_ForceUpdate();

		char *file = SD_GetLastJson(&size);
		header = _HTTP_ParseHeader(RSP_OK, CT_JSON, size, CN_CLOSE);

		NET_SendTCPData(connID, header);
		NET_SendTCPData(connID, file);

		NET_CloseConnSignal(connID);

	} OR_REQUEST("GET /all") {
		uint32_t len = SD_GetNofJsons();

		char file[100] = { 0 };
		size = sprintf(file, ALL_AMOUNT("%lu"), len);

		header = _HTTP_ParseHeader(RSP_OK, CT_JSON, size, CN_CLOSE);

		NET_SendTCPData(connID, header);
		NET_SendTCPData(connID, file);
		NET_CloseConnSignal(connID);

	} OR_REQUEST("POST /all/logs") {
		int ix = NET_GetIndexForPattern(ALL_OFFSET_PATTERN);
		if (ix == -1) {
			NET_CloseConnSignal(connID);
			return;
		}

		int i = 0;
		char tempStr[5] = { 0 };
		while (req[ix] != '}') {
			tempStr[i++] = req[ix++];
		}

		int offset = atoi(tempStr);

		char *file = SD_GetJsonFromEnd(offset, &size);
		if (file != NULL) {
			header = _HTTP_ParseHeader(RSP_OK, CT_JSON, size, CN_CLOSE);
			NET_SendTCPData(connID, header);
			NET_SendTCPData(connID, file);
		} else {
			size = strlen(ALL_AMOUNT("0"));

			header = _HTTP_ParseHeader(RSP_OK, CT_JSON, size, CN_CLOSE);
			NET_SendTCPData(connID, header);
			NET_SendTCPData(connID, ALL_AMOUNT("0"));
		}

		NET_CloseConnSignal(connID);

	} OR_REQUEST("POST /dt") {
		int index = NET_GetIndexForPattern(DT_UPDATE_PATTERN);

		int test = index;
		while (req[test++] != '}') {
			if (test > strlen(req)) {
				/* niekompletny json */
				header = _HTTP_ParseHeader(RSP_NOT_FOUND, CT_JSON, 0, CN_CLOSE);
				NET_SendTCPData(connID, header);
				NET_CloseConnSignal(connID);
				return;
			}
		}

		/* zakladamy poprawnosc w tym miejscu */
		if (index != -1) {
			uint8_t date[6] = { 0 }, time[6] = { 0 };

			for (int i = 0; i < 6; i++) {
				date[i] = req[index] - '0';
				index += 2;
			}

			/* tak o zeby jednak nie wpasc w loopa */
			while (req[index++] != '[') {
				if (index > strlen(req)) {
					header = _HTTP_ParseHeader(RSP_NOT_FOUND, CT_JSON, 0,
					CN_CLOSE);
					NET_SendTCPData(connID, header);
					NET_CloseConnSignal(connID);
					return;
				}
			}

			for (int i = 0; i < 6; i++) {
				time[i] = req[index] - '0';
				index += 2;
			}

			/* recieved */
			SD_SetDateTime(date, time);

			header = _HTTP_ParseHeader(RSP_OK, CT_JSON, strlen(DT_UPDATE_OK),
			CN_CLOSE);
			NET_SendTCPData(connID, header);
			NET_SendTCPData(connID, DT_UPDATE_OK);
			NET_CloseConnSignal(connID);
		}
	} else {
		/* nieobslugiwane zadanie */
		char *file = SD_ReadFile("error.htm", &size);
		header = _HTTP_ParseHeader(RSP_NOT_FOUND, CT_HTML, size, CN_CLOSE);
		NET_SendTCPData(connID, header);
		NET_SendTCPData(connID, file);
		NET_CloseConnSignal(connID);
	}
}
