#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
#include "dpp/dpp.h"
#include "Miniaudio.hpp" //external library

#define OFFSET_PTR(p, offset) (((ma_uint8*)(p)) + (offset))

//TODO: https://stackoverflow.com/questions/1636333/download-file-using-libcurl-in-c-c
//use libCURL to download files