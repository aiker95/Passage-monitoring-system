#ifndef INCLUDES_H

#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libpq.dll")
#pragma comment(lib, "libpq.lib")
#pragma warning(disable: 4996)
#include <iostream>

#include <WinSock2.h>
#include <winsock.h>

#include <functional>

#include <time.h>
#include <chrono>

#include <future>
#include <thread>
#include <vector>
#include <list>

#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

#include <libpq-fe.h>

#include "SServer.h"
#include "pointer.h"
#include "writer.h"
#include "Log.h"

#endif INCLUDES_H
