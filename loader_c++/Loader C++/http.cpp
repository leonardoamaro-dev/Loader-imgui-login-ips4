// This file is part of the "requests" project, https://github.com/Code-Building/requests
//   (c) 2018 Code Building https://codebuilding.org
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT


#include "C:\ProgramData\WindowsIncludes\http.h"
#include "C:\ProgramData\WindowsIncludes\utils.h"
#include <iostream>
#include <sstream>

namespace Requests
{
	request* execute(const DWORD verb,
	                 const std::string& user_agent,
	                 const std::string& hostname,
	                 const std::string& raw_uri,
	                 const std::map<std::string, std::string>& opt_headers,
	                 const std::string& pdata)
	{
		auto this_req = new request;
		std::wstring response_header;
		std::vector<char> response_body;
		std::string plain_text;
		DWORD header_size = 0;
		DWORD dw_downloaded;


		const auto pd = const_cast<char*>(pdata.c_str());
		const DWORD pdata_len = strlen(pd);


		DWORD dw_size = 0;
		LPVOID lp_out_buffer = nullptr;
		BOOL b_results = false;
		HINTERNET h_session = nullptr, h_connect = nullptr, h_request = nullptr;

		h_session = WinHttpOpen(ReqUtils::to_lpcwstr(user_agent.c_str()), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, nullptr,
		                        nullptr, 0);

		if (h_session)
			h_connect = WinHttpConnect(h_session, ReqUtils::to_lpcwstr(hostname.c_str()), INTERNET_DEFAULT_HTTP_PORT,
			                           0);

		if (h_connect)
			h_request = WinHttpOpenRequest(h_connect, verb == 1 ? L"GET" : L"POST",
			                               ReqUtils::to_lpcwstr(raw_uri.c_str()), nullptr, nullptr, nullptr, 0);

		for (auto& it : opt_headers)
		{
			auto header = it.first + ": " + it.second;
			WinHttpAddRequestHeaders(h_request,
			                         ReqUtils::to_lpcwstr(header.c_str()),
			                         -1L,
			                         WINHTTP_ADDREQ_FLAG_ADD);
		}


		if (h_request)
			b_results = WinHttpSendRequest(h_request, nullptr, 0, verb == 2 ? pd : nullptr, verb == 2 ? pdata_len : 0,
			                               verb == 2 ? pdata_len : 0, 0);

		if (b_results)
			b_results = WinHttpReceiveResponse(h_request, nullptr);

		if (b_results)
		{
			b_results = WinHttpQueryHeaders(h_request, WINHTTP_QUERY_RAW_HEADERS_CRLF, nullptr, nullptr, &header_size,
			                                nullptr);
			if ((!b_results) && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
			{
				response_header.resize(header_size / sizeof(wchar_t));
				if (response_header.empty())
					b_results = TRUE;
				else
				{
					b_results = WinHttpQueryHeaders(h_request, WINHTTP_QUERY_RAW_HEADERS_CRLF, nullptr,
					                                &response_header[0], &header_size, nullptr);
					if (!b_results) header_size = 0;
					response_header.resize(header_size / sizeof(wchar_t));
				}
			}
		}
		if (b_results)
		{
			do
			{
				dw_size = 0;
				b_results = WinHttpQueryDataAvailable(h_request, &dw_size);
				if (!b_results)
					throw std::exception("Uncaught error on WinHttpQueryDataAvailable()");

				if (dw_size == 0)
					break;

				do
				{
					const DWORD dw_offset = response_body.size();
					response_body.resize(dw_offset + dw_size);

					b_results = WinHttpReadData(h_request, &response_body[dw_offset], dw_size, &dw_downloaded);
					if (!b_results)
						throw std::exception("Uncaught exception on WinHttpReadData()");

					response_body.resize(dw_offset + dw_downloaded);

					if (dw_downloaded == 0)
						break;

					dw_size -= dw_downloaded;
				}
				while (dw_size > 0);
			}
			while (true);
		}

		for (auto& x : response_body)
			plain_text += x;

		this_req->text = plain_text;
		this_req->headers = ReqUtils::parse_res_headers(std::string(response_header.begin(), response_header.end()));
		const auto len_header_value = this_req->headers["Content-Length"].c_str();
		this_req->status_code = this_req->headers["Status"];
		this_req->content_length = atoi(len_header_value);
		return this_req;
	}

	request* get(std::string url, req_headers h_data, const u_short port)
	{
		if (ReqUtils::starts_with(url, "http://"))
			url = url.substr(7);
		else if (ReqUtils::starts_with(url, "https://"))
			url = url.substr(8);

		if (url[url.length()] != '/' && ReqUtils::string_index(url, "/") == -1)
			url += "/";

		const auto opt_headers = ReqUtils::parse_headers(h_data, 1);
		const auto ua = h_data["User-Agent"];

		const auto requested_uri_raw = ReqUtils::populate_uri(url);
		const auto request_host = ReqUtils::split(url, '/')[0];


		return execute(1, ua, request_host, requested_uri_raw, opt_headers, "");
	}

	request* post(std::string url, const post_data& pdata, req_headers h_data, const u_short port)
	{
		if (ReqUtils::starts_with(url, "http://"))
			url = url.substr(7);
		else if (ReqUtils::starts_with(url, "https://"))
			url = url.substr(8);

		if (url[url.length()] != '/' && ReqUtils::string_index(url, "/") == -1)
			url += "/";

		const auto opt_headers = ReqUtils::parse_headers(h_data, 2);
		const auto ua = h_data["User-Agent"];

		const auto requested_uri_raw = ReqUtils::populate_uri(url);
		const auto request_host = ReqUtils::split(url, '/')[0];
		const auto post_data_str = ReqUtils::generate_post(pdata);


		return execute(2, ua, request_host, requested_uri_raw, opt_headers, post_data_str);
	}
}
