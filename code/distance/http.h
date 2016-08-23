#pragma once

long httpGetRequest(char* hostname, int port, char* path, char* response);

long httpPostRequest(char* hostname, int port, char* path, char* body, char* response);