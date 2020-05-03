#ifndef HTTP_H
#define HTTP_H
#pragma once
#include <Windows.h>
#include <winhttp.h>
#include <string>
#include <map>

#pragma comment(lib, "winhttp.lib")

namespace Requests
{
	typedef std::map<std::string, std::string> post_data;
	typedef std::map<std::string, std::string> req_headers;
	typedef std::map<std::string, std::string> res_headers;

	const req_headers DEFAULT_HEADERS = {{"User-Agent", "Requests 2.0"}};
	const req_headers DEFAULT_POST_HEADERS = {
		{"Content-Type", "application/x-www-form-urlencoded"}, {"User-Agent", "Requests 2.0"}
	};

	struct request
	{
		std::string status_code;
		int content_length{};
		std::string text;
		res_headers headers;
	};


	request* execute(
		DWORD verb,
		const std::string& user_agent,
		const std::string& hostname,
		const std::string& raw_uri,
		const std::map<std::string, std::string>& opt_headers,
		const std::string& pdata
	);

	request* get(std::string url, req_headers h_data = DEFAULT_HEADERS, u_short port = 80);
	request* post(std::string url, const post_data& pdata, req_headers h_data = DEFAULT_POST_HEADERS,
	              u_short port = 80);
}
#endif
