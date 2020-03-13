#include "ExceptionHandler.h"
#include <sstream>

ExceptionHandler::ExceptionHandler(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* ExceptionHandler::what() const noexcept
{
	// Create output String Stream
	std::ostringstream oss;
	// Define output String Stream
	oss << GetType() << std::endl
		<< GetOriginString();
	// Defining whatBuffer
	whatBuffer = oss.str();
	// Returning whatBuffer string
	return whatBuffer.c_str();
}

const char* ExceptionHandler::GetType() const noexcept
{
	return "Cherry Exception";
}

int ExceptionHandler::GetLine() const noexcept
{
	return line;
}

const std::string& ExceptionHandler::GetFile() const noexcept
{
	return file;
}

std::string ExceptionHandler::GetOriginString() const noexcept
{
	// Create Output String
	std::ostringstream oss;
	// Define Output String
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	// Return Output String
	return oss.str();
}