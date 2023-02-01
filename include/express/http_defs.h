// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#pragma once

#include <ostream>

namespace Express::Http {
#define HTTP_METHODS                                                                               \
    METHOD(Options, "OPTIONS")                                                                     \
    METHOD(Get, "GET")                                                                             \
    METHOD(Post, "POST")                                                                           \
    METHOD(Head, "HEAD")                                                                           \
    METHOD(Put, "PUT")                                                                             \
    METHOD(Patch, "PATCH")                                                                         \
    METHOD(Delete, "DELETE")

#define STATUS_CODES                                                                               \
    STATUS_CODE(100, Continue, "Continue")                                                         \
    STATUS_CODE(101, Switching_Protocols, "Switching Protocols")                                   \
    STATUS_CODE(102, Processing, "Processing")                                                     \
    STATUS_CODE(103, Early_Hints, "Early Hints")                                                   \
    STATUS_CODE(200, Ok, "OK")                                                                     \
    STATUS_CODE(201, Created, "Created")                                                           \
    STATUS_CODE(202, Accepted, "Accepted")                                                         \
    STATUS_CODE(203, NonAuthoritative_Information, "Non-Authoritative Information")                \
    STATUS_CODE(204, No_Content, "No Content")                                                     \
    STATUS_CODE(205, Reset_Content, "Reset Content")                                               \
    STATUS_CODE(206, Partial_Content, "Partial Content")                                           \
    STATUS_CODE(207, MultiStatus, "Multi-Status")                                                  \
    STATUS_CODE(208, Already_Reported, "Already Reported")                                         \
    STATUS_CODE(226, IM_Used, "IM Used")                                                           \
    STATUS_CODE(300, Multiple_Choices, "Multiple Choices")                                         \
    STATUS_CODE(301, Moved_Permanently, "Moved Permanently")                                       \
    STATUS_CODE(302, Found, "Found")                                                               \
    STATUS_CODE(303, See_Other, "See Other")                                                       \
    STATUS_CODE(304, Not_Modified, "Not Modified")                                                 \
    STATUS_CODE(305, Use_Proxy, "Use Proxy")                                                       \
    STATUS_CODE(307, Temporary_Redirect, "Temporary Redirect")                                     \
    STATUS_CODE(308, Permanent_Redirect, "Permanent Redirect")                                     \
    STATUS_CODE(400, Bad_Request, "Bad Request")                                                   \
    STATUS_CODE(401, Unauthorized, "Unauthorized")                                                 \
    STATUS_CODE(402, Payment_Required, "Payment Required")                                         \
    STATUS_CODE(403, Forbidden, "Forbidden")                                                       \
    STATUS_CODE(404, Not_Found, "Not Found")                                                       \
    STATUS_CODE(405, Method_Not_Allowed, "Method Not Allowed")                                     \
    STATUS_CODE(406, Not_Acceptable, "Not Acceptable")                                             \
    STATUS_CODE(407, Proxy_Authentication_Required, "Proxy Authentication Required")               \
    STATUS_CODE(408, Request_Timeout, "Request Timeout")                                           \
    STATUS_CODE(409, Conflict, "Conflict")                                                         \
    STATUS_CODE(410, Gone, "Gone")                                                                 \
    STATUS_CODE(411, Length_Required, "Length Required")                                           \
    STATUS_CODE(412, Precondition_Failed, "Precondition Failed")                                   \
    STATUS_CODE(413, Request_Entity_Too_Large, "Request Entity Too Large")                         \
    STATUS_CODE(414, RequestURI_Too_Long, "Request-URI Too Long")                                  \
    STATUS_CODE(415, Unsupported_Media_Type, "Unsupported Media Type")                             \
    STATUS_CODE(416, Requested_Range_Not_Satisfiable, "Requested Range Not Satisfiable")           \
    STATUS_CODE(417, Expectation_Failed, "Expectation Failed")                                     \
    STATUS_CODE(418, I_m_a_teapot, "I'm a teapot")                                                 \
    STATUS_CODE(421, Misdirected_Request, "Misdirected Request")                                   \
    STATUS_CODE(422, Unprocessable_Entity, "Unprocessable Entity")                                 \
    STATUS_CODE(423, Locked, "Locked")                                                             \
    STATUS_CODE(424, Failed_Dependency, "Failed Dependency")                                       \
    STATUS_CODE(426, Upgrade_Required, "Upgrade Required")                                         \
    STATUS_CODE(428, Precondition_Required, "Precondition Required")                               \
    STATUS_CODE(429, Too_Many_Requests, "Too Many Requests")                                       \
    STATUS_CODE(431, Request_Header_Fields_Too_Large, "Request Header Fields Too Large")           \
    STATUS_CODE(444, Connection_Closed_Without_Response, "Connection Closed Without Response")     \
    STATUS_CODE(451, Unavailable_For_Legal_Reasons, "Unavailable For Legal Reasons")               \
    STATUS_CODE(499, Client_Closed_Request, "Client Closed Request")                               \
    STATUS_CODE(500, Internal_Server_Error, "Internal Server Error")                               \
    STATUS_CODE(501, Not_Implemented, "Not Implemented")                                           \
    STATUS_CODE(502, Bad_Gateway, "Bad Gateway")                                                   \
    STATUS_CODE(503, Service_Unavailable, "Service Unavailable")                                   \
    STATUS_CODE(504, Gateway_Timeout, "Gateway Timeout")                                           \
    STATUS_CODE(505, HTTP_Version_Not_Supported, "HTTP Version Not Supported")                     \
    STATUS_CODE(506, Variant_Also_Negotiates, "Variant Also Negotiates")                           \
    STATUS_CODE(507, Insufficient_Storage, "Insufficient Storage")                                 \
    STATUS_CODE(508, Loop_Detected, "Loop Detected")                                               \
    STATUS_CODE(510, Not_Extended, "Not Extended")                                                 \
    STATUS_CODE(511, Network_Authentication_Required, "Network Authentication Required")           \
    STATUS_CODE(599, Network_Connect_Timeout_Error, "Network Connect Timeout Error")

    enum class Method {
#define METHOD(m, _) m,
        HTTP_METHODS
#undef METHOD
    };

    enum class StatusCode {
#define STATUS_CODE(value, name, _) name = value,
        STATUS_CODES
#undef STATUS_CODE
    };

    auto methodString(Method method);
    auto statusCodeString(StatusCode status_code);

    auto operator<<(std::ostream& os, Method method) -> std::ostream&;
    auto operator<<(std::ostream& os, StatusCode code) -> std::ostream&;

    template <class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& crlf(std::basic_ostream<CharT, Traits>& os) {
        static constexpr char CRLF[] = { 0xD, 0xA };
        os.write(CRLF, 2);
        return os;
    }
}