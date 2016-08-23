#include "http.h"
#include <Energia.h>
#include <WiFi.h>

#define TIMEOUT (45)

long httpGetRequest(char* host, int port, char* path, char* response) {
  String hostname = String(host);
  String head_post = "GET " + String(path) + " HTTP/1.1";
  String head_host = "Host: " + hostname + ":" + port;
  String request = head_post + "\n" + head_host + "\n\n";

  uint16_t socket_handle;
  uint32_t host_ip;
  long success;

  SlTimeval_t timeout { .tv_sec = TIMEOUT, .tv_usec = 0 };
  
  // retrieve IP from Hostname
  success = sl_NetAppDnsGetHostByName((signed char*)hostname.c_str(), hostname.length(), &host_ip, SL_AF_INET);

  if (success < 0) {
    Serial.println("Could not resolve HOST");
    return success;
  }

  // create socket
  SlSockAddrIn_t socket_address {
  	.sin_family = SL_AF_INET,
  	.sin_port = sl_Htons(port),
  	.sin_addr = {
  		.s_addr = sl_Htonl(host_ip)
  	}
  };

  // opens a secure socket
  socket_handle = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, SL_IPPROTO_TCP);

  if (socket_handle < 0) {
    Serial.println("Failed to initialize handle");
    return socket_handle;
  }

  // configure the receive timeout
  success = sl_SetSockOpt(socket_handle, SL_SOL_SOCKET, SL_SO_RCVTIMEO, (const void*)&timeout, sizeof(timeout));

  if (success < 0) {
    Serial.println("Failed to set timeout");
    goto cleanup;
  }

  // connect to the server
  success = sl_Connect(socket_handle, (SlSockAddr_t*)&socket_address, sizeof(SlSockAddrIn_t));

  if (success < 0) {
    Serial.println("Failed to connect");
    goto cleanup;
  }

  // send the request to the server
  success = sl_Send(socket_handle, request.c_str(), request.length(), 0);

  if (success < 0) {
    Serial.println("Failed to send");
    goto cleanup;
  }

  // receive response packet from the server
  success = sl_Recv(socket_handle, response, 1024, 0);

  if (success < 0) {
    Serial.println("Failed to receive");
    goto cleanup;
  }

  cleanup:
  sl_Close(socket_handle);
  return success;
}

long httpPostRequest(char* host, int port, char* path, char* body, char* response) {
  String hostname = String(host);
  String bodystr = String(body);
  String head_post = "POST " + String(path) + " HTTP/1.1";
  String head_host = "Host: " + hostname + ":" + port;
  String head_contenttype = "Content-Type: plain/text; charset=utf-8";
  String head_size = String("Content-Length: ") + bodystr.length();
  String request = head_post + '\n' + 
    head_host + '\n' + 
    head_contenttype + '\n' + 
    head_size + "\n\n" + 
    bodystr;

  uint16_t socket_handle;
  uint32_t host_ip;
  long success;

  SlTimeval_t timeout { .tv_sec = TIMEOUT, .tv_usec = 0 };
  
  // retrieve IP from Hostname
  success = sl_NetAppDnsGetHostByName((signed char*)hostname.c_str(), hostname.length(), &host_ip, SL_AF_INET);

  if (success < 0) {
    Serial.println("Could not resolve HOST");
    return success;
  }

  // create socket
  SlSockAddrIn_t socket_address {
    .sin_family = SL_AF_INET,
    .sin_port = sl_Htons(port),
    .sin_addr = {
      .s_addr = sl_Htonl(host_ip)
    }
  };

  // opens a secure socket
  socket_handle = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, SL_IPPROTO_TCP);

  if (socket_handle < 0) {
    Serial.println("Failed to initialize handle");
    return socket_handle;
  }

  // configure the receive timeout
  success = sl_SetSockOpt(socket_handle, SL_SOL_SOCKET, SL_SO_RCVTIMEO, (const void*)&timeout, sizeof(timeout));

  if (success < 0) {
    Serial.println("Failed to set timeout");
    goto cleanup;
  }

  // connect to the server
  success = sl_Connect(socket_handle, (SlSockAddr_t*)&socket_address, sizeof(SlSockAddrIn_t));

  if (success < 0) {
    Serial.println("Failed to connect");
    goto cleanup;
  }

  // send the request to the server
  success = sl_Send(socket_handle, request.c_str(), request.length(), 0);

  if (success < 0) {
    Serial.println("Failed to send");
    goto cleanup;
  }

  // receive response packet from the server
  success = sl_Recv(socket_handle, response, 1024, 0);

  if (success < 0) {
    Serial.println("Failed to receive");
    goto cleanup;
  }

  cleanup:
  sl_Close(socket_handle);
  return success;
}