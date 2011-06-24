/*
  **
  ** dproxy.h
  **
  ** Part of the drpoxy package by Matthew Pratt.
  **
  ** Copyright 1999 Matthew Pratt <mattpratt@yahoo.com>
  **
  ** This software is licensed under the terms of the GNU General
  ** Public License (GPL). Please see the file COPYING for details.
  **
  **
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <syslog.h>
#include <errno.h>

#ifndef PORT
#define PORT 53
#endif

#define BUF_SIZE 2048

struct dns_header{
  short int dns_id;
  short int dns_flags;
  short int dns_no_questions;
  short int dns_no_answers;
  short int dns_no_authority;
  short int dns_no_additional;
};
struct dns_answer{
  short int dns_name;
  short int dns_type;
  short int dns_class;
  short int dns_time_to_live;
  short int dns_time_to_live2;
  short int dns_data_len;
};
struct udp_packet{
  struct dns_header dns_hdr;
  char buf[4096];
  char len;
  struct in_addr src_ip;
  int src_port;
};

